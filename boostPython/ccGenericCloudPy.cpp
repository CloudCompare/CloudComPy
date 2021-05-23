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

#include "ccGenericCloudPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ReferenceCloud.h>
#include <ccGenericPointCloud.h>
#include <PointCloudTpl.h>
#include <GenericProgressCallback.h>
#include <CCGeom.h>

#include "pyccTrace.h"
#include "ccGenericCloudPy_DocStrings.hpp"

namespace bp = boost::python;

using namespace boost::python;

std::vector<Vector3Tpl<float> > ReferenceCloud_getBoundingBox_py(CCCoreLib::ReferenceCloud& self)
{
    std::vector<Vector3Tpl<float> > bb;
    Vector3Tpl<float> a, b;
    self.getBoundingBox(a, b);
    bb.push_back(a);
    bb.push_back(b);
    return bb;
}

CCVector3 ReferenceCloud_getCurrentPointCoordinates(CCCoreLib::ReferenceCloud& self)
{
    const CCVector3* vec = self.getCurrentPointCoordinates();
    return *vec;
}

CCVector3 ReferenceCloud_getNextPoint(CCCoreLib::ReferenceCloud& self)
{
    const CCVector3* vec = self.getNextPoint();
    return *vec;
}

CCVector3 ReferenceCloud_getPoint(CCCoreLib::ReferenceCloud& self, unsigned index)
{
    const CCVector3* vec = self.getPoint(index);
    return *vec;
}

CCCoreLib::GenericIndexedCloudPersist* (CCCoreLib::ReferenceCloud::*getAssCloud1)() = &CCCoreLib::ReferenceCloud::getAssociatedCloud;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ccGenericPointCloud_computeOctree_overloads, computeOctree, 0, 2)

void export_ccGenericCloud()
{
    class_<ccHObject>("ccHObject", no_init)
        .def("setName", &ccHObject::setName, ccHObject_setName_doc)
        .def("getName", &ccHObject::getName, ccHObject_getName_doc)
        ;

    class_<ccShiftedObject, bases<ccHObject>, boost::noncopyable>("ccShiftedObject", no_init)
        ;

    class_<CCCoreLib::GenericIndexedCloudPersist, boost::noncopyable>("GenericIndexedCloudPersist", no_init)
        ;

    class_<ccGenericPointCloud, bases<CCCoreLib::GenericIndexedCloudPersist, ccShiftedObject>, boost::noncopyable>("ccGenericPointCloud", no_init)
        .def("computeOctree", &ccGenericPointCloud::computeOctree, ccGenericPointCloud_computeOctree_overloads(ccGenericPointCloud_computeOctree_doc))
        .def("getOctree", &ccGenericPointCloud::getOctree, ccGenericPointCloud_getOctree_doc)
        .def("deleteOctree", &ccGenericPointCloud::deleteOctree, ccGenericPointCloud_deleteOctree_doc)
        ;

    class_<CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>, bases<ccGenericPointCloud>, boost::noncopyable>("PointCloudTpl_ccGenericPointCloud_QString", no_init)
        ;

    class_<CCCoreLib::ReferenceCloud, bases<CCCoreLib::GenericIndexedCloudPersist> >("ReferenceCloud", ReferenceCloud_Doc,
                                                                                     init<CCCoreLib::GenericIndexedCloudPersist*>())
        .def("enableScalarField", &CCCoreLib::ReferenceCloud::enableScalarField, ReferenceCloud_enableScalarField_doc)
        .def("forwardIterator", &CCCoreLib::ReferenceCloud::forwardIterator, ReferenceCloud_forwardIterator_doc)
        .def("getAssociatedCloud", getAssCloud1, return_value_policy<reference_existing_object>(), ReferenceCloud_getAssociatedCloud_doc)
        .def("getBoundingBox", &ReferenceCloud_getBoundingBox_py, ReferenceCloud_getBoundingBox_doc)
        .def("getCurrentPointCoordinates", &ReferenceCloud_getCurrentPointCoordinates,
             ReferenceCloud_getCurrentPointCoordinates_doc)
        .def("getCurrentPointGlobalIndex", &CCCoreLib::ReferenceCloud::getCurrentPointGlobalIndex,
             ReferenceCloud_getCurrentPointGlobalIndex_doc)
        .def("getCurrentPointScalarValue", &CCCoreLib::ReferenceCloud::getCurrentPointScalarValue,
             ReferenceCloud_getCurrentPointScalarValue_doc)
        .def("getNextPoint", &ReferenceCloud_getNextPoint, ReferenceCloud_getNextPoint_doc)
        .def("getPoint", &ReferenceCloud_getPoint, ReferenceCloud_getPoint_doc)
        .def("getPointGlobalIndex", &CCCoreLib::ReferenceCloud::getPointGlobalIndex, ReferenceCloud_getPointGlobalIndex_doc)
        .def("getPointScalarValue", &CCCoreLib::ReferenceCloud::getPointScalarValue, ReferenceCloud_getPointScalarValue_doc)
        .def("isScalarFieldEnabled", &CCCoreLib::ReferenceCloud::isScalarFieldEnabled, ReferenceCloud_isScalarFieldEnabled_doc)
        .def("placeIteratorAtBeginning", &CCCoreLib::ReferenceCloud::placeIteratorAtBeginning,
             ReferenceCloud_placeIteratorAtBeginning_doc)
        .def("setCurrentPointScalarValue", &CCCoreLib::ReferenceCloud::setCurrentPointScalarValue,
             ReferenceCloud_setCurrentPointScalarValue_doc)
        .def("setPointScalarValue", &CCCoreLib::ReferenceCloud::setPointScalarValue, ReferenceCloud_setPointScalarValue_doc)
        .def("size", &CCCoreLib::ReferenceCloud::size, ReferenceCloud_size_doc)
        ;

}

