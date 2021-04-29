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

//#define BOOST_LIB_NAME "boost_python"
//#include <boost/config/auto_link.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "converters.hpp"
#include "ScalarFieldPy.hpp"
#include "ccPointCloudPy.hpp"
#include "ccMeshPy.hpp"
#include "ccPrimitivesPy.hpp"
#include "ccPolylinePy.hpp"
#include "distanceComputationToolsPy.hpp"
#include "registrationToolsPy.hpp"

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"
#include <ccGLMatrix.h>
#include <ccHObject.h>
#include <ccPointCloud.h>
#include <ScalarField.h>
#include <QString>
#include <vector>

#include "pyccTrace.h"
#include "cloudComPy_DocStrings.hpp"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

char const* greet()
{
   return "hello, world, this is CloudCompare Python Interface: 'CloudComPy'";
}

void initCC_py()
{
    PyObject *cc_module;
    cc_module = PyImport_ImportModule("cloudComPy");
    const char* modulePath = PyUnicode_AsUTF8(PyModule_GetFilenameObject(cc_module));
    CCTRACE("modulePath: " <<  modulePath);
    initCC::init(modulePath);
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


BOOST_PYTHON_FUNCTION_OVERLOADS(loadPointCloud_overloads, loadPointCloud, 1, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(loadPolyline_overloads, loadPolyline, 1, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(GetPointCloudRadius_overloads, GetPointCloudRadius, 1, 2);
BOOST_PYTHON_FUNCTION_OVERLOADS(ICP_py_overloads, ICP_py, 8, 13);

BOOST_PYTHON_MODULE(cloudComPy)
{
    using namespace boost::python;

    bnp::initialize();
    initializeConverters();

    export_ScalarField();
    export_ccPolyline();
    export_ccPointCloud();
    export_ccMesh();
    export_ccPrimitives();
    export_distanceComputationTools();
    export_registrationTools();

    // TODO: function load entities ("file.bin")
    // TODO: more methods on distanceComputationTools
    // TODO: methods from ccEntityAction.h to transpose without dialogs
    // TODO: compute octree
    // TODO: mesh Delaunay
    // TODO: explore menus edit, tools, plugins

    scope().attr("__doc__") = cloudComPy_doc;

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

    def("loadPointCloud", loadPointCloud,
        loadPointCloud_overloads(args("mode", "skip", "x", "y", "z", "filename"),
                                 cloudComPy_loadPointCloud_doc)[return_value_policy<reference_existing_object>()]);

    def("loadPolyline", loadPolyline,
        loadPolyline_overloads(args("mode", "skip", "x", "y", "z", "filename"),
                               cloudComPy_loadPolyline_doc)
        [return_value_policy<reference_existing_object>()]);

    def("SavePointCloud", SavePointCloud, cloudComPy_SavePointCloud_doc);

    def("SaveEntities", SaveEntities, cloudComPy_SaveEntities_doc);

    def("initCC", &initCC_py, cloudComPy_initCC_doc);

    def("computeCurvature", computeCurvature, cloudComPy_computeCurvature_doc);

    def("filterBySFValue", filterBySFValue, return_value_policy<reference_existing_object>(), cloudComPy_filterBySFValue_doc);

    def("GetPointCloudRadius", GetPointCloudRadius, GetPointCloudRadius_overloads(args("knn", "clouds"), cloudComPy_GetPointCloudRadius_doc));

    def("getScalarType", getScalarType, cloudComPy_getScalarType_doc);

    class_<ICPres>("ICPres", cloudComPy_ICPres_doc)
       .add_property("aligned", bp::make_getter(&ICPres::aligned, return_value_policy<return_by_value>()))
       .def_readwrite("transMat", &ICPres::transMat,
                       cloudComPy_ICPres_doc)
       .def_readwrite("finalScale", &ICPres::finalScale,
                      cloudComPy_ICPres_doc)
       .def_readwrite("finalRMS", &ICPres::finalRMS,
                      cloudComPy_ICPres_doc)
       .def_readwrite("finalPointCount", &ICPres::finalPointCount,
                      cloudComPy_ICPres_doc)
    ;

    def("ICP", ICP_py, ICP_py_overloads(cloudComPy_ICP_doc));
}
