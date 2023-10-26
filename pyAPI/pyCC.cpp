//##########################################################################
//#                                                                        #
//#                              CloudComPy                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; either version 3 of the License, or     #
//#  any later version.                                                    #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#  You should have received a copy of the GNU General Public License     #
//#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
//#                                                                        #
//#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
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
#include <ccFacet.h>
#include <ccGLMatrix.h>
#include <ccRasterGrid.h>
#include <ccRasterizeTool.h>
#include <ccClipBox.h>
#include <ManualSegmentationTools.h>
#include <SimpleMesh.h>
#include <ccMaterialSet.h>
#include <ccHObject.h>
#include <AsciiFilter.h>
#include <pyccTrace.h>

#include <ccContourLinesGenerator.h>
//libs/CCPluginStub
#include "ccPluginInterface.h"

//libs/CCAppCommon
#include "ccPluginManager.h"

//qCC
#include "ccCommon.h"

//system
#include <unordered_set>
#include <cmath>
#include <string.h>
#include <vector>
#include <exception>
#include <set>
#include <sstream>

//Qt
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>
#include <QObject>
#include <QMessageBox>

#include <viewerPy.h>
#include <viewerPyApplication.h>
#include "optdefines.h"

#ifdef PLUGIN_IO_QFBX
#include <FBXFilter.h>
#endif

#ifdef PLUGIN_IO_QLASFWF
#include <LASFWFFilter.h>
#endif

#ifdef CC_GDAL_SUPPORT
//GDAL
#include <cpl_string.h>
#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_api.h>
#endif

#ifdef PLUGIN_IO_QDRACO
bool pyccPlugins::_isPluginDraco = true;
#else
bool pyccPlugins::_isPluginDraco = false;
#endif

#ifdef PLUGIN_IO_QFBX
bool pyccPlugins::_isPluginFbx = true;
#else
bool pyccPlugins::_isPluginFbx = false;
#endif

#ifdef PLUGIN_IO_QLAS_FWF
bool pyccPlugins::_isPluginLasFwf = true;
#else
bool pyccPlugins::_isPluginLasFwf = false;
#endif

#ifdef PLUGIN_STANDARD_QM3C2
bool pyccPlugins::_isPluginM3C2 = true;
#else
bool pyccPlugins::_isPluginM3C2 = false;
#endif

#ifdef PLUGIN_STANDARD_QPCL
bool pyccPlugins::_isPluginPCL = true;
#else
bool pyccPlugins::_isPluginPCL = false;
#endif

#ifdef PLUGIN_STANDARD_QPCV
bool pyccPlugins::_isPluginPCV = true;
#else
bool pyccPlugins::_isPluginPCV = false;
#endif

#ifdef PLUGIN_STANDARD_QCSF
bool pyccPlugins::_isPluginCSF = true;
#else
bool pyccPlugins::_isPluginCSF = false;
#endif

#ifdef PLUGIN_STANDARD_QRANSAC_SD
bool pyccPlugins::_isPluginRANSAC_SD = true;
#else
bool pyccPlugins::_isPluginRANSAC_SD = false;
#endif

#ifdef PLUGIN_STANDARD_QHPR
bool pyccPlugins::_isPluginHPR = true;
#else
bool pyccPlugins::_isPluginHPR = false;
#endif

#ifdef PLUGIN_STANDARD_QMESH_BOOLEAN
bool pyccPlugins::_isPluginMeshBoolean = true;
#else
bool pyccPlugins::_isPluginMeshBoolean = false;
#endif

#ifdef PLUGIN_STANDARD_QSRA
bool pyccPlugins::_isPluginSRA = true;
#else
bool pyccPlugins::_isPluginSRA = false;
#endif

#ifdef PLUGIN_STANDARD_QCANUPO
bool pyccPlugins::_isPluginCanupo = true;
#else
bool pyccPlugins::_isPluginCanupo = false;
#endif

#ifdef PLUGIN_STANDARD_QPOISSON_RECON
bool pyccPlugins::_isPluginPoissonRecon = true;
#else
bool pyccPlugins::_isPluginPoissonRecon = false;
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
        _coordinatesShiftEnabled = &m_coordinatesShiftEnabled;
        _coordinatesShift = &m_coordinatesShift;
        extraData.setPattern(".+"); // one or more character
    }

    bool m_coordinatesShiftEnabled;
    CCVector3d m_coordinatesShift;
    QRegExp m_extraData;
};

//! internal attributes (cloned from plugins/ccCommandLineInterface.h)
struct pyCC
{
    //! Currently opened point clouds and their filename
    std::vector<CLCloudDesc> m_clouds;

    //! Currently opened meshes and their filename
    std::vector<CLMeshDesc> m_meshes;

    //! Currently opened facets and their filename
    //std::vector<CLEntityDesc> m_facets;

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
    	//viewerPyApplication::InitOpenGL();
        QDir appDir = initCC::moduleDir;
        appDir.cdUp();
        appDir.cdUp();
        QString appliDir = appDir.absolutePath() + "/bin";
    	viewerPyApplication* app = new viewerPyApplication(pyCC_argc, pyCC_argv, true, appliDir);
    	//QApplication* app = new QApplication(pyCC_argc, pyCC_argv);
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
        ccPluginManager::Get().setPaths(s_pyCCInternals->m_PluginPaths);
        for (int i = 0; i < s_pyCCInternals->m_PluginPaths.size(); ++i)
            CCTRACE("pluginPath: " << s_pyCCInternals->m_PluginPaths.at(i).toStdString());
        ccPluginManager::Get().loadPlugins();
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

