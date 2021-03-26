//##########################################################################
//#                                                                        #
//#                                PYCC                                    #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#include "pyCC.h"
#include "initCC.h"

#include <CCTypes.h>

#include <GeometricalAnalysisTools.h>

//qCC_db
#include <ccHObjectCaster.h>

//libs/qCC_io
#include<AsciiFilter.h>
#include <pyccTrace.h>

//system
#include <unordered_set>

#include "ccPluginInterface.h"
#include "ccPluginManager.h"

//qCC
#include "ccCommon.h"

//Qt
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>

// --- internal struct

//* Extended file loading parameters, from plugins/ccCommandLineInterface.h
struct CLLoadParameters: public FileIOFilter::LoadParameters
{
    CLLoadParameters() :
            FileIOFilter::LoadParameters(), m_coordinatesShiftEnabled(false), m_coordinatesShift(0, 0, 0)
    {
        shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG;
        alwaysDisplayLoadDialog = false;
        autoComputeNormals = false;
        coordinatesShiftEnabled = &m_coordinatesShiftEnabled;
        coordinatesShift = &m_coordinatesShift;
    }

    bool m_coordinatesShiftEnabled;
    CCVector3d m_coordinatesShift;
};

//! internal attributes (cloned from plugins/ccCommandLineInterface.h)
struct pyCC
{
    //! Currently opened point clouds and their filename
    std::vector<CLCloudDesc> m_clouds;

    //! Currently opened meshes and their filename
    std::vector<CLMeshDesc> m_meshes;

    //! Currently opened polys and their filename
    std::vector<CLPolyDesc> m_polys;

   //! Silent mode
    bool m_silentMode;

    //! Whether files should be automatically saved (after each process) or not
    bool m_autoSaveMode;

    //! Whether a timestamp should be automatically added to output files or not
    bool m_addTimestamp;

    //! Default numerical precision for ASCII output
    int m_precision;

    //! File loading parameters
    CLLoadParameters m_loadingParameters;

    //! Whether Global (coordinate) shift has already been defined
    bool m_coordinatesShiftWasEnabled;

    //! Global (coordinate) shift (if already defined)
    CCVector3d m_formerCoordinatesShift;

    //! Orphan entities
    ccHObject m_orphans;

    //! from ccApplicationBase
    QString m_ShaderPath;
    QString m_TranslationPath;
    QStringList m_PluginPaths;
};

static pyCC* s_pyCCInternals = nullptr;

pyCC* initCloudCompare()
{
    if (!s_pyCCInternals)
    {
        CCTRACE("initCloudCompare");
        int argc =1;
        char** argv = new char*[1];
        argv[0] = "cloudComPy";
        QApplication* app = new QApplication(argc, argv);
        s_pyCCInternals = new pyCC;
        s_pyCCInternals->m_silentMode = false;
        s_pyCCInternals->m_autoSaveMode = true;
        s_pyCCInternals->m_addTimestamp = true;
        s_pyCCInternals->m_precision = 12;
        s_pyCCInternals->m_coordinatesShiftWasEnabled = false;
        FileIOFilter::InitInternalFilters();  //load all known I/O filters (plugins will come later!)
        ccNormalVectors::GetUniqueInstance(); //force pre-computed normals array initialization
        s_pyCCInternals->m_ShaderPath = "";
        s_pyCCInternals->m_TranslationPath = "";

        // TODO: load the plugins:
        // requires compilation of ccPluginManager.cpp from CloudCompare/commons, not provided in a library (only in CloudCompare executable)
        pyCC_setupPaths(s_pyCCInternals);
        ccPluginManager::get().setPaths(s_pyCCInternals->m_PluginPaths);
        for (int i = 0; i < s_pyCCInternals->m_PluginPaths.size(); ++i)
            CCTRACE("pluginPath: " << s_pyCCInternals->m_PluginPaths.at(i).toStdString());
        ccPluginManager::get().loadPlugins();
    }
    return s_pyCCInternals;
}

