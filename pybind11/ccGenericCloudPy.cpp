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

#include "cloudComPy.hpp"

#include <ccBBox.h>
#include <ReferenceCloud.h>
#include <ccGenericPointCloud.h>
#include <PointCloudTpl.h>
#include <GenericProgressCallback.h>
#include <CCGeom.h>
#include <ccHObject.h>
#include <ccObject.h>

#include "pyccTrace.h"
#include "ccGenericCloudPy_DocStrings.hpp"

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

void showColorsPy(ccHObject& self, bool isShown)
{
    if (isShown)
    {
        self.showSF(false);
        if (self.hasColors())
            self.showColors(true);
    }
    else self.showColors(false);
}

void showSFPy(ccHObject& self, bool isShown)
{
    if (isShown)
    {
        self.showColors(false);
        if (self.hasScalarFields())
            self.showSF(true);
    }
    else self.showSF(false);
}

void showNormalsPy(ccHObject& self, bool isShown)
{
    if (isShown && self.hasNormals())
        self.showNormals(true);
    else self.showNormals(false);
}

const CCCoreLib::DgmOctree* getOctreePy(ccGenericPointCloud& self )
{
    QSharedPointer<ccOctree> shared = self.getOctree();
    //ccOctree* ptr= shared.data();
    const CCCoreLib::DgmOctree* ptr = dynamic_cast<CCCoreLib::DgmOctree*>(shared.data());
    CCTRACE("getOctreePy: " << ptr);
    return ptr;
}

const CCCoreLib::DgmOctree* computeOctreePy(ccGenericPointCloud& self, CCCoreLib::GenericProgressCallback* progressCb=nullptr, bool autoAddChild=true )
{
    QSharedPointer<ccOctree> shared = self.computeOctree(progressCb, autoAddChild);
//    std::unique_ptr<ccOctree, py::nodelete> ptr = std::unique_ptr<ccOctree, py::nodelete>(shared.data());
//    CCTRACE("computeOctreePy: " << ptr.get());
    const CCCoreLib::DgmOctree* ptr = dynamic_cast<CCCoreLib::DgmOctree*>(shared.data());
    CCVector3 bbmin, bbmax;
    ptr->getBoundingBox(bbmin, bbmax);
    CCTRACE("bbox: " << bbmin.x<<" "<< bbmin.y<<" "<< bbmin.z<<" - "<< bbmax.x<<" "<< bbmax.y<<" "<< bbmax.z);
    return ptr;
}


