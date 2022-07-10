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
#include "distanceComputationToolsPy.hpp"

#include <ccPointCloud.h>
#include <GenericIndexedCloudPersist.h>
#include <PointCloud.h>
#include <DgmOctree.h>
#include <ccScalarField.h>
#include <DistanceComputationTools.h>
#include <GenericProgressCallback.h>
#include <ccMesh.h>
#include <MeshSamplingTools.h>
#include "distanceComputationToolsPy_DocStrings.hpp"

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

struct GenericProgressCallbackWrap : CCCoreLib::GenericProgressCallback, wrapper<CCCoreLib::GenericProgressCallback>
{
    virtual void update(float percent)
    {
        this->get_override("update")(percent);
    }
    virtual void setMethodTitle(const char* methodTitle)
    {
        this->get_override("setMethodTitle")(methodTitle);
    }
    virtual void setInfo(const char* infoStr)
    {
        this->get_override("setInfo")(infoStr);
    }
    virtual void start()
    {
        this->get_override("start")();
    }
    virtual void stop()
    {
        this->get_override("stop")();
    }
    virtual bool isCancelRequested()
    {
        return this->get_override("isCancelRequested")();
    }
};
std::vector<double> computeApproxCloud2CloudDistance_py(CCCoreLib::GenericIndexedCloudPersist* comparedCloud,
                                                        CCCoreLib::GenericIndexedCloudPersist* referenceCloud,
                                                        unsigned char octreeLevel = 7,
                                                        PointCoordinateType maxSearchDist = 0,
                                                        CCCoreLib::GenericProgressCallback* progressCb=nullptr,
                                                        CCCoreLib::DgmOctree* compOctree=nullptr,
                                                        CCCoreLib::DgmOctree* refOctree=nullptr)
{
    std::vector<double> result;
    ccPointCloud* compCloud = dynamic_cast<ccPointCloud*>(comparedCloud);
    if (compCloud == nullptr)
    {
        CCTRACE("comparedCloud is not of the right type");
        return result;
    }
    //does the cloud has already a temporary scalar field that we can use?
    int sfIdx = compCloud->getScalarFieldIndexByName("Approx. distances");
    if (sfIdx < 0)
    {
        //we need to create a new scalar field
        sfIdx = compCloud->addScalarField("Approx. distances");
        if (sfIdx < 0)
        {
            CCTRACE("Couldn't allocate a new scalar field for computing approx. distances! Try to free some memory ...");
            return result;
        }
    }
    compCloud->setCurrentScalarField(sfIdx);
    int ret = CCCoreLib::DistanceComputationTools::computeApproxCloud2CloudDistance(compCloud,
                                                                                   referenceCloud,
                                                                                   octreeLevel,
                                                                                   maxSearchDist,
                                                                                   progressCb,
                                                                                   compOctree,
                                                                                   refOctree);
    if (ret < 0)
        return result;
    CCCoreLib::ScalarField* sf = compCloud->getScalarField(sfIdx);
    ScalarType mean;
    ScalarType variance;
    sf->computeMinAndMax();
    sf->computeMeanAndVariance(mean,&variance);
    result.resize(5);
    result[0] = sf->getMin();
    result[1] = sf->getMax();
    result[2] = mean;
    result[3] = variance;
    if (!compCloud->getOctree())
        compCloud->computeOctree();
    result[4] = compCloud->getOctree()->getCellSize(octreeLevel)/2.0;
    return result;
}

