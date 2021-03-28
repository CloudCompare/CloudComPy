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

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "converters.hpp"

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"
#include <QString>
#include <vector>

#include "pyccTrace.h"

char const* greet()
{
   return "hello, world";
}

struct ccPointCloudWrap : ccPointCloud, boost::python::wrapper<ccPointCloud>
{
//    std::vector<double> res;
//    const std::vector<double>& computeGravityCenter()
//    {
//        CCVector3  g = ccPointCloud::computeGravityCenter();
//        //std::vector<double> res;
//        res.resize(3);
//        res[0] = g.x; res[1] = g.y; res[2] = g.z;
//        return res;
//    }

//    virtual bool exportCoordToSF_w(bool x, bool y, bool z)
//    {
//        bool b[3];
//        b[0] =x; b[1] = y; b[2] = z;
//        return ccPointCloud::exportCoordToSF(b);
//    }
};

boost::python::tuple computeGravityCenter_py(ccPointCloud& self)
{
    CCVector3 g = self.computeGravityCenter();
    std::vector<double> res;
    res.resize(3);
    res[0] = g.x;
    res[1] = g.y;
    res[2] = g.z;
    return boost::python::make_tuple(res[0], res[1], res[2]);
}

bool exportCoordToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportCoordToSF(b);
}

void scale_py(ccPointCloud &self, double fx, double fy, double fz, boost::python::tuple center)
{
    if (boost::python::len(center) != 3)
    {
        PyErr_SetString(PyExc_TypeError, "tuple must contain 3 coordinates");
        throw boost::python::error_already_set();
    }
    double x = boost::python::extract<double>(center[0]);
    double y = boost::python::extract<double>(center[1]);
    double z = boost::python::extract<double>(center[2]);
    self.scale(fx, fy, fz, CCVector3(x, y, z));
}

void translate_py(ccPointCloud &self, boost::python::tuple vec)
{
    if (boost::python::len(vec) != 3)
    {
        PyErr_SetString(PyExc_TypeError, "tuple must contain 3 coordinates");
        throw boost::python::error_already_set();
    }
    double x = boost::python::extract<double>(vec[0]);
    double y = boost::python::extract<double>(vec[1]);
    double z = boost::python::extract<double>(vec[2]);
    self.translate(CCVector3(x, y, z));
}

BOOST_PYTHON_FUNCTION_OVERLOADS(loadPointCloud_overloads, loadPointCloud, 1, 6);

BOOST_PYTHON_MODULE(cloudComPy)
{
    using namespace boost::python;

    initializeConverters();

    class_<std::vector<double> >("DoubleVec")
        .def(boost::python::vector_indexing_suite<std::vector<double> >())
        ;

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

//    class_<ccPointCloud>("ccPointCloud", no_init)
//        .def("computeGravityCenter", &ccPointCloud::computeGravityCenter)
//        .def("crop2D", &ccPointCloud::crop2D, return_value_policy<reference_existing_object>())
//        .def("exportCoordToSF", &exportCoordToSF_py) //&ccPointCloud::exportCoordToSF) //,return_internal_reference<1>())
//        .def("getCurrentInScalarField", &ccPointCloud::getCurrentInScalarField, return_value_policy<reference_existing_object>())
//        .def("getCurrentOutScalarField", &ccPointCloud::getCurrentOutScalarField, return_value_policy<reference_existing_object>())
//        .def("getName", &ccPointCloud::getName)
//        .def("getNumberOfScalarFields", &ccPointCloud::getNumberOfScalarFields)
//        .def("getScalarField", &ccPointCloud::getScalarField, return_value_policy<reference_existing_object>())
//        .def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
//        .def("hasScalarFields", &ccPointCloud::hasScalarFields)
//        .def("renameScalarField", &ccPointCloud::renameScalarField)
//        .def("reserve", &ccPointCloud::reserve)
//        .def("scale", &ccPointCloud::scale)
//        .def("setCurrentInScalarField", &ccPointCloud::setCurrentInScalarField)
//        .def("setCurrentOutScalarField", &ccPointCloud::setCurrentOutScalarField)
//        .def("size", &ccPointCloud::size)
//        .def("translate", &ccPointCloud::translate)
//        ;
    class_<ccPointCloudWrap,boost::noncopyable>("ccPointCloud", no_init)
        .def("computeGravityCenter", &computeGravityCenter_py) //ccPointCloudWrap::computeGravityCenter, return_value_policy<reference_existing_object>())
        .def("crop2D", &ccPointCloudWrap::crop2D, return_value_policy<reference_existing_object>())
        .def("exportCoordToSF", &exportCoordToSF_py)
        .def("getCurrentInScalarField", &ccPointCloudWrap::getCurrentInScalarField, return_value_policy<reference_existing_object>())
        .def("getCurrentOutScalarField", &ccPointCloudWrap::getCurrentOutScalarField, return_value_policy<reference_existing_object>())
        .def("getName", &ccPointCloudWrap::getName)
        .def("getNumberOfScalarFields", &ccPointCloudWrap::getNumberOfScalarFields)
        .def("getScalarField", &ccPointCloudWrap::getScalarField, return_value_policy<reference_existing_object>())
        .def("getScalarFieldName", &ccPointCloudWrap::getScalarFieldName)
        .def("hasScalarFields", &ccPointCloudWrap::hasScalarFields)
        .def("renameScalarField", &ccPointCloudWrap::renameScalarField)
        .def("reserve", &ccPointCloudWrap::reserve)
        .def("scale", &scale_py)
        .def("setCurrentInScalarField", &ccPointCloudWrap::setCurrentInScalarField)
        .def("setCurrentOutScalarField", &ccPointCloudWrap::setCurrentOutScalarField)
        .def("size", &ccPointCloudWrap::size)
        .def("translate", &translate_py)
        ;


    class_<ccPolyline>("ccPolyline", no_init)
        .def("is2DMode", &ccPolyline::is2DMode);

    const char* loadPointCloud_doc= R"(
Load a 3D cloud from a file
parameters:
- filename
- shift mode from (AUTO, XYZ),  optional, default AUTO
    AUTO: automatic shift of coordinates
    XYZ:  coordinates shift given by x, y, z parameters
- skip optional parameter not used yet! default 0
- x, y, z:: optional shift values for coordinates (mode XYZ),  default 0
return a ccPointCloud object.
Usage: see ccPointCloud doc.)";

    def("loadPointCloud", loadPointCloud,
        loadPointCloud_overloads(args("mode", "skip", "x", "y", "z", "filename"),
                                 loadPointCloud_doc)[return_value_policy<reference_existing_object>()]);

    def("loadPolyline", loadPolyline,
        return_value_policy<reference_existing_object>());

    def("SavePointCloud", SavePointCloud);


}
