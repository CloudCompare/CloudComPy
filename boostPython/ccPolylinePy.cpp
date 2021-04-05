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

#include "ccPolylinePy.hpp"

//#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccPolyline.h>

//#include "PyScalarType.h"
//#include "pyccTrace.h"
//
//namespace bp = boost::python;
//namespace bnp = boost::python::numpy;

using namespace boost::python;

void export_ccPolyline()
{
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
}
