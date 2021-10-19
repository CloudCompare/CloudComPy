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

#ifndef CCGENERICCLOUDPY_DOCSTRINGS_HPP_
#define CCGENERICCLOUDPY_DOCSTRINGS_HPP_

const char* ccBBox_doc= R"(
BoundingBox object.

Can be initialized with a tuple of coordinates (minCorner, maxCorner))";

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

const char* ccGenericPointCloud_computeOctree_doc= R"(
Computes the cloud octree.

The octree bounding-box is automatically defined as the smallest
3D cube that totally encloses the cloud.

WARNING: any previously attached octree will be deleted,
even if the new octree computation failed.

:param progressCb: (default None), use None
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

const char* PointCloudTpl_ccGenericPointCloud_QString_getPoint_doc= R"(
get the ith point in the cloud array.

:return: point coordinates
:rtype: tuple)";

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
