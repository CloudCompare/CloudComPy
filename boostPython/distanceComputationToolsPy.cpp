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
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
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

BOOST_PYTHON_FUNCTION_OVERLOADS(computeCloud2CloudDistance_overloads, CCCoreLib::DistanceComputationTools::computeCloud2CloudDistance, 3, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(computeCloud2MeshDistance_overloads, CCCoreLib::DistanceComputationTools::computeCloud2MeshDistance, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(computeApproxCloud2CloudDistance_overloads, CCCoreLib::DistanceComputationTools::computeApproxCloud2CloudDistance, 3, 7)

bool setSplitDistances(CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams& self, size_t count)
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

CCCoreLib::ScalarField* getSplitDistance(CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams& self, int index)
{
    if (index < 0 || index > 2)
        return nullptr;
    return self.splitDistances[index];
}

void export_distanceComputationTools()
{

    class_<GenericProgressCallbackWrap, boost::noncopyable>("GenericProgressCallback", no_init)
        ;

    class_<CCCoreLib::ReferenceCloud>("ReferenceCloud", no_init)
        ;

    enum_<CCCoreLib::LOCAL_MODEL_TYPES>("LOCAL_MODEL_TYPES")
        .value("NO_MODEL", CCCoreLib::NO_MODEL)
        .value("LS", CCCoreLib::LS)
        .value("TRI", CCCoreLib::TRI)
        .value("QUADRIC", CCCoreLib::QUADRIC)
        ;

    class_<CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams>("Cloud2CloudDistanceComputationParams")
        .def_readwrite("octreeLevel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::octreeLevel)
        .def_readwrite("maxSearchDist", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::maxSearchDist)
        .def_readwrite("multiThread", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::multiThread)
        .def_readwrite("maxThreadCount", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::maxThreadCount)
        .def_readwrite("localModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::localModel)
        .def_readwrite("useSphericalSearchForLocalModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::useSphericalSearchForLocalModel)
        .def_readwrite("kNNForLocalModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::kNNForLocalModel)
        .def_readwrite("radiusForLocalModel", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::radiusForLocalModel)
        .def_readwrite("reuseExistingLocalModels", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::reuseExistingLocalModels)
        .def_readwrite("CPSet", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::CPSet)
        .def("setSplitDistances", &setSplitDistances)
        .def("getSplitDistance", &getSplitDistance, return_value_policy<reference_existing_object>())
        .def_readwrite("resetFormerDistances", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::resetFormerDistances)
        ;

    class_<CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams>("Cloud2MeshDistanceComputationParams")
        .def_readwrite("octreeLevel", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::octreeLevel)
        .def_readwrite("maxSearchDist", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::maxSearchDist)
        .def_readwrite("useDistanceMap", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::useDistanceMap)
        .def_readwrite("signedDistances", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::signedDistances)
        .def_readwrite("flipNormals", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::flipNormals)
        .def_readwrite("multiThread", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::multiThread)
        .def_readwrite("maxThreadCount", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::maxThreadCount)
        .def_readwrite("CPSet", &CCCoreLib::DistanceComputationTools::Cloud2MeshDistanceComputationParams::CPSet)
        ;

    class_<CCCoreLib::DistanceComputationTools, boost::noncopyable>("DistanceComputationTools", no_init)
        .def("computeCloud2CloudDistance", &CCCoreLib::DistanceComputationTools::computeCloud2CloudDistance, computeCloud2CloudDistance_overloads())
            .staticmethod("computeCloud2CloudDistance")
        .def("computeCloud2MeshDistance", &CCCoreLib::DistanceComputationTools::computeCloud2MeshDistance, computeCloud2MeshDistance_overloads())
            .staticmethod("computeCloud2MeshDistance")
        .def("computeApproxCloud2CloudDistance", &CCCoreLib::DistanceComputationTools::computeApproxCloud2CloudDistance, computeApproxCloud2CloudDistance_overloads())
            .staticmethod("computeApproxCloud2CloudDistance")
        ;
}
