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

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"
#include <ScalarField.h>
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

bool exportCoordToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportCoordToSF(b);
}

bnp::ndarray CoordsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray with copy, ownership transfered to Python");
    bnp::dtype dt = bnp::dtype::get_builtin<float>(); // coordinates always in simple precision
    size_t nRows = self.size();
    bp::tuple shape = bp::make_tuple(nRows, 3);
    bp::tuple stride = bp::make_tuple(3*sizeof(float), sizeof(float));
    float *s = (float*)self.getPoint(0);
    CCTRACE("--- copy " << 3*nRows*sizeof(float));
    float *d = new float[3*nRows];
    memcpy(d, s, 3*nRows*sizeof(float));
    CCTRACE("--- copied");
    bnp::ndarray result = bnp::from_data(d, dt, shape, stride, bp::object());
    return result;
}

bnp::ndarray CoordsToNpArray_py(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray without copy, ownership stays in C++");
    bnp::dtype dt = bnp::dtype::get_builtin<float>(); // coordinates always in simple precision
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    bp::tuple shape = bp::make_tuple(nRows, 3);
    bp::tuple stride = bp::make_tuple(3*sizeof(float), sizeof(float));
    float *s = (float*)self.getPoint(0);
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result;
}

bnp::ndarray ToNpArray_copy(CCCoreLib::ScalarField &self)
{
    CCTRACE("ScalarField ToNpArray with copy, ownership transfered to Python");
    bnp::dtype dt = bnp::dtype::get_builtin<PyScalarType>();
    size_t nRows = self.size();
    bp::tuple shape = bp::make_tuple(nRows);
    bp::tuple stride = bp::make_tuple(sizeof(PyScalarType));
    PyScalarType *s = (PyScalarType*)self.data();
    CCTRACE("--- copy " << nRows*sizeof(PyScalarType));
    PyScalarType *d = new PyScalarType[nRows];
    memcpy(d, s, nRows*sizeof(PyScalarType));
    CCTRACE("--- copied");
    bnp::ndarray result = bnp::from_data(d, dt, shape, stride, bp::object());
    return result;
}

bnp::ndarray ToNpArray_py(CCCoreLib::ScalarField &self)
{
    CCTRACE("ScalarField ToNpArray without copy, ownership stays in C++");
    bnp::dtype dt = bnp::dtype::get_builtin<PyScalarType>();
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    bp::tuple shape = bp::make_tuple(nRows);
    bp::tuple stride = bp::make_tuple( sizeof(PyScalarType));
    PyScalarType *s = (PyScalarType*)self.data();
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result;
}

void fromNPArray_copy(CCCoreLib::ScalarField &self, bnp::ndarray const & array)
{
    if (array.get_dtype() != bnp::dtype::get_builtin<PyScalarType>())
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array data type");
        bp::throw_error_already_set();
    }
    size_t nRows = self.size();
    if (array.get_nd() != 1 && array.shape(0) != nRows)
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array dimension or size");
        bp::throw_error_already_set();
    }
    PyScalarType *s = reinterpret_cast<PyScalarType*>(array.get_data());
    PyScalarType *d = self.data();
    memcpy(d, s, nRows*sizeof(PyScalarType));
    CCTRACE("copied " << nRows*sizeof(PyScalarType));
    self.computeMinAndMax();
}

ScalarType& (CCCoreLib::ScalarField::* getValue1)(std::size_t) = &CCCoreLib::ScalarField::getValue; // getValue1: pointer to member function
const ScalarType& (CCCoreLib::ScalarField::* getValue2)(std::size_t) const = &CCCoreLib::ScalarField::getValue; //pointer to member function with const qualifier
//typedef const ScalarType& (CCCoreLib::ScalarField::*gvftype)(std::size_t) const; // the same using a typedef
//gvftype getValue2 = &CCCoreLib::ScalarField::getValue;

BOOST_PYTHON_FUNCTION_OVERLOADS(loadPointCloud_overloads, loadPointCloud, 1, 6);
BOOST_PYTHON_FUNCTION_OVERLOADS(loadPolyline_overloads, loadPolyline, 1, 6);

