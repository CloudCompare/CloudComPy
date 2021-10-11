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

//libs/qCC_db
#include <CCTypes.h>
#include <GeometricalAnalysisTools.h>
#include <Garbage.h>
#include <ccHObjectCaster.h>
#include <GenericIndexedCloudPersist.h>
#include <ccGenericMesh.h>
#include <MeshSamplingTools.h>
#include <ccPointCloud.h>
#include <DistanceComputationTools.h>
#include <ParallelSort.h>
#include <PointProjectionTools.h>
#include <ccScalarField.h>
#include <ccSensor.h>
#include <ccMesh.h>
#include <ccGLMatrix.h>
#include <ccRasterGrid.h>

//libs/qCC_io
#include<AsciiFilter.h>
#include <pyccTrace.h>

//libs/CCPluginStub
#include "ccPluginInterface.h"

//libs/CCAppCommon
#include "ccPluginManager.h"

//qCC
#include "ccCommon.h"

//system
#include <unordered_set>
#include <cmath>

//Qt
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <exception>

#include "optdefines.h"
#ifdef PLUGIN_IO_QFBX
#include <FBXFilter.h>
#endif

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

static int pyCC_argc = 1;
static char* pyCC_argv[] = { strdup("cloudComPy"), NULL };

pyCC* initCloudCompare()
{
    if (!s_pyCCInternals)
    {
        CCTRACE("initCloudCompare");
        QApplication* app = new QApplication(pyCC_argc, pyCC_argv);
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

        // TODO: load the plugins(duplicated Code from CloudCompare, not in library):
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
    else if ( theDir.dirName() == "sip" || theDir.dirName() == "boostPython" ) // build
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

std::vector<ccHObject*> importFile(const char* filename, CC_SHIFT_MODE mode, double x, double y, double z)
{
    CCTRACE("Opening file: " << filename << " mode: " << mode << " x: " << x << " y: " << y << " z: " << z);
    // TODO adapted code from ccCommandLineParser::importFile
    pyCC* capi = initCloudCompare();
    FileIOFilter::Shared filter = nullptr;
    std::vector<ccHObject*> entities;
    QString fileName(filename);

    //whether Global (coordinate) shift has already been defined
    bool s_firstCoordinatesShiftEnabled = false;
    //global shift (if defined)
    CCVector3d s_firstGlobalShift;

    //default Global Shift handling parameters
    capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG;
    capi->m_loadingParameters.m_coordinatesShiftEnabled = false;
    capi->m_loadingParameters.m_coordinatesShift = CCVector3d(0, 0, 0);

    switch (mode)
    {
    case CC_SHIFT_MODE::AUTO:
        //let CC handle the global shift automatically
        capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG_AUTO_SHIFT;
        break;

    case CC_SHIFT_MODE::FIRST_GLOBAL_SHIFT:
        //use the first encountered global shift value (if any)
        capi->m_loadingParameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG_AUTO_SHIFT;
        capi->m_loadingParameters.m_coordinatesShiftEnabled = s_firstCoordinatesShiftEnabled;
        capi->m_loadingParameters.m_coordinatesShift = s_firstGlobalShift;
        break;

    case CC_SHIFT_MODE::XYZ:
        //set the user defined shift vector as default shift information
        capi->m_loadingParameters.m_coordinatesShiftEnabled = true;
        capi->m_loadingParameters.m_coordinatesShift = CCVector3d(x, y, z);
        break;

    default:
        //nothing to do
        break;
    }

    CC_FILE_ERROR result = CC_FERR_NO_ERROR;
    ccHObject* db = nullptr;
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
        CCTRACE("LoadFromFile returns no entities");
        return entities;
    }

    if (mode != CC_SHIFT_MODE::NO_GLOBAL_SHIFT)
    {
        bool s_firstTime = true;
        if (s_firstTime)
        {
            // remember the first Global Shift parameters used
            s_firstCoordinatesShiftEnabled = capi->m_loadingParameters.m_coordinatesShiftEnabled;
            s_firstGlobalShift = capi->m_loadingParameters.m_coordinatesShift;
            s_firstTime = false;
        }
    }

    std::unordered_set<unsigned> verticesIDs;
    //first look for meshes inside loaded DB (so that we don't consider mesh vertices as clouds!)
    {
        ccHObject::Container meshes;
        size_t count = 0;
        //first look for all REAL meshes (so as to no consider sub-meshes)
        if (db->filterChildren(meshes, true, CC_TYPES::MESH, true) != 0)
        {
            count += meshes.size();
            for (size_t i = 0; i < meshes.size(); ++i)
            {
                ccGenericMesh* mesh = ccHObjectCaster::ToGenericMesh(meshes[i]);
                if (mesh->getParent())
                {
                    mesh->getParent()->detachChild(mesh);
                }

                ccGenericPointCloud* vertices = mesh->getAssociatedCloud();
                if (vertices)
                {
                    verticesIDs.insert(vertices->getUniqueID());
                    CCTRACE("Found one mesh with " << mesh->size() << " faces and " << mesh->getAssociatedCloud()->size() << " vertices: " << mesh->getName().toStdString());
                    capi->m_meshes.emplace_back(mesh, fileName, count == 1 ? -1 : static_cast<int>(i));
                    entities.push_back(mesh);
                }
                else
                {
                    delete mesh;
                    mesh = nullptr;
                    assert(false);
                }
            }
        }

        //then look for the other meshes
        meshes.clear();
        if (db->filterChildren(meshes, true, CC_TYPES::MESH, false) != 0)
        {
            size_t countBefore = count;
            count += meshes.size();
            for (size_t i = 0; i < meshes.size(); ++i)
            {
                ccGenericMesh* mesh = ccHObjectCaster::ToGenericMesh(meshes[i]);
                if (mesh->getParent())
                    mesh->getParent()->detachChild(mesh);

                ccGenericPointCloud* vertices = mesh->getAssociatedCloud();
                if (vertices)
                {
                    verticesIDs.insert(vertices->getUniqueID());
                    CCTRACE("Found one mesh with " << mesh->size() << " faces and " << mesh->getAssociatedCloud()->size() << " vertices: " << mesh->getName().toStdString());
                    capi->m_meshes.emplace_back(mesh, fileName, count == 1 ? -1 : static_cast<int>(countBefore + i));
                    entities.push_back(mesh);
                }
                else
                {
                    delete mesh;
                    mesh = nullptr;
                    assert(false);
                }
            }
        }
    }

    //now look for the remaining clouds inside loaded DB
    {
        ccHObject::Container clouds;
        db->filterChildren(clouds, true, CC_TYPES::POINT_CLOUD);
        size_t count = clouds.size();
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
            capi->m_clouds.emplace_back(pc, fileName, count == 1 ? -1 : static_cast<int>(i));
            entities.push_back(pc);
        }
    }

    delete db;
    db = nullptr;

    return entities;

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

::CC_FILE_ERROR SaveMesh(ccMesh* mesh, const QString& filename)
{
    CCTRACE("saving mesh");
    pyCC* capi = initCloudCompare();
    if ((mesh == nullptr) || filename.isEmpty())
        return ::CC_FERR_BAD_ARGUMENT;
    CCTRACE("mesh: " << mesh->getName().toStdString() << " file: " << filename.toStdString());
    FileIOFilter::SaveParameters parameters;
    parameters.alwaysDisplaySaveDialog = false;
    QFileInfo fi(filename);
    QString ext = fi.suffix();
    CCTRACE("ext: " << ext.toStdString());
    QString fileFilter = "";
    const std::vector<FileIOFilter::Shared>& filters = FileIOFilter::GetFilters();
    for (const auto filter : filters)
    {
        QStringList theFilters = filter->getFileFilters(false);
        QStringList matches = theFilters.filter(ext);
        for ( const auto match : matches)
            CCTRACE("  match: " << match.toStdString());
        if (matches.size())
        {
            fileFilter = matches.first();
            break;
        }
    }

#ifdef PLUGIN_IO_QFBX
    #ifdef Q_OS_LINUX
        if (ext == "fbx")
        {
            CCTRACE("SetDefaultOutputFormat: FBX_ascii");
            FBXFilter::SetDefaultOutputFormat("FBX_ascii");
        }
    #endif
#endif

    CCTRACE("fileFilter: " << fileFilter.toStdString());
    ::CC_FILE_ERROR result = FileIOFilter::SaveToFile(mesh, filename, parameters, fileFilter); //AsciiFilter::GetFileFilter());
    return result;
}

::CC_FILE_ERROR SaveEntities(std::vector<ccHObject*> entities, const QString& filename)
{
    CCTRACE("saving entities");
    pyCC* capi = initCloudCompare();
    if ((entities.size() == 0) || filename.isEmpty())
        return ::CC_FERR_BAD_ARGUMENT;
    CCTRACE("entities.size: " << entities.size() << " file: " << filename.toStdString());
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
    //we'll regroup all selected entities in a temporary group
    ccHObject tempContainer;
    ConvertToGroup(entities, tempContainer, ccHObject::DP_NONE);

    ::CC_FILE_ERROR result = FileIOFilter::SaveToFile(&tempContainer, filename, parameters, fileFilter);
    return result;
}

bool computeCurvature(CurvatureType option, double radius, std::vector<ccHObject*> clouds)
{
    CCTRACE("computeCurvature mode: " << option << " radius: " << radius << " nbClouds: " << clouds.size());
    for (int i = 0; i < clouds.size(); ++i)
    {
        CCTRACE("entity: "<< i << " name: " << clouds[i]->getName().toStdString());
    }
    return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::Curvature, option, radius, clouds);
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

double GetPointCloudRadius(std::vector<ccHObject*> clouds, unsigned knn)
{
    CCTRACE("GetDefaultCloudKernelSize - NbPoints: " << knn << " nbClouds: " << clouds.size());
    double sigma = -1;
    for (ccHObject* cloud : clouds)
    {
        CCTRACE("cloud name: " << cloud->getName().toStdString());
        ccGenericPointCloud* gencloud=dynamic_cast<ccGenericPointCloud*>(cloud);
        if (!gencloud)
            throw std::invalid_argument("entity is not a cloud");
        PointCoordinateType sigmaCloud = pyCC_GetDefaultCloudKernelSize(gencloud, knn);

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
// TODO duplicated code from ccLibAlgorithms::ComputeGeomCharacteristic
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
        PointCoordinateType surface = std::pow(volume, static_cast<PointCoordinateType>(2.0 / 3.0));
        PointCoordinateType surfacePerPoint = surface / cloud->size();
        return std::sqrt(surfacePerPoint * knn);
    }

    return -CCCoreLib::PC_ONE;
}

bool ICP(
    ccHObject* data,
    ccHObject* model,
    ccGLMatrix& transMat,
    double& finalScale,
    double& finalRMS,
    unsigned& finalPointCount,
    double minRMSDecrease,
    unsigned maxIterationCount,
    unsigned randomSamplingLimit,
    bool removeFarthestPoints,
    CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE method,
    bool adjustScale,
    double finalOverlapRatio,
    bool useDataSFAsWeights,
    bool useModelSFAsWeights,
    int transformationFilters,
    int maxThreadCount)
{
    // TODO duplicated code from qCC / ccRegistrationTools::ICP

    // Default number of points sampled on the 'data' mesh (if any)
    const unsigned s_defaultSampledPointsOnDataMesh = 50000;
    // Default temporary registration scalar field
    const char REGISTRATION_DISTS_SF[] = "RegistrationDistances";

    bool restoreColorState = false;
    bool restoreSFState = false;

    CCCoreLib::Garbage<CCCoreLib::GenericIndexedCloudPersist> cloudGarbage;

    //if the 'model' entity is a mesh, we need to sample points on it
    CCCoreLib::GenericIndexedCloudPersist* modelCloud = nullptr;
    ccGenericMesh* modelMesh = nullptr;
    if (model->isKindOf(CC_TYPES::MESH))
    {
        modelMesh = ccHObjectCaster::ToGenericMesh(model);
        modelCloud = modelMesh->getAssociatedCloud();
    }
    else
    {
        modelCloud = ccHObjectCaster::ToGenericPointCloud(model);
    }

    //if the 'data' entity is a mesh, we need to sample points on it
    CCCoreLib::GenericIndexedCloudPersist* dataCloud = nullptr;
    if (data->isKindOf(CC_TYPES::MESH))
    {
        dataCloud = CCCoreLib::MeshSamplingTools::samplePointsOnMesh(ccHObjectCaster::ToGenericMesh(data), s_defaultSampledPointsOnDataMesh);
        if (!dataCloud)
        {
            ccLog::Error("[ICP] Failed to sample points on 'data' mesh!");
            return false;
        }
        cloudGarbage.add(dataCloud);
    }
    else
    {
        dataCloud = ccHObjectCaster::ToGenericPointCloud(data);
    }

    //we activate a temporary scalar field for registration distances computation
    CCCoreLib::ScalarField* dataDisplayedSF = nullptr;
    int oldDataSfIdx = -1;
    int dataSfIdx = -1;

    //if the 'data' entity is a real ccPointCloud, we can even create a proper temporary SF for registration distances
    if (data->isA(CC_TYPES::POINT_CLOUD))
    {
        ccPointCloud* pc = static_cast<ccPointCloud*>(data);
        restoreColorState = pc->colorsShown();
        restoreSFState = pc->sfShown();
        dataDisplayedSF = pc->getCurrentDisplayedScalarField();
        oldDataSfIdx = pc->getCurrentInScalarFieldIndex();
        dataSfIdx = pc->getScalarFieldIndexByName(REGISTRATION_DISTS_SF);
        if (dataSfIdx < 0)
            dataSfIdx = pc->addScalarField(REGISTRATION_DISTS_SF);
        if (dataSfIdx >= 0)
            pc->setCurrentScalarField(dataSfIdx);
        else
        {
            ccLog::Error("[ICP] Couldn't create temporary scalar field! Not enough memory?");
            return false;
        }
    }
    else
    {
        if (!dataCloud->enableScalarField())
        {
            ccLog::Error("[ICP] Couldn't create temporary scalar field! Not enough memory?");
            return false;
        }
    }

    //add a 'safety' margin to input ratio
    static double s_overlapMarginRatio = 0.2;
    finalOverlapRatio = std::max(finalOverlapRatio, 0.01); //1% minimum
    //do we need to reduce the input point cloud (so as to be close
    //to the theoretical number of overlapping points - but not too
    //low so as we are not registered yet ;)
    if (finalOverlapRatio < 1.0 - s_overlapMarginRatio)
    {
        //DGM we can now use 'approximate' distances as SAITO algorithm is exact (but with a coarse resolution)
        //level = 7 if < 1.000.000
        //level = 8 if < 10.000.000
        //level = 9 if > 10.000.000
        int gridLevel = static_cast<int>(std::floor(std::log10(static_cast<long double>(std::max(dataCloud->size(), modelCloud->size()))))) + 2;
            gridLevel = std::min(std::max(gridLevel, 7), 9);
        int result = -1;
        if (modelMesh)
        {
            CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams c2mParams;
            c2mParams.octreeLevel = gridLevel;
            c2mParams.maxSearchDist = 0;
            c2mParams.useDistanceMap = true;
            c2mParams.signedDistances = false;
            c2mParams.flipNormals = false;
            c2mParams.multiThread = false;
            result = CCCoreLib::DistanceComputationTools::computeCloud2MeshDistances(dataCloud, modelMesh, c2mParams);
        }
        else
        {
            result = CCCoreLib::DistanceComputationTools::computeApproxCloud2CloudDistance( dataCloud,
                                                                                        modelCloud,
                                                                                        gridLevel,
                                                                                        -1);
        }

        if (result < 0)
        {
            ccLog::Error("Failed to determine the max (overlap) distance (not enough memory?)");
            return false;
        }

        //determine the max distance that (roughly) corresponds to the input overlap ratio
        ScalarType maxSearchDist = 0;
        {
            unsigned count = dataCloud->size();
            std::vector<ScalarType> distances;
            try
            {
                distances.resize(count);
            }
            catch (const std::bad_alloc&)
            {
                ccLog::Error("Not enough memory!");
                return false;
            }
            for (unsigned i=0; i<count; ++i)
            {
                distances[i] = dataCloud->getPointScalarValue(i);
            }

            ParallelSort(distances.begin(), distances.end());

            //now look for the max value at 'finalOverlapRatio+margin' percent
            maxSearchDist = distances[static_cast<size_t>(std::max(1.0,count*(finalOverlapRatio+s_overlapMarginRatio)))-1];
        }

        //evntually select the points with distance below 'maxSearchDist'
        //(should roughly correspond to 'finalOverlapRatio + margin' percent)
        {
            CCCoreLib::ReferenceCloud* refCloud = new CCCoreLib::ReferenceCloud(dataCloud);
            cloudGarbage.add(refCloud);
            unsigned countBefore = dataCloud->size();
            unsigned baseIncrement = static_cast<unsigned>(std::max(100.0,countBefore*finalOverlapRatio*0.05));
            for (unsigned i=0; i<countBefore; ++i)
            {
                if (dataCloud->getPointScalarValue(i) <= maxSearchDist)
                {
                    if (    refCloud->size() == refCloud->capacity()
                        &&  !refCloud->reserve(refCloud->size() + baseIncrement) )
                    {
                        ccLog::Error("Not enough memory!");
                        return false;
                    }
                    refCloud->addPointIndex(i);
                }
            }
            refCloud->resize(refCloud->size());
            dataCloud = refCloud;

            unsigned countAfter = dataCloud->size();
            double keptRatio = static_cast<double>(countAfter)/countBefore;
            ccLog::Print(QString("[ICP][Partial overlap] Selecting %1 points out of %2 (%3%) for registration").arg(countAfter).arg(countBefore).arg(static_cast<int>(100*keptRatio)));

            //update the relative 'final overlap' ratio
            finalOverlapRatio /= keptRatio;
        }
    }

    //weights
    CCCoreLib::ScalarField* modelWeights = nullptr;
    CCCoreLib::ScalarField* dataWeights = nullptr;
    {
        if (!modelMesh && useModelSFAsWeights)
        {
            if (modelCloud == dynamic_cast<CCCoreLib::GenericIndexedCloudPersist*>(model) && model->isA(CC_TYPES::POINT_CLOUD))
            {
                ccPointCloud* pc = static_cast<ccPointCloud*>(model);
                modelWeights = pc->getCurrentDisplayedScalarField();
                if (!modelWeights)
                    ccLog::Warning("[ICP] 'useDataSFAsWeights' is true but model has no displayed scalar field!");
            }
            else
            {
                ccLog::Warning("[ICP] 'useDataSFAsWeights' is true but only point cloud scalar fields can be used as weights!");
            }
        }

        if (useDataSFAsWeights)
        {
            if (!dataDisplayedSF)
            {
                if (dataCloud == ccHObjectCaster::ToPointCloud(data))
                    ccLog::Warning("[ICP] 'useDataSFAsWeights' is true but data has no displayed scalar field!");
                else
                    ccLog::Warning("[ICP] 'useDataSFAsWeights' is true but only point cloud scalar fields can be used as weights!");
            }
            else
            {
                dataWeights = dataDisplayedSF;
            }
        }
    }

    CCCoreLib::ICPRegistrationTools::RESULT_TYPE result;
    CCCoreLib::PointProjectionTools::Transformation transform;
    CCCoreLib::ICPRegistrationTools::Parameters params;
    {
        params.convType = method;
        params.minRMSDecrease = minRMSDecrease;
        params.nbMaxIterations = maxIterationCount;
        params.adjustScale = adjustScale;
        params.filterOutFarthestPoints = removeFarthestPoints;
        params.samplingLimit = randomSamplingLimit;
        params.finalOverlapRatio = finalOverlapRatio;
        params.modelWeights = modelWeights;
        params.dataWeights = dataWeights;
        params.transformationFilters = transformationFilters;
        params.maxThreadCount = maxThreadCount;
    }

    result = CCCoreLib::ICPRegistrationTools::Register( modelCloud,
                                                    modelMesh,
                                                    dataCloud,
                                                    params,
                                                    transform,
                                                    finalRMS,
                                                    finalPointCount);

    if (result >= CCCoreLib::ICPRegistrationTools::ICP_ERROR)
    {
        ccLog::Error("Registration failed: an error occurred (code %i)",result);
    }
    else if (result == CCCoreLib::ICPRegistrationTools::ICP_APPLY_TRANSFO)
    {
        transMat = FromCCLibMatrix<double, float>(transform.R, transform.T, transform.s);
        finalScale = transform.s;
    }

    //remove temporary SF (if any)
    if (dataSfIdx >= 0)
    {
        assert(data->isA(CC_TYPES::POINT_CLOUD));
        ccPointCloud* pc = static_cast<ccPointCloud*>(data);
        pc->setCurrentScalarField(oldDataSfIdx);
        pc->deleteScalarField(dataSfIdx);
        pc->showColors(restoreColorState);
        pc->showSF(restoreSFState);
    }

    return (result < CCCoreLib::ICPRegistrationTools::ICP_ERROR);
}

bool computeNormals(std::vector<ccHObject*> selectedEntities,
                    CCCoreLib::LOCAL_MODEL_TYPES model,
                    bool useScanGridsForComputation,
                    PointCoordinateType defaultRadius,
                    double minGridAngle_deg,
                    bool orientNormals,
                    bool useScanGridsForOrientation,
                    bool useSensorsForOrientation,
                    ccNormalVectors::Orientation preferredOrientation,
                    bool orientNormalsMST,
                    int mstNeighbors,
                    bool computePerVertexNormals)
{
    if (selectedEntities.empty())
    {
        ccLog::Error(QObject::tr("Select at least one point cloud"));
        CCTRACE("Select at least one point cloud");
        return false;
    }

    static const QString s_NormalScaleKey("Normal scale");

    //look for clouds and meshes
    std::vector<ccPointCloud*> clouds;
    bool withScanGrid = false;
    bool withSensor = false;
    std::vector<ccMesh*> meshes;

    try
    {
        for (const auto entity : selectedEntities)
        {
            if (entity->isA(CC_TYPES::POINT_CLOUD))
            {
                ccPointCloud* cloud = static_cast<ccPointCloud*>(entity);
                clouds.push_back(cloud);

                if (!withScanGrid)
                {
                    withScanGrid = (cloud->gridCount() > 0);
                }

                if (!withSensor)
                {
                    for (unsigned i = 0; i < cloud->getChildrenNumber(); ++i)
                    {
                        if (cloud->hasSensor())
                        {
                            withSensor = true;
                            break; //no need to look anyfurther
                        }
                    }
                }

                //does the cloud have a former radius value saved as meta-data?
                if (cloud->hasMetaData(s_NormalScaleKey))
                {
                    bool ok = false;
                    double formerRadius = cloud->getMetaData(s_NormalScaleKey).toDouble(&ok);
                    if (ok)
                    {
                        //remember the largest radius
                        defaultRadius = std::max(defaultRadius, static_cast<PointCoordinateType>(formerRadius));
                    }
                    else
                    {
                        assert(false);
                    }
                }

                if (defaultRadius == 0.0)
                {
                    //default radius
                    defaultRadius = ccNormalVectors::GuessNaiveRadius(cloud);
                }
            }
            else if (entity->isKindOf(CC_TYPES::MESH))
            {
                if (entity->isA(CC_TYPES::MESH))
                {
                    ccMesh* mesh = ccHObjectCaster::ToMesh(entity);
                    meshes.push_back(mesh);
                }
                else
                {
                    ccLog::Error(QObject::tr("Can't compute normals on sub-meshes! Select the parent mesh instead"));
                    CCTRACE("Can't compute normals on sub-meshes! Select the parent mesh instead");
                    return false;
                }
            }
        }
    } catch (const std::bad_alloc&)
    {
        ccLog::Error(QObject::tr("Not enough memory!"));
        CCTRACE("Not enough memory!");
        return false;
    }

    //compute normals for each selected cloud
    if (!clouds.empty())
    {
//        static CCCoreLib::LOCAL_MODEL_TYPES s_lastModelType = CCCoreLib::LS;
//        static ccNormalVectors::Orientation s_lastNormalOrientation = ccNormalVectors::UNDEFINED;
//        static int s_lastMSTNeighborCount = 6;
//        static double s_lastMinGridAngle_deg = 1.0;

//        ccNormalComputationDlg ncDlg(withScanGrid, withSensor, nullptr);
//        ncDlg.setLocalModel(s_lastModelType);
//        ncDlg.setRadius(defaultRadius);
//        ncDlg.setPreferredOrientation(s_lastNormalOrientation);
//        ncDlg.setMSTNeighborCount(s_lastMSTNeighborCount);
//        ncDlg.setMinGridAngle_deg(s_lastMinGridAngle_deg);
//        if (clouds.size() == 1)
//        {
//            ncDlg.setCloud(clouds.front());
//        }
//
//        if (!ncDlg.exec())
//            return false;

        //normals computation
        bool useGridStructure = withScanGrid && useScanGridsForComputation;
//        double minGridAngle_deg = s_lastMinGridAngle_deg = ncDlg.getMinGridAngle_deg();

        //normals orientation
//        bool orientNormals = ncDlg.orientNormals();
        bool orientNormalsWithGrids = withScanGrid && useScanGridsForOrientation;
        bool orientNormalsWithSensors = withSensor && useSensorsForOrientation;
//        ccNormalVectors::Orientation preferredOrientation = s_lastNormalOrientation = ncDlg.getPreferredOrientation();
//        bool orientNormalsMST = ncDlg.useMSTOrientation();
//        int mstNeighbors = s_lastMSTNeighborCount = ncDlg.getMSTNeighborCount();

//        ccProgressDialog pDlg(true, parent);
//        pDlg.setAutoClose(false);

        size_t errors = 0;

        for (auto cloud : clouds)
        {
            Q_ASSERT(cloud != nullptr);

            bool result = false;
            bool normalsAlreadyOriented = false;

            if (useGridStructure && cloud->gridCount())
            {
                //compute normals with the associated scan grid(s)
                normalsAlreadyOriented = true;
                result = cloud->computeNormalsWithGrids(minGridAngle_deg, nullptr);
            }
            else
            {
                //compute normals with the octree
                normalsAlreadyOriented = orientNormals && (preferredOrientation != ccNormalVectors::UNDEFINED);
                result = cloud->computeNormalsWithOctree(
                        model, orientNormals ? preferredOrientation : ccNormalVectors::UNDEFINED, defaultRadius, nullptr);
                if (result)
                {
                    //save the normal computation radius as meta-data
                    cloud->setMetaData(s_NormalScaleKey, defaultRadius);
                }
            }

            //do we need to orient the normals? (this may have been already done if 'orientNormalsForThisCloud' is true)
            if (result && orientNormals && !normalsAlreadyOriented)
            {
                if (cloud->gridCount() && orientNormalsWithGrids)
                {
                    //we can still use the grid structure(s) to orient the normals!
                    result = cloud->orientNormalsWithGrids();
                }
                else if (cloud->hasSensor() && orientNormalsWithSensors)
                {
                    result = false;

                    // RJ: TODO: the issue here is that a cloud can have multiple sensors.
                    // As the association to sensor is not explicit in CC, given a cloud
                    // some points can belong to one sensor and some others can belongs to others sensors.
                    // so it's why here grid orientation has precedence over sensor orientation because in this
                    // case association is more explicit.
                    // Here we take the first valid viewpoint for now even if it's not a really good...
                    CCVector3 sensorPosition;
                    for (size_t i = 0; i < cloud->getChildrenNumber(); ++i)
                    {
                        ccHObject* child = cloud->getChild(static_cast<unsigned>(i));
                        if (child && child->isKindOf(CC_TYPES::SENSOR))
                        {
                            ccSensor* sensor = ccHObjectCaster::ToSensor(child);
                            if (sensor->getActiveAbsoluteCenter(sensorPosition))
                            {
                                result = cloud->orientNormalsTowardViewPoint(sensorPosition, nullptr);
                                break;
                            }
                        }
                    }
                }
                else if (orientNormalsMST)
                {
                    //use Minimum Spanning Tree to resolve normals direction
                    result = cloud->orientNormalsWithMST(mstNeighbors, nullptr);
                }
            }

            if (!result)
            {
                ++errors;
            }

            cloud->prepareDisplayForRefresh();
        }

        if (errors != 0)
        {
            if (errors < clouds.size())
            {
                ccLog::Error(QObject::tr("Failed to compute or orient the normals on some clouds! (see console)"));
                CCTRACE("Failed to compute or orient the normals on some clouds! (see console)");
            }
            else
            {
                ccLog::Error(QObject::tr("Failed to compute or orient the normals! (see console)"));
                CCTRACE("Failed to compute or orient the normals! (see console)");
            }
        }
    }

    //compute normals for each selected mesh
    if (!meshes.empty())
    {
//        QMessageBox question(QMessageBox::Question, QObject::tr("Mesh normals"),
//                             QObject::tr("Compute per-vertex normals (smooth) or per-triangle (faceted)?"),
//                             QMessageBox::NoButton, parent);
//
//        QPushButton* perVertexButton = question.addButton(QObject::tr("Per-vertex"), QMessageBox::YesRole);
//        QPushButton* perTriangleButton = question.addButton(QObject::tr("Per-triangle"), QMessageBox::NoRole);
//
//        question.exec();
//
//        bool computePerVertexNormals = (question.clickedButton() == perVertexButton);

        for (auto mesh : meshes)
        {
            Q_ASSERT(mesh != nullptr);

//            //we remove temporarily the mesh as its normals may be removed (and they can be a child object)
//            ccMainAppInterface* instance = dynamic_cast<ccMainAppInterface*>(parent);
//            ccMainAppInterface::ccHObjectContext objContext;
//            if (instance)
//                objContext = instance->removeObjectTemporarilyFromDBTree(mesh);
//            mesh->clearTriNormals();
//            mesh->showNormals(false);
            bool result = mesh->computeNormals(computePerVertexNormals);
//            if (instance)
//                instance->putObjectBackIntoDBTree(mesh, objContext);

            if (!result)
            {
                ccLog::Error(QObject::tr("Failed to compute normals on mesh '%1'").arg(mesh->getName()));
                CCTRACE("Failed to compute normals on mesh " << mesh->getName().toStdString());
                continue;
            }
            mesh->prepareDisplayForRefresh_recursive();
        }
    }

    return true;
}


//! TODO: Copied/adpated from qCC/ccVolumeCalcTool::ComputeVolume
bool ComputeVolume_(    ccRasterGrid& grid,
                        ccGenericPointCloud* ground,
                        ccGenericPointCloud* ceil,
                        const ccBBox& gridBox,
                        unsigned char vertDim,
                        double gridStep,
                        unsigned gridWidth,
                        unsigned gridHeight,
                        ccRasterGrid::ProjectionType projectionType,
                        ccRasterGrid::EmptyCellFillOption groundEmptyCellFillStrategy,
                        ccRasterGrid::EmptyCellFillOption ceilEmptyCellFillStrategy,
                        ReportInfoVol* reportInfo,
                        double groundHeight = std::numeric_limits<double>::quiet_NaN(),
                        double ceilHeight = std::numeric_limits<double>::quiet_NaN())
{
    if (    gridStep <= 1.0e-8
        ||  gridWidth == 0
        ||  gridHeight == 0
        ||  vertDim > 2)
    {
        ccLog::Warning("[Volume] Invalid input parameters");
        CCTRACE("[Volume] Invalid input parameters");
        return false;
    }

    if (!ground && !ceil)
    {
        ccLog::Warning("[Volume] No valid input cloud");
        CCTRACE("[Volume] No valid input cloud");
        return false;
    }

    if (!gridBox.isValid())
    {
        ccLog::Warning("[Volume] Invalid bounding-box");
        CCTRACE("[Volume] Invalid bounding-box");
        return false;
    }

    //grid size
    unsigned gridTotalSize = gridWidth * gridHeight;
//    if (gridTotalSize == 1)
//    {
//        if (parentWidget && QMessageBox::question(parentWidget, "Unexpected grid size", "The generated grid will only have 1 cell! Do you want to proceed anyway?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
//            return false;
//    }
//    else if (gridTotalSize > 10000000)
//    {
//        if (parentWidget && QMessageBox::question(parentWidget, "Big grid size", "The generated grid will have more than 10.000.000 cells! Do you want to proceed anyway?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
//            return false;
//    }

    //memory allocation
    CCVector3d minCorner = gridBox.minCorner();
    if (!grid.init(gridWidth, gridHeight, gridStep, minCorner))
    {
        //not enough memory
        //return SendError("Not enough memory", parentWidget);
        ccLog::Error(QObject::tr("Not enough memory!"));
        CCTRACE("Not enough memory!");
        return false;
    }

    //progress dialog
    QScopedPointer<ccProgressDialog> pDlg(nullptr);
//    if (parentWidget)
//    {
//        pDlg.reset(new ccProgressDialog(true, parentWidget));
//    }

    ccRasterGrid groundRaster;
    if (ground)
    {
        if (!groundRaster.init(gridWidth, gridHeight, gridStep, minCorner))
        {
            //not enough memory
            //return SendError("Not enough memory", parentWidget);
            ccLog::Error(QObject::tr("Not enough memory!"));
            CCTRACE("Not enough memory!");
            return false;
        }

        if (groundRaster.fillWith(  ground,
                                    vertDim,
                                    projectionType,
                                    groundEmptyCellFillStrategy == ccRasterGrid::INTERPOLATE,
                                    ccRasterGrid::INVALID_PROJECTION_TYPE,
                                    pDlg.data()))
        {
            groundRaster.fillEmptyCells(groundEmptyCellFillStrategy, groundHeight);
            ccLog::Print(QString("[Volume] Ground raster grid: size: %1 x %2 / heights: [%3 ; %4]").arg(groundRaster.width).arg(groundRaster.height).arg(groundRaster.minHeight).arg(groundRaster.maxHeight));
        }
        else
        {
            return false;
        }
    }

    //ceil
    ccRasterGrid ceilRaster;
    if (ceil)
    {
        if (!ceilRaster.init(gridWidth, gridHeight, gridStep, minCorner))
        {
            //not enough memory
            //return SendError("Not enough memory", parentWidget);
            ccLog::Error(QObject::tr("Not enough memory!"));
            CCTRACE("Not enough memory!");
            return false;
        }

        if (ceilRaster.fillWith(ceil,
                                vertDim,
                                projectionType,
                                ceilEmptyCellFillStrategy == ccRasterGrid::INTERPOLATE,
                                ccRasterGrid::INVALID_PROJECTION_TYPE,
                                pDlg.data()))
        {
            ceilRaster.fillEmptyCells(ceilEmptyCellFillStrategy, ceilHeight);
            ccLog::Print(QString("[Volume] Ceil raster grid: size: %1 x %2 / heights: [%3 ; %4]").arg(ceilRaster.width).arg(ceilRaster.height).arg(ceilRaster.minHeight).arg(ceilRaster.maxHeight));
        }
        else
        {
            return false;
        }
    }

    //update grid and compute volume
    {
//        if (pDlg)
//        {
//            pDlg->setMethodTitle(QObject::tr("Volume computation"));
//            pDlg->setInfo(QObject::tr("Cells: %1 x %2").arg(grid.width).arg(grid.height));
//            pDlg->start();
//            pDlg->show();
//            QCoreApplication::processEvents();
//        }
//        CCCoreLib::NormalizedProgress nProgress(pDlg.data(), grid.width * grid.height);

        size_t ceilNonMatchingCount = 0;
        size_t groundNonMatchingCount = 0;
        size_t cellCount = 0;

        //at least one of the grid is based on a cloud
        grid.nonEmptyCellCount = 0;
        for (unsigned i = 0; i < grid.height; ++i)
        {
            for (unsigned j = 0; j < grid.width; ++j)
            {
                ccRasterCell& cell = grid.rows[i][j];

                bool validGround = true;
                cell.minHeight = groundHeight;
                if (ground)
                {
                    cell.minHeight = groundRaster.rows[i][j].h;
                    validGround = std::isfinite(cell.minHeight);
                }

                bool validCeil = true;
                cell.maxHeight = ceilHeight;
                if (ceil)
                {
                    cell.maxHeight = ceilRaster.rows[i][j].h;
                    validCeil = std::isfinite(cell.maxHeight);
                }

                if (validGround && validCeil)
                {
                    cell.h = cell.maxHeight - cell.minHeight;
                    cell.nbPoints = 1;

                    reportInfo->volume += cell.h;
                    if (cell.h < 0)
                    {
                        reportInfo->removedVolume -= cell.h;
                    }
                    else if (cell.h > 0)
                    {
                        reportInfo->addedVolume += cell.h;
                    }
                    reportInfo->surface += 1.0;
                    ++grid.nonEmptyCellCount; // matching count
                    ++cellCount;
                }
                else
                {
                    if (validGround)
                    {
                        ++cellCount;
                        ++groundNonMatchingCount;
                    }
                    else if (validCeil)
                    {
                        ++cellCount;
                        ++ceilNonMatchingCount;
                    }
                    cell.h = std::numeric_limits<double>::quiet_NaN();
                    cell.nbPoints = 0;
                }

                cell.avgHeight = (groundHeight + ceilHeight) / 2;
                cell.stdDevHeight = 0;

//                if (pDlg && !nProgress.oneStep())
//                {
//                    ccLog::Warning("[Volume] Process cancelled by the user");
//                    return false;
//                }
            }
        }
        grid.validCellCount = grid.nonEmptyCellCount;

        //count the average number of valid neighbors
        {
            size_t validNeighborsCount = 0;
            size_t count = 0;
            for (unsigned i = 1; i < grid.height - 1; ++i)
            {
                for (unsigned j = 1; j < grid.width - 1; ++j)
                {
                    ccRasterCell& cell = grid.rows[i][j];
                    if (cell.h == cell.h)
                    {
                        for (unsigned k = i - 1; k <= i + 1; ++k)
                        {
                            for (unsigned l = j - 1; l <= j + 1; ++l)
                            {
                                if (k != i || l != j)
                                {
                                    ccRasterCell& otherCell = grid.rows[k][l];
                                    if (std::isfinite(otherCell.h))
                                    {
                                        ++validNeighborsCount;
                                    }
                                }
                            }
                        }

                        ++count;
                    }
                }
            }

            if (count)
            {
                reportInfo->averageNeighborsPerCell = static_cast<double>(validNeighborsCount) / count;
            }
        }

        reportInfo->matchingPercent = static_cast<float>(grid.validCellCount * 100) / cellCount;
        reportInfo->groundNonMatchingPercent = static_cast<float>(groundNonMatchingCount * 100) / cellCount;
        reportInfo->ceilNonMatchingPercent = static_cast<float>(ceilNonMatchingCount * 100) / cellCount;
        float cellArea = static_cast<float>(grid.gridStep * grid.gridStep);
        reportInfo->volume *= cellArea;
        reportInfo->addedVolume *= cellArea;
        reportInfo->removedVolume *= cellArea;
        reportInfo->surface *= cellArea;
    }

    grid.setValid(true);

    return true;
}

bool ComputeVolume25D(  ReportInfoVol* reportInfo,
                        ccGenericPointCloud* ground,
                        ccGenericPointCloud* ceil,
                        unsigned char vertDim,
                        double gridStep,
                        double groundHeight,
                        double ceilHeight)
{
    ccRasterGrid grid;
    ccBBox gridBBox = ceil ? ceil->getOwnBB() : ccBBox();
    if (ground)
    {
        gridBBox += ground->getOwnBB();
    }
    //compute the grid size
    unsigned gridWidth = 0;
    unsigned gridHeight = 0;
    if (!ccRasterGrid::ComputeGridSize(vertDim, gridBBox, gridStep, gridWidth, gridHeight))
    {
        ccLog::Error(QObject::tr("Failed to compute the grid dimensions (check input cloud(s) bounding-box)"));
        CCTRACE("Failed to compute the grid dimensions (check input cloud(s) bounding-box)");
    }

    bool ret = ComputeVolume_( grid,
                               ground,
                               ceil,
                               gridBBox,
                               vertDim,
                               gridStep,
                               gridWidth,
                               gridHeight,
                               ccRasterGrid::PROJ_AVERAGE_VALUE,
                               ccRasterGrid::LEAVE_EMPTY,
                               ccRasterGrid::LEAVE_EMPTY,
                               reportInfo,
                               groundHeight,
                               ceilHeight);
    return ret;
}

ReportInfoVol::ReportInfoVol() :
    volume(0), addedVolume(0), removedVolume(0), surface(0),
    matchingPercent(0), ceilNonMatchingPercent(0), groundNonMatchingPercent(0),
    averageNeighborsPerCell(0)
{
}

