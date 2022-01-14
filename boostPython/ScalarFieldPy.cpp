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

#include "ScalarFieldPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccScalarField.h>
#include <ScalarField.h>

#include "PyScalarType.h"
#include "pyccTrace.h"
#include "ScalarFieldPy_DocStrings.hpp"

#include <vector>

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
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result.copy();
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

bp::tuple computeMeanAndVariance_py(CCCoreLib::ScalarField &self)
{
    ScalarType mean, variance;
    self.computeMeanAndVariance(mean, &variance);
    bp::tuple res = bp::make_tuple(mean, variance);
    return res;
}

ScalarType& (CCCoreLib::ScalarField::* getValue1)(std::size_t) = &CCCoreLib::ScalarField::getValue; // getValue1: pointer to member function
const ScalarType& (CCCoreLib::ScalarField::* getValue2)(std::size_t) const = &CCCoreLib::ScalarField::getValue; //pointer to member function with const qualifier
//typedef const ScalarType& (CCCoreLib::ScalarField::*gvftype)(std::size_t) const; // the same using a typedef
//gvftype getValue2 = &CCCoreLib::ScalarField::getValue;

void export_ScalarField()
{
    class_<CCCoreLib::ScalarField, boost::noncopyable>("ScalarField", ScalarFieldPy_ScalarField_doc, no_init) // boost::noncopyable required to avoid issue with protected destructor
        .def("addElement", &CCCoreLib::ScalarField::addElement, ScalarFieldPy_addElement_doc)
        .def("computeMeanAndVariance", &computeMeanAndVariance_py, ScalarFieldPy_computeMeanAndVariance_doc)
        .def("computeMinAndMax", &CCCoreLib::ScalarField::computeMinAndMax, ScalarFieldPy_computeMinAndMax_doc)
        .def("currentSize", &CCCoreLib::ScalarField::currentSize, ScalarFieldPy_currentSize_doc)
        .def("fill", &CCCoreLib::ScalarField::fill, ScalarFieldPy_fill_doc)
        .def("flagValueAsInvalid", &CCCoreLib::ScalarField::flagValueAsInvalid, ScalarFieldPy_flagValueAsInvalid_doc)
        .def("fromNpArrayCopy", &fromNPArray_copy, ScalarFieldPy_fromNpArrayCopy_doc)
        .def("getMax", &CCCoreLib::ScalarField::getMax, ScalarFieldPy_getMax_doc)
        .def("getMin", &CCCoreLib::ScalarField::getMin, ScalarFieldPy_getMin_doc)
        .def("getName", &CCCoreLib::ScalarField::getName, ScalarFieldPy_getName_doc)
        .def("getValue", getValue1, ScalarFieldPy_getValue_doc, return_value_policy<copy_non_const_reference>())
        .def("getValue", getValue2, ScalarFieldPy_getValue_doc, return_value_policy<copy_const_reference>())
        .def("reserveSafe", &CCCoreLib::ScalarField::reserveSafe, ScalarFieldPy_reserveSafe_doc)
        .def("resizeSafe", &CCCoreLib::ScalarField::resizeSafe, ScalarFieldPy_resizeSafe_doc)
        .def("setName", &CCCoreLib::ScalarField::setName, ScalarFieldPy_setName_doc)
        .def("setValue", &CCCoreLib::ScalarField::setValue, ScalarFieldPy_setValue_doc)
        .def("swap", &CCCoreLib::ScalarField::swap, ScalarFieldPy_swap_doc)
        .def("toNpArray", &ToNpArray_py, ScalarFieldPy_toNpArray_doc)
        .def("toNpArrayCopy", &ToNpArray_copy, ScalarFieldPy_toNpArrayCopy_doc)
        ;
    //TODO optional parameters on resizeSafe

    class_<ccScalarField, bases<CCCoreLib::ScalarField>, boost::noncopyable >("ccScalarField", ccScalarFieldPy_ccScalarField_doc, no_init)
        .def("isSerializable", &ccScalarField::isSerializable)
        ;
    //TODO add serialization functions
}