void pyCC_setupPaths(pyCC* capi)
{
    QDir appDir = initCC::moduleDir;
    CCTRACE("appDir: " << appDir.absolutePath().toStdString());

    // Set up our shader and plugin paths
#if defined(Q_OS_MAC)
    QDir  bundleDir = appDir;

    if ( bundleDir.dirName() == "MacOS" )
    {
        bundleDir.cdUp();
    }

    m_PluginPaths << (bundleDir.absolutePath() + "/PlugIns/ccPlugins");

#if defined(CC_MAC_DEV_PATHS)
    // Used for development only - this is the path where the plugins are built
    // and the shaders are located.
    // This avoids having to install into the application bundle when developing.
    bundleDir.cdUp();
    bundleDir.cdUp();
    bundleDir.cdUp();

    capi->m_PluginPaths << (bundleDir.absolutePath() + "/ccPlugins");
    capi->m_ShaderPath = (bundleDir.absolutePath() + "/shaders");
    capi->m_TranslationPath = (bundleDir.absolutePath() + "/qCC/translations");
#else
    capi->m_ShaderPath = (bundleDir.absolutePath() + "/Shaders");
    capi->m_TranslationPath = (bundleDir.absolutePath() + "/translations");
#endif
#elif defined(Q_OS_WIN)
    capi->m_PluginPaths << (appDir.absolutePath() + "/plugins");
    capi->m_ShaderPath = (appDir.absolutePath() + "/shaders");
    capi->m_TranslationPath = (appDir.absolutePath() + "/translations");
#elif defined(Q_OS_LINUX)
    // Shaders & plugins are relative to the bin directory where the executable is found
    QDir  theDir = appDir;

    if ( theDir.dirName() == "cloudcompare" ) // install
    {
        theDir.cdUp();
        theDir.cdUp();
        CCTRACE("cloudCompare install: " << theDir.absolutePath().toStdString());
        capi->m_PluginPaths << (theDir.absolutePath() + "/lib/cloudcompare/plugins");
        capi->m_ShaderPath = (theDir.absolutePath() + "/share/cloudcompare/shaders");
        capi->m_TranslationPath = (theDir.absolutePath() + "/share/cloudcompare/translations");
    }
    else if ( theDir.dirName() == "sip" ) // build
    {
        theDir.cdUp();
        CCTRACE("cloudCompare install: " << theDir.absolutePath().toStdString());
        capi->m_PluginPaths << (theDir.absolutePath() + "/CloudCompare/plugins"); // need to create a symbolic link here for all the plugins.so
        capi->m_ShaderPath = (theDir.absolutePath() + "/shaders");
        capi->m_TranslationPath = (theDir.absolutePath() + "/CloudCompare/translations");
    }
    else
    {
        // Choose a reasonable default to look in
        capi->m_PluginPaths << "/usr/lib/cloudcompare/plugins";
        capi->m_ShaderPath = "/usr/share/cloudcompare/shaders";
        capi->m_TranslationPath = "/usr/share/cloudcompare/translations";
    }
#else
#warning Need to specify the shader path for this OS.
#endif

    // Add any app data paths to plugin paths
    // Plugins in these directories take precendence over the included ones
    // This allows users to put plugins outside of the install directories.
    const QStringList appDataPaths = QStandardPaths::standardLocations( QStandardPaths::AppDataLocation );

    for ( const QString &appDataPath : appDataPaths )
    {
        QString path = appDataPath + "/plugins";

        if (!capi->m_PluginPaths.contains(path)) //avoid duplicate entries (can happen, at least on Windows)
        {
            capi->m_PluginPaths << path;
        }
    }
}
ccPolyline* loadPolyline(
    const char* filename, CC_SHIFT_MODE mode, int skip, double x, double y, double z)
{
    CCTRACE("Opening file: " << filename << " mode: " << mode << " skip: " << skip << " x: " << x << " y: " << y << " z: " << z);
    pyCC* capi = initCloudCompare();
    ::CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    ccHObject* db = nullptr;

    FileIOFilter::Shared filter = nullptr;
    QString fileName(filename);
    if (mode == AUTO)
    {
        capi->m_loadingParameters.m_coordinatesShiftEnabled = false;
        capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG_AUTO_SHIFT;
    }
    else
    {
        capi->m_loadingParameters.m_coordinatesShiftEnabled = true;
        capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG;
        capi->m_loadingParameters.m_coordinatesShift = CCVector3d(x, y, z);
    }
    if (filter)
    {
        db = FileIOFilter::LoadFromFile(fileName, capi->m_loadingParameters, filter, result);
    }
    else
    {
        db = FileIOFilter::LoadFromFile(fileName, capi->m_loadingParameters, result, QString());
    }

    if (!db)
    {
        CCTRACE("LoadFromFile returns nullptr");
        return nullptr;
    }
//    std::unordered_set<unsigned> verticesIDs;

    // look for polylines inside loaded DB
    ccHObject::Container polys;
    db->filterChildren(polys, true, CC_TYPES::POLY_LINE);
    size_t count = polys.size();
    CCTRACE("number of polys: " << count);
    for (size_t i = 0; i < count; ++i)
    {
        ccPolyline* pc = static_cast<ccPolyline*>(polys[i]);
        if (pc->getParent())
        {
            pc->getParent()->detachChild(pc);
        }
//
//        //if the cloud is a set of vertices, we ignore it!
//        if (verticesIDs.find(pc->getUniqueID()) != verticesIDs.end())
//        {
//            capi->m_orphans.addChild(pc);
//            continue;
//        }
        CCTRACE("Found one poly with " << pc->size() << " points");
        capi->m_polys.emplace_back(pc, filename, count == 1 ? -1 : static_cast<int>(i));
    }

    delete db;
    db = nullptr;

    if (count > 0)
        return capi->m_polys.back().pc;
    return nullptr;
}

