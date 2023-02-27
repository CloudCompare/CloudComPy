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

#ifndef CCGENERICCLOUDPY_DOCSTRINGS_HPP_
#define CCGENERICCLOUDPY_DOCSTRINGS_HPP_

const char* ccBBox_doc= R"(
BoundingBox object.

Can be initialized with tuples of coordinates and a boolean (minCorner, maxCorner, True))";

const char* ccBBox_minCorner_doc= R"(
Get the min corner of the bounding box

:return: min corner (tuple of coordinates)
:rtype: tuple)";

const char* ccBBox_maxCorner_doc= R"(
Get the max corner of the bounding box

:return: max corner (tuple of coordinates)
:rtype: tuple)";

const char* ccHObject_setName_doc= R"(
Set the name of the entity

:param str name: entity name)";

const char* ccHObject_getName_doc= R"(
Get the name of the entity

:return: entity name
:rtype: str)";

const char* ccHObject_addChild_doc= R"(
Adds a child.

**warning** the C++ default for DEPENDENCY_FLAGS (DP_PARENT_OF_OTHER) is replaced by DP_NONE in Python:
The other values may induce a delete of the original child object in C++, not handled correctly on the Python side.

:param ccHObject child: child instance to append to the child list
:param DEPENDENCY_FLAGS,optional dependencyFlags: dependency flags, default DP_NONE
:param int,optional insertIndex: insertion index, default -1: if <0, child is simply appended to the children list.

:return: success
:rtype: bool)";

const char* ccHObject_getChild_doc=R"(
Get child by index.

:param int index: index of child, see :py:meth:`getChildrenNumber`.

:return: child object
:rtype: ccHObject

)";

const char* ccHObject_getChildCountRecursive_doc=R"(
Get the number of children of this object, recursively.

:return: number of children, recursively
:rtype: int
)";

const char* ccHObject_getChildrenNumber_doc=R"(
Get the number of direct children of this object.

:return: number of direct children
:rtype: int
)";

const char* ccHObject_getClassID_doc=R"(
Get the type of this object.

:return: type
:rtype: CC_TYPE
)";

const char* ccHObject_getParent_doc=R"(
Get the parent of this object.

:return: parent object
:rtype: ccHObject
)";

const char* ccHObject_hasColors_doc=R"(
Returns True if the entity has colors, False otherwise.

:return: whether the entity has colors
:rtype: bool
)";

const char* ccHObject_hasNormals_doc=R"(
Returns True if the entity has normals, False otherwise.

:return: whether the entity has normals
:rtype: bool
)";

const char* ccHObject_hasScalarFields_doc=R"(
Returns True if the entity has ScalarFields, False otherwise.

:return: whether the entity has ScalarFields
:rtype: bool
)";

const char* ccHObject_colorsShown_doc=R"(
Returns True if the colors are shown, False otherwise.
It is used to prepare the state of the entity when it is saved in a .bin format,
for reopening in the GUI in a predefined state (colors, normal, scalarFields).

:return: whether the colors are shown.
:rtype: bool
)";

const char* ccHObject_normalsShown_doc=R"(
Returns True if the normals are shown, False otherwise.
It is used to prepare the state of the entity when it is saved in a .bin format,
for reopening in the GUI in a predefined state (colors, normal, scalarFields).

:return: whether the normals are shown
:rtype: bool
)";

const char* ccHObject_sfShown_doc=R"(
Returns True if the scalar fields are shown, False otherwise.
It is used to prepare the state of the entity when it is saved in a .bin format,
for reopening in the GUI in a predefined state (colors, normal, scalarFields).

:return: whether the scalar fields are shown
:rtype: bool
)";

const char* ccHObject_isA_doc=R"(
Check the type of object.

:param CC_TYPES type: type to check

:return: OK if object is of this type
:rtype: bool
)";

const char* ccHObject_isKindOf_doc=R"(
Check the type of object.

:param CC_TYPES type: type to check

:return: OK if object is a kind of this type
:rtype: bool
)";

const char* ccHObject_showColors_doc=R"(
Prepare the state of the entity when it is saved in a .bin format,
for reopening in the GUI in a predefined state (colors, normal, scalarFields).

:param bool isShown: whether to show or not the colors.
)";

const char* ccHObject_showNormals_doc=R"(
Prepare the state of the entity when it is saved in a .bin format,
for reopening in the GUI in a predefined state (colors, normal, scalarFields).

:param bool isShown: whether to show or not the normals.
)";