BOOST_PYTHON_MODULE(cloudComPy)
{
    using namespace boost::python;

    bnp::initialize();
    initializeConverters();

    class_<std::vector<double> >("DoubleVec")
        .def(bp::vector_indexing_suite<std::vector<double> >())
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

    class_<ccPointCloud>("ccPointCloud", no_init)
        .def("computeGravityCenter", &ccPointCloud::computeGravityCenter)
        .def("crop2D", &ccPointCloud::crop2D, return_value_policy<reference_existing_object>())
        .def("exportCoordToSF", &exportCoordToSF_py)
        .def("getCurrentInScalarField", &ccPointCloud::getCurrentInScalarField, return_value_policy<reference_existing_object>())
        .def("getCurrentOutScalarField", &ccPointCloud::getCurrentOutScalarField, return_value_policy<reference_existing_object>())
        .def("getName", &ccPointCloud::getName)
        .def("getNumberOfScalarFields", &ccPointCloud::getNumberOfScalarFields)
        .def("getScalarField", &ccPointCloud::getScalarField, return_value_policy<reference_existing_object>())
        .def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
        .def("hasScalarFields", &ccPointCloud::hasScalarFields)
        .def("renameScalarField", &ccPointCloud::renameScalarField)
        .def("reserve", &ccPointCloud::reserve)
        .def("scale", &ccPointCloud::scale)
        .def("setCurrentInScalarField", &ccPointCloud::setCurrentInScalarField)
        .def("setCurrentOutScalarField", &ccPointCloud::setCurrentOutScalarField)
        .def("size", &ccPointCloud::size)
        .def("toNpArray", &CoordsToNpArray_py)
        .def("toNpArrayCopy", &CoordsToNpArray_copy)
        .def("translate", &ccPointCloud::translate)
       ;

    class_<ccPolyline>("ccPolyline", no_init)
        .def("computeLength", &ccPolyline::computeLength)
        .def("getName", &ccPolyline::getName)
        .def("is2DMode", &ccPolyline::is2DMode)
        .def("is2DMode", &ccPolyline::is2DMode)
        .def("isClosed", &ccPolyline::isClosed)
        .def("segmentCount", &ccPolyline::segmentCount)
        .def("set2DMode", &ccPolyline::set2DMode)
        .def("setClosed", &ccPolyline::setClosed)
        .def("setName", &ccPolyline::setName)
        .def("size", &ccPolyline::size)
        .def("smoothChaikin", &ccPolyline::smoothChaikin, return_value_policy<reference_existing_object>())
        ;

    class_<CCCoreLib::ScalarField, boost::noncopyable>("ScalarField", no_init) // boost::noncopyable required to avoid issue with protected destructor
        .def("addElement", &CCCoreLib::ScalarField::addElement)
        .def("computeMeanAndVariance", &CCCoreLib::ScalarField::computeMeanAndVariance)
        .def("computeMinAndMax", &CCCoreLib::ScalarField::computeMinAndMax)
        .def("currentSize", &CCCoreLib::ScalarField::currentSize)
        .def("fill", &CCCoreLib::ScalarField::fill)
        .def("flagValueAsInvalid", &CCCoreLib::ScalarField::flagValueAsInvalid)
        .def("flagValueAsInvalid", &CCCoreLib::ScalarField::flagValueAsInvalid)
        .def("fromNPArrayCopy", &fromNPArray_copy)
        .def("getMax", &CCCoreLib::ScalarField::getMax)
        .def("getMin", &CCCoreLib::ScalarField::getMin)
        .def("getName", &CCCoreLib::ScalarField::getName)
        .def("getValue", getValue1, return_value_policy<copy_non_const_reference>())
        .def("getValue", getValue2, return_value_policy<copy_const_reference>())
        .def("reserveSafe", &CCCoreLib::ScalarField::reserveSafe)
        .def("resizeSafe", &CCCoreLib::ScalarField::resizeSafe)
        .def("setName", &CCCoreLib::ScalarField::setName)
        .def("setValue", &CCCoreLib::ScalarField::setValue)
        .def("swap", &CCCoreLib::ScalarField::swap)
        .def("toNpArray", &ToNpArray_py)
        .def("toNpArrayCopy", &ToNpArray_copy)
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