ccPointCloud* loadPointCloud(const char* filename, CC_SHIFT_MODE mode, int skip, double x, double y, double z)
{
    CCTRACE("Opening file: " << filename << " mode: " << mode << " skip: " << skip << " x: " << x << " y: " << y << " z: " << z);
    // TODO process optional parameter skip
    // TODO code from ccCommandLineParser::importFile
    pyCC* capi = initCloudCompare();
    ::CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    ccHObject* db = nullptr;

    FileIOFilter::Shared filter = nullptr;
    QString fileName(filename);
    if (mode == AUTO)
    {
        capi->m_loadingParameters.m_coordinatesShiftEnabled = false;
        capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG_AUTO_SHIFT;
    }
    else
    {
        capi->m_loadingParameters.m_coordinatesShiftEnabled = true;
        capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG;
        capi->m_loadingParameters.m_coordinatesShift = CCVector3d(x, y, z);
    }
    if (filter)
    {
        db = FileIOFilter::LoadFromFile(fileName, capi->m_loadingParameters, filter, result);
    }
    else
    {
        db = FileIOFilter::LoadFromFile(fileName, capi->m_loadingParameters, result, QString());
    }

    if (!db)
    {
        CCTRACE("LoadFromFile returns nullptr");
        return nullptr;
    }

    std::unordered_set<unsigned> verticesIDs;

    // look for the remaining clouds inside loaded DB
    ccHObject::Container clouds;
    db->filterChildren(clouds, true, CC_TYPES::POINT_CLOUD);
    size_t count = clouds.size();
    CCTRACE("number of clouds: " << count);
    for (size_t i = 0; i < count; ++i)
    {
        ccPointCloud* pc = static_cast<ccPointCloud*>(clouds[i]);
        if (pc->getParent())
        {
            pc->getParent()->detachChild(pc);
        }

        //if the cloud is a set of vertices, we ignore it!
        if (verticesIDs.find(pc->getUniqueID()) != verticesIDs.end())
        {
            capi->m_orphans.addChild(pc);
            continue;
        }
        CCTRACE("Found one cloud with " << pc->size() << " points");
        capi->m_clouds.emplace_back(pc, filename, count == 1 ? -1 : static_cast<int>(i));
    }

    delete db;
    db = nullptr;

    if (count > 0)
        return capi->m_clouds.back().pc;
    return nullptr;
}

