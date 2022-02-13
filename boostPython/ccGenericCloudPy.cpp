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

#include "ccGenericCloudPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccBBox.h>
#include <ReferenceCloud.h>
#include <ccGenericPointCloud.h>
#include <PointCloudTpl.h>
#include <GenericProgressCallback.h>
#include <CCGeom.h>

#include "pyccTrace.h"
#include "ccGenericCloudPy_DocStrings.hpp"

namespace bp = boost::python;

using namespace boost::python;

CCVector3 PointCloudTpl_ccGenericPointCloud_QString_getPoint_py(CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>& self, unsigned index)
{
    const CCVector3* vec = self.getPoint(index);
    return *vec;
}

bool addPointIndex1_py(CCCoreLib::ReferenceCloud& self, unsigned globalIndex)
{
	return self.addPointIndex(globalIndex);
}

bool addPointIndex2_py(CCCoreLib::ReferenceCloud& self, unsigned firstIndex, unsigned lastIndex)
{
	return self.addPointIndex(firstIndex, lastIndex);
}

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

CCVector3 ccBBox_minCorner(ccBBox& self)
{
	const CCVector3& vec = self.minCorner();
	return vec;
}

CCVector3 ccBBox_maxCorner(ccBBox& self)
{
	const CCVector3& vec = self.maxCorner();
	return vec;
}

ccBBox ccGenericPointCloud_getOwnBB(ccGenericPointCloud& self)
{
	return self.getOwnBB(false);
}

bool addChild_py(ccHObject& self, ccHObject* child, int dependencyFlags = ccHObject::DEPENDENCY_FLAGS::DP_NONE, int insertIndex = -1)
{
	return self.addChild(child, dependencyFlags, insertIndex);
}

CCCoreLib::GenericIndexedCloudPersist* (CCCoreLib::ReferenceCloud::*getAssCloud1)() = &CCCoreLib::ReferenceCloud::getAssociatedCloud;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ccGenericPointCloud_computeOctree_overloads, computeOctree, 0, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(addChild_py_overloads, addChild_py, 2, 4)

void export_ccGenericCloud()
{
	enum_<ccHObject::DEPENDENCY_FLAGS>("DEPENDENCY_FLAGS")
		.value("DP_NONE", ccHObject::DEPENDENCY_FLAGS::DP_NONE)
		.value("DP_NOTIFY_OTHER_ON_DELETE", ccHObject::DEPENDENCY_FLAGS::DP_NOTIFY_OTHER_ON_DELETE)
		.value("DP_NOTIFY_OTHER_ON_UPDATE", ccHObject::DEPENDENCY_FLAGS::DP_NOTIFY_OTHER_ON_UPDATE)
		.value("DP_DELETE_OTHER", ccHObject::DEPENDENCY_FLAGS::DP_DELETE_OTHER)
		.value("DP_PARENT_OF_OTHER", ccHObject::DEPENDENCY_FLAGS::DP_PARENT_OF_OTHER)
		;

    class_<ccHObject>("ccHObject", no_init)
        .def("setName", &ccHObject::setName, ccHObject_setName_doc)
        .def("getName", &ccHObject::getName, ccHObject_getName_doc)
		.def("addChild", &addChild_py, addChild_py_overloads(
		     (arg("self"), arg("child"), arg("dependencyFlags")= ccHObject::DEPENDENCY_FLAGS::DP_NONE, arg("insertIndex") = -1),
		     ccHObject_addChild_doc))
        ;

    class_<ccShiftedObject, bases<ccHObject>, boost::noncopyable>("ccShiftedObject", no_init)
        ;

    class_<CCCoreLib::GenericIndexedCloudPersist, boost::noncopyable>("GenericIndexedCloudPersist", no_init)
        ;

    class_<ccBBox>("ccBBox", ccBBox_doc)
    		.def(init<const CCVector3&, const CCVector3&>())
			.def("minCorner", &ccBBox_minCorner, ccBBox_minCorner_doc)
			.def("maxCorner", &ccBBox_maxCorner, ccBBox_maxCorner_doc)
		;

    class_<ccGenericPointCloud, bases<CCCoreLib::GenericIndexedCloudPersist, ccShiftedObject>, boost::noncopyable>("ccGenericPointCloud", no_init)
        .def("computeOctree", &ccGenericPointCloud::computeOctree,
             ccGenericPointCloud_computeOctree_overloads(
             (arg("progressCb")=0, arg("autoAddChild")=true),
             ccGenericPointCloud_computeOctree_doc ))
        .def("getOctree", &ccGenericPointCloud::getOctree, ccGenericPointCloud_getOctree_doc)
        .def("deleteOctree", &ccGenericPointCloud::deleteOctree, ccGenericPointCloud_deleteOctree_doc)
		.def("getOwnBB", &ccGenericPointCloud_getOwnBB, ccGenericPointCloud_getOwnBB_doc)
        ;

    class_<CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>, bases<ccGenericPointCloud>, boost::noncopyable>("PointCloudTpl_ccGenericPointCloud_QString", no_init)
        .def("getPoint", &PointCloudTpl_ccGenericPointCloud_QString_getPoint_py, PointCloudTpl_ccGenericPointCloud_QString_getPoint_doc)
        .def("reserve", &CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>::reserve, PointCloudTpl_reserve_doc)
        .def("resize", &CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>::resize, PointCloudTpl_resize_doc)
        .def("addPoint", &CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>::addPoint, PointCloudTpl_addPoint_doc)
		;

    class_<CCCoreLib::ReferenceCloud, bases<CCCoreLib::GenericIndexedCloudPersist> >("ReferenceCloud", ReferenceCloud_Doc,
                                                                                     init<CCCoreLib::GenericIndexedCloudPersist*>())
        .def("addPointIndexGlobal", addPointIndex1_py, ReferenceCloud_addPointIndexGlobal_doc)
        .def("addPointIndex", addPointIndex2_py, ReferenceCloud_addPointIndex_doc)
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

    // TODO: some methods may lead to abort: for instance getPoint with bad index
}

