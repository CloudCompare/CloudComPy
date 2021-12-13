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

#include "distanceComputationToolsPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccPointCloud.h>
#include <GenericIndexedCloudPersist.h>
#include <PointCloud.h>
#include <DgmOctree.h>
#include <ccScalarField.h>
#include <DistanceComputationTools.h>
#include <GenericProgressCallback.h>
#include <ccMesh.h>
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

BOOST_PYTHON_FUNCTION_OVERLOADS(computeCloud2CloudDistances_overloads, CCCoreLib::DistanceComputationTools::computeCloud2CloudDistances, 3, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(computeCloud2MeshDistances_overloads, CCCoreLib::DistanceComputationTools::computeCloud2MeshDistances, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(computeApproxCloud2CloudDistance_overloads, CCCoreLib::DistanceComputationTools::computeApproxCloud2CloudDistance, 3, 7)

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
             &CCCoreLib::DistanceComputationTools::computeCloud2CloudDistances,
             computeCloud2CloudDistances_overloads(distanceComputationToolsPy_computeCloud2CloudDistances_doc))
            .staticmethod("computeCloud2CloudDistances")
        .def("computeCloud2MeshDistances",
             &CCCoreLib::DistanceComputationTools::computeCloud2MeshDistances,
             computeCloud2MeshDistances_overloads(distanceComputationToolsPy_computeCloud2MeshDistances_doc))
            .staticmethod("computeCloud2MeshDistances")
        .def("computeApproxCloud2CloudDistance",
             &CCCoreLib::DistanceComputationTools::computeApproxCloud2CloudDistance,
             computeApproxCloud2CloudDistance_overloads(distanceComputationToolsPy_computeApproxCloud2CloudDistance_doc))
            .staticmethod("computeApproxCloud2CloudDistance")
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