::CC_FILE_ERROR SavePointCloud(ccPointCloud* cloud, const QString& filename)
{
    CCTRACE("saving cloud");
    pyCC* capi = initCloudCompare();
    if ((cloud == nullptr) || filename.isEmpty())
        return ::CC_FERR_BAD_ARGUMENT;
    CCTRACE("cloud: " << cloud->getName().toStdString() << " file: " << filename.toStdString());
    FileIOFilter::SaveParameters parameters;
    parameters.alwaysDisplaySaveDialog = false;
    QFileInfo fi(filename);
    QString ext = fi.suffix();
    QString fileFilter = "";
    const std::vector<FileIOFilter::Shared>& filters = FileIOFilter::GetFilters();
    for (const auto filter : filters)
    {
        QStringList theFilters = filter->getFileFilters(false);
        QStringList matches = theFilters.filter(ext);
        if (matches.size())
        {
            fileFilter = matches.first();
            break;
        }
    }
    CCTRACE("fileFilter: " << fileFilter.toStdString());
    ::CC_FILE_ERROR result = FileIOFilter::SaveToFile(cloud, filename, parameters, fileFilter); //AsciiFilter::GetFileFilter());
    return result;
}

bool computeCurvature(CurvatureType option, double radius, QList<ccPointCloud*> clouds)
{
    CCTRACE("computeCurvature mode: " << option << " radius: " << radius << " nbClouds: " << clouds.size());
    ccHObject::Container entities;
    entities.resize(clouds.size());
    for (int i = 0; i < clouds.size(); ++i)
    {
        CCTRACE("entity: "<< i << " name: " << clouds.at(i)->getName().toStdString());
        entities[i] = clouds.at(i);
    }
    return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::Curvature, option, radius, entities);
}

ccPointCloud* filterBySFValue(double minVal, double maxVal, ccPointCloud* cloud)
{
    CCTRACE("filterBySFValue min: " << minVal << " max: " << maxVal << " cloudName: " << cloud->getName().toStdString());
    CCCoreLib::ScalarField* sf = cloud->getCurrentOutScalarField();
    ccPointCloud* fitleredCloud = nullptr;
    if (sf)
    {
        ScalarType vmin = minVal;
        ScalarType vmax = maxVal;
        fitleredCloud = cloud->filterPointsByScalarValue(vmin, vmax, false);
    }
    return fitleredCloud;
}

double GetPointCloudRadius(QList<ccPointCloud*> clouds, unsigned knn)
{
    CCTRACE("GetDefaultCloudKernelSize - NbPoints: " << knn << " nbClouds: " << clouds.size());
    double sigma = -1;
    for (ccPointCloud* cloud : clouds)
    {
        CCTRACE("cloud name: " << cloud->getName().toStdString());
        PointCoordinateType sigmaCloud = pyCC_GetDefaultCloudKernelSize(cloud, knn);

        //we keep the smallest value
        if (sigma < 0 || sigmaCloud < sigma)
            sigma = sigmaCloud;
    }
    CCTRACE("radius: "<< sigma);
    return sigma;
}