const char* ccHObject_showSF_doc=R"(
Prepare the state of the entity when it is saved in a .bin format,
for reopening in the GUI in a predefined state (colors, normal, scalarFields).

:param bool isShown: whether to show or not the ScalarFields.
)";

const char* ccHObject_incRef_doc=R"(
Increment the Python reference counter.
Do not use, for debug and tests only.
)";

const char* ccHObject_decRef_doc=R"(
Decrement the Python reference counter.
Do not use, for debug and tests only.
)";

const char* ccHObject_countRef_doc=R"(
Get the value of the Python reference counter.
Do not use, for debug and tests only.
)";

const char* ccGenericPointCloud_computeOctree_doc= R"(
Computes the cloud octree.

The octree bounding-box is automatically defined as the smallest
3D cube that totally encloses the cloud.

WARNING: any previously attached octree will be deleted,
even if the new octree computation failed.

:param progressCb,optional: (default None), use None
:param bool,optional autoAddChild: (default `True`) whether to automatically add the computed octree as child of this cloud or not

:return: the computed octree
:rtype: ccOctree)";

const char* ccGenericPointCloud_deleteOctree_doc= R"(
Erases the octree)";

const char* ccGenericPointCloud_getOctree_doc= R"(
Returns the associated octree (if any).

:return: octree
:rtype: ccOctree or None)";

const char* ccGenericPointCloud_getOwnBB_doc= R"(
Returns the bounding box object.

:return: bounding box
:rtype: ccBBox)";

const char* ccShiftedObject_getGlobalShift_doc= R"(
Returns the shift applied to original coordinates.

Original coordinates are equal to '(P/scale - shift)'
The shift is typically used when saving to a file.

:return: applied shift (x, y, z)
:rtype: tuple
)";

const char* ccShiftedObject_setGlobalShift_doc= R"(
Sets shift applied to original coordinates (information storage only)

Such a shift can typically be applied at loading time.
Original coordinates are equal to '(P/scale - shift)'

:param double x: x shift
:param double y: y shift
:param double z: z shift
)";

const char* ccShiftedObject_getGlobalScale_doc= R"(
Returns the scale applied to original coordinates.

:return: the scale applied to original coordinates.
:rtype: double
)";

const char* ccShiftedObject_setGlobalScale_doc= R"(
Sets the scale applied to original coordinates (information storage only).

:param double scale: scale applied
)";

const char* ccShiftedObject_isShifted_doc= R"(
Returns whether the cloud is shifted or not

:return: whether the cloud is shifted or not
:rtype: bool
)";

const char* PointCloudTpl_ccGenericPointCloud_QString_getPoint_doc= R"(
get the ith point in the cloud array.

:return: point coordinates
:rtype: tuple)";

const char* PointCloudTpl_addPoint_doc= R"(
Adds a 3D point to the database

To ensure the best efficiency, the database memory must have already
been reserved (with PointCloud::reserve). Otherwise nothing happens.

:param tuple P: a 3D point)";

const char* PointCloudTpl_reserve_doc= R"(
Reserves memory for the point database.

This method tries to reserve some memory to store points
that will be inserted later (with PointCloud::addPoint).
If the new number of points is smaller than the actual one,
nothing happens.

:param int newNumberOfPoints: the new number of points

:return: true if the method succeeds, false otherwise
:rtype: bool)";

const char* PointCloudTpl_resize_doc= R"(
Resizes the point database.

The cloud database is resized with the specified size. If the new size
is smaller, the overflooding points will be deleted. If its greater,
the database is filled with blank points 
**warning** the PointCloud::addPoint method will insert points after those ones.

:param int newNumberOfPoints: the new number of points

:return: true if the method succeeds, false otherwise
:rtype: bool)";

const char* ReferenceCloud_Doc= R"(
A very simple point cloud (no point duplication).

Implements the GenericIndexedCloudPersist interface. A simple point cloud
that stores references to Generic3dPoint instances in a vector.
Partial thread safety for all methods that can change the size of the cloud
or that change or rely on point ordering.

Several methods provide a ReferenceCloud as a result, which can be seen as a selection
on an original ccPointCloud (the associatedCloud).
The ReferenceCloud can be transformed in a ccPointCloud, using the partialClone method
of the associated ccPointCloud.
)";

const char* ReferenceCloud_addPointIndex_doc= R"(
Point global index insertion mechanism (range), thread safe.

:param int firstIndex: first point global index of range
:param int lastIndex: last point global index of range (excluded)