void export_ccGenericCloud(py::module &m0)
{
    py::enum_<CC_TYPES::CCTYPES>(m0, "CC_TYPES")
       .value("OBJECT", CC_TYPES::OBJECT)
       .value("HIERARCHY_OBJECT", CC_TYPES::HIERARCHY_OBJECT)
       .value("POINT_CLOUD", CC_TYPES::POINT_CLOUD)
       .value("MESH", CC_TYPES::MESH)
       .value("SUB_MESH", CC_TYPES::SUB_MESH)
       .value("MESH_GROUP", CC_TYPES::MESH_GROUP)
       .value("FACET", CC_TYPES::FACET)
       .value("POINT_OCTREE", CC_TYPES::POINT_OCTREE)
       .value("POINT_KDTREE", CC_TYPES::POINT_KDTREE)
       .value("POLY_LINE", CC_TYPES::POLY_LINE)
       .value("IMAGE", CC_TYPES::IMAGE)
       .value("CALIBRATED_IMAGE", CC_TYPES::CALIBRATED_IMAGE)
       .value("SENSOR", CC_TYPES::SENSOR)
       .value("GBL_SENSOR", CC_TYPES::GBL_SENSOR)
       .value("CAMERA_SENSOR", CC_TYPES::CAMERA_SENSOR)
       .value("PRIMITIVE", CC_TYPES::PRIMITIVE)
       .value("PLANE", CC_TYPES::PLANE)
       .value("SPHERE", CC_TYPES::SPHERE)
       .value("TORUS", CC_TYPES::TORUS)
       .value("CONE", CC_TYPES::CONE)
       .value("OLD_CYLINDER_ID", CC_TYPES::OLD_CYLINDER_ID)
       .value("CYLINDER", CC_TYPES::CYLINDER)
       .value("BOX", CC_TYPES::BOX)
       .value("DISH", CC_TYPES::DISH)
       .value("EXTRU", CC_TYPES::EXTRU)
       .value("QUADRIC", CC_TYPES::QUADRIC)
       .value("MATERIAL_SET", CC_TYPES::MATERIAL_SET)
       .value("ARRAY", CC_TYPES::ARRAY)
       .value("NORMALS_ARRAY", CC_TYPES::NORMALS_ARRAY)
       .value("NORMAL_INDEXES_ARRAY", CC_TYPES::NORMAL_INDEXES_ARRAY)
       .value("RGB_COLOR_ARRAY", CC_TYPES::RGB_COLOR_ARRAY)
       .value("RGBA_COLOR_ARRAY", CC_TYPES::RGBA_COLOR_ARRAY)
       .value("TEX_COORDS_ARRAY", CC_TYPES::TEX_COORDS_ARRAY)
       .value("LABEL_2D", CC_TYPES::LABEL_2D)
       .value("VIEWPORT_2D_OBJECT", CC_TYPES::VIEWPORT_2D_OBJECT)
       .value("VIEWPORT_2D_LABEL", CC_TYPES::VIEWPORT_2D_LABEL)
       .value("CLIPPING_BOX", CC_TYPES::CLIPPING_BOX)
       .value("TRANS_BUFFER", CC_TYPES::TRANS_BUFFER)
       .value("COORDINATESYSTEM", CC_TYPES::COORDINATESYSTEM)
       .value("CUSTOM_H_OBJECT", CC_TYPES::CUSTOM_H_OBJECT)
       .value("CUSTOM_LEAF_OBJECT", CC_TYPES::CUSTOM_LEAF_OBJECT)
       .export_values();

    py::enum_<ccHObject::DEPENDENCY_FLAGS>(m0, "DEPENDENCY_FLAGS")
		.value("DP_NONE", ccHObject::DEPENDENCY_FLAGS::DP_NONE)
		.value("DP_NOTIFY_OTHER_ON_DELETE", ccHObject::DEPENDENCY_FLAGS::DP_NOTIFY_OTHER_ON_DELETE)
		.value("DP_NOTIFY_OTHER_ON_UPDATE", ccHObject::DEPENDENCY_FLAGS::DP_NOTIFY_OTHER_ON_UPDATE)
		.value("DP_DELETE_OTHER", ccHObject::DEPENDENCY_FLAGS::DP_DELETE_OTHER)
		.value("DP_PARENT_OF_OTHER", ccHObject::DEPENDENCY_FLAGS::DP_PARENT_OF_OTHER)
		.export_values();

    py::class_<ccHObject>(m0, "ccHObject")
        .def("setName", &ccHObject::setName, ccHObject_setName_doc)
        .def("getName", &ccHObject::getName, ccHObject_getName_doc)
		.def("addChild", &addChild_py,
		     py::arg("child"), py::arg("dependencyFlags")= ccHObject::DEPENDENCY_FLAGS::DP_NONE, py::arg("insertIndex") = -1,
		     ccHObject_addChild_doc)
        .def("getChild", &ccHObject::getChild, py::return_value_policy::reference, ccHObject_getChild_doc)
		.def("getChildrenNumber", &ccHObject::getChildrenNumber, ccHObject_getChildrenNumber_doc)
		.def("getChildCountRecursive", &ccHObject::getChildCountRecursive, ccHObject_getChildCountRecursive_doc)
        .def("getClassID", &ccHObject::getClassID, ccHObject_getClassID_doc)
        .def("getParent", &ccHObject::getParent, py::return_value_policy::reference, ccHObject_getParent_doc)
        .def("hasColors", &ccHObject::hasColors, ccHObject_hasColors_doc)
        .def("hasNormals", &ccHObject::hasNormals, ccHObject_hasNormals_doc)
        .def("colorsShown", &ccHObject::colorsShown, ccHObject_colorsShown_doc)
        .def("normalsShown", &ccHObject::normalsShown, ccHObject_normalsShown_doc)
        .def("sfShown", &ccHObject::sfShown, ccHObject_sfShown_doc)
        .def("hasScalarFields", &ccHObject::hasScalarFields, ccHObject_hasScalarFields_doc)
		.def("isA", &ccHObject::isA, ccHObject_isA_doc)
        .def("isKindOf", &ccHObject::isKindOf, ccHObject_isKindOf_doc)
        .def("showColors", & showColorsPy, ccHObject_showColors_doc)
        .def("showNormals", & showNormalsPy, ccHObject_showNormals_doc)
        .def("showSF", & showSFPy, ccHObject_showSF_doc)
        .def("incRef",   [](pybind11::handle handle) { handle.inc_ref();}, ccHObject_incRef_doc)
        .def("decRef",   [](pybind11::handle handle) { handle.dec_ref();}, ccHObject_decRef_doc)
        .def("countRef", [](pybind11::object obj)    { return obj.ref_count();}, ccHObject_countRef_doc)
        ;

    py::class_<ccShiftedObject, ccHObject>(m0, "ccShiftedObject")
        .def("getGlobalShift", &ccShiftedObject::getGlobalShift, ccShiftedObject_getGlobalShift_doc)
        .def("setGlobalShift", static_cast<void (ccShiftedObject::*)(double,double,double)>(&ccShiftedObject::setGlobalShift),
             ccShiftedObject_setGlobalShift_doc)
        .def("isShifted", &ccShiftedObject::isShifted, ccShiftedObject_isShifted_doc)
        .def("getGlobalScale", &ccShiftedObject::getGlobalScale, ccShiftedObject_getGlobalScale_doc)
        .def("setGlobalScale", &ccShiftedObject::setGlobalScale, ccShiftedObject_setGlobalScale_doc)
        ;

    py::class_<CCCoreLib::GenericIndexedCloudPersist>(m0, "GenericIndexedCloudPersist")
        ;

    py::class_<ccBBox>(m0, "ccBBox", ccBBox_doc)
    		.def(py::init<const CCVector3&, const CCVector3&, bool>())
			.def("minCorner", &ccBBox_minCorner, ccBBox_minCorner_doc)
			.def("maxCorner", &ccBBox_maxCorner, ccBBox_maxCorner_doc)
		;

    py::class_<ccGenericPointCloud, CCCoreLib::GenericIndexedCloudPersist, ccShiftedObject>(m0, "ccGenericPointCloud")
        .def("computeOctree", &computeOctreePy,
             py::arg("progressCb")=nullptr, py::arg("autoAddChild")=true,
             ccGenericPointCloud_computeOctree_doc, py::return_value_policy::reference )
        .def("getOctree", &getOctreePy, ccGenericPointCloud_getOctree_doc, py::return_value_policy::reference)
        .def("deleteOctree", &ccGenericPointCloud::deleteOctree, ccGenericPointCloud_deleteOctree_doc)
		.def("getOwnBB", &ccGenericPointCloud_getOwnBB, ccGenericPointCloud_getOwnBB_doc)
        ;

    py::class_<CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>, ccGenericPointCloud>(m0, "PointCloudTpl_ccGenericPointCloud_QString")
        .def("getPoint", &PointCloudTpl_ccGenericPointCloud_QString_getPoint_py, PointCloudTpl_ccGenericPointCloud_QString_getPoint_doc)
        .def("reserve", &CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>::reserve, PointCloudTpl_reserve_doc)
        .def("resize", &CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>::resize, PointCloudTpl_resize_doc)
        .def("addPoint", &CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>::addPoint, PointCloudTpl_addPoint_doc)
		;

    py::class_<CCCoreLib::ReferenceCloud, CCCoreLib::GenericIndexedCloudPersist,
               std::unique_ptr<CCCoreLib::ReferenceCloud, py::nodelete>>(m0, "ReferenceCloud", ReferenceCloud_Doc)
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist*>())
        .def("addPointIndexGlobal", addPointIndex1_py, ReferenceCloud_addPointIndexGlobal_doc)
        .def("addPointIndex", addPointIndex2_py, ReferenceCloud_addPointIndex_doc)
        .def("enableScalarField", &CCCoreLib::ReferenceCloud::enableScalarField, ReferenceCloud_enableScalarField_doc)
        .def("forwardIterator", &CCCoreLib::ReferenceCloud::forwardIterator, ReferenceCloud_forwardIterator_doc)
        .def("getAssociatedCloud", getAssCloud1, py::return_value_policy::reference, ReferenceCloud_getAssociatedCloud_doc)
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