bool pyCC_ComputeGeomCharacteristic(
    CCCoreLib::GeometricalAnalysisTools::GeomCharacteristic c,
    int subOption,
    PointCoordinateType radius,
    ccHObject::Container& entities)
{
// TODO code from ccLibAlgorithms::ComputeGeomCharacteristic
    CCTRACE("pyCCComputeGeomCharacteristic "<< subOption << " radius: " << radius);
    size_t selNum = entities.size();
    if (selNum < 1)
        return false;

//generate the right SF name
    QString sfName;

    switch (c)
    {
    case CCCoreLib::GeometricalAnalysisTools::Feature:
    {
        switch (subOption)
        {
        case CCCoreLib::Neighbourhood::EigenValuesSum:
            sfName = "Eigenvalues sum";
            break;
        case CCCoreLib::Neighbourhood::Omnivariance:
            sfName = "Omnivariance";
            break;
        case CCCoreLib::Neighbourhood::EigenEntropy:
            sfName = "Eigenentropy";
            break;
        case CCCoreLib::Neighbourhood::Anisotropy:
            sfName = "Anisotropy";
            break;
        case CCCoreLib::Neighbourhood::Planarity:
            sfName = "Planarity";
            break;
        case CCCoreLib::Neighbourhood::Linearity:
            sfName = "Linearity";
            break;
        case CCCoreLib::Neighbourhood::PCA1:
            sfName = "PCA1";
            break;
        case CCCoreLib::Neighbourhood::PCA2:
            sfName = "PCA2";
            break;
        case CCCoreLib::Neighbourhood::SurfaceVariation:
            sfName = "Surface variation";
            break;
        case CCCoreLib::Neighbourhood::Sphericity:
            sfName = "Sphericity";
            break;
        case CCCoreLib::Neighbourhood::Verticality:
            sfName = "Verticality";
            break;
        case CCCoreLib::Neighbourhood::EigenValue1:
            sfName = "1st eigenvalue";
            break;
        case CCCoreLib::Neighbourhood::EigenValue2:
            sfName = "2nd eigenvalue";
            break;
        case CCCoreLib::Neighbourhood::EigenValue3:
            sfName = "3rd eigenvalue";
            break;
        default:
            assert(false);
            ccLog::Error("Internal error: invalid sub option for Feature computation");
            return false;
        }

        sfName += QString(" (%1)").arg(radius);
    }
        break;

    case CCCoreLib::GeometricalAnalysisTools::Curvature:
    {
        switch (subOption)
        {
        case CCCoreLib::Neighbourhood::GAUSSIAN_CURV:
            sfName = CC_CURVATURE_GAUSSIAN_FIELD_NAME;
            break;
        case CCCoreLib::Neighbourhood::MEAN_CURV:
            sfName = CC_CURVATURE_MEAN_FIELD_NAME;
            break;
        case CCCoreLib::Neighbourhood::NORMAL_CHANGE_RATE:
            sfName = CC_CURVATURE_NORM_CHANGE_RATE_FIELD_NAME;
            break;
        default:
            assert(false);
            ccLog::Error("Internal error: invalid sub option for Curvature computation");
            return false;
        }
        sfName += QString(" (%1)").arg(radius);
    }
        break;

    case CCCoreLib::GeometricalAnalysisTools::LocalDensity:
        sfName = pyCC_GetDensitySFName(static_cast<CCCoreLib::GeometricalAnalysisTools::Density>(subOption), false, radius);
        break;

    case CCCoreLib::GeometricalAnalysisTools::ApproxLocalDensity:
        sfName = pyCC_GetDensitySFName(static_cast<CCCoreLib::GeometricalAnalysisTools::Density>(subOption), true);
        break;

    case CCCoreLib::GeometricalAnalysisTools::Roughness:
        sfName = CC_ROUGHNESS_FIELD_NAME + QString(" (%1)").arg(radius);
        break;

    case CCCoreLib::GeometricalAnalysisTools::MomentOrder1:
        sfName = CC_MOMENT_ORDER1_FIELD_NAME + QString(" (%1)").arg(radius);
        break;

    default:
        assert(false);
        return false;
    }

    for (size_t i = 0; i < selNum; ++i)
    {
        //is the ith selected data is eligible for processing?
        if (entities[i]->isKindOf(CC_TYPES::POINT_CLOUD))
        {
            ccGenericPointCloud* cloud = ccHObjectCaster::ToGenericPointCloud(entities[i]);

            ccPointCloud* pc = 0;
            int sfIdx = -1;
            if (cloud->isA(CC_TYPES::POINT_CLOUD))
            {
                pc = static_cast<ccPointCloud*>(cloud);

                sfIdx = pc->getScalarFieldIndexByName(qPrintable(sfName));
                if (sfIdx < 0)
                    sfIdx = pc->addScalarField(qPrintable(sfName));
                if (sfIdx >= 0)
                    pc->setCurrentScalarField(sfIdx);
                else
                {
                    continue;
                }
            }

            ccOctree::Shared octree = cloud->getOctree();
            if (!octree)
            {
                octree = cloud->computeOctree(nullptr);
                if (!octree)
                {
                    break;
                }
            }

            CCCoreLib::GeometricalAnalysisTools::ErrorCode result = CCCoreLib::GeometricalAnalysisTools::ComputeCharactersitic(
                    c, subOption, cloud, radius, nullptr, octree.data());

            if (result == CCCoreLib::GeometricalAnalysisTools::NoError)
            {
                if (pc && sfIdx >= 0)
                {
                    pc->setCurrentDisplayedScalarField(sfIdx);
                    pc->showSF(sfIdx >= 0);
                    pc->getCurrentInScalarField()->computeMinAndMax();
                }
                cloud->prepareDisplayForRefresh();
            }
            else
            {
                QString errorMessage;
                switch (result)
                {
                case CCCoreLib::GeometricalAnalysisTools::InvalidInput:
                    errorMessage = "Internal error (invalid input)";
                    break;
                case CCCoreLib::GeometricalAnalysisTools::NotEnoughPoints:
                    errorMessage = "Not enough points";
                    break;
                case CCCoreLib::GeometricalAnalysisTools::OctreeComputationFailed:
                    errorMessage = "Failed to compute octree (not enough memory?)";
                    break;
                case CCCoreLib::GeometricalAnalysisTools::ProcessFailed:
                    errorMessage = "Process failed";
                    break;
                case CCCoreLib::GeometricalAnalysisTools::UnhandledCharacteristic:
                    errorMessage = "Internal error (unhandled characteristic)";
                    break;
                case CCCoreLib::GeometricalAnalysisTools::NotEnoughMemory:
                    errorMessage = "Not enough memory";
                    break;
                case CCCoreLib::GeometricalAnalysisTools::ProcessCancelledByUser:
                    errorMessage = "Process cancelled by user";
                    break;
                default:
                    assert(false);
                    errorMessage = "Unknown error";
                    break;
                }

                if (pc && sfIdx >= 0)
                {
                    pc->deleteScalarField(sfIdx);
                    sfIdx = -1;
                }

                return false;
            }
        }
    }

    return true;
}