:return: false if not enough memory
:rtype: bool)";

const char* ReferenceCloud_addPointIndexGlobal_doc= R"(
Point global index insertion mechanism, Thread safe.

:param int globalIndex: a point global index

:return: false if not enough memory
:rtype: bool)";

const char* ReferenceCloud_enableScalarField_doc= R"(
Enables the scalar field associated to the cloud.

If the scalar field structure is not yet initialized/allocated,
this method gives the signal for its creation. Otherwise, if possible
the structure size should be pre-reserved with the same number of
elements as the point cloud.

**Warning** If the cloud is empty, the scalar field will be empty as well.
The scalar field will be reserved with the same capacity as the cloud.

:return: success
:rtype: bool)";

const char* ReferenceCloud_forwardIterator_doc= R"(
Forwards the local element iterator.)";

const char* ReferenceCloud_getAssociatedCloud_doc= R"(
Returns the associated (source) cloud.

:return: the source cloud
:rtype: GenericIndexedCloudPersist)";

const char* ReferenceCloud_getBoundingBox_doc= R"(
Returns the octree bounding box.

Method to request the octree bounding box limits.

:return limits: ((Xmin,Ymin,Zmin), (Xmax,Ymax,Zmax))
:rtype: tuple)";

const char* ReferenceCloud_getCurrentPointCoordinates_doc= R"(
Returns the coordinates of the point pointed by the current element.

:return: coordinates
:rtype: tuple)";

const char* ReferenceCloud_getCurrentPointGlobalIndex_doc= R"(
Returns the global index of the point pointed by the current element.

:return: global index in the cloud
:rtype: int
)";

const char* ReferenceCloud_getCurrentPointScalarValue_doc= R"(
Returns the current point associated scalar value.

:return: scalar value
:rtype: float
)";

const char* ReferenceCloud_getNextPoint_doc= R"(
Returns the next point (relatively to the global iterator position).

Virtual method to handle the cloud global iterator.
Global iterator position should be increased by one each time
this method is called.

Warning:

- the returned object may not be persistent!
- THIS METHOD MAY NOT BE COMPATIBLE WITH PARALLEL STRATEGIES

(see the DgmOctree::executeFunctionForAllCellsAtLevel_MT and
DgmOctree::executeFunctionForAllCellsAtStartingLevel_MT methods).

:return: coordinates of next point (TODO: what if no more?)
:rtype: tuple)";

const char* ReferenceCloud_getPoint_doc= R"(
Returns the ith point.

Method to request a point with a specific index.

WARNINGS:

- the returned object may not be persistent!
- THIS METHOD MAY NOT BE COMPATIBLE WITH PARALLEL STRATEGIES

(see the DgmOctree::executeFunctionForAllCellsAtLevel_MT and
DgmOctree::executeFunctionForAllCellsAtStartingLevel_MT methods).
Consider the other version of getPoint instead or the
GenericIndexedCloudPersist class.

:param int index: index of the requested point (between 0 and the cloud size minus 1)

:return: the requested point coordinates (undefined behavior if index is invalid)
:rtype: tuple)";

const char* ReferenceCloud_getPointGlobalIndex_doc= R"(
Returns global index (i.e. relative to the associated cloud) of a given element.

:param int localIndex: local index (i.e. relative to the internal index container)

:return: global index
:rtype: int)";

const char* ReferenceCloud_getPointScalarValue_doc= R"(
Returns the ith point associated scalar value.

:param int index: local index

:return: scalar value
:rtype: float
)";

const char* ReferenceCloud_isScalarFieldEnabled_doc= R"(
Returns `True` if the scalar field is enabled, `False` otherwise

:return: is the ScalarField enabled?
:rtype: bool)";

const char* ReferenceCloud_placeIteratorAtBeginning_doc= R"(
Sets the cloud iterator at the beginning.

Virtual method to handle the cloud global iterator)";

const char* ReferenceCloud_setCurrentPointScalarValue_doc= R"(
Sets the current point associated scalar value.

:param float value: scalar value)";

const char* ReferenceCloud_setPointScalarValue_doc= R"(
Sets the ith point associated scalar value.

:param int index: local index
:param float value: scalar value)";

const char* ReferenceCloud_size_doc= R"(
Returns the number of points in the cloud

:return: number of points
:rtype: int)";

// const char* = R"()";

#endif /* CCGENERICCLOUDPY_DOCSTRINGS_HPP_ */
