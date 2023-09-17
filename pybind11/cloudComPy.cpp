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

#include "cloudComPy.hpp"

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"
#include <ccGLMatrix.h>
#include <ccHObject.h>
#include <ccPointCloud.h>
#include <ccOctree.h>
#include <ccMesh.h>
#include <ccFacet.h>
#include <ScalarField.h>
#include <ccNormalVectors.h>
#include <ccHObjectCaster.h>
#include <ccRasterGrid.h>
#include <ccClipBox.h>
#include <ccCommon.h>
#include <AutoSegmentationTools.h>
#include <ParallelSort.h>
#include <ccPointCloudInterpolator.h>
#include <ReferenceCloud.h>
#include <PointProjectionTools.h>

//#include <ccMainAppInterface.h>
#include <viewerPy.h>
#include <viewerPyApplication.h>

#include <QString>
#include <QSharedPointer>
#include <QApplication>
#include <vector>
#include <tuple>
#include <set>

#include "optdefines.h"

#include "pyccTrace.h"
#include "cloudComPy_DocStrings.hpp"

QString greet()
{
   return QString("Hello, World, this is CloudCompare Python Interface: 'CloudComPy'");
}

void initCC_py()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
    PyObject *cc_module;
    cc_module = PyImport_ImportModule("cloudComPy");
    const char* modulePath = PyUnicode_AsUTF8(PyModule_GetFilenameObject(cc_module));
    CCTRACE("modulePath: " <<  modulePath);
    initCC::init(modulePath);
}

void setTraces_py(bool isActive)
{
    if (isActive)
    {
        ccLogTrace::settrace(1);
    }
    else
    {
        ccLogTrace::settrace(0);
    }
}

 void initCloudCompare_py()
{
    pyCC* pyCCInternals = initCloudCompare();
}

const char* getScalarType()
{
    //Get the scalar type used in cloudCompare under the form defined in Numpy: 'float32' or 'float64'
    return CC_NPY_FLOAT_STRING;
}

struct ICPres
{
    ccPointCloud* aligned;
    ccGLMatrix transMat;
    double finalScale;
    double finalRMS;
    unsigned finalPointCount;
};

ICPres ICP_py(  ccHObject* data,
                ccHObject* model,
                double minRMSDecrease,
                unsigned maxIterationCount,
                unsigned randomSamplingLimit,
                bool removeFarthestPoints,
                CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE method,
                bool adjustScale,
                double finalOverlapRatio = 1.0,
                bool useDataSFAsWeights = false,
                bool useModelSFAsWeights = false,
                int transformationFilters = CCCoreLib::RegistrationTools::SKIP_NONE,
                int maxThreadCount = 0)
{
    ICPres a;
    ICP(data,
        model,
        a.transMat,
        a.finalScale,
        a.finalRMS,
        a.finalPointCount,
        minRMSDecrease,
        maxIterationCount,
        randomSamplingLimit,
        removeFarthestPoints,
        method,
        adjustScale,
        finalOverlapRatio,
        useDataSFAsWeights,
        useModelSFAsWeights,
        transformationFilters,
        maxThreadCount);
    a.aligned = dynamic_cast<ccPointCloud*>(data);
    return a;
}

py::tuple importFilePy(const char* filename,
    CC_SHIFT_MODE mode = AUTO,
    double x = 0,
    double y = 0,
    double z = 0,
    QString extraData = QString())
{
    std::vector<ccMesh*> meshes;
    std::vector<ccPointCloud*> clouds;
    std::vector<ccPolyline*> polys;
    std::vector<ccFacet*> facets;
    std::vector<QString> structure;
    std::vector<ccHObject*> entities = importFile(filename, mode, x, y, z, extraData, &structure);
    for( auto entity : entities)
    {
       ccMesh* mesh = ccHObjectCaster::ToMesh(entity);
        if (mesh)
        {
            meshes.push_back(mesh);
            continue;
        }
        ccPolyline* poly = ccHObjectCaster::ToPolyline(entity);
        if (poly)
        {
            polys.push_back(poly);
            continue;
        }
        ccFacet* facet = ccHObjectCaster::ToFacet(entity);
        if (facet)
        {
            facets.push_back(facet);
            continue;
        }
        ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(entity);
        if (cloud)
        {
            clouds.push_back(cloud);
            continue;
        }
    }
    py::tuple res = py::make_tuple(meshes, clouds, facets, polys, structure);
    return res;
}

ccPointCloud* loadPointCloudPy(
    const char* filename,
    CC_SHIFT_MODE mode = AUTO,
    int skip = 0,
    double x = 0,
    double y = 0,
    double z = 0,
    QString extraData = QString())
{
    std::vector<ccMesh*> meshes;
    std::vector<ccPointCloud*> clouds;
    std::vector<ccHObject*> entities = importFile(filename, mode, x, y, z, extraData);
    for( auto entity : entities)
    {
        ccMesh* mesh = ccHObjectCaster::ToMesh(entity);
        if (mesh)
        {
            meshes.push_back(mesh);
        }
        else
        {
            ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(entity);
            if (cloud)
            {
                clouds.push_back(cloud);
            }
        }
    }
    if (clouds.size())
        return clouds.back();
    else
        return nullptr;
}


ccMesh* loadMeshPy(
    const char* filename,
    CC_SHIFT_MODE mode = AUTO,
    int skip = 0,
    double x = 0,
    double y = 0,
    double z = 0,
    QString extraData = QString())
{
    std::vector<ccMesh*> meshes;
    std::vector<ccPointCloud*> clouds;
    std::vector<ccHObject*> entities = importFile(filename, mode, x, y, z, extraData);
    for( auto entity : entities)
    {
        ccMesh* mesh = ccHObjectCaster::ToMesh(entity);
        if (mesh)
        {
            meshes.push_back(mesh);
        }
        else
        {
            ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(entity);
            if (cloud)
            {
                clouds.push_back(cloud);
            }
        }
    }
    if (meshes.size())
        return meshes.back();
    else
        return nullptr;
}

void deleteEntity(ccHObject* entity)
{
    delete entity;
}

py::tuple ExtractSlicesAndContours_py
    (
    std::vector<ccHObject*> entities,
    ccBBox bbox,
    ccGLMatrix bboxTrans = ccGLMatrix(),
    bool singleSliceMode = true,
    bool processRepeatX =false,
    bool processRepeatY =false,
    bool processRepeatZ =true,
    bool extractEnvelopes = false,
    PointCoordinateType maxEdgeLength = 0,
    int envelopeType = 0,
    bool extractLevelSet = false,
    double levelSetGridStep = 0,
    int levelSetMinVertCount= 0,
    PointCoordinateType gap = 0,
    bool multiPass = false,
    bool splitEnvelopes = false,
    bool projectOnBestFitPlane = false,
    bool generateRandomColors = false)
{
    std::vector<ccGenericPointCloud*> clouds;
    std::vector<ccGenericMesh*> meshes;
    for(auto obj: entities)
    {
        if (obj->isKindOf(CC_TYPES::MESH))
        {
            ccMesh* mesh = ccHObjectCaster::ToMesh(obj);
            meshes.push_back(mesh);
        }
        else if(obj->isKindOf(CC_TYPES::POINT_CLOUD))
        {
            ccGenericPointCloud* cloud = ccHObjectCaster::ToGenericPointCloud(obj);
            clouds.push_back(cloud);
        }
    }
    CCTRACE("clouds: " << clouds.size() << " meshes: " << meshes.size());
    ccClipBox clipBox;
    clipBox.set(bbox, bboxTrans);
    clipBox.enableGLTransformation(true);
    bool processDimensions[3] = {processRepeatX, processRepeatY, processRepeatZ};

    Envelope_Type val[3] = {LOWER, UPPER, FULL};
    if (envelopeType <0) envelopeType = 0;
    if (envelopeType >2) envelopeType = 2;
    Envelope_Type envelType = val[envelopeType];

    std::vector<ccHObject*> outputSlices;
    std::vector<ccPolyline*> outputEnvelopes;
    std::vector<ccPolyline*> levelSet;
    ExtractSlicesAndContoursClone(clouds, meshes, clipBox, singleSliceMode, processDimensions, outputSlices,
                             extractEnvelopes, maxEdgeLength, envelType, outputEnvelopes,
                             extractLevelSet, levelSetGridStep, levelSetMinVertCount, levelSet,
                             gap, multiPass, splitEnvelopes, projectOnBestFitPlane, false, generateRandomColors, nullptr);
    py::tuple res = py::make_tuple(outputSlices, outputEnvelopes, levelSet);
    return res;
}

struct ComponentIndexAndSize
{
    unsigned index;
    unsigned size;

    ComponentIndexAndSize(unsigned i, unsigned s) : index(i), size(s) {}

    static bool DescendingCompOperator(const ComponentIndexAndSize& a, const ComponentIndexAndSize& b)
    {
        return a.size > b.size;
    }
};

//! from MainWindow::createComponentsClouds
std::tuple <std::vector<ccPointCloud*>, std::vector<ccPointCloud*>>
createComponentsClouds_(ccGenericPointCloud* cloud,
                        CCCoreLib::ReferenceCloudContainer& components,
                        unsigned minPointsPerComponent,
                        bool randomColors,
                        bool sortBysize=true)
{
    CCTRACE("createComponentsClouds_ " << randomColors);
    std::vector<ccPointCloud*> resultClouds;
    std::vector<ccPointCloud*> residualClouds;
    if (!cloud || components.empty())
        return {resultClouds, residualClouds};

    std::vector<ComponentIndexAndSize> sortedIndexes;
    std::vector<ComponentIndexAndSize>* _sortedIndexes = nullptr;
    if (sortBysize)
    {
        try
        {
            sortedIndexes.reserve(components.size());
        }
        catch (const std::bad_alloc&)
        {
            CCTRACE("[CreateComponentsClouds] Not enough memory to sort components by size!");
            sortBysize = false;
        }

        if (sortBysize) //still ok?
        {
            unsigned compCount = static_cast<unsigned>(components.size());
            for (unsigned i = 0; i < compCount; ++i)
            {
                sortedIndexes.emplace_back(i, components[i]->size());
            }

            ParallelSort(sortedIndexes.begin(), sortedIndexes.end(), ComponentIndexAndSize::DescendingCompOperator);

            _sortedIndexes = &sortedIndexes;
        }
    }

    //we create "real" point clouds for all input components
    {
        ccPointCloud* pc = cloud->isA(CC_TYPES::POINT_CLOUD) ? static_cast<ccPointCloud*>(cloud) : nullptr;
        CCCoreLib::ReferenceCloud* refCloud = new CCCoreLib::ReferenceCloud(cloud);
        ccPointCloud* residualCloud = nullptr;

        //for each component
        int nbComp =0;
        for (size_t i = 0; i < components.size(); ++i)
        {
            CCCoreLib::ReferenceCloud* compIndexes = _sortedIndexes ? components[_sortedIndexes->at(i).index] : components[i];

            //if it has enough points
            if (compIndexes->size() >= minPointsPerComponent)
            {
                //we create a new entity
                ccPointCloud* compCloud = (pc ? pc->partialClone(compIndexes) : ccPointCloud::From(compIndexes));
                if (compCloud)
                {
                    //shall we colorize it with random color?
                    if (randomColors)
                    {
                        ccColor::Rgb col = ccColor::Generator::Random();
                        compCloud->setColor(col);
                    }

                    //'shift on load' information
                    if (pc)
                    {
                        compCloud->copyGlobalShiftAndScale(*pc);
                    }
                    compCloud->setName(QString("CC#%1").arg(nbComp));

                    //we add new CC to group
                    resultClouds.push_back(compCloud);
                    nbComp++;
                }
                else
                {
                    CCTRACE("[CreateComponentsClouds] Failed to create component " << nbComp << "(not enough memory)");
                }
            }
            else
            {
                // regroup all small chunks in one entity
                unsigned numberOfPoints = compIndexes->size();
                for (unsigned i = 0; i < numberOfPoints; ++i)
                {
                    //add the point to the current component
                    if (!refCloud->addPointIndex(compIndexes->getPointGlobalIndex(i)))
                    {
                        //not enough memory
                        CCTRACE("not enough memory!");
                        delete refCloud;
                        refCloud = nullptr;
                        break;
                    }
                }
            }

            delete compIndexes;
            compIndexes = nullptr;
        }

        if (refCloud)
        {
            residualCloud = (pc ? pc->partialClone(refCloud) : ccPointCloud::From(refCloud));
            residualClouds.push_back(residualCloud);
            delete refCloud;
            refCloud = nullptr;
        }
        components.clear();

        if (nbComp == 0)
        {
            CCTRACE("No component was created! Check the minimum size...");
        }
        else
        {
            CCTRACE("[CreateComponentsClouds] " << nbComp << " component(s) were created from cloud " << cloud->getName().toStdString());
        }
    }
    return {resultClouds, residualClouds};
}

