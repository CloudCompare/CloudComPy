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

#include "ScalarFieldPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ScalarField.h>

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;


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

void export_ScalarField()
{
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
}
