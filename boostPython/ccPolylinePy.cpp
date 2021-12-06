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
#include <Polyline.h>
#include <ccShiftedObject.h>
#include <GenericIndexedCloudPersist.h>
#include "ccPolylinePy_DocStrings.hpp"

//#include "PyScalarType.h"
//#include "pyccTrace.h"
//
//namespace bp = boost::python;
//namespace bnp = boost::python::numpy;

using namespace boost::python;

void export_ccPolyline()
{

	class_<CCCoreLib::Polyline, bases<CCCoreLib::ReferenceCloud> >("Polyline",
			ccPolylinePy_Polyline_doc, init<CCCoreLib::GenericIndexedCloudPersist*>())
         .def("isClosed", &ccPolyline::isClosed, ccPolylinePy_isClosed_doc)
	     .def("setClosed", &ccPolyline::setClosed, ccPolylinePy_setClosed_doc)
		 ;

    class_<ccPolyline, bases<CCCoreLib::Polyline, ccShiftedObject> >("ccPolyline",
    		ccPolylinePy_ccPolyline_doc, init<CCCoreLib::GenericIndexedCloudPersist*>())
        .def("computeLength", &ccPolyline::computeLength, ccPolylinePy_computeLength_doc)
        .def("getName", &ccPolyline::getName, ccPolylinePy_getName_doc)
        .def("is2DMode", &ccPolyline::is2DMode, ccPolylinePy_is2DMode_doc)
        .def("segmentCount", &ccPolyline::segmentCount, ccPolylinePy_segmentCount_doc)
        .def("set2DMode", &ccPolyline::set2DMode, ccPolylinePy_set2DMode_doc)
        .def("setName", &ccPolyline::setName, ccPolylinePy_setName_doc)
        .def("size", &ccPolyline::size, ccPolylinePy_size_doc)
        .def("smoothChaikin", &ccPolyline::smoothChaikin,
             ccPolylinePy_smoothChaikin_doc, return_value_policy<reference_existing_object>())
        ;
}