py::tuple ExtractConnectedComponents_py(std::vector<ccHObject*> entities,
                                        int octreeLevel=8,
                                        int minComponentSize=100,
                                        int maxNumberComponents=100,
                                        bool randomColors=false)
{
    CCTRACE("ExtractConnectedComponents_py");
    int realComponentCount = 0;
    int residualComponentCount = 0;
    int nbCloudDone = 0;

    std::vector<ccHObject*> resultComponents;
    std::vector<ccHObject*> residualComponents;
    py::tuple res = py::make_tuple(nbCloudDone, resultComponents, residualComponents);

    std::vector<ccGenericPointCloud*> clouds;
    {
        for ( ccHObject *entity : entities )
        {
            if (entity->isKindOf(CC_TYPES::POINT_CLOUD))
                clouds.push_back(ccHObjectCaster::ToGenericPointCloud(entity));
        }
    }

    size_t count = clouds.size();
    if (count == 0)
        return res;

    bool randColors = randomColors;

    for ( ccGenericPointCloud *cloud : clouds )
    {
        if (cloud && cloud->isA(CC_TYPES::POINT_CLOUD))
        {
            CCTRACE("cloud");
            ccPointCloud* pc = static_cast<ccPointCloud*>(cloud);

            ccOctree::Shared theOctree = cloud->getOctree();
            if (!theOctree)
            {
                theOctree = cloud->computeOctree(nullptr);
                if (!theOctree)
                {
                    CCTRACE("Couldn't compute octree for cloud " <<cloud->getName().toStdString());
                    break;
                }
            }

            //we create/activate CCs label scalar field
            int sfIdx = pc->getScalarFieldIndexByName(CC_CONNECTED_COMPONENTS_DEFAULT_LABEL_NAME);
            if (sfIdx < 0)
            {
                sfIdx = pc->addScalarField(CC_CONNECTED_COMPONENTS_DEFAULT_LABEL_NAME);
            }
            if (sfIdx < 0)
            {
                CCTRACE("Couldn't allocate a new scalar field for computing CC labels! Try to free some memory ...");
                break;
            }
            pc->setCurrentScalarField(sfIdx);

            //we try to label all CCs
            CCCoreLib::ReferenceCloudContainer components;
            int componentCount = CCCoreLib::AutoSegmentationTools::labelConnectedComponents(cloud,
                                                                                        static_cast<unsigned char>(octreeLevel),
                                                                                        false,
                                                                                        nullptr,
                                                                                        theOctree.data());

            if (componentCount >= 0)
            {
                //if successful, we extract each CC (stored in "components")

                pc->getCurrentInScalarField()->computeMinAndMax();
                if (!CCCoreLib::AutoSegmentationTools::extractConnectedComponents(cloud, components))
                {
                    CCTRACE("[ExtractConnectedComponents] Something went wrong while extracting CCs from cloud " << cloud->getName().toStdString());
                }

                //safety test
                {
                    for (size_t i = 0; i < components.size(); ++i)
                    {
                        if (components[i]->size() >= minComponentSize)
                        {
                            ++realComponentCount;
                        }
                        else
                        {
                            ++residualComponentCount;
                        }
                    }
                }
                CCTRACE("total components: " << componentCount << " with " << realComponentCount << " components of size > " << minComponentSize);
                CCTRACE("residual components count: " << residualComponentCount);

                if (realComponentCount > maxNumberComponents)
                {
                    //too many components
                    CCTRACE("Too many components: " << realComponentCount << " for a maximum of: " << maxNumberComponents);
                    CCTRACE("Extraction incomplete, modify some parameters and retry");
                }
            }
            else
            {
                CCTRACE("[ExtractConnectedComponents] Something went wrong while extracting CCs from cloud " << cloud->getName().toStdString());
            }

            //we delete the CCs label scalar field (we don't need it anymore)
            pc->deleteScalarField(sfIdx);
            sfIdx = -1;

            //we create "real" point clouds for all CCs
            if (!components.empty())
            {
                std::vector<ccPointCloud*> resultClouds;
                std::vector<ccPointCloud*> residualClouds;
                std::tie(resultClouds, residualClouds) = createComponentsClouds_(cloud, components, minComponentSize, randColors, true);
                for(int i=0; i<std::min(realComponentCount, maxNumberComponents); i++)
                    resultComponents.push_back(resultClouds[i]);
                for (ccPointCloud* cloud : residualClouds)
                    residualComponents.push_back(cloud);
            }
            nbCloudDone++;
            CCTRACE("nbCloudDone: " << nbCloudDone);
        }
    }
    res = py::make_tuple(nbCloudDone, resultComponents, residualComponents);
    return res;
}

int LabelConnectedComponents_py(std::vector<ccHObject*> entities,
                                int octreeLevel=8)
{
    CCTRACE("LabelConnectedComponents_py");
    int totalComponentCount = 0;
    int nbCloudDone = 0;

    std::vector<ccGenericPointCloud*> clouds;
    {
        for ( ccHObject *entity : entities )
        {
            if (entity->isKindOf(CC_TYPES::POINT_CLOUD))
                clouds.push_back(ccHObjectCaster::ToGenericPointCloud(entity));
        }
    }

    size_t count = clouds.size();
    if (count == 0)
        return 0;

    for ( ccGenericPointCloud *cloud : clouds )
    {
        if (cloud && cloud->isA(CC_TYPES::POINT_CLOUD))
        {
            CCTRACE("cloud");
            ccPointCloud* pc = static_cast<ccPointCloud*>(cloud);

            ccOctree::Shared theOctree = cloud->getOctree();
            if (!theOctree)
            {
                theOctree = cloud->computeOctree(nullptr);
                if (!theOctree)
                {
                    CCTRACE("Couldn't compute octree for cloud " <<cloud->getName().toStdString());
                    break;
                }
            }

            //we create/activate CCs label scalar field
            int sfIdx = pc->getScalarFieldIndexByName(CC_CONNECTED_COMPONENTS_DEFAULT_LABEL_NAME);
            if (sfIdx < 0)
            {
                sfIdx = pc->addScalarField(CC_CONNECTED_COMPONENTS_DEFAULT_LABEL_NAME);
            }
            if (sfIdx < 0)
            {
                CCTRACE("Couldn't allocate a new scalar field for computing CC labels! Try to free some memory ...");
                break;
            }
            pc->setCurrentScalarField(sfIdx);

            //we try to label all CCs
            CCTRACE("---");
            CCCoreLib::ReferenceCloudContainer components;
            int componentCount = CCCoreLib::AutoSegmentationTools::labelConnectedComponents(cloud,
                                                                                        static_cast<unsigned char>(octreeLevel),
                                                                                        false,
                                                                                        nullptr,
                                                                                        theOctree.data());
            totalComponentCount += componentCount;
        }
    }
    return totalComponentCount;
}

viewerPy* getOrInitializeViewer()
{
	CCTRACE("getOrInitializeViewer");
	QCoreApplication *coreApp = QCoreApplication::instance();
	viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
	if (!app)
	{
		CCTRACE("cannot find viewerPyApplication!");
		return nullptr;
	}
	viewerPy* w = app->getViewer();
	if (!w)
	{
	    CCTRACE("initialize viewerPy");
        w = new viewerPy();
        app->setViewer(w);
        CCTRACE("viewerPy initialized");
	}
    CCTRACE("--- w " << w);
	return w;
}

void setOrthoView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setOrthoView();
}

void setCenteredPerspectiveView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setCenteredPerspectiveView();
}

void setViewerPerspectiveView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setViewerPerspectiveView();
}

void setGlobalZoom()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setGlobalZoom();
}

void zoomOnSelectedEntity()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->zoomOnSelectedEntity();
}

void setFrontView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setFrontView();
}

void setBottomView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setBottomView();
}

void setTopView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setTopView();
}

void setBackView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setBackView();
}

void setLeftView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setLeftView();
}

void setRightView()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setRightView();
}

void setIsoView1()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setIsoView1();
}

void setIsoView2()
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setIsoView2();
}

void setCustomView(const CCVector3d& forward, const CCVector3d& up)
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setCustomView(forward, up);
}

void setCameraPos(const CCVector3d& P)
{
	viewerPy* w = getOrInitializeViewer();
	if (w) w->setCameraPos(P);

}

void addToRenderScene(ccHObject* obj, bool showScalar=true)
{
	CCTRACE("addToRenderScene");
	QCoreApplication *coreApp = QCoreApplication::instance();
	viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
	if (!app)
	{
		CCTRACE("cannot find viewerPyApplication!");
		return;
	}
	viewerPy* w = getOrInitializeViewer();

	w->addToDB(obj);
	w->show();

	w->selectEntity(obj);
	w->toggleScalarShown(showScalar);
	obj->setSelected(false);
}

void removeFromRenderScene(ccHObject* obj)
{
	CCTRACE("removeFromRenderScene");
	QCoreApplication *coreApp = QCoreApplication::instance();
	viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
	if (!app)
	{
		CCTRACE("cannot find viewerPyApplication!");
		return;
	}
	viewerPy* w = getOrInitializeViewer();
	w->removeFromDB(obj);
	w->show();
}

void setBackgroundColor(bool gradient=false, unsigned char r=255, unsigned char g=255, unsigned char b=255)
{
    QCoreApplication *coreApp = QCoreApplication::instance();
    viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
    if (!app)
    {
        CCTRACE("cannot find viewerPyApplication!");
        return;
    }
    viewerPy* w = getOrInitializeViewer();
    w->setBackgroundColor(gradient, r, g ,b);
}

void setTextDefaultCol(unsigned char r=0, unsigned char g=0, unsigned char b=0, unsigned char a=255)
{
    QCoreApplication *coreApp = QCoreApplication::instance();
    viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
    if (!app)
    {
        CCTRACE("cannot find viewerPyApplication!");
        return;
    }
    viewerPy* w = getOrInitializeViewer();
    w->setTextDefaultCol(r, g, b, a);
}

void setColorScaleShowHistogram(bool showHist=true)
{
    QCoreApplication *coreApp = QCoreApplication::instance();
    viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
    if (!app)
    {
        CCTRACE("cannot find viewerPyApplication!");
        return;
    }
    viewerPy* w = getOrInitializeViewer();
    w->setColorScaleShowHistogram(showHist);
}

