//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
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

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "converters.hpp"
#include "ccDocStrings.hpp"

#include "ScalarFieldPy.hpp"
#include "ccPointCloudPy.hpp"
#include "ccPolylinePy.hpp"

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"
#include <ScalarField.h>
#include <DistanceComputationTools.h>
#include <GenericProgressCallback.h>
#include <QString>
#include <vector>

#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

char const* greet()
{
   return "hello, world";
}

void initCC_py()
{
    PyObject *cc_module;
    cc_module = PyImport_ImportModule("cloudComPy");
    const char* modulePath = PyUnicode_AsUTF8(PyModule_GetFilenameObject(cc_module));
    CCTRACE("modulePath: " <<  modulePath);
    initCC::init(modulePath);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(loadPointCloud_overloads, loadPointCloud, 1, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(loadPolyline_overloads, loadPolyline, 1, 6);

BOOST_PYTHON_MODULE(cloudComPy)
{
    using namespace boost::python;

    bnp::initialize();
    initializeConverters();

    export_ScalarField();
    export_ccPolyline();
    export_ccPointCloud();

    def("greet", greet);

    enum_<CC_SHIFT_MODE>("CC_SHIFT_MODE")
        .value("AUTO", AUTO)
        .value("XYZ", XYZ)
        ;

    enum_<CC_FILE_ERROR>("CC_FILE_ERROR")
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
        ;

    enum_<CurvatureType>("CurvatureType")
        .value("GAUSSIAN_CURV", GAUSSIAN_CURV)
        .value("MEAN_CURV", MEAN_CURV)
        .value("NORMAL_CHANGE_RATE", NORMAL_CHANGE_RATE)
        ;

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

    def("loadPointCloud", loadPointCloud,
        loadPointCloud_overloads(args("mode", "skip", "x", "y", "z", "filename"),
                                 loadPointCloud_doc)[return_value_policy<reference_existing_object>()]);

    def("loadPolyline", loadPolyline,
        loadPolyline_overloads(args("mode", "skip", "x", "y", "z", "filename"))
        [return_value_policy<reference_existing_object>()]);

    def("SavePointCloud", SavePointCloud);

    def("initCC", &initCC_py);
}