std::vector<double> computeApproxCloud2MeshDistance_py(CCCoreLib::GenericIndexedCloudPersist* cloud,
                                                       CCCoreLib::GenericIndexedMesh* mesh)
{
    std::vector<double> result;
    ccPointCloud* compCloud = dynamic_cast<ccPointCloud*>(cloud);
    if (compCloud == nullptr)
    {
        CCTRACE("comparedCloud is not of the right type");
        return result;
    }
    //does the cloud has already a temporary scalar field that we can use?
    int sfIdx = compCloud->getScalarFieldIndexByName("Approx. distances");
    if (sfIdx < 0)
    {
        //we need to create a new scalar field
        sfIdx = compCloud->addScalarField("Approx. distances");
        if (sfIdx < 0)
        {
            CCTRACE("Couldn't allocate a new scalar field for computing approx. distances! Try to free some memory ...");
            return result;
        }
    }
    compCloud->setCurrentScalarField(sfIdx);
    CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams c2mParams;
    {
        c2mParams.octreeLevel = 7;
        c2mParams.maxSearchDist = 0;
        c2mParams.useDistanceMap = true;
        c2mParams.signedDistances = false;
        c2mParams.flipNormals = false;
        c2mParams.multiThread = false;
    }
    int ret = CCCoreLib::DistanceComputationTools::computeCloud2MeshDistances( compCloud,
                                                                               mesh,
                                                                               c2mParams,
                                                                               nullptr,
                                                                               nullptr);
    if (ret != 1)
        return result;
    CCCoreLib::ScalarField* sf = compCloud->getScalarField(sfIdx);
    ScalarType mean;
    ScalarType variance;
    sf->computeMinAndMax();
    sf->computeMeanAndVariance(mean,&variance);
    result.resize(5);
    result[0] = sf->getMin();
    result[1] = sf->getMax();
    result[2] = mean;
    result[3] = variance;
    if (!compCloud->getOctree())
        compCloud->computeOctree();
    result[4] = compCloud->getOctree()->getCellSize(7)/2.0;
    return result;
}

int computeCloud2CloudDistances_py( CCCoreLib::GenericIndexedCloudPersist* comparedCloud,
                                    CCCoreLib::GenericIndexedCloudPersist* referenceCloud,
                                    CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams& params,
                                    CCCoreLib::GenericProgressCallback* progressCb=nullptr,
                                    CCCoreLib::DgmOctree* compOctree=nullptr,
                                    CCCoreLib::DgmOctree* refOctree=nullptr)
{
    ccPointCloud* compCloud = dynamic_cast<ccPointCloud*>(comparedCloud);
    if (compCloud == nullptr)
        return CCCoreLib::DistanceComputationTools::ERROR_NULL_COMPAREDCLOUD;
    //does the cloud has already a temporary scalar field that we can use?
    int sfIdx = compCloud->getScalarFieldIndexByName("Temp. approx. distances");
    if (sfIdx < 0)
    {
        //we need to create a new scalar field
        sfIdx = compCloud->addScalarField("Temp. approx. distances");
        if (sfIdx < 0)
        {
            CCTRACE("Couldn't allocate a new scalar field for computing distances! Try to free some memory ...");
            return CCCoreLib::DistanceComputationTools::ERROR_OUT_OF_MEMORY;
        }
    }
    compCloud->setCurrentScalarField(sfIdx);
    int ret = CCCoreLib::DistanceComputationTools::computeCloud2CloudDistances(compCloud, referenceCloud, params,
                                                                               progressCb, compOctree, refOctree);
    CCTRACE("return code computeCloud2CloudDistances: " << ret);
    if (ret <= 0)
        return ret;
    CCCoreLib::ScalarField* sf = compCloud->getScalarField(sfIdx);
    sf->computeMinAndMax();
    sf->setName("C2C absolute distances");
    const char* sfNames[] = {"C2C absolute distances (X)", "C2C absolute distances (Y)", "C2C absolute distances (Z)"};
    for (int i=0; i<3; i++)
    if (ccScalarField* sfi = dynamic_cast<ccScalarField*>(params.splitDistances[i]))
    {
        sfi->computeMinAndMax();
        sfi->setName(sfNames[i]);
        int isf = compCloud->addScalarField(sfi);
        params.splitDistances[i] = nullptr;
    }
    return 1;
}