void renderPy(QString filename, int width=1500, int height=1000,bool isInteractive=false)
{
	CCTRACE("renderPy");
	QCoreApplication *coreApp = QCoreApplication::instance();
	viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
	if (!app)
	{
		CCTRACE("cannot find viewerPyApplication!");
		return;
	}
	viewerPy* w = getOrInitializeViewer();
	w->resize(width+46, height+88);
	w->show();

	w->doActionRenderToFile(filename, isInteractive);
	app->exec();
}

bool InterpolateScalarFieldsFrom_py(ccPointCloud* destCloud,
                                    ccPointCloud* srcCloud,
                                    std::vector<int> sfIndexes,
                                    const ccPointCloudInterpolator::Parameters& params,
                                    unsigned char octreeLevel = 0)
{
    CCTRACE("InterpolateScalarFieldsFrom_py");
    return ccPointCloudInterpolator::InterpolateScalarFieldsFrom(destCloud, srcCloud, sfIndexes, params, nullptr, octreeLevel);
}

// from MainWindow::AddToRemoveList helper for MergePy
void AddToRemoveListPy(ccHObject* toRemove, ccHObject::Container& toBeRemovedList)
{
    // is a parent or sibling already in the "toBeRemoved" list?
    size_t count = toBeRemovedList.size();
    for (size_t j = 0; j < count;)
    {
        if (toBeRemovedList[j]->isAncestorOf(toRemove))
        {
            // nothing to do, we already have an ancestor
            return;
        }
        else if (toRemove->isAncestorOf(toBeRemovedList[j]))
        {
            // we don't need to keep the children
            toBeRemovedList[j] = toBeRemovedList.back();
            toBeRemovedList.pop_back();
            count--;
        }
        else
        {
            // forward
            ++j;
        }
    }

    toBeRemovedList.push_back(toRemove);
}

//! from MainWindow::doActionMerge
ccHObject* MergeEntitiesPy(std::vector<ccHObject*> entities,
                           bool deleteOriginalClouds=false,
                           bool createSFcloudIndex=false,
                           bool createSubMeshes=false)
{
    CCTRACE("MergeEntitiesPy");
    //let's look for clouds or meshes (warning: we don't mix them)
    std::vector<ccPointCloud*> clouds;
    std::vector<ccMesh*> meshes;

    try
    {
        for ( ccHObject *entity : entities )
        {
            if (!entity)
                continue;

            if (entity->isA(CC_TYPES::POINT_CLOUD))
            {
                ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(entity);
                clouds.push_back(cloud);

                // check whether this cloud is an ancestor of the first cloud in the selection
                if (clouds.size() > 1)
                {
                    if (clouds.back()->isAncestorOf(clouds.front()))
                    {
                        // this way we are sure that the first cloud is not below any other cloud
                        std::swap(clouds.front(), clouds.back());
                    }
                }
            }
            else if (entity->isKindOf(CC_TYPES::MESH))
            {
                ccMesh* mesh = ccHObjectCaster::ToMesh(entity);
                //this is a purely theoretical test for now!
                if (mesh && mesh->getAssociatedCloud() && mesh->getAssociatedCloud()->isA(CC_TYPES::POINT_CLOUD))
                {
                    meshes.push_back(mesh);
                }
                else
                {
                    CCTRACE("Only meshes with standard vertices are handled for now! Can't merge entity " << entity->getName().toStdString());
                }
            }
            else
            {
                CCTRACE("Entity " << entity->getName().toStdString() << " is neither a cloud nor a mesh, can't merge it!");
            }
        }
    }
    catch (const std::bad_alloc&)
    {
        CCTRACE("Not enough memory!");
        return nullptr;
    }

    if (clouds.empty() && meshes.empty())
    {
        CCTRACE("Select only clouds or meshes!");
        return nullptr;
    }
    if (!clouds.empty() && !meshes.empty())
    {
        CCTRACE("Can't mix point clouds and meshes!");
    }

    //merge clouds?
    if (!clouds.empty())
    {
        CCTRACE("clouds");

        //we will remove the useless clouds/meshes later
        ccHObject::Container toBeRemoved;

        ccPointCloud* firstCloud = nullptr;

        //whether to generate the 'original cloud index' scalar field or not
        CCCoreLib::ScalarField* ocIndexSF = nullptr;
        size_t cloudIndex = 0;
        int sfIdx = -1;

        for (size_t i = 0; i < clouds.size(); ++i)
        {
            CCTRACE("cloud: " << i);
            ccPointCloud* pc = clouds[i];
            if (!firstCloud)
            {
                if (deleteOriginalClouds)
                {
                    firstCloud = pc;
                }
                else
                {
                    firstCloud = pc->cloneThis();
                }

                if (createSFcloudIndex)
                {
                    sfIdx = firstCloud->getScalarFieldIndexByName(CC_ORIGINAL_CLOUD_INDEX_SF_NAME);
                    if (sfIdx < 0)
                    {
                        sfIdx = firstCloud->addScalarField(CC_ORIGINAL_CLOUD_INDEX_SF_NAME);
                    }
                    if (sfIdx < 0)
                    {
                        CCTRACE("Couldn't allocate a new scalar field for storing the original cloud index! Try to free some memory ...");
                        return nullptr;
                    }
                    else
                    {
                        ocIndexSF = firstCloud->getScalarField(sfIdx);
                        ocIndexSF->fill(0);
                        firstCloud->setCurrentDisplayedScalarField(sfIdx);
                        CCTRACE("NumberOfScalarFields: " << firstCloud->getNumberOfScalarFields());
                        CCTRACE("SF name: " << ocIndexSF->getName());
                    }
                }
            }
            else
            {
                unsigned countBefore = firstCloud->size();
                unsigned countAdded = pc->size();
                if (deleteOriginalClouds)
                    *firstCloud += pc;
                else
                    *firstCloud += pc->cloneThis();
                CCTRACE("  new size: " << firstCloud->size());
                //success?
                if (firstCloud->size() == countBefore + countAdded)
                {
                    ccHObject* toRemove = nullptr;
                    //if the entity to remove is inside a group with a unique child, we can remove the group as well
                    ccHObject* parent = pc->getParent();
                    if (parent && parent->isA(CC_TYPES::HIERARCHY_OBJECT) && parent->getChildrenNumber() == 1 ) //&& parent != firstCloudContext.parent)
                        toRemove = parent;
                    else
                        toRemove = pc;

                    if (deleteOriginalClouds)
                        AddToRemoveListPy(toRemove, toBeRemoved);

                    if (ocIndexSF)
                    {
                        CCTRACE("  ocIndexSF")
                        ocIndexSF->resizeSafe(firstCloud->size());
                        ScalarType index = static_cast<ScalarType>(++cloudIndex);
                        for (unsigned i = 0; i < countAdded; ++i)
                        {
                            ocIndexSF->setValue(countBefore + i, index);
                        }
                        CCTRACE("  ocIndexSF")
                    }
                }
                else
                {
                    CCTRACE("Fusion failed! (not enough memory?)");
                    break;
                }
                pc = nullptr;
            }
        }

        if (ocIndexSF)
        {
            CCTRACE("SF computeMinAndMax SF: " << sfIdx);
            CCTRACE("NumberOfScalarFields: " << firstCloud->getNumberOfScalarFields());
            ocIndexSF->computeMinAndMax();
            firstCloud->setCurrentDisplayedScalarField(sfIdx);
            firstCloud->showSF(true);
        }

        //something to remove?
        if (deleteOriginalClouds)
        {
        for (ccHObject* toRemove : toBeRemoved)
            {
                if (toRemove->getParent())
                    toRemove->getParent()->removeChild(toRemove);
                else
                    delete toRemove;
            }
            toBeRemoved.clear();
        }
        return firstCloud;
    }
    //merge meshes?
    else if (!meshes.empty())
    {
        //meshes are merged
        ccPointCloud* baseVertices = new ccPointCloud("vertices");
        ccMesh* baseMesh = new ccMesh(baseVertices);
        baseMesh->setName("Merged mesh");
        baseMesh->addChild(baseVertices);
        baseVertices->setEnabled(false);

        for (ccMesh *mesh : meshes)
        {
            if (!baseMesh->merge(mesh, createSubMeshes))
            {
                CCTRACE("Fusion failed! (not enough memory?)");
                break;
            }
        }
        baseMesh->setDisplay_recursive(meshes.front()->getDisplay());
        baseMesh->setVisible(true);
        return baseMesh;
    }
    return nullptr;
}

// ----------------------------------------------------------------------------
// following adapted from qCC for extract sections...

struct Segment2D
{
    Segment2D() : s(0), lAB(0) {}

    CCVector2 A, B, uAB;
    PointCoordinateType lAB;
    PointCoordinateType s; //curvilinear coordinate
};

enum EnvelopeType { ENV_LOWER, ENV_UPPER, ENV_FULL };
enum HullPointFlags {   POINT_NOT_USED  = 0,
                        POINT_USED      = 1,
                        POINT_IGNORED   = 2,
                        POINT_FROZEN    = 3,
};

using Vertex2D = CCCoreLib::PointProjectionTools::IndexedCCVector2;
using Hull2D = std::list<Vertex2D *>;
using VertexIterator = std::list<Vertex2D *>::iterator;
using ConstVertexIterator = std::list<Vertex2D *>::const_iterator;

namespace
{
    struct Edge
    {
        Edge() : nearestPointIndex(0), nearestPointSquareDist(-1.0f) {}

        Edge(const VertexIterator& A, unsigned _nearestPointIndex, float _nearestPointSquareDist)
            : itA(A)
            , nearestPointIndex(_nearestPointIndex)
            , nearestPointSquareDist(_nearestPointSquareDist)
        {}

        //operator
        inline bool operator< (const Edge& e) const { return nearestPointSquareDist < e.nearestPointSquareDist; }

        VertexIterator itA;
        unsigned nearestPointIndex;
        float nearestPointSquareDist;
    };
}

struct Segment
{
    Segment()
        : A(0, 0)
        , B(0, 0)
        , u(0, 0)
        , d(0)
        , curvPos(0)
    {}

    CCVector2 A, B, u;
    PointCoordinateType d, curvPos;
};