    capi->m_PluginPaths << (bundleDir.absolutePath() + "/CloudCompare/CloudCompare.app/Contents/PlugIns/ccPlugins");

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
    capi->m_ShaderPath = (bundleDir.absolutePath() + "/CloudCompare/CloudCompare.app/Contents/Shaders");
    capi->m_TranslationPath = (bundleDir.absolutePath() + "/CloudCompare/CloudCompare.app/Contents/translations");
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
    else if ( theDir.dirName() == "sip" || theDir.dirName() == "pybind11" ) // build
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

std::vector<QString>* exploreDB(ccHObject* db, std::vector<QString>* dbtext, int level)
{
    std::stringstream levstr;
    levstr << level;
    std::string lstr = levstr.str();
    int nbChildren = db->getChildrenNumber();

    for (int i=0; i<nbChildren; i++)
    {
        ccHObject* child = db->getChild(i);
        QString item ="";
        item += lstr.c_str();
        item += " ";
        item += child->getName();
        dbtext->push_back(item);
        dbtext = exploreDB(child, dbtext, level+1);
    }
    return dbtext;
}

std::vector<ccHObject*> importFile(const char* filename, CC_SHIFT_MODE mode,
                                   double x, double y, double z,
                                   const QString& extraData, std::vector<QString>* structure)
{
    CCTRACE("Opening file: " << filename << " mode: " << mode
            << " x: " << x << " y: " << y << " z: " << z << " extraData: " << extraData.toStdString());
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

    if (!extraData.isEmpty())
    {
        capi->m_loadingParameters.extraData.setPattern(extraData);
    }

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
    if (structure)
    {
        structure = exploreDB(db, structure, 0);
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
    // look for facets inside loaded DB
    {
        ccHObject::Container facets;
        db->filterChildren(facets, true, CC_TYPES::FACET);
        size_t count = facets.size();
        CCTRACE("number of facets: " << count);
        for (size_t i = 0; i < count; ++i)
        {
            ccFacet* pc = static_cast<ccFacet*>(facets[i]);
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
            CCTRACE("Found one facet");
            //capi->m_facets.emplace_back(pc, filename, count == 1 ? -1 : static_cast<int>(i));
            entities.push_back(pc);
        }
    }
    // look for polylines inside loaded DB
    {
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

            //if the cloud is a set of vertices, we ignore it!
            if (verticesIDs.find(pc->getUniqueID()) != verticesIDs.end())
            {
                capi->m_orphans.addChild(pc);
                continue;
            }
            CCTRACE("Found one poly with " << pc->size() << " points");
            capi->m_polys.emplace_back(pc, filename, count == 1 ? -1 : static_cast<int>(i));
            entities.push_back(pc);
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

::CC_FILE_ERROR SavePointCloud(ccPointCloud* cloud, const QString& filename, const QString& version, int pointFormat)
{
    CCTRACE("saving cloud, version " << version.toStdString());
    pyCC* capi = initCloudCompare();
    if ((cloud == nullptr) || filename.isEmpty())
        return ::CC_FERR_BAD_ARGUMENT;
    CCTRACE("cloud: " << cloud->getName().toStdString() << " file: " << filename.toStdString());
    FileIOFilter::SaveParameters parameters;
    parameters.alwaysDisplaySaveDialog = false;
    QFileInfo fi(filename);
    QString ext = fi.suffix();

    if (!version.isEmpty())
    {
        if (ext == "las" || ext == "laz")
        {
            if  (version =="1.2")
                parameters.minorVersion = 2;
            if  (version =="1.3")
                parameters.minorVersion = 3;
            if  (version =="1.4")
                parameters.minorVersion = 4;
            CCTRACE("parameters.minorVersion: " << parameters.minorVersion);
        }
    }
    if (pointFormat != -1)
    {
        parameters.pointFormat = pointFormat;
        CCTRACE("parameters.pointFormat: " << parameters.pointFormat);
    }

    QString fileFilter = "";
    const std::vector<FileIOFilter::Shared>& filters = FileIOFilter::GetFilters();
    for (const auto filter : filters)
    {
        //CCTRACE("filter:" << filter->getDefaultExtension().toStdString());
        QStringList theFilters = filter->getFileFilters(false);
//        for(auto filt : theFilters)
//            CCTRACE("filter: " << filt.toStdString());
        QStringList matches = theFilters.filter(ext);
        if (matches.size())
        {
//            for (auto filt : matches)
//                CCTRACE("filter: " << filt.toStdString());
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

bool computeFeature(CCCoreLib::Neighbourhood::GeomFeature option, double radius, std::vector<ccHObject*> clouds)
{
	return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::Feature, option, radius, clouds);
}

bool computeLocalDensity(CCCoreLib::GeometricalAnalysisTools::Density option, double radius, std::vector<ccHObject*> clouds)
{
	return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::LocalDensity, option, radius, clouds);
}

bool computeApproxLocalDensity(CCCoreLib::GeometricalAnalysisTools::Density option, double radius, std::vector<ccHObject*> clouds)
{
	return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::ApproxLocalDensity, option, radius, clouds);
}

bool computeRoughnessPy(double radius, std::vector<ccHObject*> clouds, CCVector3 roughnessUpDir)
{
    if (roughnessUpDir.norm2() == 0)
    {
        CCTRACE("computeRoughness without up direction");
        return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::Roughness, 0, radius, clouds, nullptr);
    }
    else
    {
        CCTRACE("computeRoughness with up direction");
        return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::Roughness, 0, radius, clouds, &roughnessUpDir);
    }
}

bool computeMomentOrder1(double radius, std::vector<ccHObject*> clouds)
{
	return pyCC_ComputeGeomCharacteristic(CCCoreLib::GeometricalAnalysisTools::MomentOrder1, 0, radius, clouds);
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
    ccHObject::Container& entities,
    const CCVector3* roughnessUpDir)
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
                    CCTRACE("Failed to create scalar field on cloud (not enough memory?): " << pc->getName().toStdString());
                    continue;
                }
            }

            ccOctree::Shared octree = cloud->getOctree();
            if (!octree)
            {
                octree = cloud->computeOctree(nullptr);
                if (!octree)
                {
                    CCTRACE("Couldn't compute octree for cloud " << cloud->getName().toStdString());
                    break;
                }
            }

            CCCoreLib::GeometricalAnalysisTools::ErrorCode result = CCCoreLib::GeometricalAnalysisTools::ComputeCharactersitic(
                    c, subOption, cloud, radius, roughnessUpDir, nullptr, octree.data());

            if (result == CCCoreLib::GeometricalAnalysisTools::NoError)
            {
                if (pc && sfIdx >= 0)
                {
                    pc->setCurrentDisplayedScalarField(sfIdx);
                    pc->showSF(sfIdx >= 0);
                    pc->getCurrentInScalarField()->computeMinAndMax();
                    if (c == CCCoreLib::GeometricalAnalysisTools::Roughness && roughnessUpDir != nullptr)
                    {
                        // signed roughness should be displayed with a symmetrical color scale
                        ccScalarField* sf = dynamic_cast<ccScalarField*>(pc->getCurrentInScalarField());
                        if (sf)
                        {
                            sf->setSymmetricalScale(true);
                        }
                        else
                        {
                            assert(false);
                        }
                    }
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

                CCTRACE("Failed to apply processing to cloud " << cloud->getName().toStdString());

                if (pc && sfIdx >= 0)
                {
                    pc->deleteScalarField(sfIdx);
                    sfIdx = -1;
                }

                ccLog::Error(errorMessage);
                CCTRACE(errorMessage.toStdString());
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
                    defaultRadius = ccOctree::GuessNaiveRadius(cloud);
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

bool invertNormals(std::vector<ccHObject*> selectedEntities)
{
    for (ccHObject* ent : selectedEntities)
    {
        // is it a mesh?
        ccMesh* mesh = ccHObjectCaster::ToMesh(ent);
        if (mesh && mesh->hasNormals())
        {
            mesh->invertNormals();
            continue;
        }

        // is it a cloud?
        ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(ent);
        if (cloud && cloud->hasNormals())
        {
            cloud->invertNormals();
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
                        double groundMaxEdgeLength,
                        ccRasterGrid::EmptyCellFillOption ceilEmptyCellFillStrategy,
                        double ceilMaxEdgeLength,
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

    //memory allocation
    CCVector3d minCorner = gridBox.minCorner();
    if (!grid.init(gridWidth, gridHeight, gridStep, minCorner))
    {
        //not enough memory
        ccLog::Error(QObject::tr("Not enough memory!"));
        CCTRACE("Not enough memory!");
        return false;
    }

    //progress dialog
    QScopedPointer<ccProgressDialog> pDlg(nullptr);

    ccRasterGrid groundRaster;
    if (ground)
    {
        if (!groundRaster.init(gridWidth, gridHeight, gridStep, minCorner))
        {
            //not enough memory
            ccLog::Error(QObject::tr("Not enough memory!"));
            CCTRACE("Not enough memory!");
            return false;
        }

        ccRasterGrid::InterpolationType interpolationType = ccRasterGrid::InterpolationTypeFromEmptyCellFillOption(groundEmptyCellFillStrategy);
        ccRasterGrid::DelaunayInterpolationParams dInterpParams;
        void* interpolationParams = nullptr;
        switch (interpolationType)
        {
        case ccRasterGrid::InterpolationType::DELAUNAY:
            dInterpParams.maxEdgeLength = groundMaxEdgeLength;
            interpolationParams = (void*)&dInterpParams;
            break;
        case ccRasterGrid::InterpolationType::KRIGING:
            // not supported yet
            assert(false);
            break;
        default:
            // do nothing
            break;
        }

        if (groundRaster.fillWith(  ground,
                                    vertDim,
                                    projectionType,
                                    interpolationType,
                                    interpolationParams,
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
            ccLog::Error(QObject::tr("Not enough memory!"));
            CCTRACE("Not enough memory!");
            return false;
        }

        ccRasterGrid::InterpolationType interpolationType = ccRasterGrid::InterpolationTypeFromEmptyCellFillOption(ceilEmptyCellFillStrategy);
        ccRasterGrid::DelaunayInterpolationParams dInterpParams;
        void* interpolationParams = nullptr;
        switch (interpolationType)
        {
        case ccRasterGrid::InterpolationType::DELAUNAY:
            dInterpParams.maxEdgeLength = ceilMaxEdgeLength;
            interpolationParams = (void*)&dInterpParams;
            break;
        case ccRasterGrid::InterpolationType::KRIGING:
            // not supported yet
            assert(false);
            break;
        default:
            // do nothing
            break;
        }

        if (ceilRaster.fillWith(ceil,
                                vertDim,
                                projectionType,
                                interpolationType,
                                interpolationParams,
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
                    if (std::isfinite(cell.h))
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
                        double ceilHeight,
                        ccRasterGrid::ProjectionType projectionType,
                        ccRasterGrid::EmptyCellFillOption groundEmptyCellFillStrategy,
                        double groundMaxEdgeLength,
                        ccRasterGrid::EmptyCellFillOption ceilEmptyCellFillStrategy,
                        double ceilMaxEdgeLength)
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
                               projectionType,
                               groundEmptyCellFillStrategy,
                               groundMaxEdgeLength,
                               ceilEmptyCellFillStrategy,
                               ceilMaxEdgeLength,
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

ccPointCloud* RasterizeToCloud(
	ccGenericPointCloud* cloud,
	double gridStep,
	CC_DIRECTION vertDir,
	bool outputRasterZ,
	bool outputRasterSFs,
	bool outputRasterRGB,
	std::string pathToImages,
	bool resample,
	ccRasterGrid::ProjectionType projectionType,
	ccRasterGrid::ProjectionType sfProjectionType,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy,
    double DelaunayMaxEdgeLength,
    int KrigingParamsKNN,
	double customHeight,
	ccBBox gridBBox,
    double percentile,
    bool export_perCellCount,
    bool export_perCellMinHeight,
    bool export_perCellMaxHeight,
    bool export_perCellAvgHeight,
    bool export_perCellHeightStdDev,
    bool export_perCellHeightRange,
    bool export_perCellMedian,
    bool export_perCellPercentile,
    bool export_perCellUniqueCount)
{
	CCTRACE("RasterizeToCloud");
    std::vector<ccRasterGrid::ExportableFields> extraScalarFields = {};
    if (export_perCellCount) extraScalarFields.push_back(ccRasterGrid::PER_CELL_COUNT);
    if (export_perCellMinHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MIN_VALUE);
    if (export_perCellMaxHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MAX_VALUE);
    if (export_perCellAvgHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_AVG_VALUE);
    if (export_perCellHeightStdDev) extraScalarFields.push_back(ccRasterGrid::PER_CELL_VALUE_STD_DEV);
    if (export_perCellHeightRange) extraScalarFields.push_back(ccRasterGrid::PER_CELL_VALUE_RANGE);
    if (export_perCellMedian) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MEDIAN_VALUE);
    if (export_perCellPercentile) extraScalarFields.push_back(ccRasterGrid::PER_CELL_PERCENTILE_VALUE);
    if (export_perCellUniqueCount) extraScalarFields.push_back(ccRasterGrid::PER_CELL_UNIQUE_COUNT_VALUE);
	ccHObject* entity = Rasterize_(cloud, gridStep, vertDir, 1,
			outputRasterZ, outputRasterSFs, outputRasterRGB, pathToImages, resample,
			projectionType, sfProjectionType, emptyCellFillStrategy,
			DelaunayMaxEdgeLength, KrigingParamsKNN,
			customHeight, gridBBox, percentile, extraScalarFields);
	CCTRACE("entity:" << entity);
	ccPointCloud* rcloud = ccHObjectCaster::ToPointCloud(entity);
	return rcloud;
}

ccMesh* RasterizeToMesh(
	ccGenericPointCloud* cloud,
	double gridStep,
	CC_DIRECTION vertDir,
	bool outputRasterZ,
	bool outputRasterSFs,
	bool outputRasterRGB,
	std::string pathToImages,
	bool resample,
	ccRasterGrid::ProjectionType projectionType,
	ccRasterGrid::ProjectionType sfProjectionType,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy,
    double DelaunayMaxEdgeLength,
    int KrigingParamsKNN,
	double customHeight,
	ccBBox gridBBox,
    double percentile,
    bool export_perCellCount,
    bool export_perCellMinHeight,
    bool export_perCellMaxHeight,
    bool export_perCellAvgHeight,
    bool export_perCellHeightStdDev,
    bool export_perCellHeightRange,
    bool export_perCellMedian,
    bool export_perCellPercentile,
    bool export_perCellUniqueCount)
{
    CCTRACE("RasterizeToMesh");
    std::vector<ccRasterGrid::ExportableFields> extraScalarFields = {};
    if (export_perCellCount) extraScalarFields.push_back(ccRasterGrid::PER_CELL_COUNT);
    if (export_perCellMinHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MIN_VALUE);
    if (export_perCellMaxHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MAX_VALUE);
    if (export_perCellAvgHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_AVG_VALUE);
    if (export_perCellHeightStdDev) extraScalarFields.push_back(ccRasterGrid::PER_CELL_VALUE_STD_DEV);
    if (export_perCellHeightRange) extraScalarFields.push_back(ccRasterGrid::PER_CELL_VALUE_RANGE);
    if (export_perCellMedian) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MEDIAN_VALUE);
    if (export_perCellPercentile) extraScalarFields.push_back(ccRasterGrid::PER_CELL_PERCENTILE_VALUE);
    if (export_perCellUniqueCount) extraScalarFields.push_back(ccRasterGrid::PER_CELL_UNIQUE_COUNT_VALUE);
	ccHObject* entity = Rasterize_(cloud, gridStep, vertDir, 2,
			outputRasterZ, outputRasterSFs, outputRasterRGB, pathToImages, resample,
			projectionType, sfProjectionType, emptyCellFillStrategy,
            DelaunayMaxEdgeLength, KrigingParamsKNN,
			customHeight, gridBBox, percentile, extraScalarFields);
	CCTRACE("entity:" << entity);
	ccMesh* mesh = ccHObjectCaster::ToMesh(entity);
	return mesh;
}

ccHObject* RasterizeGeoTiffOnly(
	ccGenericPointCloud* cloud,
	double gridStep,
	CC_DIRECTION vertDir,
	bool outputRasterZ,
	bool outputRasterSFs,
	bool outputRasterRGB,
	std::string pathToImages,
	bool resample,
	ccRasterGrid::ProjectionType projectionType,
	ccRasterGrid::ProjectionType sfProjectionType,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy,
    double DelaunayMaxEdgeLength,
    int KrigingParamsKNN,
	double customHeight,
	ccBBox gridBBox,
    double percentile,
    bool export_perCellCount,
    bool export_perCellMinHeight,
    bool export_perCellMaxHeight,
    bool export_perCellAvgHeight,
    bool export_perCellHeightStdDev,
    bool export_perCellHeightRange,
    bool export_perCellMedian,
    bool export_perCellPercentile,
    bool export_perCellUniqueCount)
{
    CCTRACE("RasterizeGeoTiffOnly");
    std::vector<ccRasterGrid::ExportableFields> extraScalarFields = {};
    if (export_perCellCount) extraScalarFields.push_back(ccRasterGrid::PER_CELL_COUNT);
    if (export_perCellMinHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MIN_VALUE);
    if (export_perCellMaxHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MAX_VALUE);
    if (export_perCellAvgHeight) extraScalarFields.push_back(ccRasterGrid::PER_CELL_AVG_VALUE);
    if (export_perCellHeightStdDev) extraScalarFields.push_back(ccRasterGrid::PER_CELL_VALUE_STD_DEV);
    if (export_perCellHeightRange) extraScalarFields.push_back(ccRasterGrid::PER_CELL_VALUE_RANGE);
    if (export_perCellMedian) extraScalarFields.push_back(ccRasterGrid::PER_CELL_MEDIAN_VALUE);
    if (export_perCellPercentile) extraScalarFields.push_back(ccRasterGrid::PER_CELL_PERCENTILE_VALUE);
    if (export_perCellUniqueCount) extraScalarFields.push_back(ccRasterGrid::PER_CELL_UNIQUE_COUNT_VALUE);
	ccHObject* entity = Rasterize_(cloud, gridStep, vertDir, 2,
			outputRasterZ, outputRasterSFs, outputRasterRGB, pathToImages, resample,
			projectionType, sfProjectionType, emptyCellFillStrategy,
            DelaunayMaxEdgeLength, KrigingParamsKNN,
			customHeight, gridBBox, percentile, extraScalarFields);
	return nullptr; // only image files created, no object returned
}

//! clone of ccRasterizeTool::ExportGeoTiff
bool ExportGeoTiff_(const QString& outputFilename,
					const ccRasterizeTool::ExportBands& exportBands,
					ccRasterGrid::EmptyCellFillOption fillEmptyCellsStrategy,
					const ccRasterGrid& grid,
					const ccBBox& gridBBox,
					unsigned char Z,
					double customHeightForEmptyCells/*=std::numeric_limits<double>::quiet_NaN()*/,
					ccGenericPointCloud* originCloud/*=0*/,
					int visibleSfIndex=-1)
{
	CCTRACE("fillEmptyCellsStrategy:" << fillEmptyCellsStrategy);
#ifdef CC_GDAL_SUPPORT

	if (exportBands.visibleSF && visibleSfIndex < 0)
	{
		assert(false);
		return false;
	}

	//vertical dimension
	assert(Z <= 2);
	const unsigned char X = Z == 2 ? 0 : Z + 1;
	const unsigned char Y = X == 2 ? 0 : X + 1;

    double stepX = grid.gridStep;
    double stepY = grid.gridStep;

    //global shift
    assert(gridBBox.isValid());
    double shiftX = gridBBox.minCorner().u[X] - stepX / 2; //we will declare the raster grid as 'Pixel-is-area'!
    double shiftY = gridBBox.maxCorner().u[Y] + stepY / 2; //we will declare the raster grid as 'Pixel-is-area'!
    double shiftZ = 0.0;

	if (originCloud)
	{
		const CCVector3d& shift = originCloud->getGlobalShift();
		shiftX -= shift.u[X];
		shiftY -= shift.u[Y];
		shiftZ -= shift.u[Z];

		double scale = originCloud->getGlobalScale();
		assert(scale != 0);
		stepX /= scale;
		stepY /= scale;
	}

	int totalBands = 0;
	bool onlyRGBA = true;

	if (exportBands.height)
	{
		++totalBands;
		onlyRGBA = false;
	}

	bool rgbaMode = false;
	if (exportBands.rgb)
	{
		totalBands += 3; //one per component
		if (fillEmptyCellsStrategy == ccRasterGrid::LEAVE_EMPTY && grid.validCellCount < grid.height * grid.width)
		{
			rgbaMode = true;
			++totalBands; //alpha
		}
	}
	else
	{
		onlyRGBA = false;
	}

	if (exportBands.density)
	{
		++totalBands;
		onlyRGBA = false;
	}

	if (exportBands.allSFs)
	{
		for (const auto & scalarField : grid.scalarFields)
		{
			if (!scalarField.empty())
			{
				++totalBands;
				onlyRGBA = false;
			}
		}
	}
	else if (exportBands.visibleSF && visibleSfIndex >= 0)
	{
		++totalBands;
		onlyRGBA = false;
	}

	if (totalBands == 0)
	{
		CCTRACE("Can't output a raster with no band! (check export parameters)");
		return false;
	}

	GDALAllRegister();
	CCTRACE("GDAL drivers: " << GetGDALDriverManager()->GetDriverCount());

	const char pszFormat[] = "GTiff";
	GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	if (!poDriver)
	{
		CCTRACE("[GDAL] Driver is not supported " << pszFormat);
		return false;
	}

	char** papszMetadata = poDriver->GetMetadata();
	if (!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE))
	{
		CCTRACE("[GDAL] Driver doesn't support Create() method " << pszFormat);
		return false;
	}

	char **papszOptions = nullptr;
	GDALDataset* poDstDS = poDriver->Create(qPrintable(outputFilename),
											static_cast<int>(grid.width),
											static_cast<int>(grid.height),
											totalBands,
											onlyRGBA ? GDT_Byte : GDT_Float64,
											papszOptions);

	if (!poDstDS)
	{
		ccLog::Error("[GDAL] Failed to create output raster (not enough memory?)");
		return false;
	}

	poDstDS->SetMetadataItem("AREA_OR_POINT", "AREA");

	double adfGeoTransform[6] = {	shiftX,		//top left x
									stepX,		//w-e pixel resolution (can be negative)
									0,			//0
									shiftY,		//top left y
									0,			//0
									-stepY		//n-s pixel resolution (can be negative)
	};

	poDstDS->SetGeoTransform( adfGeoTransform );

	//OGRSpatialReference oSRS;
	//oSRS.SetUTM( 11, TRUE );
	//oSRS.SetWellKnownGeogCS( "NAD27" );
	//char *pszSRS_WKT = nullptr;
	//oSRS.exportToWkt( &pszSRS_WKT );
	//poDstDS->SetProjection( pszSRS_WKT );
	//CPLFree( pszSRS_WKT );

	int currentBand = 0;

	//exort RGB band?
	if (exportBands.rgb)
	{
		GDALRasterBand* rgbBands[3] = { poDstDS->GetRasterBand(++currentBand),
										poDstDS->GetRasterBand(++currentBand),
										poDstDS->GetRasterBand(++currentBand) };
		rgbBands[0]->SetColorInterpretation(GCI_RedBand);
		rgbBands[1]->SetColorInterpretation(GCI_GreenBand);
		rgbBands[2]->SetColorInterpretation(GCI_BlueBand);

		unsigned char* cLine = static_cast<unsigned char*>(CPLMalloc(sizeof(unsigned char)*grid.width));
		if (!cLine)
		{
			CCTRACE("[GDAL] Not enough memory");
			GDALClose(poDstDS);
			return false;
		}

		bool error = false;

		//export the R, G and B components
		for (unsigned k = 0; k < 3; ++k)
		{
			rgbBands[k]->SetStatistics(0, 255, 128, 0); //warning: arbitrary average and std. dev. values

			for (unsigned j = 0; j<grid.height; ++j)
			{
				const ccRasterGrid::Row& row = grid.rows[grid.height - 1 - j]; //the first row is the northest one (i.e. Ymax)
				for (unsigned i = 0; i<grid.width; ++i)
				{
					cLine[i] = (std::isfinite(row[i].h) ? static_cast<unsigned char>(std::max(0.0, std::min(255.0, row[i].color.u[k]))) : 0);
				}

				if (rgbBands[k]->RasterIO(GF_Write, 0, static_cast<int>(j), static_cast<int>(grid.width), 1, cLine, static_cast<int>(grid.width), 1, GDT_Byte, 0, 0) != CE_None)
				{
					error = true;
					k = 3; //early stop
					break;
				}
			}
		}

		//export the alpha band (if necessary)
		if (!error && rgbaMode)
		{
			GDALRasterBand* aBand = poDstDS->GetRasterBand(++currentBand);
			aBand->SetColorInterpretation(GCI_AlphaBand);
			aBand->SetStatistics(0, 255, 255, 0); //warning: arbitrary average and std. dev. values

			for (unsigned j = 0; j<grid.height; ++j)
			{
				const ccRasterGrid::Row& row = grid.rows[grid.height - 1 - j];
				for (unsigned i = 0; i<grid.width; ++i)
				{
					cLine[i] = (std::isfinite(row[i].h) ? 255 : 0);
				}

				if (aBand->RasterIO(GF_Write, 0, static_cast<int>(j), static_cast<int>(grid.width), 1, cLine, static_cast<int>(grid.width), 1, GDT_Byte, 0, 0) != CE_None)
				{
					error = true;
					break;
				}
			}
		}

		CPLFree(cLine);

		if (error)
		{
			CCTRACE("[GDAL] An error occurred while writing the color bands!");
			GDALClose(poDstDS);
			return false;
		}
	}

	double* scanline = static_cast<double*>(CPLMalloc(sizeof(double)*grid.width));
	if (!scanline)
	{
		CCTRACE("[GDAL] Not enough memory");
		GDALClose(poDstDS);
		return false;
	}

	//exort height band?
	if (exportBands.height)
	{
		GDALRasterBand* poBand = poDstDS->GetRasterBand(++currentBand);
		assert(poBand);
		poBand->SetColorInterpretation(GCI_Undefined);

		double emptyCellHeight = 0;
		switch (fillEmptyCellsStrategy)
		{
		case ccRasterGrid::LEAVE_EMPTY:
			emptyCellHeight = grid.minHeight - 1.0;
			poBand->SetNoDataValue(emptyCellHeight); //should be transparent!
			break;
		case ccRasterGrid::FILL_MINIMUM_HEIGHT:
			emptyCellHeight = grid.minHeight;
			break;
		case ccRasterGrid::FILL_MAXIMUM_HEIGHT:
			emptyCellHeight = grid.maxHeight;
			break;
		case ccRasterGrid::FILL_CUSTOM_HEIGHT:
		case ccRasterGrid::INTERPOLATE_DELAUNAY:
			emptyCellHeight = customHeightForEmptyCells;
			break;
		case ccRasterGrid::FILL_AVERAGE_HEIGHT:
			emptyCellHeight = grid.meanHeight;
			break;
		default:
			assert(false);
		}

		emptyCellHeight += shiftZ;

		for (unsigned j = 0; j < grid.height; ++j)
		{
			const ccRasterGrid::Row& row = grid.rows[grid.height - 1 - j];
			for (unsigned i = 0; i<grid.width; ++i)
			{
				scanline[i] = std::isfinite(row[i].h) ? row[i].h + shiftZ : emptyCellHeight;
			}

			if (poBand->RasterIO(GF_Write, 0, static_cast<int>(j), static_cast<int>(grid.width), 1, scanline, static_cast<int>(grid.width), 1, GDT_Float64, 0, 0) != CE_None)
			{
				CCTRACE("[GDAL] An error occurred while writing the height band!");
				if (scanline)
					CPLFree(scanline);
				GDALClose(poDstDS);
				return false;
			}
		}
	}

	//export density band
	if (exportBands.density)
	{
		GDALRasterBand* poBand = poDstDS->GetRasterBand(++currentBand);
		assert(poBand);
		poBand->SetColorInterpretation(GCI_Undefined);
		for (unsigned j = 0; j < grid.height; ++j)
		{
			const ccRasterGrid::Row& row = grid.rows[grid.height - 1 - j];
			for (unsigned i = 0; i < grid.width; ++i)
			{
				scanline[i] = row[i].nbPoints;
			}

			if (poBand->RasterIO(GF_Write, 0, static_cast<int>(j), static_cast<int>(grid.width), 1, scanline, static_cast<int>(grid.width), 1, GDT_Float64, 0, 0) != CE_None)
			{
				CCTRACE("[GDAL] An error occurred while writing the height band!");
				if (scanline)
					CPLFree(scanline);
				GDALClose(poDstDS);
				return false;
			}
		}
	}

	//export SF bands
	if (exportBands.allSFs || (exportBands.visibleSF && visibleSfIndex >= 0))
	{
		for (size_t k = 0; k < grid.scalarFields.size(); ++k)
		{
			assert(!grid.scalarFields[k].empty());
			if (exportBands.allSFs || (exportBands.visibleSF && visibleSfIndex == static_cast<int>(k)))
			{
				const double* sfGrid = grid.scalarFields[k].data();
				GDALRasterBand* poBand = poDstDS->GetRasterBand(++currentBand);

				double sfNanValue = std::numeric_limits<ccRasterGrid::SF::value_type>::quiet_NaN();
				poBand->SetNoDataValue(sfNanValue); //should be transparent!
				assert(poBand);
				poBand->SetColorInterpretation(GCI_Undefined);

				for (unsigned j = 0; j < grid.height; ++j)
				{
					const ccRasterGrid::Row& row = grid.rows[grid.height - 1 - j];
					const double* sfRow = sfGrid + (grid.height - 1 - j) * grid.width;
					for (unsigned i = 0; i < grid.width; ++i)
					{
						scanline[i] = row[i].nbPoints ? sfRow[i] : sfNanValue;
					}

					if (poBand->RasterIO(	GF_Write,
											0,
											static_cast<int>(j),
											static_cast<int>(grid.width),
											1,
											scanline,
											static_cast<int>(grid.width),
											1,
											GDT_Float64, 0, 0 ) != CE_None)
					{
						//the corresponding SF should exist on the input cloud
						CCTRACE("[GDAL] An error occurred while writing a scalar field band!");
						k = grid.scalarFields.size(); //quick stop
						break;
					}
				}
			}
		}
	}

	if (scanline)
		CPLFree(scanline);
	scanline = nullptr;

	/* Once we're done, close properly the dataset */
	GDALClose(poDstDS);

	CCTRACE("[Rasterize] Raster successfully saved");
	return true;

#else
	assert(false);
	CCTRACE("[Rasterize] GDAL not supported by this version! Can't generate a raster...");
	return false;
#endif
}

//! see CommandRasterize::process
ccHObject* Rasterize_(
	ccGenericPointCloud* cloud,
	double gridStep,
	unsigned short vertDir,
	unsigned short outputCloudOrMesh, //0,1,2
	bool outputRasterZ,
	bool outputRasterSFs,
	bool outputRasterRGB,
	std::string pathToImages,
	bool resample,
	ccRasterGrid::ProjectionType projectionType,
	ccRasterGrid::ProjectionType sfProjectionType,
	ccRasterGrid::EmptyCellFillOption emptyCellFillStrategy,
    double DelaunayMaxEdgeLength,
    int KrigingParamsKNN,
	double customHeight,
	ccBBox gridBBox,
	double percentile,
    const std::vector<ccRasterGrid::ExportableFields>& extraScalarFields)
{
	CCTRACE("Rasterize_");
	CCTRACE("Cloud:" << cloud);
    ccRasterGrid::DelaunayInterpolationParams dInterpParams;
    ccRasterGrid::KrigingParams krigingParams;
    // force auto-guess
    krigingParams.autoGuess = true;
	ccHObject* entity = nullptr;
	bool isCellCount = false;
	if (gridStep <= 0)
	{
		CCTRACE("Invalid grid step value!");
		return nullptr;
	}
	if (vertDir > 2)
	{
		CCTRACE("Invalid direction!");
		return nullptr;
	}
	if (outputCloudOrMesh > 2)
	{
		CCTRACE("Invalid output option!");
		return nullptr;
	}
	if (emptyCellFillStrategy == ccRasterGrid::FILL_CUSTOM_HEIGHT && std::isnan(customHeight))
	{
		CCTRACE("[Rasterize] The filling strategy is set to 'fill with custom height' but no custom height was defined...");
		return nullptr;
	}
    if ((gridBBox.minCorner().norm2() < 1.e-12) && (gridBBox.maxCorner().norm2() < 1.e-12))
    {
    	gridBBox = cloud->getOwnBB();
    }
	//compute the grid size
	unsigned gridWidth = 0;
	unsigned gridHeight = 0;
	if (!ccRasterGrid::ComputeGridSize(vertDir, gridBBox, gridStep, gridWidth, gridHeight))
	{
		CCTRACE("Failed to compute the grid dimensions (check input cloud(s) bounding-box)");
		return nullptr;
	}

	if (gridWidth * gridHeight > (1 << 26)) //64 million of cells
	{
		CCTRACE("WARNING, Huge grid detected!");
	}

	ccRasterGrid grid;
	{
		//memory allocation
		CCVector3d minCorner = gridBBox.minCorner();
		if (!grid.init(gridWidth, gridHeight, gridStep, minCorner))
		{
			//not enough memory
			CCTRACE("Not enough memory");
			return nullptr;
		}

        ccRasterGrid::InterpolationType interpolationType = ccRasterGrid::InterpolationTypeFromEmptyCellFillOption(emptyCellFillStrategy);
        void* interpolationParams = nullptr;
        switch (interpolationType)
        {
        case ccRasterGrid::InterpolationType::DELAUNAY:
            interpolationParams = (void*)&dInterpParams;
            break;
        case ccRasterGrid::InterpolationType::KRIGING:
            interpolationParams = (void*)&krigingParams;
            break;
        default:
            // do nothing
            break;
        }

		if (grid.fillWith(cloud,
		                  vertDir,
		                  projectionType,
                          interpolationType,
                          interpolationParams,
                          sfProjectionType))
		{
			grid.fillEmptyCells(emptyCellFillStrategy, customHeight);
		}
		else
		{
			CCTRACE("Rasterize process failed");
			return nullptr;
		}

		//generate the result entity (cloud by default)
		bool outputCloud = (outputCloudOrMesh == 1);
		bool outputMesh = (outputCloudOrMesh == 2);
		if (outputCloud || outputMesh)
		{
		    CCTRACE("--- outputCloud:" << outputCloud << " outputMesh:"<< outputMesh);
            ccPointCloud* rasterCloud = nullptr;
            try
            {
                //we always compute the default 'height' layer
                std::vector<ccRasterGrid::ExportableFields> exportedStatistics(1);
                exportedStatistics.back() = ccRasterGrid::PER_CELL_VALUE;
                for(auto & item : extraScalarFields)
                {
                    if (item != ccRasterGrid::PER_CELL_VALUE)
                        exportedStatistics.push_back(item);
                    if (item == ccRasterGrid::PER_CELL_COUNT)
                        isCellCount = true;
                }

                rasterCloud = grid.convertToCloud(  true,
                                                    false,
                                                    exportedStatistics,
                                                    true,
                                                    true,
                                                    resample,
                                                    resample,
                                                    cloud,
                                                    vertDir,
                                                    gridBBox,
                                                    percentile,
                                                    true,
                                                    true,
                                                    nullptr
                                                );

            }
            catch (const std::bad_alloc&)
            {
                CCTRACE("Not enough memory");
                return nullptr;
            }

			if (!rasterCloud)
			{
				CCTRACE("Failed to output the raster grid as a cloud");
				return nullptr;
			}

			rasterCloud->showColors(cloud->hasColors());
			if (rasterCloud->hasScalarFields())
			{
				rasterCloud->showSF(!cloud->hasColors());
				rasterCloud->setCurrentDisplayedScalarField(0);
			}
			//don't forget the original shift
			rasterCloud->copyGlobalShiftAndScale(*cloud);

			if (outputCloud)
			{
				rasterCloud->setName(cloud->getName() += QString("_RASTER"));
				entity = rasterCloud;
				CCTRACE("entity:" << entity);
			}
			else if (outputMesh)
			{
				CCTRACE("--- outputMesh");
				std::string errorStr;
				CCCoreLib::GenericIndexedMesh* baseMesh =
					CCCoreLib::PointProjectionTools::computeTriangulation(rasterCloud,
																		  CCCoreLib::DELAUNAY_2D_AXIS_ALIGNED,
																		  CCCoreLib::PointProjectionTools::IGNORE_MAX_EDGE_LENGTH,
																		  vertDir,
																		  errorStr);

				if (baseMesh)
				{
					ccMesh* rasterMesh = new ccMesh(baseMesh, rasterCloud);
					delete baseMesh;
					baseMesh = nullptr;

					rasterCloud->setEnabled(false);
					rasterCloud->setVisible(true);
					rasterMesh->addChild(rasterCloud);
					rasterMesh->setName(rasterCloud->getName());
					//rasterCloud->setName("vertices");
					rasterMesh->showSF(rasterCloud->sfShown());
					rasterMesh->showColors(rasterCloud->colorsShown());
					rasterCloud = nullptr; //to avoid deleting it later
					CCTRACE("[Rasterize] Mesh successfully generated " << rasterMesh->getName().toStdString());
					entity = rasterMesh;
					CCTRACE("entity:" << entity);
				}
				else
				{
					CCTRACE("[Rasterize] Failed to create output mesh " << errorStr);
				}
				if (rasterCloud)
				{
					delete rasterCloud;
					rasterCloud = nullptr;
				}
			}
		}

		CCTRACE("entity:" << entity);
		if (outputRasterZ)
		{
			CCTRACE("outputRasterZ");
			ccRasterizeTool::ExportBands bands;
			{
				bands.height = true;
				bands.rgb = false; //not a good idea to mix RGB and height values!
				bands.allSFs = outputRasterSFs;
				bands.density = isCellCount;
			}
			QString exportFilename = QString(pathToImages.c_str()) + "/" + cloud->getName() + "_RASTER_Z";
			if (outputRasterSFs)
			{
				CCTRACE("outputRasterSFs");
				exportFilename += "_AND_SF";
			}
			exportFilename += ".tif";

			ExportGeoTiff_(	exportFilename, bands, emptyCellFillStrategy, grid,
							gridBBox, vertDir, customHeight, cloud);
		}

		if (outputRasterRGB)
		{
			CCTRACE("outputRasterRGB");
			ccRasterizeTool::ExportBands bands;
			{
				bands.rgb = true;
				bands.height = false; //not a good idea to mix RGB and height values!
				bands.allSFs = outputRasterSFs;
                bands.density = isCellCount;
			}
			QString exportFilename = QString(pathToImages.c_str()) + "/" + cloud->getName() + "_RASTER_RGB.tif";

			ExportGeoTiff_(	exportFilename, bands, emptyCellFillStrategy, grid,
							gridBBox, vertDir, customHeight, cloud);
		}
	}
    CCTRACE(entity->getName().toStdString());
	return entity;
}

// ===========================================================================
// ==== following used for ExtractSlicesAndContours (copy/adaptation from qCC)

//list of already used point to avoid hull's inner loops
enum HullPointFlags {   POINT_NOT_USED  = 0,
                        POINT_USED      = 1,
                        POINT_IGNORED   = 2,
                        POINT_FROZEN    = 3,
};

using Vertex2D = CCCoreLib::PointProjectionTools::IndexedCCVector2;
using VertexIterator = std::list<Vertex2D *>::iterator;
using ConstVertexIterator = std::list<Vertex2D *>::const_iterator;
using Hull2D = std::list<Vertex2D *>;

namespace
{
    struct Edge_
    {
        Edge_() : nearestPointIndex(0), nearestPointSquareDist(-1.0f) {}

        Edge_(const VertexIterator& A, unsigned _nearestPointIndex, float _nearestPointSquareDist)
        : itA(A)
        , nearestPointIndex(_nearestPointIndex)
        , nearestPointSquareDist(_nearestPointSquareDist)
    {}

        //operator
        inline bool operator< (const Edge_& e) const { return nearestPointSquareDist < e.nearestPointSquareDist; }

        VertexIterator itA;
        unsigned nearestPointIndex;
        float nearestPointSquareDist;
    };
}

namespace
{
    //Last envelope or contour unique ID
    //std::vector<unsigned> s_lastContourUniqueIDs_;

    //Envelope extraction parameters (global)
    //double s_maxEnvelopeEdgeLength_ = -1.0;

    //Meta-data key: origin entity UUID
    constexpr char s_originEntityUUID_[] = "OriginEntityUUID";
    //Meta-data key: slice (unique) ID
    constexpr char s_sliceID_[] = "SliceID";
}

#ifndef CC_GDAL_SUPPORT
#include "ccIsolines.h" //old alternative code to generate contour lines (doesn't work very well :( )
#include <QCoreApplication>
#else
//GDAL
#include <cpl_string.h>
#include <gdal.h>
#include <gdal_alg.h>

struct ContourGenerationParameters_
{
    std::vector<ccPolyline*> contourLines;
    const ccRasterGrid* grid = nullptr;
    bool projectContourOnAltitudes = false;
};

CPLErr ContourWriter_(  double dfLevel,
                        int nPoints,
                        double *padfX,
                        double *padfY,
                        void * userData);
#endif //CC_GDAL_SUPPORT

class ccContourLinesGenerator_
{
public:
//! Contour lines generation parameters
    struct Parameters
    {
        double startAltitude = 0.0;
        double maxAltitude = 0.0;
        double step = 0.0; //gap between levels
        ccScalarField* altitudes = nullptr; //optional scalar field that stores the 'altitudes' (may be null, in which case the grid 'h' values are used directly)
        int minVertexCount = 3; //minimum number of vertices per contour line
        bool projectContourOnAltitudes = false;
        double emptyCellsValue = std::numeric_limits<double>::quiet_NaN();

        /* The parameters below are only required if GDAL is not required */
        QWidget* parentWidget = nullptr; //for progress dialog
        bool ignoreBorders = false;
    };

    //! Additional meta-data key for generated polylines (see ccPolyline)
    static const char* MetaKeySubIndex() { return "SubIndex"; }

    static bool GenerateContourLines(   ccRasterGrid* rasterGrid,
                                        const CCVector2d& gridMinCornerXY,
                                        const Parameters& params,
                                        std::vector<ccPolyline*>& contourLines)
    {
        CCTRACE("GenerateContourLines");
        if (!rasterGrid || !rasterGrid->isValid())
        {
            CCTRACE("Need a valid raster/cloud to compute contours!");
            assert(false);
            return false;
        }
        if (params.startAltitude > params.maxAltitude)
        {
            CCTRACE("Start value is above the layer maximum value!");
            assert(false);
            return false;
        }
        if (params.step < 0)
        {
            CCTRACE("Invalid step value");
            assert(false);
            return false;
        }
        if (params.minVertexCount < 3)
        {
            CCTRACE("Invalid input parameter: can't have less than 3 vertices per contour line");
            assert(false);
            return false;
        }

        bool sparseLayer = (params.altitudes && params.altitudes->currentSize() != rasterGrid->height * rasterGrid->width);
        if (sparseLayer && !std::isfinite(params.emptyCellsValue))
        {
            CCTRACE("Invalid empty cell value (sparse layer)");
            assert(false);
            return false;
        }

        unsigned levelCount = 1;
        if (CCCoreLib::GreaterThanEpsilon(params.step))
        {
            levelCount += static_cast<unsigned>(floor((params.maxAltitude - params.startAltitude) / params.step));
        }

        try
        {
    #ifdef CC_GDAL_SUPPORT //use GDAL (more robust) - otherwise we will use an old code found on the Internet (with a strange behavior)

            //invoke the GDAL 'Contour Generator'
            ContourGenerationParameters_ gdalParams;
            gdalParams.grid = rasterGrid;
            gdalParams.projectContourOnAltitudes = params.projectContourOnAltitudes;
            GDALContourGeneratorH hCG = GDAL_CG_Create( rasterGrid->width,
                                                        rasterGrid->height,
                                                        std::isnan(params.emptyCellsValue) ? FALSE : TRUE,
                                                        params.emptyCellsValue,
                                                        params.step,
                                                        params.startAltitude,
                                                        ContourWriter_,
                                                        &gdalParams);
            if (!hCG)
            {
                CCTRACE("[GDAL] Failed to create contour generator");
                return false;
            }

            //feed the scan lines
            {
                double* scanline = static_cast<double*>(CPLMalloc(sizeof(double) * rasterGrid->width));
                if (!scanline)
                {
                    CCTRACE("[GDAL] Not enough memory");
                    return false;
                }

                unsigned layerIndex = 0;

                for (unsigned j = 0; j < rasterGrid->height; ++j)
                {
                    const ccRasterGrid::Row& cellRow = rasterGrid->rows[j];
                    for (unsigned i = 0; i < rasterGrid->width; ++i)
                    {
                        if (cellRow[i].nbPoints || !sparseLayer)
                        {
                            if (params.altitudes)
                            {
                                ScalarType value = params.altitudes->getValue(layerIndex++);
                                scanline[i] = ccScalarField::ValidValue(value) ? value : params.emptyCellsValue;
                            }
                            else
                            {
                                scanline[i] = std::isfinite(cellRow[i].h) ? cellRow[i].h : params.emptyCellsValue;
                            }
                        }
                        else
                        {
                            scanline[i] = params.emptyCellsValue;
                        }
                    }

                    CPLErr error = GDAL_CG_FeedLine(hCG, scanline);
                    if (error != CE_None)
                    {
                        CCTRACE("[GDAL] An error occurred during contour lines generation");
                        break;
                    }
                }

                if (scanline)
                {
                    CPLFree(scanline);
                }
                scanline = nullptr;

                //have we generated any contour line?
                if (!gdalParams.contourLines.empty())
                {
                    //reproject contour lines from raster C.S. to the cloud C.S.
                    for (ccPolyline*& poly : gdalParams.contourLines)
                    {
                        if (static_cast<int>(poly->size()) < params.minVertexCount)
                        {
                            delete poly;
                            poly = nullptr;
                            continue;
                        }

                        double height = std::numeric_limits<double>::quiet_NaN();
                        for (unsigned i = 0; i < poly->size(); ++i)
                        {
                            CCVector3* P2D = const_cast<CCVector3*>(poly->getAssociatedCloud()->getPoint(i));
                            if (i == 0)
                            {
                                height = P2D->z;
                            }

                            CCVector3 P(    static_cast<PointCoordinateType>((P2D->x - 0.5) * rasterGrid->gridStep + gridMinCornerXY.x),
                                            static_cast<PointCoordinateType>((P2D->y - 0.5) * rasterGrid->gridStep + gridMinCornerXY.y),
                                            P2D->z );
                            *P2D = P;
                        }

                        //add contour
                        poly->setName(QString("Contour line value = %1 (#%2)").arg(height).arg(poly->getMetaData(ccContourLinesGenerator_::MetaKeySubIndex()).toUInt()));
                        contourLines.push_back(poly);
                    }

                    gdalParams.contourLines.clear(); //just in case
                }
            }

            GDAL_CG_Destroy(hCG);
    #else
            unsigned xDim = rasterGrid->width;
            unsigned yDim = rasterGrid->height;

            int margin = 0;
            if (!params.ignoreBorders)
            {
                margin = 1;
                xDim += 2;
                yDim += 2;
            }
            std::vector<double> grid(xDim * yDim, 0);

            //fill grid
            {
                unsigned layerIndex = 0;
                for (unsigned j = 0; j < rasterGrid->height; ++j)
                {
                    const ccRasterGrid::Row& cellRow = rasterGrid->rows[j];
                    double* row = &(grid[(j + margin)*xDim + margin]);
                    for (unsigned i = 0; i < rasterGrid->width; ++i)
                    {
                        if (cellRow[i].nbPoints || !sparseLayer)
                        {
                            if (params.altitudes)
                            {
                                ScalarType value = params.altitudes->getValue(layerIndex++);
                                row[i] = ccScalarField::ValidValue(value) ? value : params.emptyCellsValue;
                            }
                            else
                            {
                                row[i] = std::isfinite(cellRow[i].h) ? cellRow[i].h : params.emptyCellsValue;
                            }
                        }
                        else
                        {
                            row[i] = params.emptyCellsValue;
                        }
                    }
                }
            }

            //generate contour lines
            {
                Isolines<double> iso(static_cast<int>(xDim), static_cast<int>(yDim));
                if (!params.ignoreBorders)
                {
                    iso.createOnePixelBorder(grid.data(), params.startAltitude - 1.0);
                }

                ccProgressDialog pDlg(true, params.parentWidget);
                pDlg.setMethodTitle(QObject::tr("Contour plot"));
                pDlg.setInfo(QObject::tr("Levels: %1\nCells: %2 x %3").arg(levelCount).arg(rasterGrid->width).arg(rasterGrid->height));
                pDlg.start();
                pDlg.show();
                QCoreApplication::processEvents();
                CCCoreLib::NormalizedProgress nProgress(&pDlg, levelCount);

                for (double v = params.startAltitude; v <= params.maxAltitude; v += params.step)
                {
                    //extract contour lines for the current level
                    iso.setThreshold(v);
                    int lineCount = iso.find(grid.data());

                    ccLog::PrintDebug(QString("[Rasterize][Isolines] value=%1 : %2 lines").arg(v).arg(lineCount));
                    CCTRACE("[Rasterize][Isolines] value=" << v << " : " << lineCount << "lines");

                    //convert them to poylines
                    int realCount = 0;
                    for (int i = 0; i < lineCount; ++i)
                    {
                        int vertCount = iso.getContourLength(i);
                        if (vertCount >= params.minVertexCount)
                        {
                            int startVi = 0; //we may have to split the polyline in multiple chunks
                            while (startVi < vertCount)
                            {
                                ccPointCloud* vertices = new ccPointCloud("vertices");
                                ccPolyline* poly = new ccPolyline(vertices);
                                poly->addChild(vertices);
                                bool isClosed = (startVi == 0 ? iso.isContourClosed(i) : false);
                                if (poly->reserve(vertCount - startVi) && vertices->reserve(vertCount - startVi))
                                {
                                    unsigned localIndex = 0;
                                    for (int vi = startVi; vi < vertCount; ++vi)
                                    {
                                        ++startVi;

                                        double x = iso.getContourX(i, vi) - margin;
                                        double y = iso.getContourY(i, vi) - margin;

                                        CCVector3 P;
                                        //DGM: we will only do the dimension mapping at export time
                                        //(otherwise the contour lines appear in the wrong orientation compared to the grid/raster which
                                        // is in the XY plane by default!)
                                        /*P.u[X] = */P.x = static_cast<PointCoordinateType>((x + 0.5) * rasterGrid->gridStep + gridMinCornerXY.x);
                                        /*P.u[Y] = */P.y = static_cast<PointCoordinateType>((y + 0.5) * rasterGrid->gridStep + gridMinCornerXY.y);
                                        if (params.projectContourOnAltitudes)
                                        {
                                            int xi = std::min(std::max(static_cast<int>(x), 0), static_cast<int>(rasterGrid->width) - 1);
                                            int yi = std::min(std::max(static_cast<int>(y), 0), static_cast<int>(rasterGrid->height) - 1);
                                            double h = rasterGrid->rows[yi][xi].h;
                                            if (std::isfinite(h))
                                            {
                                                /*P.u[Z] = */P.z = static_cast<PointCoordinateType>(h);
                                            }
                                            else
                                            {
                                                //DGM: we stop the current polyline
                                                isClosed = false;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            /*P.u[Z] = */P.z = static_cast<PointCoordinateType>(v);
                                        }

                                        vertices->addPoint(P);
                                        assert(localIndex < vertices->size());
                                        poly->addPointIndex(localIndex++);
                                    }

                                    assert(poly);
                                    if (poly->size() > 1)
                                    {
                                        poly->setClosed(isClosed); //if we have less vertices, it means we have 'chopped' the original contour
                                        vertices->setEnabled(false);

                                        ++realCount;
                                        poly->setMetaData(ccContourLinesGenerator::MetaKeySubIndex(), realCount);

                                        //add the 'const altitude' meta-data as well
                                        poly->setMetaData(ccPolyline::MetaKeyConstAltitude(), QVariant(v));

                                        //add contour
                                        poly->setName(QString("Contour line value = %1 (#%2)").arg(v).arg(realCount));
                                        try
                                        {
                                            contourLines.push_back(poly);
                                        }
                                        catch (const std::bad_alloc&)
                                        {
                                            CCTRACE("[ccContourLinesGenerator] Not enough memory");
                                            return false;
                                        }
                                    }
                                    else
                                    {
                                        delete poly;
                                        poly = nullptr;
                                    }
                                }
                                else
                                {
                                    delete poly;
                                    poly = nullptr;
                                    CCTRACE("Not enough memory!");
                                    return false;
                                }
                            }
                        }
                    }

                    if (!nProgress.oneStep())
                    {
                        //process cancelled by user
                        break;
                    }
                }
            }
    #endif
        }
        catch (const std::bad_alloc&)
        {
            CCTRACE("[ccContourLinesGenerator] Not enough memory");
            return false;
        }

        ccLog::Print(QString("[ccContourLinesGenerator] %1 iso-lines generated (%2 levels)").arg(contourLines.size()).arg(levelCount));
        CCTRACE("[ccContourLinesGenerator] " <<  contourLines.size() << " iso-lines generated (" << levelCount << " levels)");
        return true;
    };
};


#ifdef CC_GDAL_SUPPORT
CPLErr ContourWriter_(  double dfLevel,
                        int nPoints,
                        double *padfX,
                        double *padfY,
                        void * userData)
{
    //CCTRACE("ContourWriter_");
    if (nPoints < 2)
    {
        //nothing to do
        assert(false);
        return CE_None;
    }

    ContourGenerationParameters_* params = reinterpret_cast<ContourGenerationParameters_*>(userData);
    if (!params || !params->grid)
    {
        assert(false);
        return CE_Failure;
    }

    ccPointCloud* vertices = nullptr;
    ccPolyline* poly = nullptr;

    unsigned subIndex = 0;
    for (int i = 0; i < nPoints; ++i)
    {
        CCVector3 P(padfX[i], padfY[i], dfLevel);

        if (params->projectContourOnAltitudes)
        {
            int xi = std::min(std::max(static_cast<int>(padfX[i]), 0), static_cast<int>(params->grid->width) - 1);
            int yi = std::min(std::max(static_cast<int>(padfY[i]), 0), static_cast<int>(params->grid->height) - 1);
            double h = params->grid->rows[yi][xi].h;
            if (std::isfinite(h))
            {
                P.z = static_cast<PointCoordinateType>(h);
            }
            else
            {
                //DGM: we stop the current polyline
                if (poly)
                {
                    if (poly->size() < 2)
                    {
                        delete poly;
                        params->contourLines.pop_back();
                    }
                    poly = nullptr;
                    vertices = nullptr;
                }
                continue;
            }
        }

        if (!poly)
        {
            //we need to instantiate a new polyline
            vertices = new ccPointCloud("vertices");
            vertices->setEnabled(false);
            poly = new ccPolyline(vertices);
            poly->addChild(vertices);
            poly->setMetaData(ccContourLinesGenerator_::MetaKeySubIndex(), ++subIndex);
            poly->setClosed(false);

            //add the 'const altitude' meta-data as well
            poly->setMetaData(ccPolyline::MetaKeyConstAltitude(), QVariant(dfLevel));

            if (!vertices->reserve(nPoints - i) || !poly->reserve(nPoints - i))
            {
                //not enough memory
                delete poly;
                poly = nullptr;
                return CE_Failure;
            }

            try
            {
                params->contourLines.push_back(poly);
            }
            catch (const std::bad_alloc&)
            {
                return CE_Failure;
            }
        }

        assert(vertices);
        poly->addPointIndex(vertices->size());
        vertices->addPoint(P);
    }

    return CE_None;
}
#endif //CC_GDAL_SUPPORT

//! Finds the nearest (available) point to an edge
/** \return The nearest point distance (or -1 if no point was found!)
**/
PointCoordinateType FindNearestCandidate_(  unsigned& minIndex,
                                            const VertexIterator& itA,
                                            const VertexIterator& itB,
                                            const std::vector<Vertex2D>& points,
                                            const std::vector<HullPointFlags>& pointFlags,
                                            PointCoordinateType minSquareEdgeLength,
                                            bool allowLongerChunks = false,
                                            double minCosAngle = -1.0)
{
    //CCTRACE("FindNearestCandidate_");
    //look for the nearest point in the input set
    PointCoordinateType minDist2 = -1;
    const CCVector2 AB = **itB-**itA;
    const PointCoordinateType squareLengthAB = AB.norm2();
    const unsigned pointCount = static_cast<unsigned>(points.size());

#ifdef CC_CORE_LIB_USES_TBB
    tbb::parallel_for( static_cast<unsigned int>(0), pointCount, [&](unsigned int i) {
        const Vertex2D& P = points[i];
        if (pointFlags[P.index] != POINT_NOT_USED)
            return;

        //skip the edge vertices!
        if (P.index == (*itA)->index || P.index == (*itB)->index)
        {
            return;
        }

        //we only consider 'inner' points
        const CCVector2 AP = P-**itA;
        if (AB.x * AP.y - AB.y * AP.x < 0)
        {
            return;
        }

        //check the angle
        if (minCosAngle > -1.0)
        {
            const CCVector2 PB = **itB - P;
            const PointCoordinateType dotProd = AP.x * PB.x + AP.y * PB.y;
            const PointCoordinateType minDotProd = static_cast<PointCoordinateType>(minCosAngle * std::sqrt(AP.norm2() * PB.norm2()));
            if (dotProd < minDotProd)
            {
                return;
            }
        }

        const PointCoordinateType dot = AB.dot(AP); // = cos(PAB) * ||AP|| * ||AB||
        if (dot >= 0 && dot <= squareLengthAB)
        {
            const CCVector2 HP = AP - AB * (dot / squareLengthAB);
            const PointCoordinateType dist2 = HP.norm2();
            if (minDist2 < 0 || dist2 < minDist2)
            {
                //the 'nearest' point must also be a valid candidate
                //(i.e. at least one of the created edges is smaller than the original one
                //and we don't create too small edges!)
                const PointCoordinateType squareLengthAP = AP.norm2();
                const PointCoordinateType squareLengthBP = (P-**itB).norm2();
                if (    squareLengthAP >= minSquareEdgeLength
                    &&  squareLengthBP >= minSquareEdgeLength
                    &&  (allowLongerChunks || (squareLengthAP < squareLengthAB || squareLengthBP < squareLengthAB))
                    )
                {
                    minDist2 = dist2;
                    minIndex = i;
                }
            }
        }
    } );
#else
    for (unsigned i = 0; i < pointCount; ++i)
    {
        const Vertex2D& P = points[i];
        if (pointFlags[P.index] != POINT_NOT_USED)
            continue;

        //skip the edge vertices!
        if (P.index == (*itA)->index || P.index == (*itB)->index)
        {
            continue;
        }

        //we only consider 'inner' points
        CCVector2 AP = P - **itA;
        if (AB.x * AP.y - AB.y * AP.x < 0)
        {
            continue;
        }

        //check the angle
        if (minCosAngle > -1.0)
        {
            CCVector2 PB = **itB - P;
            PointCoordinateType dotProd = AP.x * PB.x + AP.y * PB.y;
            PointCoordinateType minDotProd = static_cast<PointCoordinateType>(minCosAngle * std::sqrt(AP.norm2() * PB.norm2()));
            if (dotProd < minDotProd)
            {
                continue;
            }
        }

        PointCoordinateType dot = AB.dot(AP); // = cos(PAB) * ||AP|| * ||AB||
        if (dot >= 0 && dot <= squareLengthAB)
        {
            CCVector2 HP = AP - AB * (dot / squareLengthAB);
            PointCoordinateType dist2 = HP.norm2();
            if (minDist2 < 0 || dist2 < minDist2)
            {
                //the 'nearest' point must also be a valid candidate
                //(i.e. at least one of the created edges is smaller than the original one
                //and we don't create too small edges!)
                PointCoordinateType squareLengthAP = AP.norm2();
                PointCoordinateType squareLengthBP = (P - **itB).norm2();
                if (    squareLengthAP >= minSquareEdgeLength
                    &&  squareLengthBP >= minSquareEdgeLength
                    &&  (allowLongerChunks || (squareLengthAP < squareLengthAB || squareLengthBP < squareLengthAB))
                    )
                {
                    minDist2 = dist2;
                    minIndex = i;
                }
            }
        }
    }
#endif

    return (minDist2 < 0 ? minDist2 : minDist2/squareLengthAB);
}

bool ExtractConcaveHull2D_( std::vector<Vertex2D>& points,
                            std::list<Vertex2D*>& hullPoints,
                            Envelope_Type envelopeType,
                            bool allowMultiPass,
                            PointCoordinateType maxSquareEdgeLength=0,
                            bool enableVisualDebugMode=false,
                            double maxAngleDeg=0.0)
{
    CCTRACE("ExtractConcaveHull2D_");
    //first compute the Convex hull
    if (!CCCoreLib::PointProjectionTools::extractConvexHull2D(points,hullPoints))
        return false;

    //do we really need to compute the concave hull?
    if (hullPoints.size() < 2 || maxSquareEdgeLength < 0)
        return true;

    unsigned pointCount = static_cast<unsigned>(points.size());

    std::vector<HullPointFlags> pointFlags;
    try
    {
        pointFlags.resize(pointCount, POINT_NOT_USED);
    }
    catch(...)
    {
        //not enough memory
        return false;
    }

    double minCosAngle = maxAngleDeg <= 0 ? -1.0 : std::cos(maxAngleDeg * M_PI / 180.0);

    //hack: compute the theoretical 'minimal' edge length
    PointCoordinateType minSquareEdgeLength = 0;
    {
        CCVector2 minP;
        CCVector2 maxP;
        for (size_t i=0; i<pointCount; ++i)
        {
            const Vertex2D& P = points[i];
            if (i)
            {
                minP.x = std::min(P.x,minP.x);
                minP.y = std::min(P.y,minP.y);
                maxP.x = std::max(P.x,maxP.x);
                maxP.y = std::max(P.y,maxP.y);
            }
            else
            {
                minP = maxP = P;
            }
        }
        minSquareEdgeLength = (maxP - minP).norm2() / static_cast<PointCoordinateType>(1.0e7); //10^-7 of the max bounding rectangle side
        minSquareEdgeLength = std::min(minSquareEdgeLength, maxSquareEdgeLength / 10);

        //we remove very small edges
        for (VertexIterator itA = hullPoints.begin(); itA != hullPoints.end(); ++itA)
        {
            VertexIterator itB = itA; ++itB;
            if (itB == hullPoints.end())
                itB = hullPoints.begin();
            if ((**itB-**itA).norm2() < minSquareEdgeLength)
            {
                pointFlags[(*itB)->index] = POINT_FROZEN;
                hullPoints.erase(itB);
            }
        }

        if (envelopeType != FULL)
        {
            //we will now try to determine which part of the envelope is the 'upper' one and which one is the 'lower' one

            //search for the min and max vertices
            VertexIterator itLeft = hullPoints.begin();
            VertexIterator itRight = hullPoints.begin();
            {
                for (VertexIterator it = hullPoints.begin(); it != hullPoints.end(); ++it)
                {
                    if ((*it)->x < (*itLeft)->x || ((*it)->x == (*itLeft)->x && (*it)->y < (*itLeft)->y))
                    {
                        itLeft = it;
                    }
                    if ((*it)->x > (*itRight)->x || ((*it)->x == (*itRight)->x && (*it)->y < (*itRight)->y))
                    {
                        itRight = it;
                    }
                }
            }
            assert(itLeft != itRight);
            //find the right way to go
            {
                VertexIterator itBefore = itLeft;
                if (itBefore == hullPoints.begin())
                    itBefore = hullPoints.end(); --itBefore;
                VertexIterator itAfter = itLeft; ++itAfter;
                if (itAfter == hullPoints.end())
                    itAfter = hullPoints.begin();

                bool forward = ((**itBefore - **itLeft).cross(**itAfter - **itLeft) < 0 && envelopeType == LOWER);
                if (!forward)
                    std::swap(itLeft,itRight);
            }

            //copy the right part
            std::list<Vertex2D*> halfHullPoints;
            try
            {
                for (VertexIterator it = itLeft; ; ++it)
                {
                    if (it == hullPoints.end())
                        it = hullPoints.begin();
                    halfHullPoints.push_back(*it);
                    if (it == itRight)
                        break;
                }
            }
            catch (const std::bad_alloc&)
            {
                //not enough memory
                return false;
            }
            //replace the input hull by the selected part
            hullPoints = halfHullPoints;
        }

        if (hullPoints.size() < 2)
        {
            //no more edges?!
            return false;
        }
    }


    //DEBUG MECHANISM
    ccPointCloud* debugCloud = nullptr;
    ccPolyline* debugEnvelope = nullptr;
    ccPointCloud* debugEnvelopeVertices = nullptr;

    //Warning: high STL containers usage ahead ;)
    unsigned step = 0;
    bool somethingHasChanged = true;
    while (somethingHasChanged)
    {
        try
        {
            somethingHasChanged = false;
            ++step;

            //build the initial edge list & flag the convex hull points
            std::multiset<Edge_> edges;
            //initial number of edges
            assert(hullPoints.size() >= 2);
            size_t initEdgeCount = hullPoints.size();
            if (envelopeType != FULL)
                --initEdgeCount;

            VertexIterator itB = hullPoints.begin();
            for (size_t i = 0; i < initEdgeCount; ++i)
            {
                VertexIterator itA = itB; ++itB;
                if (itB == hullPoints.end())
                    itB = hullPoints.begin();

                //we will only process the edges that are longer than the maximum specified length
                if ((**itB - **itA).norm2() > maxSquareEdgeLength)
                {
                    unsigned nearestPointIndex = 0;
                    PointCoordinateType minSquareDist = FindNearestCandidate_(
                        nearestPointIndex,
                        itA,
                        itB,
                        points,
                        pointFlags,
                        minSquareEdgeLength,
                        step > 1,
                        minCosAngle);

                    if (minSquareDist >= 0)
                    {
                        Edge_ e(itA, nearestPointIndex, minSquareDist);
                        edges.insert(e);
                    }
                }

                pointFlags[(*itA)->index] = POINT_USED;
            }

            //flag the last vertex as well for non closed envelopes!
            if (envelopeType != FULL)
                pointFlags[(*hullPoints.rbegin())->index] = POINT_USED;

            while (!edges.empty())
            {
                //current edge (AB)
                //this should be the edge with the nearest 'candidate'
                Edge_ e = *edges.begin();
                edges.erase(edges.begin());

                VertexIterator itA = e.itA;
                VertexIterator itB = itA; ++itB;
                if (itB == hullPoints.end())
                {
                    assert(envelopeType == FULL);
                    itB = hullPoints.begin();
                }

                //nearest point
                const Vertex2D& P = points[e.nearestPointIndex];
                assert(pointFlags[P.index] == POINT_NOT_USED); //we don't consider already used points!

                //create labels
                cc2DLabel* edgeLabel = nullptr;
                cc2DLabel* label = nullptr;

                //last check: the new segments must not intersect with the actual hull!
                bool intersect = false;
                //if (false)
                {
                    for (VertexIterator itJ = hullPoints.begin(), itI = itJ++; itI != hullPoints.end(); ++itI, ++itJ)
                    {
                        if (itJ == hullPoints.end())
                        {
                            if (envelopeType == FULL)
                                itJ = hullPoints.begin();
                            else
                                break;
                        }

                        if (    ((*itI)->index != (*itA)->index && (*itJ)->index != (*itA)->index && CCCoreLib::PointProjectionTools::segmentIntersect(**itI,**itJ,**itA,P))
                            ||  ((*itI)->index != (*itB)->index && (*itJ)->index != (*itB)->index && CCCoreLib::PointProjectionTools::segmentIntersect(**itI,**itJ,P,**itB)) )
                        {
                            intersect = true;
                            break;
                        }
                    }
                }
                if (!intersect)
                {
                    //add point to concave hull
                    VertexIterator itP = hullPoints.insert(itB == hullPoints.begin() ? hullPoints.end() : itB, &points[e.nearestPointIndex]);

                    //we won't use P anymore!
                    pointFlags[P.index] = POINT_USED;

                    somethingHasChanged = true;

                    //update all edges that were having 'P' as their nearest candidate as well
                    if (!edges.empty())
                    {
                        std::vector<VertexIterator> removed;
                        std::multiset<Edge_>::const_iterator lastValidIt = edges.end();
                        for (std::multiset<Edge_>::const_iterator it = edges.begin(); it != edges.end(); ++it)
                        {
                            if ((*it).nearestPointIndex == e.nearestPointIndex)
                            {
                                //we'll have to put them back afterwards!
                                removed.push_back((*it).itA);

                                edges.erase(it);
                                if (edges.empty())
                                    break;
                                if (lastValidIt != edges.end())
                                    it = lastValidIt;
                                else
                                    it = edges.begin();
                            }
                            else
                            {
                                lastValidIt = it;
                            }
                        }

                        //update the removed edges info and put them back in the main list
                        for (size_t i = 0; i < removed.size(); ++i)
                        {
                            VertexIterator itC = removed[i];
                            VertexIterator itD = itC; ++itD;
                            if (itD == hullPoints.end())
                                itD = hullPoints.begin();

                            unsigned nearestPointIndex = 0;
                            PointCoordinateType minSquareDist = FindNearestCandidate_(
                                nearestPointIndex,
                                itC,
                                itD,
                                points,
                                pointFlags,
                                minSquareEdgeLength,
                                false,
                                minCosAngle);

                            if (minSquareDist >= 0)
                            {
                                Edge_ e(itC, nearestPointIndex, minSquareDist);
                                edges.insert(e);
                            }
                        }
                    }

                    //we'll inspect the two new segments later (if necessary)
                    if ((P-**itA).norm2() > maxSquareEdgeLength)
                    {
                        unsigned nearestPointIndex = 0;
                        PointCoordinateType minSquareDist = FindNearestCandidate_(
                            nearestPointIndex,
                            itA,
                            itP,
                            points,
                            pointFlags,
                            minSquareEdgeLength,
                            false,
                            minCosAngle);

                        if (minSquareDist >= 0)
                        {
                            Edge_ e(itA,nearestPointIndex,minSquareDist);
                            edges.insert(e);
                        }
                    }
                    if ((**itB-P).norm2() > maxSquareEdgeLength)
                    {
                        unsigned nearestPointIndex = 0;
                        PointCoordinateType minSquareDist = FindNearestCandidate_(
                            nearestPointIndex,
                            itP,
                            itB,
                            points,
                            pointFlags,
                            minSquareEdgeLength,
                            false,
                            minCosAngle);

                        if (minSquareDist >= 0)
                        {
                            Edge_ e(itP,nearestPointIndex,minSquareDist);
                            edges.insert(e);
                        }
                    }
                }
            }
        }
        catch (...)
        {
            //not enough memory
            return false;
        }

        if (!allowMultiPass)
            break;
    }

    return true;
}

ccPolyline* ExtractFlatEnvelope__(  CCCoreLib::GenericIndexedCloudPersist* points,
                                    bool allowMultiPass,
                                    PointCoordinateType maxEdgeLength=0,
                                    const PointCoordinateType* preferredNormDim=nullptr,
                                    const PointCoordinateType* preferredUpDir=nullptr,
                                    Envelope_Type envelopeType=FULL,
                                    std::vector<unsigned>* originalPointIndexes=nullptr,
                                    bool enableVisualDebugMode=false,
                                    double maxAngleDeg=0.0)
{
    CCTRACE("ExtractFlatEnvelope__");
    assert(points);

    if (!points)
        return nullptr;

    unsigned ptsCount = points->size();

    if (ptsCount < 3)
        return nullptr;

    CCCoreLib::Neighbourhood Yk(points);

    //local base
    CCVector3 O;
    CCVector3 X;
    CCVector3 Y;

    CCCoreLib::Neighbourhood::InputVectorsUsage vectorsUsage = CCCoreLib::Neighbourhood::None;

    //we project the input points on a plane
    std::vector<Vertex2D> points2D;
    PointCoordinateType* planeEq = nullptr;

    if (preferredUpDir != nullptr)
    {
        Y = CCVector3(preferredUpDir);
        vectorsUsage = CCCoreLib::Neighbourhood::UseYAsUpDir;
    }

    //if the user has specified a default direction, we'll use it as 'projecting plane'
    PointCoordinateType preferredPlaneEq[4] = { 0, 0, 1, 0 };

    if (preferredNormDim != nullptr)
    {
        const CCVector3* G = points->getPoint(0); //any point through which the plane passes is ok
        preferredPlaneEq[0] = preferredNormDim[0];
        preferredPlaneEq[1] = preferredNormDim[1];
        preferredPlaneEq[2] = preferredNormDim[2];
        CCVector3::vnormalize(preferredPlaneEq);
        preferredPlaneEq[3] = CCVector3::vdot(G->u, preferredPlaneEq);
        planeEq = preferredPlaneEq;

        if (preferredUpDir != nullptr)
        {
            O = *G;
            //Y = CCVector3(preferredUpDir); //already done above
            X = Y.cross(CCVector3(preferredNormDim));
            vectorsUsage = CCCoreLib::Neighbourhood::UseOXYasBase;
        }
    }

    if (!Yk.projectPointsOn2DPlane<Vertex2D>(points2D, planeEq, &O, &X, &Y, vectorsUsage))
    {
        CCTRACE("[ExtractFlatEnvelope] Failed to project the points on the LS plane (not enough memory?)!");
        return nullptr;
    }

    //update the points indexes (not done by Neighbourhood::projectPointsOn2DPlane)
    {
        for (unsigned i = 0; i < ptsCount; ++i)
        {
            points2D[i].index = i;
        }
    }

    //try to get the points on the convex/concave hull to build the envelope and the polygon
    Hull2D hullPoints;
    if (!ExtractConcaveHull2D_( points2D,
                                hullPoints,
                                envelopeType,
                                allowMultiPass,
                                maxEdgeLength*maxEdgeLength,
                                enableVisualDebugMode,
                                maxAngleDeg))
    {
        CCTRACE("[ExtractFlatEnvelope] Failed to compute the convex hull of the input points!");
        return nullptr;
    }

    if (originalPointIndexes)
    {
        try
        {
            originalPointIndexes->resize(hullPoints.size(), 0);
        }
        catch (const std::bad_alloc&)
        {
            //not enough memory
            CCTRACE("[ExtractFlatEnvelope] Not enough memory!");
            return nullptr;
        }

        unsigned i = 0;
        for (Hull2D::const_iterator it = hullPoints.begin(); it != hullPoints.end(); ++it, ++i)
        {
            (*originalPointIndexes)[i] = (*it)->index;
        }
    }

    unsigned hullPtsCount = static_cast<unsigned>(hullPoints.size());

    //create vertices
    ccPointCloud* envelopeVertices = new ccPointCloud();
    {
        if (!envelopeVertices->reserve(hullPtsCount))
        {
            delete envelopeVertices;
            envelopeVertices = nullptr;
            CCTRACE("[ExtractFlatEnvelope] Not enough memory!");
            return nullptr;
        }

        //projection on the LS plane (in 3D)
        for (Hull2D::const_iterator it = hullPoints.begin(); it != hullPoints.end(); ++it)
        {
            envelopeVertices->addPoint(O + X*(*it)->x + Y*(*it)->y);
        }

        envelopeVertices->setName("vertices");
        envelopeVertices->setEnabled(false);
    }

    //we create the corresponding (3D) polyline
    ccPolyline* envelopePolyline = new ccPolyline(envelopeVertices);
    if (envelopePolyline->reserve(hullPtsCount))
    {
        envelopePolyline->addPointIndex(0, hullPtsCount);
        envelopePolyline->setClosed(envelopeType == FULL);
        envelopePolyline->setVisible(true);
        envelopePolyline->setName("envelope");
        envelopePolyline->addChild(envelopeVertices);
    }
    else
    {
        delete envelopePolyline;
        envelopePolyline = nullptr;
        CCTRACE("[ExtractFlatEnvelope] Not enough memory to create the envelope polyline!");
    }

    return envelopePolyline;
}

bool ExtractFlatEnvelope_(  CCCoreLib::GenericIndexedCloudPersist* points,
                            bool allowMultiPass,
                            PointCoordinateType maxEdgeLength,
                            std::vector<ccPolyline*>& parts,
                            Envelope_Type envelopeType=FULL,
                            bool allowSplitting=true,
                            const PointCoordinateType* preferredNormDir=nullptr,
                            const PointCoordinateType* preferredUpDir=nullptr,
                            bool enableVisualDebugMode=false)
{
    CCTRACE("ExtractFlatEnvelope_");
    parts.clear();

    //extract whole envelope
    ccPolyline* basePoly = ExtractFlatEnvelope__(points, allowMultiPass, maxEdgeLength, preferredNormDir,
                                                 preferredUpDir, envelopeType, nullptr, enableVisualDebugMode);
    if (!basePoly)
    {
        return false;
    }
    else if (!allowSplitting)
    {
        parts.push_back(basePoly);
        return true;
    }

    //and split it if necessary
    bool success = basePoly->split(maxEdgeLength, parts);

    delete basePoly;
    basePoly = nullptr;

    return success;
}

//! see ccCropTool::Crop
ccHObject* Crop_(ccHObject* entity, const ccBBox& box, bool inside/*=true*/, const ccGLMatrix* meshRotation/*=nullptr*/)
{
    //CCTRACE("Crop_");
    assert(entity);
    if (!entity)
    {
        return nullptr;
    }

    if (entity->isA(CC_TYPES::POINT_CLOUD))
    {
        ccPointCloud* cloud = static_cast<ccPointCloud*>(entity);

        CCCoreLib::ReferenceCloud* selection = cloud->crop(box, inside);
        if (!selection)
        {
            //process failed!
            CCTRACE(QString("[Crop] Failed to crop cloud '%1'!").arg(cloud->getName()).toStdString());
            return nullptr;
        }

        if (selection->size() == 0)
        {
            //no points fall inside selection!
            CCTRACE(QString("[Crop] No point of the cloud '%1' falls %2side the input box!").arg(cloud->getName(), (inside ? "in" : "out")).toStdString());
            delete selection;
            return nullptr;
        }

        //crop
        ccPointCloud* croppedEnt = cloud->partialClone(selection);
        delete selection;
        selection = nullptr;

        return croppedEnt;
    }
    else if (entity->isKindOf(CC_TYPES::MESH))
    {
        ccGenericMesh* mesh = static_cast<ccGenericMesh*>(entity);
        CCCoreLib::ManualSegmentationTools::MeshCutterParams params;
        params.bbMin = box.minCorner();
        params.bbMax = box.maxCorner();
        params.generateOutsideMesh = !inside;
        params.trackOrigIndexes = mesh->hasColors() || mesh->hasScalarFields() || mesh->hasMaterials();

        ccGenericPointCloud* origVertices = mesh->getAssociatedCloud();
        assert(origVertices);
        ccGenericPointCloud* cropVertices = origVertices;
        if (meshRotation)
        {
            ccPointCloud* rotatedVertices = ccPointCloud::From(origVertices);
            if (!rotatedVertices)
            {
                CCTRACE(QString("[Crop] Failed to crop mesh '%1'! (not enough memory)").arg(mesh->getName()).toStdString());
                return nullptr;
            }
            rotatedVertices->setGLTransformation(*meshRotation);
            rotatedVertices->applyGLTransformation_recursive();
            cropVertices = rotatedVertices;
        }

        if (!CCCoreLib::ManualSegmentationTools::segmentMeshWithAABox(mesh, cropVertices, params))
        {
            //process failed!
            CCTRACE(QString("[Crop] Failed to crop mesh '%1'!").arg(mesh->getName()).toStdString());
        }

        if (cropVertices != origVertices)
        {
            //don't need those anymore
            delete cropVertices;
            cropVertices = origVertices;
        }

        CCCoreLib::SimpleMesh* tempMesh = inside ? params.insideMesh : params.outsideMesh;

        //output
        ccMesh* croppedMesh = nullptr;

        if (tempMesh)
        {
            ccPointCloud* croppedVertices = ccPointCloud::From(tempMesh->vertices());
            if (croppedVertices)
            {
                if (meshRotation)
                {
                    //apply inverse transformation
                    croppedVertices->setGLTransformation(meshRotation->inverse());
                    croppedVertices->applyGLTransformation_recursive();
                }
                croppedMesh = new ccMesh(tempMesh, croppedVertices);
                croppedMesh->addChild(croppedVertices);
                croppedVertices->setEnabled(false);
                if (croppedMesh->size() == 0)
                {
                    //no points fall inside selection!
                    ccLog::Warning(QString("[Crop] No triangle of the mesh '%1' falls %2side the input box!").arg(mesh->getName(), (inside ? "in" : "out")));
                    delete croppedMesh;
                    croppedMesh = nullptr;
                }
                else
                {
                    assert(origVertices);

                    //import parameters
                    croppedVertices->importParametersFrom(origVertices);
                    croppedMesh->importParametersFrom(mesh);

                    //compute normals if necessary
                    if (mesh->hasNormals())
                    {
                        bool success = false;
                        if (mesh->hasTriNormals())
                            success = croppedMesh->computePerTriangleNormals();
                        else
                            success = croppedMesh->computePerVertexNormals();

                        if (!success)
                        {
                            ccLog::Warning("[Crop] Failed to compute normals on the output mesh (not enough memory)");
                        }
                        croppedMesh->showNormals(success && mesh->normalsShown());
                    }

                    //import other features if necessary
                    if (params.trackOrigIndexes)
                    {
                        const std::vector<unsigned>& origTriIndexes = inside ? params.origTriIndexesMapInside : params.origTriIndexesMapOutside;

                        try
                        {
                            //per vertex features (RGB color & scalar fields)
                            if (origVertices->hasColors() || origVertices->hasScalarFields())
                            {
                                //we use flags to avoid processing the same vertex multiple times
                                std::vector<bool> vertProcessed(croppedVertices->size(), false);

                                //colors
                                bool importColors = false;
                                if (origVertices->hasColors())
                                {
                                    importColors = croppedVertices->resizeTheRGBTable();
                                    if (!importColors)
                                        ccLog::Warning("[Crop] Failed to transfer RGB colors on the output mesh (not enough memory)");
                                }

                                //scalar fields
                                std::vector<ccScalarField*> importedSFs;
                                ccPointCloud* origVertices_pc = nullptr;
                                if (origVertices->hasScalarFields())
                                {
                                    origVertices_pc = origVertices->isA(CC_TYPES::POINT_CLOUD) ? static_cast<ccPointCloud*>(origVertices) : nullptr;
                                    unsigned sfCount = origVertices_pc ? origVertices_pc->getNumberOfScalarFields() : 1;

                                    //now try to import each SF
                                    for (unsigned i = 0; i < sfCount; ++i)
                                    {
                                        int sfIdx = croppedVertices->addScalarField(origVertices_pc ? origVertices_pc->getScalarField(i)->getName() : "Scalar field");
                                        if (sfIdx >= 0)
                                        {
                                            ccScalarField* sf = static_cast<ccScalarField*>(croppedVertices->getScalarField(i));
                                            sf->fill(CCCoreLib::NAN_VALUE);
                                            if (origVertices_pc)
                                            {
                                                //import display parameters if possible
                                                ccScalarField* originSf = static_cast<ccScalarField*>(origVertices_pc->getScalarField(i));
                                                assert(originSf);
                                                //copy display parameters
                                                sf->importParametersFrom(originSf);
                                            }
                                            importedSFs.push_back(sf);
                                        }
                                        else
                                        {
                                            ccLog::Warning("[Crop] Failed to transfer one or several scalar fields on the output mesh (not enough memory)");
                                            //we can stop right now as all SFs have the same size!
                                            break;
                                        }
                                    }

                                    //default displayed SF
                                    if (origVertices_pc)
                                        croppedVertices->setCurrentDisplayedScalarField(std::max(static_cast<int>(croppedVertices->getNumberOfScalarFields())-1, origVertices_pc->getCurrentDisplayedScalarFieldIndex()));
                                    else
                                        croppedVertices->setCurrentDisplayedScalarField(0);
                                }
                                bool importSFs = !importedSFs.empty();

                                if (importColors || importSFs)
                                {
                                    //for each new triangle
                                    for (unsigned i = 0; i < croppedMesh->size(); ++i)
                                    {
                                        //get the origin triangle
                                        unsigned origTriIndex = origTriIndexes[i];
                                        const CCCoreLib::VerticesIndexes* tsio = mesh->getTriangleVertIndexes(origTriIndex);

                                        //get the new triangle
                                        const CCCoreLib::VerticesIndexes* tsic = croppedMesh->getTriangleVertIndexes(i);

                                        //we now have to test the 3 vertices of the new triangle
                                        for (unsigned j = 0; j < 3; ++j)
                                        {
                                            unsigned vertIndex = tsic->i[j];

                                            if (vertProcessed[vertIndex])
                                            {
                                                //vertex has already been process
                                                continue;
                                            }

                                            const CCVector3* Vcj = croppedVertices->getPoint(vertIndex);

                                            //we'll deduce its color and SFs values by triangulation
                                            if (importColors)
                                            {
                                                ccColor::Rgb col;
                                                if (mesh->interpolateColors(origTriIndex, *Vcj, col))
                                                {
                                                    croppedVertices->setPointColor(vertIndex, col);
                                                }
                                            }

                                            if (importSFs)
                                            {
                                                CCVector3d w;
                                                mesh->computeInterpolationWeights(origTriIndex, *Vcj, w);

                                                //import SFs
                                                for (unsigned s = 0; s < static_cast<unsigned>(importedSFs.size()); ++s)
                                                {
                                                    CCVector3d scalarValues(0, 0, 0);
                                                    if (origVertices_pc)
                                                    {
                                                        const CCCoreLib::ScalarField* sf = origVertices_pc->getScalarField(s);
                                                        scalarValues.x = sf->getValue(tsio->i1);
                                                        scalarValues.y = sf->getValue(tsio->i2);
                                                        scalarValues.z = sf->getValue(tsio->i3);
                                                    }
                                                    else
                                                    {
                                                        assert(s == 0);
                                                        scalarValues.x = origVertices->getPointScalarValue(tsio->i1);
                                                        scalarValues.y = origVertices->getPointScalarValue(tsio->i2);
                                                        scalarValues.z = origVertices->getPointScalarValue(tsio->i3);
                                                    }

                                                    ScalarType sVal = static_cast<ScalarType>(scalarValues.dot(w));
                                                    importedSFs[s]->setValue(vertIndex,sVal);
                                                }
                                            }

                                            //update 'processed' flag
                                            vertProcessed[vertIndex] = true;
                                        }
                                    }

                                    for (size_t s = 0; s < importedSFs.size(); ++s)
                                    {
                                        importedSFs[s]->computeMinAndMax();
                                    }

                                    croppedVertices->showColors(importColors && origVertices->colorsShown());
                                    croppedVertices->showSF(importSFs && origVertices->sfShown());
                                    croppedMesh->showColors(importColors && mesh->colorsShown());
                                    croppedMesh->showSF(importSFs && mesh->sfShown());
                                }
                            }

                            //per-triangle features (materials)
                            if (mesh->hasMaterials())
                            {
                                const ccMaterialSet* origMaterialSet = mesh->getMaterialSet();
                                assert(origMaterialSet);

                                if (origMaterialSet && !origMaterialSet->empty() && croppedMesh->reservePerTriangleMtlIndexes())
                                {
                                    std::vector<int> materialUsed(origMaterialSet->size(),-1);

                                    //per-triangle materials
                                    for (unsigned i = 0; i < croppedMesh->size(); ++i)
                                    {
                                        //get the origin triangle
                                        unsigned origTriIndex = origTriIndexes[i];
                                        int mtlIndex = mesh->getTriangleMtlIndex(origTriIndex);
                                        croppedMesh->addTriangleMtlIndex(mtlIndex);

                                        if (mtlIndex >= 0)
                                            materialUsed[mtlIndex] = 1;
                                    }

                                    //import materials
                                    {
                                        size_t materialUsedCount = 0;
                                        {
                                            for (size_t i = 0; i < materialUsed.size(); ++i)
                                                if (materialUsed[i] == 1)
                                                    ++materialUsedCount;
                                        }

                                        if (materialUsedCount == materialUsed.size())
                                        {
                                            //nothing to do, we use all input materials
                                            croppedMesh->setMaterialSet(origMaterialSet->clone());
                                        }
                                        else
                                        {
                                            //create a subset of the input materials
                                            ccMaterialSet* matSet = new ccMaterialSet(origMaterialSet->getName());
                                            {
                                                matSet->reserve(materialUsedCount);
                                                for (size_t i = 0; i < materialUsed.size(); ++i)
                                                {
                                                    if (materialUsed[i] >= 0)
                                                    {
                                                        matSet->push_back(ccMaterial::Shared(new ccMaterial(*origMaterialSet->at(i))));
                                                        //update index
                                                        materialUsed[i] = static_cast<int>(matSet->size()) - 1;
                                                    }
                                                }
                                            }
                                            croppedMesh->setMaterialSet(matSet);

                                            //and update the materials indexes!
                                            for (unsigned i = 0; i < croppedMesh->size(); ++i)
                                            {
                                                int mtlIndex = croppedMesh->getTriangleMtlIndex(i);
                                                if (mtlIndex >= 0)
                                                {
                                                    assert(mtlIndex < static_cast<int>(materialUsed.size()));
                                                    croppedMesh->setTriangleMtlIndex(i,materialUsed[mtlIndex]);
                                                }
                                            }
                                        }
                                    }

                                    croppedMesh->showMaterials(mesh->materialsShown());
                                }
                                else
                                {
                                    ccLog::Warning("[Crop] Failed to transfer materials on the output mesh (not enough memory)");
                                }

                                //per-triangle texture coordinates
                                if (mesh->hasPerTriangleTexCoordIndexes())
                                {
                                    TextureCoordsContainer* texCoords = new TextureCoordsContainer;
                                    if (    croppedMesh->reservePerTriangleTexCoordIndexes()
                                        &&  texCoords->reserveSafe(croppedMesh->size()*3))
                                    {
                                        //for each new triangle
                                        for (unsigned i = 0; i < croppedMesh->size(); ++i)
                                        {
                                            //get the origin triangle
                                            unsigned origTriIndex = origTriIndexes[i];
                                            TexCoords2D* tx1 = nullptr;
                                            TexCoords2D* tx2 = nullptr;
                                            TexCoords2D* tx3 = nullptr;
                                            mesh->getTriangleTexCoordinates(origTriIndex, tx1, tx2, tx3);

                                            //get the new triangle
                                            const CCCoreLib::VerticesIndexes* tsic = croppedMesh->getTriangleVertIndexes(i);

                                            //for each vertex of the new triangle
                                            int texIndexes[3] = { -1, -1, -1 };
                                            for (unsigned j = 0; j < 3; ++j)
                                            {
                                                unsigned vertIndex = tsic->i[j];
                                                const CCVector3* Vcj = croppedVertices->getPoint(vertIndex);

                                                //intepolation weights
                                                CCVector3d w;
                                                mesh->computeInterpolationWeights(origTriIndex, *Vcj, w);

                                                if (    (tx1 || CCCoreLib::LessThanEpsilon( w.u[0] ) )
                                                    &&  (tx2 || CCCoreLib::LessThanEpsilon( w.u[1] ) )
                                                    &&  (tx3 || CCCoreLib::LessThanEpsilon( w.u[2] ) ) )
                                                {
                                                    TexCoords2D t(  static_cast<float>((tx1 ? tx1->tx*w.u[0] : 0.0) + (tx2 ? tx2->tx*w.u[1] : 0.0) + (tx3 ? tx3->tx*w.u[2] : 0.0)),
                                                                    static_cast<float>((tx1 ? tx1->ty*w.u[0] : 0.0) + (tx2 ? tx2->ty*w.u[1] : 0.0) + (tx3 ? tx3->ty*w.u[2] : 0.0)) );

                                                    texCoords->addElement(t);
                                                    texIndexes[j] = static_cast<int>(texCoords->currentSize()) - 1;
                                                }
                                            }

                                            croppedMesh->addTriangleTexCoordIndexes(texIndexes[0], texIndexes[1], texIndexes[2]);
                                        }
                                        croppedMesh->setTexCoordinatesTable(texCoords);
                                    }
                                    else
                                    {
                                        ccLog::Warning("[Crop] Failed to transfer texture coordinates on the output mesh (not enough memory)");
                                        delete texCoords;
                                        texCoords = nullptr;
                                    }
                                }
                            }
                        }
                        catch (const std::bad_alloc&)
                        {
                            ccLog::Warning("[Crop] Failed to transfer per-vertex features (color, SF values, etc.) on the output mesh (not enough memory)");
                            croppedVertices->unallocateColors();
                            croppedVertices->deleteAllScalarFields();
                        }
                    }
                }
            }
            else
            {
                ccLog::Warning("[Crop] Failed to create output mesh vertices (not enough memory)");
            }
        }

        //clean memory
        if (params.insideMesh)
        {
            delete params.insideMesh;
            params.insideMesh = nullptr;
        }
        if (params.outsideMesh)
        {
            delete params.outsideMesh;
            params.outsideMesh = nullptr;
        }

        if (croppedMesh)
        {
            croppedMesh->setDisplay_recursive(entity->getDisplay());
        }
        return croppedMesh;
    }

    //unhandled entity
    ccLog::Warning("[Crop] Unhandled entity type");
    return nullptr;
}

//! see ccClippingBoxTool.cpp
ccHObject* GetSlice_(ccHObject* obj, ccClipBox* clipBox, bool silent)
{
    CCTRACE("GetSlice_");
    assert(clipBox);
    if (!obj)
    {
        assert(false);
        return nullptr;
    }

    if (obj->isKindOf(CC_TYPES::POINT_CLOUD))
    {
        CCTRACE("cloud");
        ccGenericPointCloud* inputCloud = ccHObjectCaster::ToGenericPointCloud(obj);

        ccGenericPointCloud::VisibilityTableType selectionTable;
        try
        {
            selectionTable.resize(inputCloud->size());
        }
        catch (const std::bad_alloc&)
        {
            CCTRACE("Not enough memory!");
            return nullptr;
        }
        clipBox->flagPointsInside(inputCloud, &selectionTable);

        ccGenericPointCloud* sliceCloud = inputCloud->createNewCloudFromVisibilitySelection(false, &selectionTable, nullptr, true);
        if (!sliceCloud)
        {
            CCTRACE("Not enough memory!");
        }
        else if (sliceCloud->size() == 0)
        {
            CCTRACE("empty slice!");
            delete sliceCloud;
            sliceCloud = nullptr;
        }
        return sliceCloud;
    }
    else if (obj->isKindOf(CC_TYPES::MESH))
    {
        CCTRACE("mesh");
        const ccGLMatrix* _transformation = nullptr;
        ccGLMatrix transformation;
        if (clipBox->isGLTransEnabled())
        {
            transformation = clipBox->getGLTransformation().inverse();
            _transformation = &transformation;
        }

        const ccBBox& cropBox = clipBox->getBox();
        ccHObject* mesh = Crop_(obj, cropBox, true, _transformation);
        if (!mesh)
        {
            CCTRACE("Failed to segment the mesh!");
            return nullptr;
        }
        return mesh;
    }
    CCTRACE("neither cloud nor mesh?");
    return nullptr;
}

unsigned ComputeGridDimensions_(const ccBBox& localBox,
                                const bool processDim[3],
                                int indexMins[3],
                                int indexMaxs[3],
                                int gridDim[3],
                                const CCVector3& gridOrigin,
                                const CCVector3& cellSizePlusGap)
{
    CCTRACE("ComputeGridDimensions_");
    //compute 'grid' extents in the local clipping box ref.
    for (int i = 0; i < 3; ++i)
    {
        indexMins[i] = 0;
        indexMaxs[i] = 0;
        gridDim[i]   = 1;
    }
    unsigned cellCount = 1;

    for (unsigned char d = 0; d < 3; ++d)
    {
        if (processDim[d])
        {
            if (CCCoreLib::LessThanEpsilon(cellSizePlusGap.u[d]))
            {
                CCTRACE("Box size (plus gap) is null! Can't apply repetitive process!");
                return 0;
            }

            PointCoordinateType a = (localBox.minCorner().u[d] - gridOrigin.u[d]) / cellSizePlusGap.u[d]; //don't forget the user defined gap between 'cells'
            PointCoordinateType b = (localBox.maxCorner().u[d] - gridOrigin.u[d]) / cellSizePlusGap.u[d];

            indexMins[d] = static_cast<int>(floor(a + static_cast<PointCoordinateType>(1.0e-6)));
            indexMaxs[d] = static_cast<int>(ceil(b - static_cast<PointCoordinateType>(1.0e-6))) - 1;

            assert(indexMaxs[d] >= indexMins[d]);
            gridDim[d] = std::max(indexMaxs[d] - indexMins[d] + 1, 1);
            cellCount *= static_cast<unsigned>(gridDim[d]);
        }
    }

    return cellCount;
}

//! see ccClippingBoxTool::ExtractSlicesAndContours
bool ExtractSlicesAndContoursClone
(
    const std::vector<ccGenericPointCloud*>& clouds,
    const std::vector<ccGenericMesh*>& meshes,
    ccClipBox& clipBox,
    bool singleSliceMode,
    bool repeatDimensions[3],
    std::vector<ccHObject*>& outputSlices,

    bool extractEnvelopes,
    PointCoordinateType maxEdgeLength,
    Envelope_Type envelopeType,
    std::vector<ccPolyline*>& outputEnvelopes,

    bool extractLevelSet,
    double levelSetGridStep,
    int levelSetMinVertCount,
    std::vector<ccPolyline*>& levelSet,

    PointCoordinateType gap/*=0*/,
    bool multiPass/*=false*/,
    bool splitEnvelopes/*=false*/,
    bool projectOnBestFitPlane/*=false*/,
    bool visualDebugMode/*=false*/,
    bool generateRandomColors/*=false*/,
    ccProgressDialog* progressDialog/*=nullptr*/)
{
    CCTRACE("ExtractSlicesAndContours");
    //check input
    if (clouds.empty() && meshes.empty())
    {
        CCTRACE("no clouds, no meshes!");
        return false;
    }

    //repeat dimensions
    int repeatDimensionsSum = static_cast<int>(repeatDimensions[0])
                            + static_cast<int>(repeatDimensions[1])
                            + static_cast<int>(repeatDimensions[2]);

    if (!singleSliceMode && repeatDimensionsSum == 0)
    {
        CCTRACE("No dimension selected to repeat the segmentation process?!");
        return false;
    }

    if (extractLevelSet && repeatDimensionsSum != 1)
    {
        CCTRACE("Only one repeat/flat dimension should be defined for level set extraction");
        extractLevelSet = false;
    }

    //compute the cloud bounding box in the local clipping box ref.
    ccGLMatrix localTrans;
    {
        CCTRACE("isGLTransEnabled:" << clipBox.isGLTransEnabled());
        if (clipBox.isGLTransEnabled())
            localTrans = clipBox.getGLTransformation().inverse();
        else
            localTrans.toIdentity();
    }

    CCVector3 gridOrigin = clipBox.getOwnBB().minCorner();
    CCVector3 cellSize = clipBox.getOwnBB().getDiagVec();
    CCVector3 cellSizePlusGap = cellSize + CCVector3(gap, gap, gap);

    //apply process
    try
    {
        bool error = false;
        bool warningsIssued = false;
        size_t cloudSliceCount = 0;

        if (singleSliceMode)
        {
            CCTRACE("singleSliceMode");
            //single slice: easy
            outputSlices.reserve(clouds.size());
            for (size_t ci = 0; ci != clouds.size(); ++ci)
            {
                ccHObject* slice = GetSlice_(clouds[ci], &clipBox, false);
                if (slice)
                {
                    CCTRACE("a sclice");
                    slice->setName(clouds[ci]->getName() + QString(".slice"));

                    //set meta-data
                    slice->setMetaData(s_originEntityUUID_, clouds[ci]->getUniqueID());
                    slice->setMetaData(s_sliceID_, "slice");
                    if (slice->isKindOf(CC_TYPES::POINT_CLOUD))
                    {
                        slice->setMetaData("slice.origin.dim(0)", gridOrigin.x);
                        slice->setMetaData("slice.origin.dim(1)", gridOrigin.y);
                        slice->setMetaData("slice.origin.dim(2)", gridOrigin.z);
                    }

                    outputSlices.push_back(slice);
                }
            }

            if (outputSlices.empty())
            {
                CCTRACE("outputSlices.empty");
                //error message already issued
                return false;
            }
            cloudSliceCount = outputSlices.size();
        }
        else //repeat mode
        {
            CCTRACE("repeat mode");
            if (!clouds.empty()) //extract sections from clouds
            {
                //compute 'grid' extents in the local clipping box ref.
                ccBBox localBox;
                for (ccGenericPointCloud* cloud : clouds)
                {
                    for (unsigned i = 0; i < cloud->size(); ++i)
                    {
                        CCVector3 P = *cloud->getPoint(i);
                        localTrans.apply(P);
                        localBox.add(P);
                    }
                }

                int indexMins[3]{ 0, 0, 0 };
                int indexMaxs[3]{ 0, 0, 0 };
                int gridDim[3]{ 0, 0, 0 };
                unsigned cellCount = ComputeGridDimensions_(localBox, repeatDimensions, indexMins, indexMaxs,
                                                            gridDim, gridOrigin, cellSizePlusGap);

                //we'll potentially create up to one (ref.) cloud per input loud and per cell
                std::vector<CCCoreLib::ReferenceCloud*> refClouds;
                refClouds.resize(cellCount * clouds.size(), nullptr);

                unsigned subCloudsCount = 0;

                //project points into grid
                for (size_t ci = 0; ci != clouds.size(); ++ci)
                {
                    ccGenericPointCloud* cloud = clouds[ci];
                    unsigned pointCount = cloud->size();

                    for (unsigned i = 0; i < pointCount; ++i)
                    {
                        CCVector3 P = *cloud->getPoint(i);
                        localTrans.apply(P);

                        //relative coordinates (between 0 and 1)
                        P -= gridOrigin;
                        P.x /= cellSizePlusGap.x;
                        P.y /= cellSizePlusGap.y;
                        P.z /= cellSizePlusGap.z;

                        int xi = static_cast<int>(floor(P.x));
                        xi = std::min(std::max(xi, indexMins[0]), indexMaxs[0]);
                        int yi = static_cast<int>(floor(P.y));
                        yi = std::min(std::max(yi, indexMins[1]), indexMaxs[1]);
                        int zi = static_cast<int>(floor(P.z));
                        zi = std::min(std::max(zi, indexMins[2]), indexMaxs[2]);

                        if (gap == 0 ||
                            (   (P.x - static_cast<PointCoordinateType>(xi))*cellSizePlusGap.x <= cellSize.x
                            &&  (P.y - static_cast<PointCoordinateType>(yi))*cellSizePlusGap.y <= cellSize.y
                            &&  (P.z - static_cast<PointCoordinateType>(zi))*cellSizePlusGap.z <= cellSize.z))
                        {
                            int cloudIndex = ((zi - indexMins[2]) * static_cast<int>(gridDim[1]) + (yi - indexMins[1])) * static_cast<int>(gridDim[0]) + (xi - indexMins[0]);
                            assert(cloudIndex >= 0 && static_cast<size_t>(cloudIndex)* clouds.size() + ci < refClouds.size());

                            CCCoreLib::ReferenceCloud*& destCloud = refClouds[cloudIndex * clouds.size() + ci];
                            if (!destCloud)
                            {
                                destCloud = new CCCoreLib::ReferenceCloud(cloud);
                                ++subCloudsCount;
                            }

                            if (!destCloud->addPointIndex(i))
                            {
                                CCTRACE("Not enough memory!");
                                error = true;
                                break;
                            }
                        }
                    }

                } //project points into grid

                //reset count
                subCloudsCount = 0;

                //now create the real clouds
                for (int i = indexMins[0]; i <= indexMaxs[0]; ++i)
                {
                    for (int j = indexMins[1]; j <= indexMaxs[1]; ++j)
                    {
                        for (int k = indexMins[2]; k <= indexMaxs[2]; ++k)
                        {
                            int cloudIndex = ((k - indexMins[2]) * static_cast<int>(gridDim[1]) + (j - indexMins[1])) * static_cast<int>(gridDim[0]) + (i - indexMins[0]);
                            assert(cloudIndex >= 0 && static_cast<size_t>(cloudIndex)* clouds.size() < refClouds.size());

                            for (size_t ci = 0; ci != clouds.size(); ++ci)
                            {
                                ccGenericPointCloud* cloud = clouds[ci];
                                CCCoreLib::ReferenceCloud* destCloud = refClouds[cloudIndex * clouds.size() + ci];
                                if (destCloud) //some slices can be empty!
                                {
                                    //generate slice from previous selection
                                    int warnings = 0;
                                    ccPointCloud* sliceCloud = cloud->isA(CC_TYPES::POINT_CLOUD) ? static_cast<ccPointCloud*>(cloud)->partialClone(destCloud, &warnings) : ccPointCloud::From(destCloud, cloud);
                                    warningsIssued |= (warnings != 0);

                                    if (sliceCloud)
                                    {
                                        if (generateRandomColors)
                                        {
                                            ccColor::Rgb col = ccColor::Generator::Random();
                                            if (!sliceCloud->setColor(col))
                                            {
                                                CCTRACE("Not enough memory!");
                                                error = true;
                                                i = indexMaxs[0];
                                                j = indexMaxs[1];
                                                k = indexMaxs[2];
                                            }
                                            sliceCloud->showColors(true);
                                        }

                                        sliceCloud->setEnabled(true);
                                        sliceCloud->setVisible(true);
                                        sliceCloud->setDisplay(cloud->getDisplay());

                                        CCVector3 cellOrigin(   gridOrigin.x + i * cellSizePlusGap.x,
                                                                gridOrigin.y + j * cellSizePlusGap.y,
                                                                gridOrigin.z + k * cellSizePlusGap.z);
                                        QString slicePosStr = QString("(%1 ; %2 ; %3)").arg(cellOrigin.x).arg(cellOrigin.y).arg(cellOrigin.z);
                                        sliceCloud->setName(cloud->getName() + QString(".slice @ ") + slicePosStr);

                                        //set meta-data
                                        sliceCloud->setMetaData(s_originEntityUUID_, cloud->getUniqueID());
                                        sliceCloud->setMetaData(s_sliceID_, slicePosStr);
                                        sliceCloud->setMetaData("slice.origin.dim(0)", cellOrigin.x);
                                        sliceCloud->setMetaData("slice.origin.dim(1)", cellOrigin.y);
                                        sliceCloud->setMetaData("slice.origin.dim(2)", cellOrigin.z);

                                        //add slice to group
                                        outputSlices.push_back(sliceCloud);
                                        ++subCloudsCount;
                                    }
                                }
                            }
                        }
                    }
                } //now create the real clouds

                //release memory
                {
                    for (size_t i = 0; i < refClouds.size(); ++i)
                        if (refClouds[i])
                            delete refClouds[i];
                    refClouds.clear();
                }

                cloudSliceCount = outputSlices.size();

            } //extract sections from clouds

            if (!meshes.empty()) //extract sections from meshes
            {
                //compute 'grid' extents in the local clipping box ref.
                ccBBox localBox;
                for (ccGenericMesh* mesh : meshes)
                {
                    ccGenericPointCloud* cloud = mesh->getAssociatedCloud();
                    for (unsigned i = 0; i < cloud->size(); ++i)
                    {
                        CCVector3 P = *cloud->getPoint(i);
                        localTrans.apply(P);
                        localBox.add(P);
                    }
                }

                int indexMins[3]{ 0, 0, 0 };
                int indexMaxs[3]{ 0, 0, 0 };
                int gridDim[3]{ 0, 0, 0 };
                unsigned cellCount = ComputeGridDimensions_(localBox, repeatDimensions, indexMins, indexMaxs, gridDim, gridOrigin, cellSizePlusGap);

                const ccGLMatrix* _transformation = nullptr;
                ccGLMatrix transformation;
                if (clipBox.isGLTransEnabled())
                {
                    transformation = clipBox.getGLTransformation().inverse();
                    _transformation = &transformation;
                }

                //now extract the slices
                for (int i = indexMins[0]; i <= indexMaxs[0]; ++i)
                {
                    for (int j = indexMins[1]; j <= indexMaxs[1]; ++j)
                    {
                        for (int k = indexMins[2]; k <= indexMaxs[2]; ++k)
                        {
                            int sliceIndex = ((k - indexMins[2]) * static_cast<int>(gridDim[1]) + (j - indexMins[1])) * static_cast<int>(gridDim[0]) + (i - indexMins[0]);

                            CCVector3 C = gridOrigin + CCVector3(i*cellSizePlusGap.x, j*cellSizePlusGap.y, k*cellSizePlusGap.z);
                            ccBBox cropBox(C, C + cellSize, true);

                            for (size_t mi = 0; mi != meshes.size(); ++mi)
                            {
                                ccGenericMesh* mesh = meshes[mi];
                                ccHObject* croppedEnt = Crop_(mesh, cropBox, true, _transformation);
                                if (croppedEnt)
                                {
                                    if (generateRandomColors)
                                    {
                                        ccPointCloud* croppedVertices = ccHObjectCaster::ToPointCloud(mesh->getAssociatedCloud());
                                        if (croppedVertices)
                                        {
                                            ccColor::Rgb col = ccColor::Generator::Random();
                                            if (!croppedVertices->setColor(col))
                                            {
                                                CCTRACE("Not enough memory!");
                                                error = true;
                                                i = indexMaxs[0];
                                                j = indexMaxs[1];
                                                k = indexMaxs[2];
                                            }
                                            croppedVertices->showColors(true);
                                            mesh->showColors(true);
                                        }
                                    }

                                    croppedEnt->setEnabled(true);
                                    croppedEnt->setVisible(true);
                                    croppedEnt->setDisplay(mesh->getDisplay());

                                    QString slicePosStr = QString("(%1 ; %2 ; %3)").arg(C.x).arg(C.y).arg(C.z);
                                    croppedEnt->setName(mesh->getName() + QString(".slice @ ") + slicePosStr);

                                    //set meta-data
                                    croppedEnt->setMetaData(s_originEntityUUID_, mesh->getUniqueID());
                                    croppedEnt->setMetaData(s_sliceID_, slicePosStr);
                                    croppedEnt->setMetaData("slice.origin.dim(0)", C.x);
                                    croppedEnt->setMetaData("slice.origin.dim(1)", C.y);
                                    croppedEnt->setMetaData("slice.origin.dim(2)", C.z);

                                    //add slice to group
                                    outputSlices.push_back(croppedEnt);
                                }
                            }
                        }
                    }
                }
            } //extract sections from meshes

        } //repeat mode

        //extract level set (optionaly)
        if (!error && extractLevelSet && cloudSliceCount != 0)
        {
            for (int iteration = 0; iteration < 1; ++iteration) //fake loop for easy break
            {
                CCTRACE("extract level set");

                int Z = 2;
                assert(repeatDimensionsSum == 1);
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        if (repeatDimensions[i])
                        {
                            Z = i;
                            break;
                        }
                    }
                }
                int X = (Z == 2 ? 0 : Z + 1);
                int Y = (X == 2 ? 0 : X + 1);

                CCVector3 gridOrigin = clipBox.getOwnBB().minCorner();
                CCVector3 gridSize = clipBox.getOwnBB().getDiagVec();
                ccGLMatrix globalTrans = localTrans.inverse();

                assert(false == CCCoreLib::LessThanEpsilon(levelSetGridStep));
                unsigned gridWidth = 1 + static_cast<unsigned>(gridSize.u[X] / levelSetGridStep + 0.5);
                unsigned gridHeight = 1 + static_cast<unsigned>(gridSize.u[Y] / levelSetGridStep + 0.5);

                //add a margin to avoid issues in the level set generation
                gridWidth += 2;
                gridHeight += 2;
                gridOrigin.u[X] -= levelSetGridStep;
                gridOrigin.u[Y] -= levelSetGridStep;

                ccRasterGrid grid;
                if (!grid.init(gridWidth, gridHeight, levelSetGridStep, CCVector3d(0, 0, 0)))
                {
                    CCTRACE("Not enough memory!");
                    error = true;
                    break;
                }

                //process all the slices originating from point clouds
                assert(cloudSliceCount <= outputSlices.size());
                for (size_t i = 0; i < cloudSliceCount; ++i)
                {
                    ccPointCloud* sliceCloud = ccHObjectCaster::ToPointCloud(outputSlices[i]);
                    assert(sliceCloud);

                    double sliceZ = sliceCloud->getMetaData(QString("slice.origin.dim(%1)").arg(Z)).toDouble();
                    sliceZ += gridSize.u[Z] / 2;

                    //grid.reset();
                    for (ccRasterGrid::Row& row : grid.rows)
                    {
                        for (ccRasterCell& cell : row)
                        {
                            cell.h = 0.0;
                            cell.nbPoints = 0;
                        }
                    }

                    //project the slice in 2D
                    for (unsigned pi = 0; pi != sliceCloud->size(); ++pi)
                    {
                        CCVector3 relativePos = *sliceCloud->getPoint(pi);
                        localTrans.apply(relativePos);
                        relativePos -= gridOrigin;

                        int i = static_cast<int>(relativePos.u[X] / levelSetGridStep + 0.5);
                        int j = static_cast<int>(relativePos.u[Y] / levelSetGridStep + 0.5);

                        //we skip points that fall outside of the grid!
                        if (    i < 0 || i >= static_cast<int>(gridWidth)
                            ||  j < 0 || j >= static_cast<int>(gridHeight))
                        {
                            //there shouldn't be any actually
                            assert(false);
                            continue;
                        }

                        ccRasterCell& cell = grid.rows[j][i];
                        cell.h = 1.0;
                        ++cell.nbPoints;
                    }

                    grid.updateNonEmptyCellCount();
                    grid.updateCellStats();
                    grid.setValid(true);

                    //now extract the contour lines
                    ccContourLinesGenerator_::Parameters params;
                    params.emptyCellsValue = std::numeric_limits<double>::quiet_NaN();
                    params.minVertexCount = levelSetMinVertCount;
                    params.parentWidget = nullptr; //progressDialog->parentWidget();
                    params.startAltitude = 0.0;
                    params.maxAltitude = 1.0;
                    params.step = 1.0;

                    std::vector<ccPolyline*> contours;
                    if (ccContourLinesGenerator_::GenerateContourLines(&grid, CCVector2d(gridOrigin.u[X],
                                                                                         gridOrigin.u[Y]),
                                                                       params, contours))
                    {
                        for (size_t k = 0; k < contours.size(); ++k)
                        {
                            ccPolyline* poly = contours[k];
                            CCCoreLib::GenericIndexedCloudPersist* vertices = poly->getAssociatedCloud();
                            for (unsigned pi = 0; pi < vertices->size(); ++pi)
                            {
                                //convert the vertices from the local coordinate system to the global one
                                const CCVector3* Pconst = vertices->getPoint(pi);
                                CCVector3 P;
                                P.u[X] = Pconst->x;
                                P.u[Y] = Pconst->y;
                                P.u[Z] = sliceZ;
                                *const_cast<CCVector3*>(Pconst) = globalTrans * P;
                            }

                            static char s_dimNames[3] = { 'X', 'Y', 'Z' };
                            poly->setName(QString("Contour line %1=%2 (#%3)").arg(s_dimNames[Z]).arg(sliceZ).arg(k + 1));
                            poly->copyGlobalShiftAndScale(*sliceCloud);
                            poly->setMetaData(ccPolyline::MetaKeyConstAltitude(), QVariant(sliceZ)); //replace the 'altitude' meta-data by the right value

                            //set meta-data
                            poly->setMetaData(s_originEntityUUID_, sliceCloud->getMetaData(s_originEntityUUID_));
                            poly->setMetaData(s_sliceID_, sliceCloud->getMetaData(s_sliceID_));
                            poly->setMetaData("slice.origin.dim(0)", sliceCloud->getMetaData("slice.origin.dim(0)"));
                            poly->setMetaData("slice.origin.dim(1)", sliceCloud->getMetaData("slice.origin.dim(1)"));
                            poly->setMetaData("slice.origin.dim(2)", sliceCloud->getMetaData("slice.origin.dim(2)"));

                            levelSet.push_back(poly);
                        }
                    }
                    else
                    {
                        CCTRACE("Failed to generate contour lines for cloud #" << (i+1));
                    }
                }
            }
        }

        //extract envelopes as polylines (optionaly)
        if (!error && extractEnvelopes && cloudSliceCount != 0)
        {
            CCTRACE("extract envelopes as polylines");

            //preferred dimension?
            PointCoordinateType* preferredNormDir = nullptr;
            PointCoordinateType* preferredUpDir = nullptr;
            if (repeatDimensionsSum == 1)
            {
                for (int i = 0; i < 3; ++i)
                {
                    if (repeatDimensions[i])
                    {
                        ccGLMatrix invLocalTrans = localTrans.inverse();
                        if (!projectOnBestFitPlane) //otherwise the normal will be automatically computed
                            preferredNormDir = invLocalTrans.getColumn(i);
                        preferredUpDir = invLocalTrans.getColumn(i < 2 ? 2 : 0);
                        break;
                    }
                }
            }

            assert(cloudSliceCount <= outputSlices.size());

            //process all the slices originating from point clouds
            for (size_t i = 0; i < cloudSliceCount; ++i)
            {
                ccPointCloud* sliceCloud = ccHObjectCaster::ToPointCloud(outputSlices[i]);
                assert(sliceCloud);

                std::vector<ccPolyline*> polys;
                if (ExtractFlatEnvelope_(sliceCloud,
                    multiPass,
                    maxEdgeLength,
                    polys,
                    envelopeType,
                    splitEnvelopes,
                    preferredNormDir,
                    preferredUpDir,
                    visualDebugMode))
                {
                    if (!polys.empty())
                    {
                        for (size_t p = 0; p < polys.size(); ++p)
                        {
                            ccPolyline* poly = polys[p];
                            poly->setColor(ccColor::green);
                            poly->showColors(true);
                            poly->setGlobalScale(sliceCloud->getGlobalScale());
                            poly->setGlobalShift(sliceCloud->getGlobalShift());
                            QString envelopeName = sliceCloud->getName();
                            envelopeName.replace("slice", "envelope");
                            if (polys.size() > 1)
                            {
                                envelopeName += QString(" (part %1)").arg(p + 1);
                            }
                            poly->setName(envelopeName);

                            //set meta-data
                            poly->setMetaData(s_originEntityUUID_, sliceCloud->getMetaData(s_originEntityUUID_));
                            poly->setMetaData(s_sliceID_, sliceCloud->getMetaData(s_sliceID_));
                            poly->setMetaData("slice.origin.dim(0)", sliceCloud->getMetaData("slice.origin.dim(0)"));
                            poly->setMetaData("slice.origin.dim(1)", sliceCloud->getMetaData("slice.origin.dim(1)"));
                            poly->setMetaData("slice.origin.dim(2)", sliceCloud->getMetaData("slice.origin.dim(2)"));

                            outputEnvelopes.push_back(poly);
                        }
                    }
                    else
                    {
                        CCTRACE(sliceCloud->getName().toStdString() << " : points are too far from each other! Increase the max edge length");
                        warningsIssued = true;
                    }
                }
                else
                {
                    CCTRACE(sliceCloud->getName().toStdString() << " : envelope extraction failed!");
                    warningsIssued = true;
                }
            }

        } //extract envelope polylines

        //release memory
        if (error) // || singleSliceMode)
        {
            for (ccHObject* slice : outputSlices)
            {
                delete slice;
            }
            outputSlices.resize(0);
        }

        if (error)
        {
            for (ccPolyline* poly : outputEnvelopes)
            {
                delete poly;
            }
            return false;
        }
        else if (warningsIssued)
        {
            CCTRACE("[ExtractSlicesAndContours] Warnings were issued during the process! (result may be incomplete)");
        }
    }
    catch (const std::bad_alloc&)
    {
        CCTRACE("Not enough memory!");
        return false;
    }

    return true;
}
// ===========================================================================

