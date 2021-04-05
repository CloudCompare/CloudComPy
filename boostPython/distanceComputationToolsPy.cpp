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
#include <DistanceComputationTools.h>
#include <GenericProgressCallback.h>

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;


void export_distanceComputationTools()
{
    class_<CCCoreLib::ReferenceCloud>("ReferenceCloud", no_init)
        ;

//    class_<CCCoreLib::GenericProgressCallback>("GenericProgressCallback", no_init)
//        ;

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
//        .def_readwrite("splitDistances_X", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::splitDistances[0])
//        .def_readwrite("splitDistances_Y", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::splitDistances[1])
//        .def_readwrite("splitDistances_Z", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::splitDistances[2])
        .def_readwrite("resetFormerDistances", &CCCoreLib::DistanceComputationTools::Cloud2CloudDistanceComputationParams::resetFormerDistances)
        ;

    class_<CCCoreLib::DistanceComputationTools, boost::noncopyable>("DistanceComputationTools", no_init)
//        .def("computeCloud2CloudDistance", CCCoreLib::DistanceComputationTools::computeCloud2CloudDistance)
//            .staticmethod("computeCloud2CloudDistance")
        ;
}