//! from ccEnvelopeExtractor
//! from Finds the nearest (available) point to an edge
/** \return The nearest point distance (or -1 if no point was found!)
**/
PointCoordinateType FindNearestCandidate(unsigned& minIndex,
                                                const VertexIterator& itA,
                                                const VertexIterator& itB,
                                                const std::vector<Vertex2D>& points,
                                                const std::vector<HullPointFlags>& pointFlags,
                                                PointCoordinateType minSquareEdgeLength,
                                                bool allowLongerChunks = false,
                                                double minCosAngle = -1.0)
{
    //look for the nearest point in the input set
    PointCoordinateType minDist2 = -1;
    const CCVector2 AB = **itB-**itA;
    const PointCoordinateType squareLengthAB = AB.norm2();
    const unsigned pointCount = static_cast<unsigned>(points.size());

#ifdef CC_CORE_LIB_USES_TBB
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

//! from ccEnvelopeExtractor::ExtractConcaveHull2D
bool ExtractConcaveHull2D(  std::vector<Vertex2D>& points,
                            std::list<Vertex2D*>& hullPoints,
                            EnvelopeType envelopeType,
                            bool allowMultiPass,
                            PointCoordinateType maxSquareEdgeLength/*=0*/,
                            double maxAngleDeg/*=0.0*/)
{
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

        if (envelopeType != ENV_FULL)
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

                bool forward = ((**itBefore - **itLeft).cross(**itAfter - **itLeft) < 0 && envelopeType == ENV_LOWER);
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
            std::multiset<Edge> edges;
            //initial number of edges
            assert(hullPoints.size() >= 2);
            size_t initEdgeCount = hullPoints.size();
            if (envelopeType != ENV_FULL)
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
                    PointCoordinateType minSquareDist = FindNearestCandidate(
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
                        Edge e(itA, nearestPointIndex, minSquareDist);
                        edges.insert(e);
                    }
                }

                pointFlags[(*itA)->index] = POINT_USED;
            }

            //flag the last vertex as well for non closed envelopes!
            if (envelopeType != ENV_FULL)
                pointFlags[(*hullPoints.rbegin())->index] = POINT_USED;

            while (!edges.empty())
            {
                //current edge (AB)
                //this should be the edge with the nearest 'candidate'
                Edge e = *edges.begin();
                edges.erase(edges.begin());

                VertexIterator itA = e.itA;
                VertexIterator itB = itA; ++itB;
                if (itB == hullPoints.end())
                {
                    assert(envelopeType == ENV_FULL);
                    itB = hullPoints.begin();
                }

                //nearest point
                const Vertex2D& P = points[e.nearestPointIndex];
                assert(pointFlags[P.index] == POINT_NOT_USED); //we don't consider already used points!

                //last check: the new segments must not intersect with the actual hull!
                bool intersect = false;
                //if (false)
                {
                    for (VertexIterator itJ = hullPoints.begin(), itI = itJ++; itI != hullPoints.end(); ++itI, ++itJ)
                    {
                        if (itJ == hullPoints.end())
                        {
                            if (envelopeType == ENV_FULL)
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
                        std::multiset<Edge>::const_iterator lastValidIt = edges.end();
                        for (std::multiset<Edge>::const_iterator it = edges.begin(); it != edges.end(); ++it)
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
                            PointCoordinateType minSquareDist = FindNearestCandidate(
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
                                Edge e(itC, nearestPointIndex, minSquareDist);
                                edges.insert(e);
                            }
                        }
                    }

                    //we'll inspect the two new segments later (if necessary)
                    if ((P-**itA).norm2() > maxSquareEdgeLength)
                    {
                        unsigned nearestPointIndex = 0;
                        PointCoordinateType minSquareDist = FindNearestCandidate(
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
                            Edge e(itA,nearestPointIndex,minSquareDist);
                            edges.insert(e);
                        }
                    }
                    if ((**itB-P).norm2() > maxSquareEdgeLength)
                    {
                        unsigned nearestPointIndex = 0;
                        PointCoordinateType minSquareDist = FindNearestCandidate(
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
                            Edge e(itP,nearestPointIndex,minSquareDist);
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

//! from ccEnvelopeExtractor::ExtractFlatEnvelope
ccPolyline* ExtractFlatEnvelope(CCCoreLib::GenericIndexedCloudPersist* points,
                                bool allowMultiPass,
                                PointCoordinateType maxEdgeLength/*=0*/,
                                const PointCoordinateType* preferredNormDim/*=nullptr*/,
                                const PointCoordinateType* preferredUpDir/*=nullptr*/,
                                EnvelopeType envelopeType/*=ENV_FULL*/,
                                std::vector<unsigned>* originalPointIndexes/*=nullptr*/,
                                double maxAngleDeg=0.0)
{
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
    if (!ExtractConcaveHull2D(  points2D,
                                hullPoints,
                                envelopeType,
                                allowMultiPass,
                                maxEdgeLength*maxEdgeLength,
                                maxAngleDeg))
    {
        ccLog::Warning("[ExtractFlatEnvelope] Failed to compute the convex hull of the input points!");
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
            ccLog::Error("[ExtractFlatEnvelope] Not enough memory!");
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
            ccLog::Error("[ExtractFlatEnvelope] Not enough memory!");
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
        envelopePolyline->setClosed(envelopeType == ENV_FULL);
        envelopePolyline->setVisible(true);
        envelopePolyline->setName("envelope");
        envelopePolyline->addChild(envelopeVertices);
    }
    else
    {
        delete envelopePolyline;
        envelopePolyline = nullptr;
        ccLog::Warning("[ExtractFlatEnvelope] Not enough memory to create the envelope polyline!");
    }

    return envelopePolyline;
}

//! from ccSectionExtractionTool::extractSectionEnvelope
std::vector<ccPolyline*> extractSectionEnvelope(const ccPolyline* originalSection,
                                                const ccPointCloud* originalSectionCloud,
                                                ccPointCloud* unrolledSectionCloud,
                                                unsigned sectionIndex,
                                                EnvelopeType envelopeType,
                                                PointCoordinateType maxEdgeLength,
                                                bool multiPass,
                                                bool splitEnvelope,
                                                bool& envelopeGenerated,
                                                bool& error)
{
    envelopeGenerated = false;
    error = false;
    std::vector<ccPolyline*> results;

    if (!originalSectionCloud || !unrolledSectionCloud)
    {
        CCTRACE("[ccSectionExtractionTool][extract envelope] Internal error: invalid input parameter(s)");
        error = true;
        return results;
    }

    if (originalSectionCloud->size() < 2)
    {
        //nothing to do
        CCTRACE("[ccSectionExtractionTool][extract envelope] Section #" << sectionIndex << " contains less than 2 points and will be ignored");
        error = true;
        return results;
    }

    //by default, the points in 'unrolledSectionCloud' are 2D (X = curvilinear coordinate, Y = height, Z = 0)
    CCVector3 N(0, 0, 1);
    CCVector3 Y(0, 1, 0);

    std::vector<unsigned> vertIndexes;
    ccPolyline* envelope = ExtractFlatEnvelope( unrolledSectionCloud,
                                                multiPass,
                                                maxEdgeLength,
                                                N.u,
                                                Y.u,
                                                envelopeType,
                                                &vertIndexes);

    if (envelope)
    {
        //update vertices (to replace 'unrolled' points by 'original' ones
        {
            CCCoreLib::GenericIndexedCloud* vertices = envelope->getAssociatedCloud();
            if (vertIndexes.size() == static_cast<size_t>(vertices->size()))
            {
                for (unsigned i = 0; i < vertices->size(); ++i)
                {
                    const CCVector3* P = vertices->getPoint(i);
                    assert(vertIndexes[i] < originalSectionCloud->size());
                    *const_cast<CCVector3*>(P) = *originalSectionCloud->getPoint(vertIndexes[i]);
                }

                ccPointCloud* verticesAsPC = dynamic_cast<ccPointCloud*>(vertices);
                if (verticesAsPC)
                    verticesAsPC->refreshBB();
                //if (!splitEnvelope) results.push_back(envelope);
            }
            else
            {
                ccLog::Warning("[ccSectionExtractionTool][extract envelope] Internal error (couldn't fetch original points indexes?!)");
                delete envelope;
                error = true;
                return results;
            }
        }

        std::vector<ccPolyline*> parts;
        if (splitEnvelope)
        {
#ifdef QT_DEBUG
            //compute some stats on the envelope
            {
                double minLength = 0;
                double maxLength = 0;
                double sumLength = 0;
                unsigned count = envelope->size();
                if (!envelope->isClosed())
                    --count;
                for (unsigned i = 0; i < count; ++i)
                {
                    const CCVector3* A = envelope->getPoint(i);
                    const CCVector3* B = envelope->getPoint((i+1) % envelope->size());
                    CCVector3 e = *B - *A;
                    double l = e.norm();
                    if (i != 0)
                    {
                        minLength = std::min(minLength,l);
                        maxLength = std::max(maxLength,l);
                        sumLength += l;
                    }
                    else
                    {
                        minLength = maxLength = sumLength = l;
                    }
                }
                ccLog::PrintDebug(QString("Envelope: min = %1 / avg = %2 / max = %3").arg(minLength).arg(sumLength/count).arg(maxLength));
            }
#endif

            /*bool success = */envelope->split(maxEdgeLength, parts);
            delete envelope;
            envelope = nullptr;
        }
        else
        {
            parts.push_back(envelope);
        }

        for (size_t p = 0; p < parts.size(); ++p)
        {
            ccPolyline* envelopePart = parts[p];
            QString name = QString("Section envelope #%1").arg(sectionIndex);
            if (parts.size() > 1)
            {
                name += QString("(part %1/%2)").arg(p + 1).arg(parts.size());
            }
            envelopePart->setName(name);
            envelopePart->copyGlobalShiftAndScale(*originalSectionCloud);
            envelopePart->setColor(ccColor::green);
            envelopePart->showColors(true);
            //copy meta-data (import for Mascaret export!)
            {
                const QVariantMap& metaData = originalSection->metaData();
                for (QVariantMap::const_iterator it = metaData.begin(); it != metaData.end(); ++it)
                {
                    envelopePart->setMetaData(it.key(), it.value());
                }
            }
            results.push_back(envelopePart);
        }

        envelopeGenerated = true;
    }
    return results;
}

//! from ccSectionExtractionTool::extractSectionCloud
ccPointCloud* extractSectionCloud(  const std::vector<CCCoreLib::ReferenceCloud*>& refClouds,
                                    std::vector<ccGenericPointCloud*> clouds,
                                    unsigned sectionIndex,
                                    bool& cloudGenerated,
                                    bool& error)
{
    cloudGenerated = false;
    error = false;

    ccPointCloud* sectionCloud = nullptr;
    for (int i = 0; i < static_cast<int>(refClouds.size()); ++i)
    {
        if (!refClouds[i])
            continue;
        assert(clouds[i]); //a valid ref. cloud must have a valid counterpart!

        //extract part/section from each cloud
        ccPointCloud* part = nullptr;

        //if the cloud is a ccPointCloud, we can keep a lot more information
        //when extracting the section cloud
        ccPointCloud* pc = dynamic_cast<ccPointCloud*>(clouds[i]);
        if (pc)
        {
            part = pc->partialClone(refClouds[i]);
        }
        else
        {
            part = ccPointCloud::From(refClouds[i], clouds[i]);
        }

        if (part)
        {
            if (i == 0)
            {
                //we simply use this 'part' cloud as the section cloud
                sectionCloud = part;
            }
            else
            {
                //fuse it with the global cloud
                unsigned cloudSizeBefore = sectionCloud->size();
                unsigned partSize = part->size();
                sectionCloud->append(part, cloudSizeBefore, true);

                //don't need it anymore
                delete part;
                part = nullptr;
                //check that it actually worked!
                if (sectionCloud->size() != cloudSizeBefore + partSize)
                {
                    //not enough memory
                    CCTRACE("[ccSectionExtractionTool][extract cloud] Not enough memory");
                    delete sectionCloud;
                    error = true;
                    return nullptr;
                }
            }
        }
        else
        {
            //not enough memory
            CCTRACE("[ccSectionExtractionTool][extract cloud] Not enough memory");
            delete sectionCloud;
            error = true;
            return nullptr;
        }
    }

    if (sectionCloud)
    {
        sectionCloud->setName(QString("Section cloud #%1").arg(sectionIndex));
        cloudGenerated = true;
    }

    return sectionCloud;
}

//! from ccSectionExtractionTool::extractPoints
std::vector<ccHObject*> extractPointsAlongSections( std::vector<ccGenericPointCloud*> clouds,
                                                    std::vector<ccPolyline*> sections,
                                                    double defaultSectionThickness = -1.0,
                                                    double envelopeMaxEdgeLength = 0,
                                                    bool extractSectionsAsClouds = false,
                                                    bool extractSectionsAsEnvelopes = true,
                                                    bool multipass = false,
                                                    bool splitEnvelope = false,
                                                    EnvelopeType s_extractSectionsType = ENV_LOWER,
                                                    int vertDim = 2)
{
    std::vector<ccHObject*> results;
    //number of eligible sections
    unsigned sectionCount = 0;
    {
        for (auto & section : sections)
        {
            if (section && section->size() > 1)
                ++sectionCount;
        }
    }
    if (sectionCount == 0)
    {
        CCTRACE("No (valid) section!");
        return results;
    }

    //compute loaded clouds bounding-box
    ccBBox box;
    unsigned pointCount = 0;

    for (auto & cloud : clouds)
    {
        if (cloud)
        {
            box += cloud->getOwnBB();
            pointCount += cloud->size();
        }
    }

    if (defaultSectionThickness <= 0)
    {
        defaultSectionThickness = box.getMaxBoxDim() / 500.0;
    }
    if (envelopeMaxEdgeLength <= 0)
    {
        envelopeMaxEdgeLength = box.getMaxBoxDim() / 500.0;
    }

    int xDim = (vertDim < 2 ? vertDim + 1 : 0);
    int yDim = (xDim < 2 ? xDim + 1 : 0);

    //we consider half of the total thickness as points can be on both sides!
    double sectionThicknessSq = std::pow(defaultSectionThickness / 2.0, 2.0);
    bool error = false;

    unsigned generatedEnvelopes = 0;
    unsigned generatedClouds = 0;

    try
    {
        //for each slice
        for (int s = 0; s < sections.size(); ++s)
        {
            ccPolyline* poly = sections[s];
            if (poly)
            {
                unsigned polyVertCount = poly->size();
                if (polyVertCount < 2)
                {
                    CCTRACE("polyVertCount < 2");
                    continue;
                }
                unsigned polySegmentCount = poly->isClosed() ? polyVertCount : polyVertCount - 1;

                //project the section in '2D'
                std::vector<Segment2D> polySegments2D;
                {
                    polySegments2D.reserve(polySegmentCount);
                    PointCoordinateType s = 0;
                    for (unsigned j = 0; j < polySegmentCount; ++j)
                    {
                        Segment2D seg2D;
                        const CCVector3* A = poly->getPoint(j);
                        const CCVector3* B = poly->getPoint((j + 1) % polyVertCount);
                        seg2D.A = CCVector2(A->u[xDim], A->u[yDim]);
                        seg2D.B = CCVector2(B->u[xDim], B->u[yDim]);
                        seg2D.uAB = seg2D.B - seg2D.A; //(unit) direction
                        seg2D.lAB = seg2D.uAB.norm(); //length
                        seg2D.s = s;
                        s += seg2D.lAB;

                        if (CCCoreLib::LessThanEpsilon(seg2D.lAB))
                        {
                            //ignore too small segments
                            continue;
                        }

                        seg2D.uAB /= seg2D.lAB;
                        polySegments2D.push_back(seg2D);
                    }

                    if (polySegments2D.empty())
                    {
                        CCTRACE("polySegments2D.empty");
                        continue;
                    }
                    polySegments2D.shrink_to_fit();
                }

                int cloudCount = clouds.size();
                std::vector<CCCoreLib::ReferenceCloud*> refClouds;
                if (extractSectionsAsClouds)
                {
                    refClouds.resize(cloudCount, nullptr);
                }

                //for envelope extraction as a polyline
                ccPointCloud* originalSlicePoints = nullptr;
                ccPointCloud* unrolledSlicePoints = nullptr;
                if (extractSectionsAsEnvelopes)
                {
                    originalSlicePoints = new ccPointCloud("section.orig");
                    unrolledSlicePoints = new ccPointCloud("section.unroll");

                    //assign them the default (first!) global shift & scale info
                    if (clouds.empty())
                        {
                        CCTRACE("no clouds!");
                        return results;
                        }
                    ccGenericPointCloud* cloud = clouds.front();
                    originalSlicePoints->copyGlobalShiftAndScale(*cloud);
                }

                //for each cloud
                for (int c = 0; c < cloudCount; ++c)
                {
                    ccGenericPointCloud* cloud = clouds[c];
                    if (cloud)
                    {
                        //for envelope extraction as a cloud
                        CCCoreLib::ReferenceCloud* refCloud = nullptr;
                        if (extractSectionsAsClouds)
                        {
                            refCloud = new CCCoreLib::ReferenceCloud(cloud);
                        }

                        //compute the distance of each point to the current polyline segment
                        for (unsigned i = 0; i < cloud->size(); ++i)
                        {
                            const CCVector3* P = cloud->getPoint(i);
                            CCVector2 P2D(P->u[xDim], P->u[yDim]);

                            //for each vertex
                            PointCoordinateType minSquareDist = -CCCoreLib::PC_ONE;
                            PointCoordinateType curvilinearPos = 0.0;
                            size_t minIndex = 0;
                            for (size_t j = 0; j < polySegments2D.size(); ++j)
                            {
                                const Segment2D& seg2D = polySegments2D[j];
                                CCVector2 AP2D = P2D - seg2D.A;

                                //square distance to the polyline
                                PointCoordinateType squareDist = 0;

                                //longitudinal 'distance'
                                double dotprod = seg2D.uAB.dot(AP2D);
                                if (dotprod < 0)
                                {
                                    if (j == 0 && !poly->isClosed())
                                        continue;
                                    squareDist = AP2D.norm2();
                                }
                                else if (dotprod > seg2D.lAB)
                                {
                                    if (j + 1 == polySegments2D.size() && !poly->isClosed())
                                        continue;
                                    squareDist = (P2D - seg2D.B).norm2();
                                }
                                else
                                {
                                    //orthogonal distance
                                    squareDist = (AP2D - seg2D.uAB * dotprod).norm2();
                                }

                                if (minSquareDist < 0 || squareDist < minSquareDist)
                                {
                                    minSquareDist = squareDist;
                                    curvilinearPos = dotprod;
                                    minIndex = j;
                                }
                            }

                            //elligible point?
                            if (minSquareDist >= 0 && minSquareDist < sectionThicknessSq)
                            {
                                //if we extract the section as cloud(s), we add the point to the (current) ref. cloud
                                if (extractSectionsAsClouds)
                                {
                                    assert(refCloud);
                                    unsigned refCloudSize = refCloud->size();
                                    if (refCloudSize == refCloud->capacity())
                                    {
                                        refCloudSize += (refCloudSize / 2 + 1);
                                        if (!refCloud->reserve(refCloudSize))
                                        {
                                            //not enough memory
                                            CCTRACE("[ccSectionExtractionTool] Not enough memory");
                                            error = true;
                                            break;
                                        }
                                    }
                                    refCloud->addPointIndex(i);
                                }

                                //if we extract the section as envelope(s), we add it to the 2D points set
                                if (extractSectionsAsEnvelopes)
                                {
                                    assert(originalSlicePoints && unrolledSlicePoints);
                                    assert(originalSlicePoints->size() == unrolledSlicePoints->size());

                                    unsigned cloudSize = originalSlicePoints->size();
                                    if (cloudSize == originalSlicePoints->capacity())
                                    {
                                        cloudSize += (cloudSize / 2 + 1);
                                        if (!originalSlicePoints->reserve(cloudSize)
                                            || !unrolledSlicePoints->reserve(cloudSize))
                                        {
                                            //not enough memory
                                            CCTRACE("[ccSectionExtractionTool] Not enough memory");
                                            error = true;
                                            break;
                                        }
                                    }

                                    const Segment2D& seg2D = polySegments2D[minIndex];

                                    //we project the 'real' 3D point in the section plane
                                    CCVector3 Pproj3D;
                                    {
                                        Pproj3D.u[xDim] = seg2D.A.x + seg2D.uAB.x * curvilinearPos;
                                        Pproj3D.u[yDim] = seg2D.A.y + seg2D.uAB.y * curvilinearPos;
                                        Pproj3D.u[vertDim] = P->u[vertDim];
                                    }
                                    originalSlicePoints->addPoint(Pproj3D);
                                    unrolledSlicePoints->addPoint(CCVector3(seg2D.s + curvilinearPos, P->u[vertDim], 0));
                                }
                            }

                            if (error)
                            {
                                break;
                            }

                        } //for each point

                        if (refCloud)
                        {
                            assert(extractSectionsAsClouds);
                            if (error || refCloud->size() == 0)
                            {
                                delete refCloud;
                                refCloud = nullptr;
                            }
                            else
                            {
                                refClouds[c] = refCloud;
                            }
                        }

                    }

                    if (error)
                    {
                        break;
                    }

                } //for each cloud

                if (!error)
                {
                    //Extract sections as (polyline) envelopes
                    if (/*!error && */extractSectionsAsEnvelopes)
                    {
                        assert(originalSlicePoints && unrolledSlicePoints);
                        bool envelopeGenerated = false;
                        std::vector<ccPolyline*> pls = extractSectionEnvelope(  poly,
                                                                                originalSlicePoints,
                                                                                unrolledSlicePoints,
                                                                                s + 1,
                                                                                s_extractSectionsType,
                                                                                envelopeMaxEdgeLength,
                                                                                multipass,
                                                                                splitEnvelope,
                                                                                envelopeGenerated,
                                                                                error);
                        if (!error)
                        {
                            for (auto pl : pls)
                                results.push_back(pl);
                        }

                        if (envelopeGenerated)
                        {
                            ++generatedEnvelopes;
                        }
                    }

                    //Extract sections as clouds
                    if (!error && extractSectionsAsClouds)
                    {
                        assert(static_cast<int>(refClouds.size()) == cloudCount);
                        bool cloudGenerated = false;
                        ccPointCloud* cl = extractSectionCloud(refClouds, clouds, s + 1, cloudGenerated, error);
                        if (!error) results.push_back(cl);
                        if (cloudGenerated)
                            ++generatedClouds;
                    }
                }

                //release memory
                for (auto & refCloud : refClouds)
                {
                    delete refCloud;
                    refCloud = nullptr;
                }

                delete originalSlicePoints;
                originalSlicePoints = nullptr;

                delete unrolledSlicePoints;
                unrolledSlicePoints = nullptr;
            } //if (poly)

            if (error)
                break;
        } //for (int s=0; s<m_sections.size(); ++s)
    }
    catch (const std::bad_alloc&)
    {
        error = true;
    }

    if (error)
    {
        CCTRACE("An error occurred in extractPointsFromClouds!");
    }
    else
    {
        CCTRACE("[ccSectionExtractionTool] Job done (" << generatedEnvelopes << " envelope(s) and "<< generatedClouds << " cloud(s) were generated)");
    }
    return results;
}

//! from ccSectionExtractionTool::unfoldPoints
std::vector<ccPointCloud*> unfoldPointsAlongPolylines(std::vector<ccGenericPointCloud*> clouds,
                                                      std::vector<ccPolyline*> polylines,
                                                      double thickness,
                                                      int vertDim =2)
{
    std::vector<ccPointCloud*> unfoldedClouds;
    //compute loaded clouds bounding-box
    ccBBox box;
    unsigned totalPointCount = 0;
    {
        for (auto & cloud : clouds)
        {
            if (cloud)
            {
                box += cloud->getOwnBB();
                totalPointCount += cloud->size();
            }
        }
    }

    static double s_defaultThickness = -1.0;
    if (s_defaultThickness <= 0)
    {
        s_defaultThickness = box.getMaxBoxDim() / 10.0;
    }

    s_defaultThickness = thickness;

    //projection direction
    int xDim = (vertDim < 2 ? vertDim + 1 : 0);
    int yDim = (xDim < 2 ? xDim + 1 : 0);

    //we consider half of the total thickness as points can be on both sides!
    double maxSquareDistToPolyline = (thickness / 2) * (thickness / 2);

    unsigned exportedClouds = 0;

    for (ccPolyline* poly : polylines)
    {
        unsigned polyVertCount = poly->size();
        if (polyVertCount < 2)
        {
            assert(false);
            CCTRACE("Invalid polyline (less than 2 vertices)");
            continue;
        }

        //prepare the computation of 2D distances
        std::vector<Segment> segments;
        unsigned polySegmentCount = poly->isClosed() ? polyVertCount : polyVertCount - 1;
        {
            try
            {
                segments.reserve(polySegmentCount);
            }
            catch (const std::bad_alloc&)
            {
                //not enough memory
                ccLog::Error("Not enough memory");
                return unfoldedClouds;
            }

            PointCoordinateType curvPos = 0;
            for (unsigned j = 0; j < polySegmentCount; ++j)
            {
                //current polyline segment
                const CCVector3* A = poly->getPoint(j);
                const CCVector3* B = poly->getPoint((j + 1) % polyVertCount);

                Segment s;
                {
                    s.A = CCVector2(A->u[xDim], A->u[yDim]);
                    s.B = CCVector2(B->u[xDim], B->u[yDim]);
                    s.u = s.B - s.A;
                    s.d = s.u.norm();
                    if (CCCoreLib::GreaterThanEpsilon(s.d))
                    {
                        s.curvPos = curvPos;
                        s.u /= s.d;
                        segments.push_back(s);
                    }
                }

                //update curvilinear pos
                curvPos += (*B - *A).norm();
            }
        }

        //for each cloud
        for (auto & pc : clouds)
        {
            ccGenericPointCloud* cloud = pc;
            if (!cloud)
            {
                assert(false);
                continue;
            }

            CCCoreLib::ReferenceCloud unfoldedIndexes(cloud);
            if (!unfoldedIndexes.reserve(cloud->size()))
            {
                ccLog::Error("Not enough memory");
                return unfoldedClouds;
            }
            std::vector<CCVector3> unfoldedPoints;
            try
            {
                unfoldedPoints.reserve(cloud->size());
            }
            catch (const std::bad_alloc&)
            {
                ccLog::Error("Not enough memory");
                return unfoldedClouds;
            }

            //now test each point and see if it's close to the current polyline (in 2D)
            for (unsigned i = 0; i < cloud->size(); ++i)
            {
                const CCVector3* P = cloud->getPoint(i);
                CCVector2 P2D(P->u[xDim], P->u[yDim]);

                //test each segment
                int closestSegment = -1;
                PointCoordinateType minSquareDist = -CCCoreLib::PC_ONE;
                for (unsigned j = 0; j < polySegmentCount; ++j)
                {
                    const Segment& s = segments[j];
                    CCVector2 AP2D = P2D - s.A;

                    //longitudinal 'distance'
                    PointCoordinateType dotprod = s.u.dot(AP2D);

                    PointCoordinateType squareDist = 0;
                    if (dotprod < 0.0f)
                    {
                        //dist to nearest vertex
                        squareDist = AP2D.norm2();
                    }
                    else if (dotprod > s.d)
                    {
                        //dist to nearest vertex
                        squareDist = (P2D - s.B).norm2();
                    }
                    else
                    {
                        //orthogonal distance
                        squareDist = (AP2D - s.u*dotprod).norm2();
                    }

                    if (squareDist <= maxSquareDistToPolyline)
                    {
                        if (closestSegment < 0 || squareDist < minSquareDist)
                        {
                            minSquareDist = squareDist;
                            closestSegment = static_cast<int>(j);
                        }
                    }
                }

                if (closestSegment >= 0)
                {
                    const Segment& s = segments[closestSegment];

                    //we use the curvilinear position of the point in the X dimension (and Y is 0)
                    CCVector3 Q;
                    {
                        CCVector2 AP2D = P2D - s.A;
                        PointCoordinateType dotprod = s.u.dot(AP2D);
                        PointCoordinateType d = (AP2D - s.u*dotprod).norm();

                        //compute the sign of 'minDist'
                        PointCoordinateType crossprod = AP2D.y * s.u.x - AP2D.x * s.u.y;

                        Q.u[xDim] = s.curvPos + dotprod;
                        Q.u[yDim] = crossprod < 0 ? -d : d; //signed orthogonal distance to the polyline
                        Q.u[vertDim] = P->u[vertDim];
                    }

                    unfoldedIndexes.addPointIndex(i);
                    unfoldedPoints.push_back(Q);
                }

            } //for each point

            if (unfoldedIndexes.size() != 0)
            {
                //assign the default global shift & scale info
                ccPointCloud* unfoldedCloud = nullptr;
                {
                    if (cloud->isA(CC_TYPES::POINT_CLOUD))
                        unfoldedCloud = static_cast<ccPointCloud*>(cloud)->partialClone(&unfoldedIndexes);
                    else
                        unfoldedCloud = ccPointCloud::From(&unfoldedIndexes, cloud);
                }
                if (!unfoldedCloud)
                {
                    CCTRACE("Not enough memory");
                    return unfoldedClouds;
                }

                assert(unfoldedCloud->size() == unfoldedPoints.size());
                CCVector3 C = box.minCorner();
                C.u[vertDim] = 0;
                C.u[xDim] = box.minCorner().u[xDim]; //we start at the bounding-box limit
                for (unsigned i = 0; i < unfoldedCloud->size(); ++i)
                {
                    //update the points positions
                    *const_cast<CCVector3*>(unfoldedCloud->getPoint(i)) = unfoldedPoints[i] + C;
                }
                unfoldedCloud->invalidateBoundingBox();

                unfoldedCloud->setName(cloud->getName() + ".unfolded");
                unfoldedCloud->copyGlobalShiftAndScale(*cloud);

                unfoldedCloud->shrinkToFit();

                unfoldedClouds.push_back(unfoldedCloud);
                ++exportedClouds;
            }
            else
            {
                CCTRACE("[Unfold] No point of the cloud " << cloud->getName().toStdString() << " were unfolded (check parameters)");
            }

        } //for each cloud
    }

    CCTRACE("[Unfold] " << exportedClouds << " cloud(s) exported");
    return unfoldedClouds;
}

// end of adapted from qCC for extract sections...
// ----------------------------------------------------------------------------


PYBIND11_MODULE(_cloudComPy, m0)
{
    export_colors(m0);
    export_ScalarField(m0);
    export_ccGenericCloud(m0);
    export_ccPolyline(m0);
    export_ccOctree(m0);
    export_ccPointCloud(m0);
    export_ccMesh(m0);
    export_ccPrimitives(m0);
    export_distanceComputationTools(m0);
    export_geometricalAnalysisTools(m0);
    export_registrationTools(m0);
    export_cloudSamplingTools(m0);
    export_ccFacet(m0);
    export_ccSensor(m0);
    export_Neighbourhood(m0);

    m0.doc() = cloudComPy_doc;

    m0.def("greet", &greet);

    py::enum_<CC_SHIFT_MODE>(m0, "CC_SHIFT_MODE")
        .value("AUTO", CC_SHIFT_MODE::AUTO)
        .value("XYZ", CC_SHIFT_MODE::XYZ)
        .value("FIRST_GLOBAL_SHIFT", CC_SHIFT_MODE::FIRST_GLOBAL_SHIFT)
        .value("NO_GLOBAL_SHIFT", CC_SHIFT_MODE::NO_GLOBAL_SHIFT)
        .export_values();

    py::enum_<CC_DIRECTION>(m0, "CC_DIRECTION")
		.value("X", CC_DIRECTION::X)
		.value("Y", CC_DIRECTION::Y)
		.value("Z", CC_DIRECTION::Z)
        .export_values();

    py::enum_<CC_FILE_ERROR>(m0, "CC_FILE_ERROR")
        .value("CC_FERR_NO_ERROR", CC_FERR_NO_ERROR)
        .value("CC_FERR_BAD_ARGUMENT", CC_FERR_BAD_ARGUMENT)
        .value("CC_FERR_UNKNOWN_FILE", CC_FERR_UNKNOWN_FILE)
        .value("CC_FERR_WRONG_FILE_TYPE", CC_FERR_WRONG_FILE_TYPE)
        .value("CC_FERR_WRITING", CC_FERR_WRITING)
        .value("CC_FERR_READING", CC_FERR_READING)
        .value("CC_FERR_NO_SAVE", CC_FERR_NO_SAVE)
        .value("CC_FERR_NO_LOAD", CC_FERR_NO_LOAD)
        .value("CC_FERR_BAD_ENTITY_TYPE", CC_FERR_BAD_ENTITY_TYPE)
        .value("CC_FERR_CANCELED_BY_USER", CC_FERR_CANCELED_BY_USER)
        .value("CC_FERR_NOT_ENOUGH_MEMORY", CC_FERR_NOT_ENOUGH_MEMORY)
        .value("CC_FERR_MALFORMED_FILE", CC_FERR_MALFORMED_FILE)
        .value("CC_FERR_CONSOLE_ERROR", CC_FERR_CONSOLE_ERROR)
        .value("CC_FERR_BROKEN_DEPENDENCY_ERROR", CC_FERR_BROKEN_DEPENDENCY_ERROR)
        .value("CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN", CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN)
        .value("CC_FERR_THIRD_PARTY_LIB_FAILURE", CC_FERR_THIRD_PARTY_LIB_FAILURE)
        .value("CC_FERR_THIRD_PARTY_LIB_EXCEPTION", CC_FERR_THIRD_PARTY_LIB_EXCEPTION)
        .value("CC_FERR_NOT_IMPLEMENTED", CC_FERR_NOT_IMPLEMENTED)
        .export_values();

    py::enum_<CurvatureType>(m0, "CurvatureType")
        .value("GAUSSIAN_CURV", GAUSSIAN_CURV)
        .value("MEAN_CURV", MEAN_CURV)
        .value("NORMAL_CHANGE_RATE", NORMAL_CHANGE_RATE)
        .export_values();

    py::enum_<CCCoreLib::LOCAL_MODEL_TYPES>(m0, "LOCAL_MODEL_TYPES")
        .value("NO_MODEL", CCCoreLib::NO_MODEL )
        .value("LS", CCCoreLib::LS )
        .value("TRI", CCCoreLib::TRI )
        .value("QUADRIC", CCCoreLib::QUADRIC )
        .export_values();

    py::enum_<ccNormalVectors::Orientation>(m0, "Orientation")
        .value("PLUS_X", ccNormalVectors::PLUS_X )
        .value("MINUS_X", ccNormalVectors::MINUS_X )
        .value("PLUS_Y", ccNormalVectors::PLUS_Y )
        .value("MINUS_Y", ccNormalVectors::MINUS_Y )
        .value("PLUS_Z", ccNormalVectors::PLUS_Z )
        .value("MINUS_Z", ccNormalVectors::MINUS_Z )
        .value("PLUS_BARYCENTER", ccNormalVectors::PLUS_BARYCENTER )
        .value("MINUS_BARYCENTER", ccNormalVectors::MINUS_BARYCENTER )
        .value("PLUS_ORIGIN", ccNormalVectors::PLUS_ORIGIN )
        .value("MINUS_ORIGIN", ccNormalVectors::MINUS_ORIGIN )
        .value("PREVIOUS", ccNormalVectors::PREVIOUS )
        .value("PLUS_SENSOR_ORIGIN", ccNormalVectors::PLUS_SENSOR_ORIGIN )
        .value("MINUS_SENSOR_ORIGIN", ccNormalVectors::MINUS_SENSOR_ORIGIN )
        .value("UNDEFINED", ccNormalVectors::UNDEFINED )
        .export_values();

    py::enum_<ccRasterGrid::ProjectionType>(m0, "ProjectionType")
		.value("PROJ_MINIMUM_VALUE", ccRasterGrid::PROJ_MINIMUM_VALUE)
		.value("PROJ_AVERAGE_VALUE", ccRasterGrid::PROJ_AVERAGE_VALUE)
        .value("PROJ_MAXIMUM_VALUE", ccRasterGrid::PROJ_MAXIMUM_VALUE)
        .value("PROJ_MEDIAN_VALUE", ccRasterGrid::PROJ_MEDIAN_VALUE)
        .value("PROJ_INVERSE_VAR_VALUE", ccRasterGrid::PROJ_INVERSE_VAR_VALUE)
		.value("INVALID_PROJECTION_TYPE", ccRasterGrid::INVALID_PROJECTION_TYPE)
        .export_values();

    py::enum_<ccRasterGrid::EmptyCellFillOption>(m0, "EmptyCellFillOption")
		.value("LEAVE_EMPTY", ccRasterGrid::LEAVE_EMPTY)
		.value("FILL_MINIMUM_HEIGHT", ccRasterGrid::FILL_MINIMUM_HEIGHT)
		.value("FILL_MAXIMUM_HEIGHT", ccRasterGrid::FILL_MAXIMUM_HEIGHT)
		.value("FILL_CUSTOM_HEIGHT", ccRasterGrid::FILL_CUSTOM_HEIGHT)
		.value("FILL_AVERAGE_HEIGHT", ccRasterGrid::FILL_AVERAGE_HEIGHT)
		.value("INTERPOLATE_DELAUNAY", ccRasterGrid::INTERPOLATE_DELAUNAY)
        .value("KRIGING", ccRasterGrid::KRIGING)
        .export_values();

    py::enum_<ccRasterGrid::ExportableFields>(m0, "ExportableFields")
        .value("PER_CELL_HEIGHT", ccRasterGrid::PER_CELL_VALUE)
        .value("PER_CELL_COUNT", ccRasterGrid::PER_CELL_COUNT)
        .value("PER_CELL_MIN_HEIGHT", ccRasterGrid::PER_CELL_MIN_VALUE)
        .value("PER_CELL_MAX_HEIGHT", ccRasterGrid::PER_CELL_MAX_VALUE)
        .value("PER_CELL_AVG_HEIGHT", ccRasterGrid::PER_CELL_AVG_VALUE)
        .value("PER_CELL_HEIGHT_STD_DEV", ccRasterGrid::PER_CELL_VALUE_STD_DEV)
        .value("PER_CELL_HEIGHT_RANGE", ccRasterGrid::PER_CELL_VALUE_RANGE)
        .value("PER_CELL_INVALID", ccRasterGrid::PER_CELL_INVALID)
        .export_values();

    py::enum_<ccPointCloudInterpolator::Parameters::Method>(m0, "INTERPOL_METHOD")
        .value("NEAREST_NEIGHBOR", ccPointCloudInterpolator::Parameters::Method::NEAREST_NEIGHBOR)
        .value("K_NEAREST_NEIGHBORS", ccPointCloudInterpolator::Parameters::Method::K_NEAREST_NEIGHBORS)
        .value("RADIUS", ccPointCloudInterpolator::Parameters::Method::RADIUS)
        .export_values();

    py::enum_<ccPointCloudInterpolator::Parameters::Algo>(m0, "INTERPOL_ALGO")
        .value("AVERAGE", ccPointCloudInterpolator::Parameters::Algo::AVERAGE)
        .value("MEDIAN", ccPointCloudInterpolator::Parameters::Algo::MEDIAN)
        .value("NORMAL_DIST", ccPointCloudInterpolator::Parameters::Algo::NORMAL_DIST)
        .export_values();

    py::enum_<EnvelopeType>(m0, "EnvelopeType")
        .value("ENV_LOWER", EnvelopeType::ENV_LOWER)
        .value("ENV_UPPER", EnvelopeType::ENV_UPPER)
        .value("ENV_FULL", EnvelopeType::ENV_FULL)
        .export_values();

    m0.def("importFile", &importFilePy,
           py::arg("filename"), py::arg("mode")=AUTO, py::arg("x")=0, py::arg("y")=0, py::arg("z")=0, py::arg("extraData")="",
           cloudComPy_importFile_doc);

    py::class_<ccPointCloudInterpolator::Parameters>(m0, "interpolatorParameters", cloudComPy_interpolatorParameters_doc)
        .def(py::init<>())
        .def_readwrite("method", &ccPointCloudInterpolator::Parameters::method,
                       cloudComPy_interpolatorParameters_doc)
        .def_readwrite("algos", &ccPointCloudInterpolator::Parameters::algo,
                       cloudComPy_interpolatorParameters_doc)
        .def_readwrite("knn", &ccPointCloudInterpolator::Parameters::knn,
                       cloudComPy_interpolatorParameters_doc)
        .def_readwrite("radius", &ccPointCloudInterpolator::Parameters::radius,
                       cloudComPy_interpolatorParameters_doc)
        .def_readwrite("sigma", &ccPointCloudInterpolator::Parameters::sigma,
                       cloudComPy_interpolatorParameters_doc)
        ;

    m0.def("interpolateScalarFieldsFrom", &InterpolateScalarFieldsFrom_py,
           py::arg("destCloud"), py::arg("srcCloud"), py::arg("sfIndexes"), py::arg("params"), py::arg("octreeLevel")=0,
           cloudComPy_interpolateScalarFieldsFrom_doc);

    m0.def("loadPointCloud", &loadPointCloudPy,
           py::arg("filename"),
           py::arg("mode")=AUTO, py::arg("skip")=0, py::arg("x")=0, py::arg("y")=0, py::arg("z")=0, py::arg("extraData")="",
           cloudComPy_loadPointCloud_doc, py::return_value_policy::reference);

    m0.def("loadMesh", &loadMeshPy,
           py::arg("filename"),py::arg("mode")=AUTO, py::arg("skip")=0, py::arg("x")=0, py::arg("y")=0, py::arg("z")=0, py::arg("extraData")="",
           cloudComPy_loadMesh_doc, py::return_value_policy::reference);

    m0.def("loadPolyline", &loadPolyline,
           py::arg("filename"), py::arg("mode")=AUTO, py::arg("skip")=0, py::arg("x")=0, py::arg("y")=0, py::arg("z")=0,
           cloudComPy_loadPolyline_doc, py::return_value_policy::reference);

    m0.def("deleteEntity", &deleteEntity, cloudComPy_deleteEntity_doc);

    m0.def("SaveMesh", &SaveMesh, cloudComPy_SaveMesh_doc);

    m0.def("SavePointCloud", &SavePointCloud,
           py::arg("cloud"), py::arg("filename"), py::arg("version")=QString(""), py::arg("pointFormat")=-1,
           cloudComPy_SavePointCloud_doc);

    m0.def("SaveEntities", &SaveEntities, cloudComPy_SaveEntities_doc);

    m0.def("initCC", &initCC_py, cloudComPy_initCC_doc);

    m0.def("setTraces", &setTraces_py, cloudComPy_setTraces_doc);

    m0.def("initCloudCompare", &initCloudCompare_py, cloudComPy_initCloudCompare_doc);

    m0.def("isPluginDraco", &pyccPlugins::isPluginDraco, cloudComPy_isPluginDraco_doc);

    m0.def("isPluginFbx", &pyccPlugins::isPluginFbx, cloudComPy_isPluginFbx_doc);

    m0.def("isPluginHPR", &pyccPlugins::isPluginHPR, cloudComPy_isPluginHPR_doc);

    m0.def("isPluginM3C2", &pyccPlugins::isPluginM3C2, cloudComPy_isPluginM3C2_doc);

    m0.def("isPluginMeshBoolean", &pyccPlugins::isPluginMeshBoolean, cloudComPy_isPluginMeshBoolean_doc);

    m0.def("isPluginPCL", &pyccPlugins::isPluginPCL, cloudComPy_isPluginPCL_doc);

    m0.def("isPluginPCV", &pyccPlugins::isPluginPCV, cloudComPy_isPluginPCV_doc);

    m0.def("isPluginCSF", &pyccPlugins::isPluginCSF, cloudComPy_isPluginCSF_doc);

    m0.def("isPluginCanupo", &pyccPlugins::isPluginCanupo, cloudComPy_isPluginCanupo_doc);

    m0.def("isPluginSRA", &pyccPlugins::isPluginSRA, cloudComPy_isPluginSRA_doc);

    m0.def("isPluginRANSAC_SD", &pyccPlugins::isPluginRANSAC_SD, cloudComPy_isPluginRANSAC_SD_doc);

    m0.def("isPluginPoissonRecon", &pyccPlugins::isPluginPoissonRecon, cloudComPy_isPluginPoissonRecon_doc);

    m0.def("computeCurvature", &computeCurvature, cloudComPy_computeCurvature_doc);

    m0.def("computeFeature", &computeFeature, cloudComPy_computeFeature_doc);

    m0.def("computeLocalDensity", &computeLocalDensity, cloudComPy_computeLocalDensity_doc);

    m0.def("computeApproxLocalDensity", &computeApproxLocalDensity, cloudComPy_computeApproxLocalDensity_doc);

    m0.def("computeRoughness", &computeRoughnessPy,
           py::arg("radius"), py::arg("clouds"), py::arg("roughnessUpDir")=CCVector3(0,0,0),
           cloudComPy_computeRoughness_doc);

    m0.def("computeMomentOrder1", &computeMomentOrder1, cloudComPy_computeMomentOrder1_doc);

    m0.def("filterBySFValue", &filterBySFValue, py::return_value_policy::reference, cloudComPy_filterBySFValue_doc);

    m0.def("GetPointCloudRadius", &GetPointCloudRadius,
           py::arg("clouds"), py::arg("nodes")=12, cloudComPy_GetPointCloudRadius_doc);

    m0.def("getScalarType", &getScalarType, cloudComPy_getScalarType_doc);

    py::class_<ICPres>(m0, "ICPres", cloudComPy_ICPres_doc)
       .def(py::init<>())
       .def_readonly("aligned", &ICPres::aligned, py::return_value_policy::reference_internal)
       .def_readwrite("transMat", &ICPres::transMat,
                       cloudComPy_ICPres_doc)
       .def_readwrite("finalScale", &ICPres::finalScale,
                      cloudComPy_ICPres_doc)
       .def_readwrite("finalRMS", &ICPres::finalRMS,
                      cloudComPy_ICPres_doc)
       .def_readwrite("finalPointCount", &ICPres::finalPointCount,
                      cloudComPy_ICPres_doc)
    ;

    m0.def("ICP", &ICP_py,
           py::arg("data"), py::arg("model"), py::arg("minRMSDecrease"), py::arg("maxIterationCount"), py::arg("randomSamplingLimit"),
           py::arg("removeFarthestPoints"), py::arg("method"), py::arg("adjustScale"), py::arg("finalOverlapRatio")=1.0,
           py::arg("useDataSFAsWeights")=false, py::arg("useModelSFAsWeights")=false,
           py::arg("transformationFilters")=CCCoreLib::RegistrationTools::SKIP_NONE,
           py::arg("maxThreadCount")=0,
           cloudComPy_ICP_doc);

    m0.def("computeNormals", &computeNormals,
           py::arg("selectedEntities"), py::arg("model")=CCCoreLib::LS,
           py::arg("useScanGridsForComputation")=true, py::arg("defaultRadius")=0.0, py::arg("minGridAngle_deg")=1.0,
           py::arg("orientNormals")=true, py::arg("useScanGridsForOrientation")=true,
           py::arg("useSensorsForOrientation")=true, py::arg("preferredOrientation")=ccNormalVectors::UNDEFINED,
           py::arg("orientNormalsMST")=true, py::arg("mstNeighbors")=6, py::arg("computePerVertexNormals")=true,
           cloudComPy_computeNormals_doc);

    py::class_<ReportInfoVol>(m0, "ReportInfoVol", cloudComPy_ReportInfoVol_doc)
        .def(py::init<>())
		.def_readonly("volume", &ReportInfoVol::volume)
		.def_readonly("addedVolume", &ReportInfoVol::addedVolume)
		.def_readonly("removedVolume", &ReportInfoVol::removedVolume)
		.def_readonly("surface", &ReportInfoVol::surface)
		.def_readonly("matchingPercent", &ReportInfoVol::matchingPercent)
		.def_readonly("ceilNonMatchingPercent", &ReportInfoVol::ceilNonMatchingPercent)
		.def_readonly("groundNonMatchingPercent", &ReportInfoVol::groundNonMatchingPercent)
		.def_readonly("averageNeighborsPerCell", &ReportInfoVol::averageNeighborsPerCell)
		;

    m0.def("ComputeVolume25D", &ComputeVolume25D,
           py::arg("reportInfo"), py::arg("ground"), py::arg("ceil"), py::arg("vertDim"),
           py::arg("gridStep"), py::arg("groundHeight"), py::arg("ceilHeight"),
           py::arg("projectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("groundEmptyCellFillStrategy")=ccRasterGrid::LEAVE_EMPTY,
           py::arg("groundMaxEdgeLength")=0.0,
           py::arg("ceilEmptyCellFillStrategy")=ccRasterGrid::LEAVE_EMPTY,
           py::arg("ceilMaxEdgeLength")=0,
           cloudComPy_ComputeVolume25D_doc);

    m0.def("invertNormals", &invertNormals, cloudComPy_invertNormals_doc);

    m0.def("ExtractConnectedComponents", &ExtractConnectedComponents_py,
           py::arg("clouds"),
           py::arg("octreeLevel")=8,
           py::arg("minComponentSize")=100,
           py::arg("maxNumberComponents")=100,
           py::arg("randomColors")=false,
           cloudComPy_ExtractConnectedComponents_doc);

    m0.def("LabelConnectedComponents", &LabelConnectedComponents_py,
           py::arg("clouds"),
           py::arg("octreeLevel")=8,
           cloudComPy_LabelConnectedComponents_doc);

    m0.def("ExtractSlicesAndContours", &ExtractSlicesAndContours_py,
            py::arg("entities"),
            py::arg("bbox"),
            py::arg("bboxTrans")=ccGLMatrix(),
            py::arg("singleSliceMode")=true,
            py::arg("processRepeatX")=false,
            py::arg("processRepeatY")=false,
            py::arg("processRepeatZ")=true,
            py::arg("extractEnvelopes")=false,
            py::arg("maxEdgeLength")=0,
            py::arg("envelopeType")=0,
            py::arg("extractLevelSet")=false,
            py::arg("levelSetGridStep")=0,
            py::arg("levelSetMinVertCount")=0,
            py::arg("gap")=0,
            py::arg("multiPass")=false,
            py::arg("splitEnvelopes")=false,
            py::arg("projectOnBestFitPlane")=false,
            py::arg("generateRandomColors")=false,
            cloudComPy_ExtractSlicesAndContours_doc);

    m0.def("MergeEntities", &MergeEntitiesPy,
           py::arg("entities"),
           py::arg("deleteOriginalClouds")=false,
           py::arg("createSFcloudIndex")=false,
           py::arg("createSubMeshes")=false,
           cloudComPy_MergeEntities_doc);

    m0.def("RasterizeToCloud", &RasterizeToCloud,
           py::arg("cloud"),
           py::arg("gridStep"),
           py::arg("vertDir") = CC_DIRECTION::Z,
           py::arg("outputRasterZ")=false,
           py::arg("outputRasterSFs")=false,
           py::arg("outputRasterRGB")=false,
           py::arg("pathToImages")=".",
           py::arg("resample")=false,
           py::arg("projectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("sfProjectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("emptyCellFillStrategy")=ccRasterGrid::LEAVE_EMPTY,
           py::arg("DelaunayMaxEdgeLength")=1.0,
           py::arg("KrigingParamsKNN")=8,
           py::arg("customHeight")=std::numeric_limits<double>::quiet_NaN(),
           py::arg("gridBBox")=ccBBox(),
           py::arg("percentile")=50.0,
           py::arg("export_perCellCount")=false,
           py::arg("export_perCellMinHeight")=false,
           py::arg("export_perCellMaxHeight")=false,
           py::arg("export_perCellAvgHeight")=false,
           py::arg("export_perCellHeightStdDev")=false,
           py::arg("export_perCellHeightRange")=false,
           py::arg("export_perCellMedian")=false,
           py::arg("export_perCellPercentile")=false,
           py::arg("export_perCellUniqueCount")=false,
           cloudComPy_RasterizeToCloud_doc,
           py::return_value_policy::reference);

    m0.def("RasterizeToMesh", &RasterizeToMesh,
           py::arg("cloud"),
           py::arg("gridStep"),
           py::arg("vertDir") = CC_DIRECTION::Z,
           py::arg("outputRasterZ")=false,
           py::arg("outputRasterSFs")=false,
           py::arg("outputRasterRGB")=false,
           py::arg("pathToImages")=".",
           py::arg("resample")=false,
           py::arg("projectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("sfProjectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("emptyCellFillStrategy")=ccRasterGrid::LEAVE_EMPTY,
           py::arg("DelaunayMaxEdgeLength")=1.0,
           py::arg("KrigingParamsKNN")=8,
           py::arg("customHeight")=std::numeric_limits<double>::quiet_NaN(),
           py::arg("gridBBox")=ccBBox(),
           py::arg("percentile")=50.0,
           py::arg("export_perCellCount")=false,
           py::arg("export_perCellMinHeight")=false,
           py::arg("export_perCellMaxHeight")=false,
           py::arg("export_perCellAvgHeight")=false,
           py::arg("export_perCellHeightStdDev")=false,
           py::arg("export_perCellHeightRange")=false,
           py::arg("export_perCellMedian")=false,
           py::arg("export_perCellPercentile")=false,
           py::arg("export_perCellUniqueCount")=false,
           cloudComPy_RasterizeToMesh_doc,
           py::return_value_policy::reference);

    m0.def("RasterizeGeoTiffOnly", &RasterizeGeoTiffOnly,
           py::arg("cloud"),
           py::arg("gridStep"),
           py::arg("vertDir") = CC_DIRECTION::Z,
           py::arg("outputRasterZ")=false,
           py::arg("outputRasterSFs")=false,
           py::arg("outputRasterRGB")=false,
           py::arg("pathToImages")=".",
           py::arg("resample")=false,
           py::arg("projectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("sfProjectionType")=ccRasterGrid::PROJ_AVERAGE_VALUE,
           py::arg("emptyCellFillStrategy")=ccRasterGrid::LEAVE_EMPTY,
           py::arg("DelaunayMaxEdgeLength")=1.0,
           py::arg("KrigingParamsKNN")=8,
           py::arg("customHeight")=std::numeric_limits<double>::quiet_NaN(),
           py::arg("gridBBox")=ccBBox(),
           py::arg("percentile")=50.0,
           py::arg("export_perCellCount")=false,
           py::arg("export_perCellMinHeight")=false,
           py::arg("export_perCellMaxHeight")=false,
           py::arg("export_perCellAvgHeight")=false,
           py::arg("export_perCellHeightStdDev")=false,
           py::arg("export_perCellHeightRange")=false,
           py::arg("export_perCellMedian")=false,
           py::arg("export_perCellPercentile")=false,
           py::arg("export_perCellUniqueCount")=false,
           cloudComPy_RasterizeGeoTiffOnly_doc,
           py::return_value_policy::reference);

    m0.def("extractPointsAlongSections", &extractPointsAlongSections,
           py::arg("clouds"),
           py::arg("sections"),
           py::arg("defaultSectionThickness")=-1.0,
           py::arg("envelopeMaxEdgeLength")=0,
           py::arg("extractSectionsAsClouds")=false,
           py::arg("extractSectionsAsEnvelopes")=true,
           py::arg("multipass")=false,
           py::arg("splitEnvelope")=false,
           py::arg("s_extractSectionsType")=EnvelopeType::ENV_LOWER,
           py::arg("vertDim")=2,
           cloudComPy_extractPointsAlongSections_doc);

    m0.def("unfoldPointsAlongPolylines", &unfoldPointsAlongPolylines,
           py::arg("clouds"),
           py::arg("polylines"),
           py::arg("thickness"),
           py::arg("vertDim")=2,
           cloudComPy_unfoldPointsAlongPolylines_doc);

    m0.def("addToRenderScene", &addToRenderScene,
    		py::arg("obj"), py::arg("showScalar")=true, cloudComPy_addToRenderScene_doc);

    m0.def("removeFromRenderScene", &removeFromRenderScene, cloudComPy_removeFromRenderScene_doc);

    m0.def("render", &renderPy,
    		py::arg("filename"), py::arg("width")=1500, py::arg("height")=1000, py::arg("isInteractive")=false,
    		cloudComPy_render_doc);

    m0.def("setOrthoView", &setOrthoView, cloudComPy_setOrthoView_doc);
    m0.def("setCenteredPerspectiveView", &setCenteredPerspectiveView, cloudComPy_setCenteredPerspectiveView_doc);
    m0.def("setViewerPerspectiveView", &setViewerPerspectiveView, cloudComPy_setViewerPerspectiveView_doc);
    m0.def("setGlobalZoom", &setGlobalZoom, cloudComPy_setGlobalZoom_doc);
    m0.def("zoomOnSelectedEntity", &zoomOnSelectedEntity, cloudComPy_zoomOnSelectedEntity_doc);
    m0.def("setFrontView", &setFrontView, cloudComPy_setFrontView_doc);
    m0.def("setBottomView", &setBottomView, cloudComPy_setBottomView_doc);
    m0.def("setTopView", &setTopView, cloudComPy_setTopView_doc);
    m0.def("setBackView", &setBackView, cloudComPy_setBackView_doc);
    m0.def("setLeftView", &setLeftView, cloudComPy_setLeftView_doc);
    m0.def("setRightView", &setRightView, cloudComPy_setRightView_doc);
    m0.def("setIsoView1", &setIsoView1, cloudComPy_setIsoView1_doc);
    m0.def("setIsoView2", &setIsoView2, cloudComPy_setIsoView2_doc);
    m0.def("setCustomView", &setCustomView, cloudComPy_setCustomView_doc);
    m0.def("setCameraPos", &setCameraPos, cloudComPy_setCameraPos_doc);
    m0.def("setBackgroundColor", &setBackgroundColor,
           py::arg("gradient")=false , py::arg("r")=255, py::arg("g")=255, py::arg("b")=255,
           cloudComPy_setBackgroundColor_doc);
    m0.def("setTextDefaultCol", &setTextDefaultCol,
           py::arg("r")=0 , py::arg("g")=0, py::arg("b")=0, py::arg("a")=255,
           cloudComPy_setTextDefaultCol_doc);
    m0.def("setColorScaleShowHistogram", & setColorScaleShowHistogram,
           py::arg("showHist")=true,
           cloudComPy_setColorScaleShowHistogram_doc);
}