int computeCloud2MeshDistances_py(  CCCoreLib::GenericIndexedCloudPersist* pointCloud,
                                    CCCoreLib::GenericIndexedMesh* mesh,
                                    CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams& params,
                                    CCCoreLib::GenericProgressCallback* progressCb=nullptr,
                                    CCCoreLib::DgmOctree* cloudOctree=nullptr)
{
    ccPointCloud* compCloud = dynamic_cast<ccPointCloud*>(pointCloud);
    if (compCloud == nullptr)
        return CCCoreLib::DistanceComputationTools::ERROR_NULL_COMPAREDCLOUD;
    //does the cloud has already a temporary scalar field that we can use?
    int sfIdx = compCloud->getScalarFieldIndexByName("Temp. approx. distances");
    if (sfIdx < 0)
    {
        //we need to create a new scalar field
        sfIdx = compCloud->addScalarField("Temp. approx. distances");
        if (sfIdx < 0)
        {
            CCTRACE("Couldn't allocate a new scalar field for computing distances! Try to free some memory ...");
            return CCCoreLib::DistanceComputationTools::ERROR_OUT_OF_MEMORY;
        }
    }
    compCloud->setCurrentScalarField(sfIdx);
    int ret = CCCoreLib::DistanceComputationTools::computeCloud2MeshDistances(compCloud, mesh, params,
                                                                              progressCb, cloudOctree);
    if (ret != 1)
        return ret;
    CCCoreLib::ScalarField* sf = compCloud->getScalarField(sfIdx);
    sf->computeMinAndMax();
    sf->setName("C2M absolute distances");
    return 1;
}