QString pyCC_GetDensitySFName(
    CCCoreLib::GeometricalAnalysisTools::Density densityType,
    bool approx,
    double densityKernelSize)
{
// --- from ccLibAlgorithms::GetDensitySFName
    CCTRACE("pyCCGetDensitySFName");
    QString sfName;

//update the name with the density type
    switch (densityType)
    {
    case CCCoreLib::GeometricalAnalysisTools::DENSITY_KNN:
        sfName = CC_LOCAL_KNN_DENSITY_FIELD_NAME;
        break;
    case CCCoreLib::GeometricalAnalysisTools::DENSITY_2D:
        sfName = CC_LOCAL_SURF_DENSITY_FIELD_NAME;
        break;
    case CCCoreLib::GeometricalAnalysisTools::DENSITY_3D:
        sfName = CC_LOCAL_VOL_DENSITY_FIELD_NAME;
        break;
    default:
        assert(false);
        break;
    }

    sfName += QString(" (r=%2)").arg(densityKernelSize);

    if (approx)
        sfName += " [approx]";

    return sfName;
}

PointCoordinateType pyCC_GetDefaultCloudKernelSize(ccGenericPointCloud* cloud, unsigned knn)
{
    assert(cloud);
    if (cloud && cloud->size() != 0)
    {
        //we compute the cloud bounding box volume, then guess the cloud surface and get a surface per point
        ccBBox box = cloud->getOwnBB();
        CCVector3 d = box.getDiagVec();
        PointCoordinateType volume = d[0] * d[1] * d[2];
        PointCoordinateType surface = pow(volume, static_cast<PointCoordinateType>(2.0 / 3.0));
        PointCoordinateType surfacePerPoint = surface / cloud->size();
        return sqrt(surfacePerPoint * knn);
    }

    return -CCCoreLib::PC_ONE;
}
