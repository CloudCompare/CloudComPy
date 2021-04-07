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
#include <DgmOctree.h>
#include <ccScalarField.h>
#include <DistanceComputationTools.h>
#include <GenericProgressCallback.h>

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

int computeCloud2CloudDistance1(ccPointCloud* comparedCloud,
    ccPointCloud* referenceCloud,
    CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams& params)
{
    return CCCoreLib::DistanceComputationTools::computeCloud2CloudDistance(comparedCloud, referenceCloud, params,
                                                                           nullptr, nullptr, nullptr);
}

int computeCloud2CloudDistance2(ccPointCloud* comparedCloud,
    ccPointCloud* referenceCloud,
    CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams& params,
    CCCoreLib::DgmOctree* compOctree)
{
    return CCCoreLib::DistanceComputationTools::computeCloud2CloudDistance(comparedCloud, referenceCloud, params,
                                                                           nullptr, compOctree, nullptr);
}

int computeCloud2CloudDistance3(ccPointCloud* comparedCloud,
    ccPointCloud* referenceCloud,
    CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams& params,
    CCCoreLib::DgmOctree* compOctree,
    CCCoreLib::DgmOctree* refOctree)
{
    return CCCoreLib::DistanceComputationTools::computeCloud2CloudDistance(comparedCloud, referenceCloud, params,
                                                                           nullptr, compOctree, refOctree);
}

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

    class_<CCCoreLib::DistanceComputationTools, boost::noncopyable>("DistanceComputationTools", no_init)
        .def("computeCloud2CloudDistance", computeCloud2CloudDistance1)
            .staticmethod("computeCloud2CloudDistance")
        .def("computeCloud2CloudDistance2", computeCloud2CloudDistance2)
            .staticmethod("computeCloud2CloudDistance2")
        .def("computeCloud2CloudDistance3", computeCloud2CloudDistance3)
        .staticmethod("computeCloud2CloudDistance3")
        ;
}