int determineBestOctreeLevel_py(ccPointCloud* compCloud,
                                CCCoreLib::GenericIndexedMesh* refMesh=nullptr,
                                ccPointCloud* refCloud=nullptr,
                                double maxSearchDist=0.)
{
    // adapted from ccComparisonDlg::determineBestOctreeLevel
    if ((refMesh == nullptr && refCloud == nullptr) || (refMesh != nullptr && refCloud != nullptr))
    {
        CCTRACE("give either the reference mesh or the reference cloud");
        return -1;
    }

    //make sure a the temporary dist. SF is activated
    int sfIdx = compCloud->getScalarFieldIndexByName("Approx. distances");
    if (sfIdx < 0)
    {
        CCTRACE("Compute approx. distances");
        std::vector<double> res;
        if (refMesh)
            res = computeApproxCloud2MeshDistance_py(compCloud, refMesh);
        else
            res = computeApproxCloud2CloudDistance_py(compCloud, refCloud);
        if (res.size() < 5)
        {
            //failed to compute approx distances?!
            return -1;
        }
        sfIdx = compCloud->getScalarFieldIndexByName("Approx. distances");
    }

    const CCCoreLib::ScalarField* approxDistances = compCloud->getScalarField(sfIdx);
    if (!approxDistances)
    {
        CCTRACE("determineBestOctreeLevel, internal error");
        return -1;
    }

    //evalutate the theoretical time for each octree level
    const int MAX_OCTREE_LEVEL = refMesh ? 9 : CCCoreLib::DgmOctree::MAX_OCTREE_LEVEL; //DGM: can't go higher than level 9 with a mesh as the grid is 'plain' and would take too much memory!
    std::vector<double> timings;
    try
    {
        timings.resize(MAX_OCTREE_LEVEL, 0);
    }
    catch (const std::bad_alloc&)
    {
        CCTRACE("Can't determine best octree level: not enough memory!");
        return -1;
    }

    //if the reference is a mesh
    double meanTriangleSurface = 1.0;
    CCCoreLib::GenericIndexedMesh* mesh = nullptr;
    ccOctree::Shared refOctree = nullptr;
    if (refCloud)
    {
        refOctree = refCloud->getOctree();
        if (!refOctree)
        {
            //CCTRACE("compute octree for ref cloud...")
            refOctree = refCloud->computeOctree();
        }
    }
    if (!refOctree)
    {
        if (!refMesh)
        {
            CCTRACE("Internal error: reference entity should be a mesh!");
            return -1;
        }
        mesh = static_cast<CCCoreLib::GenericIndexedMesh*>(refMesh);
        if (!mesh || mesh->size() == 0)
        {
            CCTRACE("Can't determine best octree level: mesh is empty!");
            return -1;
        }
        //total mesh surface
        double meshSurface = CCCoreLib::MeshSamplingTools::computeMeshArea(mesh);
        //average triangle surface
        if (meshSurface > 0)
        {
            meanTriangleSurface = meshSurface / mesh->size();
        }
    }

    //we skip the lowest subdivision levels (useless + incompatible with below formulas ;)
    static const int s_minOctreeLevel = 6;
    int theBestOctreeLevel = s_minOctreeLevel;

    //we don't test the very first and very last level

    bool maxDistanceDefined = maxSearchDist > 0;
    PointCoordinateType maxDistance = maxSearchDist;

    uint64_t maxNeighbourhoodVolume = static_cast<uint64_t>(1) << (3 * MAX_OCTREE_LEVEL);

    ccOctree::Shared compOctree = nullptr;
    compOctree = compCloud->getOctree();
    if (!compOctree)
    {
        CCTRACE("Strange, octree should be computed here...")
        compOctree = compCloud->computeOctree();
    }

    //for each level
    for (int level = s_minOctreeLevel; level < MAX_OCTREE_LEVEL; ++level)
    {
        const unsigned char bitDec = CCCoreLib::DgmOctree::GET_BIT_SHIFT(level);
        unsigned numberOfPointsInCell = 0;
        unsigned index = 0;
        double cellDist = -1;

        //we compute a 'correction factor' that converts an approximate distance into an
        //approximate size of the neighborhood (in terms of cells)

        PointCoordinateType cellSize = compOctree->getCellSize(static_cast<unsigned char>(level));

        //we also use the reference cloud density (points/cell) if we have the info
        double refListDensity = 1.0;
        if (refOctree)
        {
            refListDensity = refOctree->computeMeanOctreeDensity(static_cast<unsigned char>(level));
        }

        CCCoreLib::DgmOctree::CellCode tempCode = 0xFFFFFFFF;

        //scan the octree structure
        const CCCoreLib::DgmOctree::cellsContainer& compCodes = compOctree->pointsAndTheirCellCodes();
        for (CCCoreLib::DgmOctree::cellsContainer::const_iterator c = compCodes.begin(); c != compCodes.end(); ++c)
        {
            CCCoreLib::DgmOctree::CellCode truncatedCode = (c->theCode >> bitDec);

            //new cell?
            if (truncatedCode != tempCode)
            {
                //if it's a real cell
                if (numberOfPointsInCell != 0)
                {
                    //if 'maxSearchDist' has been defined by the user, we must take it into account!
                    //(in this case we skip the cell if its approx. distance is superior)
                    if (maxSearchDist <= 0 || cellDist <= maxSearchDist)
                    {
                        //approx. neighborhood radius
                        cellDist /= cellSize;

                        //approx. neighborhood width (in terms of cells)
                        double neighbourSize = 2.0*cellDist + 1.0;

                        //if the reference is a mesh
                        if (mesh)
                        {
                            //(integer) approximation of the neighborhood size (in terms of cells)
                            int nCell = static_cast<int>(ceil(cellDist));

                            //Probable mesh surface in this neighborhood
                            double crossingMeshSurface = (2.0*nCell+1.0) * cellSize;
                            //squared surface!
                            crossingMeshSurface *= crossingMeshSurface;

                            //neighborhood "volume" (in terms of cells)
                            double neighbourSize3 = (neighbourSize*neighbourSize*neighbourSize) / maxNeighbourhoodVolume;

                            //TIME = NEIGHBORS SEARCH + proportional factor * POINTS/TRIANGLES COMPARISONS
                            timings[level] += neighbourSize3 + ((0.5 * numberOfPointsInCell) / maxNeighbourhoodVolume) * (crossingMeshSurface / meanTriangleSurface);
                        }
                        else
                        {
                            //we ignore the "central" cell
                            neighbourSize -= 1.0;
                            //neighborhood "volume" (in terms of cells)
                            double neighbourSize3 = (neighbourSize*neighbourSize*neighbourSize) / maxNeighbourhoodVolume;
                            //volume of the last "slice" (in terms of cells)
                            //=V(n)-V(n-1) = (2*n+1)^3 - (2*n-1)^3 = 24 * n^2 + 2 (if n > 0)
                            double lastSliceCellCount = (cellDist > 0 ? cellDist*cellDist * 24.0 + 2.0 : 1.0);
                            //TIME = NEIGHBORS SEARCH + proportional factor * POINTS/TRIANGLES COMPARISONS
                            //(we admit that the filled cells roughly correspond to the sqrt of the total number of cells)
                            timings[level] += neighbourSize3 + 0.1 * ((numberOfPointsInCell * sqrt(lastSliceCellCount) * refListDensity) / maxNeighbourhoodVolume);
                        }
                    }
                }
                numberOfPointsInCell = 0;
                cellDist = 0;
                tempCode = truncatedCode;
            }
            ScalarType pointDist = approxDistances->getValue(index);
            if (maxDistanceDefined && pointDist > maxDistance)
            {
                pointDist = maxDistance;
            }
            //cellDist += pointDist;
            cellDist = std::max<double>(cellDist, pointDist);
            ++index;
            ++numberOfPointsInCell;
        }
       if (timings[level] * 1.05 < timings[theBestOctreeLevel]) //avoid increasing the octree level for super small differences (which is generally counter productive)
        {
            theBestOctreeLevel = level;
        }
    }
    CCTRACE("[Distances] Best level: " << theBestOctreeLevel << " (maxSearchDist = " << maxSearchDist << ")");
    return theBestOctreeLevel;
}

BOOST_PYTHON_FUNCTION_OVERLOADS(computeCloud2CloudDistances_overloads, computeCloud2CloudDistances_py, 3, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(computeCloud2MeshDistances_overloads, computeCloud2MeshDistances_py, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(computeApproxCloud2CloudDistance_overloads, computeApproxCloud2CloudDistance_py, 2, 7)
BOOST_PYTHON_FUNCTION_OVERLOADS(determineBestOctreeLevel_overloads, determineBestOctreeLevel_py, 1, 4)

bool setSplitDistances(CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams& self, size_t count)
{
    bool success = true;
    for (unsigned j = 0; j < 3; ++j)
    {
        ccScalarField* sfDim = new ccScalarField();
        if (sfDim->resizeSafe(count))
        {
            sfDim->link();
            self.splitDistances[j] = sfDim;
        }
        else
        {
            success = false;
            break;
        }
    }
    if (!success)
    {
        CCTRACE("[ComputeDistances] Not enough memory to generate 3D split fields!");

        for (unsigned j = 0; j < 3; ++j)
        {
            if (self.splitDistances[j])
            {
                self.splitDistances[j]->release();
                self.splitDistances[j] = nullptr;
            }
        }
    }
    return success;
}

CCCoreLib::ScalarField* getSplitDistance(CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams& self, int index)
{
    if (index < 0 || index > 2)
        return nullptr;
    return self.splitDistances[index];
}

void export_distanceComputationTools()
{

    class_<GenericProgressCallbackWrap, boost::noncopyable>("GenericProgressCallback", no_init)
        ;

    class_<CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams>("Cloud2CloudDistancesComputationParams",
                                                                                      distanceComputationToolsPy_Cloud2CloudDistancesComputationParams_doc)
        .def_readwrite("octreeLevel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::octreeLevel,
                       distanceComputationToolsPy_octreeLevel_doc)
        .def_readwrite("maxSearchDist", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::maxSearchDist,
                       distanceComputationToolsPy_maxSearchDist_doc)
        .def_readwrite("multiThread", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::multiThread,
                       distanceComputationToolsPy_multiThread_doc)
        .def_readwrite("maxThreadCount", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::maxThreadCount,
                       distanceComputationToolsPy_maxThreadCount_doc)
        .def_readwrite("localModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::localModel,
                       distanceComputationToolsPy_localModel_doc)
        .def_readwrite("useSphericalSearchForLocalModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::useSphericalSearchForLocalModel,
                       distanceComputationToolsPy_useSphericalSearchForLocalModel_doc)
        .def_readwrite("kNNForLocalModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::kNNForLocalModel,
                       distanceComputationToolsPy_kNNForLocalModel_doc)
        .def_readwrite("radiusForLocalModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::radiusForLocalModel,
                       distanceComputationToolsPy_radiusForLocalModel_doc)
        .def_readwrite("reuseExistingLocalModels", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::reuseExistingLocalModels,
                       distanceComputationToolsPy_reuseExistingLocalModels_doc)
        .def_readwrite("CPSet", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::CPSet,
                       distanceComputationToolsPy_CPSet_doc)
        .def("setSplitDistances", &setSplitDistances,
             distanceComputationToolsPy_setSplitDistances_doc)
        .def("getSplitDistance", &getSplitDistance,distanceComputationToolsPy_getSplitDistance_doc,
             return_value_policy<reference_existing_object>())
        .def_readwrite("resetFormerDistances", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistancesComputationParams::resetFormerDistances,
                       distanceComputationToolsPy_resetFormerDistances_doc)
        ;

    class_<CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams>("Cloud2MeshDistancesComputationParams",
                                                                                     distanceComputationToolsPy_Cloud2MeshDistancesComputationParams_doc)
        .def_readwrite("octreeLevel", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::octreeLevel,
                       distanceComputationToolsPy_octreeLevel_doc)
        .def_readwrite("maxSearchDist", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::maxSearchDist,
                       distanceComputationToolsPy_maxSearchDist_doc)
        .def_readwrite("useDistanceMap", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::useDistanceMap,
                       distanceComputationToolsPy_useDistanceMap_doc)
        .def_readwrite("signedDistances", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::signedDistances,
                       distanceComputationToolsPy_signedDistances_doc)
        .def_readwrite("flipNormals", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::flipNormals,
                       distanceComputationToolsPy_flipNormals_doc)
        .def_readwrite("multiThread", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::multiThread,
                       distanceComputationToolsPy_multiThread_doc)
        .def_readwrite("maxThreadCount", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::maxThreadCount,
                       distanceComputationToolsPy_maxThreadCount_doc)
        .def_readwrite("CPSet", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistancesComputationParams::CPSet,
                       distanceComputationToolsPy_CPSet_doc)
        ;

    class_<CCCoreLib::DistanceComputationTools, boost::noncopyable>("DistanceComputationTools",
                                                                    distanceComputationToolsPy_DistanceComputationTools_doc, no_init)
        .def("computeCloud2CloudDistances",
             &computeCloud2CloudDistances_py,
             computeCloud2CloudDistances_overloads(
             (arg("comparedCloud"), arg("referenceCloud"), arg("params"),
              arg("progressCb")=bp::ptr((CCCoreLib::GenericProgressCallback*)nullptr),
              arg("compOctree")=bp::ptr((CCCoreLib::DgmOctree*)nullptr),
              arg("refOctree")=bp::ptr((CCCoreLib::DgmOctree*)nullptr)),
             distanceComputationToolsPy_computeCloud2CloudDistances_doc))
            .staticmethod("computeCloud2CloudDistances")
        .def("computeCloud2MeshDistances",
             &computeCloud2MeshDistances_py,
             computeCloud2MeshDistances_overloads(
             (arg("pointCloud"), arg("mesh"), arg("params"),
                     arg("progressCb")=bp::ptr((CCCoreLib::GenericProgressCallback*)nullptr),
                     arg("cloudOctree")=bp::ptr((CCCoreLib::DgmOctree*)nullptr)),
             distanceComputationToolsPy_computeCloud2MeshDistances_doc))
            .staticmethod("computeCloud2MeshDistances")
        .def("computeApproxCloud2CloudDistance",
             &computeApproxCloud2CloudDistance_py,
             computeApproxCloud2CloudDistance_overloads(
             (arg("comparedCloud"), arg("referenceCloud"), arg("octreeLevel")=7,
              arg("maxSearchDist")=0,
              arg("progressCb")=bp::ptr((CCCoreLib::GenericProgressCallback*)nullptr),
              arg("compOctree")=bp::ptr((CCCoreLib::DgmOctree*)nullptr),
              arg("refOctree")=bp::ptr((CCCoreLib::DgmOctree*)nullptr)),
             distanceComputationToolsPy_computeApproxCloud2CloudDistance_doc))
            .staticmethod("computeApproxCloud2CloudDistance")
        .def("computeApproxCloud2MeshDistance",
              &computeApproxCloud2MeshDistance_py,
              distanceComputationToolsPy_computeApproxCloud2MeshDistance_doc)
             .staticmethod("computeApproxCloud2MeshDistance")
        .def("determineBestOctreeLevel",
             &determineBestOctreeLevel_py,
             determineBestOctreeLevel_overloads(
             (arg("compCloud"),
                     arg("refMesh")=bp::ptr((CCCoreLib::GenericIndexedMesh*)nullptr),
                     arg("refCloud")=bp::ptr((ccPointCloud*)nullptr),
                     arg("maxSearchDist")=0),
             distanceComputationToolsPy_determineBestOctreeLevel_doc))
            .staticmethod("determineBestOctreeLevel")
        ;
    // TODO: methods to add
//    static int computeCloud2ConeEquation(GenericIndexedCloudPersist* cloud, const CCVector3& coneP1, const CCVector3& coneP2, const PointCoordinateType coneR1, const PointCoordinateType coneR2, bool signedDistances = true, bool solutionType = false, double* rms = nullptr);
//    static int computeCloud2CylinderEquation(GenericIndexedCloudPersist* cloud, const CCVector3& cylinderP1, const CCVector3& cylinderP2, const PointCoordinateType cylinderRadius, bool signedDistances = true, bool solutionType = false, double* rms = nullptr);
//    static int computeCloud2SphereEquation(GenericIndexedCloudPersist *cloud, const CCVector3& sphereCenter, const PointCoordinateType sphereRadius, bool signedDistances = true, double* rms = nullptr);
//    static int computeCloud2PlaneEquation(GenericIndexedCloudPersist* cloud, const PointCoordinateType* planeEquation, bool signedDistances = true, double * rms = nullptr);
//    static int computeCloud2RectangleEquation(GenericIndexedCloudPersist *cloud, PointCoordinateType widthX, PointCoordinateType widthY, const SquareMatrix& rotationTransform, const CCVector3& center, bool signedDist = true, double* rms = nullptr);
//    static int computeCloud2BoxEquation(GenericIndexedCloudPersist* cloud, const CCVector3& boxDimensions, const SquareMatrix& rotationTransform, const CCVector3& boxCenter, bool signedDist = true, double* rms = nullptr);
//    static int computeCloud2PolylineEquation(GenericIndexedCloudPersist* cloud, const Polyline* polyline, double* rms = nullptr);
    // TODO: other methods to check

}
