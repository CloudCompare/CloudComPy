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

#ifndef CCPOINTCLOUDPY_DOCSTRINGS_HPP_
#define CCPOINTCLOUDPY_DOCSTRINGS_HPP_

const char* ccPointCloudPy_ccPointCloud_doc= R"(
A 3D cloud and its associated features ( normals, scalar fields, etc.)

A point cloud can have multiple features:

- normals (compressed) (TODO)
- scalar fields
- an octree structure
- other children objects (meshes, calibrated pictures, etc.) (TODO) )";

const char* ccPointCloudPy_addScalarField_doc= R"(
Creates a new scalar field and registers it.

Warnings:

- the name must be unique (the method will fail if a SF with the same name already exists)
- this method DOES resize the scalar field to match the current cloud size

:param str uniqueName: scalar field name (must be unique)

:return: index of this new scalar field (or -1 if an error occurred)
:rtype: int
)";

const char* ccPointCloudPy_applyRigidTransformation_doc= R"(
Applies a GL transformation to the entity::

  this = rotMat*(this-rotCenter)+(rotCenter+trans)

:param ccGLMatrix trans: a ccGLMatrix structure)";

const char* ccPointCloudPy_cloneThis_doc= R"(
Clones this entity.

All the main features of the entity are cloned, except from the octree and
the points visibility information.

:param ccPointCloud,optional destCloud: the destination cloud can be provided here
:param bool,optional ignoreChildren: whether to ignore the cloud's children or not (in which case they will be cloned as well)

:return: a copy of this entity
:rtype: ccPointCloud)";

const char* ccPointCloudPy_computeGravityCenter_doc= R"(
Return a tuple of the 3 coordinates of the gravity center of the cloud.

:return: gravity center coordiinates
:rtype: tuple of float )";

const char* ccPointCloudPy_coordsFromNPArray_copy_doc= R"(
Set cloud coordinates from a Numpy array (nbPoints,3).

Cloud memory is reserved /resized automatically)";

const char* ccPointCloudPy_crop2D_doc= R"(
Crop the point cloud using a 2D polyline.

:param ccPolyline poly: polyline object
:param int orthoDim: normal plane, value in (0, 1, 2) 0 = oY, 1 = oX, 2 = oZ
:param bool inside: boolean

:return: the cropped cloud. Points are copied, the original cloud is not modified.
:rtype: ccPointCloud
)";

const char* ccPointCloudPy_deleteAllScalarFields_doc= R"(
Deletes all scalar fields associated to this cloud)";

const char* ccPointCloudPy_deleteScalarField_doc= R"(
Deletes a specific scalar field.

WARNING: this operation may modify the scalar fields order
(especially if the deleted SF is not the last one). 
However current IN & OUT scalar fields will stay up-to-date
(while their index may change).

:param int index: index of scalar field to be deleted)";

const char* ccPointCloudPy_exportCoordToSF_doc= R"(
Export coordinates to ScalarFields.

:param bool isXexported: export X coordinates
:param bool isYexported: export Y coordinates
:param bool isZexported: export Z coordinates

:return: status
:rtype: bool
)";

const char* ccPointCloudPy_exportNormalToSF_doc= R"(
Export normals to ScalarFields.

:param bool isXexported: export X normals coordinate
:param bool isYexported: export Y normals coordinate
:param bool isZexported: export Z normals coordinate

:return: status
:rtype: bool
)";

const char* ccPointCloudPy_filterPointsByScalarValue_doc= R"(
Filters out points whose scalar values falls into an interval.

Threshold values should be expressed relatively to the current displayed scalar field.
See :py:meth:`setCurrentDisplayedScalarField` method.

:param float minVal: minimum value
:param float maxVal: maximum value
:param bool,optional outside: default False, whether to select the points inside or outside of the specified interval

:return: resulting cloud (remaining points)
:rtype: ccPointCloud
)";

const char* ccPointCloudPy_fuse_doc= R"(
Append in place another cloud.

No return.

:param ccPointCloud other: cloud to fuse with this one, modification in place.
)";
const char* ccPointCloudPy_getCurrentDisplayedScalarField_doc= R"(
Returns the currently displayed scalar (or None if none)

Current displayed scalar field is used in some methods such as :py:meth:`filterPointsByScalarValue`

:return: currently displayed scalar field
:rtype: ScalarField or None
)";

const char* ccPointCloudPy_getCurrentDisplayedScalarFieldIndex_doc= R"(
Returns the currently displayed scalar field index (or -1 if none)

Current displayed scalar field is used in some methods such as :py:meth:`filterPointsByScalarValue`

:return: currently displayed scalar field index
:rtype: int
)";

const char* ccPointCloudPy_getCurrentInScalarField_doc= R"(
Return the current 'in' ScalarField if there is one, otherwise None.

:return: current 'in' ScalarField
:rtype: ScalarField or None
)";

const char* ccPointCloudPy_getCurrentOutScalarField_doc= R"(
Return the current 'out' ScalarField if there is one, otherwise None.

:return: current 'out' ScalarField
:rtype: ScalarField or None
)";

const char* ccPointCloudPy_getNumberOfScalarFields_doc= R"(
Return the number of scalar fields associated to the cloud.

:return: number of scalar fields
:rtype: int )";

const char* ccPointCloudPy_getScalarField_doc= R"(
Find by index: return a ScalarField if index is valid, otherwise None.

:param int index: index of the ScalarField

:return: the ScalarField at index
:rtype: ScalarField or None
)";

const char* ccPointCloudPy_getScalarFieldByName_doc= R"(
Find by name: return a ScalarField if there is one with this name, otherwise None.

:param str name: name of the ScalarField

:return: ScalarField
:rtype: ScalarField or None
)";

const char* ccPointCloudPy_getScalarFieldDic_doc= R"(
Return a dictionary [ScalarField name] --> ScalarField index.

:return: a dictionary [name]->index
:rtype: dic
)";

const char* ccPointCloudPy_getScalarFieldName_doc= R"(
Return the ScalarField name if index is valid, otherwise None.

:param int index: index of the ScalarField

:return: ScalarField name at index
:rtype: str or None
)";

const char* ccPointCloudPy_hasScalarFields_doc= R"(
Return whether the cloud has ScalarFields.

:return: `True` or `False`
:rtype: bool
)";

const char* ccPointCloudPy_partialClone_doc= R"(
Creates a new point cloud object from a ReferenceCloud (selection)

'Reference clouds' are a set of indexes referring to a real point cloud.
See CClib documentation for more information about ReferenceClouds.

**Warning**: the ReferenceCloud structure must refer to this cloud.

:param ReferenceCloud selection: a ReferenceCloud structure (pointing to source)

:return: a tuple(:class:`ccPointCloud`, :class:`CLONE_WARNINGS`) warning status, if not 0, 
         indicate out of memory errors (see :class:`CLONE_WARNINGS`)
:rtype: tuple
)";

const char* ccPointCloudPy_renameScalarField_doc= R"(
Rename the ScalarField if index is valid.

Warning: the name should not be already used by other ScalarFields of the cloud!
TODO: check the names in use.

:param int index: index of the ScalarField
:param str name: new name

:return: `True` in case of success
:rtype: bool
)";

const char* ccPointCloudPy_reserve_doc= R"(
Reserves memory for all the active features.

This method is meant to be called before increasing the cloud population.
Only the already allocated features will be re-reserved.

:param int nbPts: number of points

:return:  `True` if ok, `False` if there's not enough memory
:rtype: bool
)";

const char* ccPointCloudPy_resize_doc= R"(
Resizes all the active features arrays.

This method is meant to be called after having increased the cloud population
(if the final number of insterted point is lower than the reserved size).
Otherwise, it fills all new elements with blank values.

:return: `True` if ok, `False` if there's not enough memory
:rtype: bool
)";

const char* ccPointCloudPy_scale_doc= R"(
Scale the cloud with separate factors along the 3 directions x,y,z and an optional center (default: (0,0,0)).

:param float x: scale x
:param float y: scale y
:param float z: scale z
:param tuple,optional center: (xc, yc, zc), default (0,0,0))";

const char* ccPointCloudPy_setCurrentDisplayedScalarField_doc= R"(
Sets the currently displayed scalar field.

Current displayed scalar field is used in some methods such as :py:meth:`filterPointsByScalarValue`

**Warning**: this scalar field will automatically be set as the OUTPUT one!

:param int index: index of the displayed ScalarField
)";

const char* ccPointCloudPy_setCurrentScalarField_doc= R"(
Set the current 'in' and 'out ScalarField index. No validity check. Use -1 to set None.

:param int index: index of the 'in' and 'out' ScalarField)";

const char* ccPointCloudPy_setCurrentInScalarField_doc= R"(
Set the current 'in' ScalarField index. No validity check. Use -1 to set None.

:param int index: index of the 'in' ScalarField)";

const char* ccPointCloudPy_setCurrentOutScalarField_doc= R"(
Set the current 'out' ScalarField index. No validity check. Use -1 to set None.

:param int index: index of the 'out' ScalarField)";

const char* ccPointCloudPy_shrinkToFit_doc= R"(
Removes unused capacity)";

const char* ccPointCloudPy_size_doc= R"(
Return the number of points in the cloud.

:return: number of points in the cloud
:rtype: int)";

const char* ccPointCloudPy_toNpArray_doc= R"(
Wrap the PointCloud coordinates into a numpy Array, without copy.

Returns a numpy Array of shape (number of Points, 3).
Data is not copied, the numpy Array object does not own the data.

:return: numpy Array of shape (number of Points, 3)
:rtype: ndarray
)";

const char* ccPointCloudPy_toNpArrayCopy_doc= R"(
Wrap the PointCloud coordinates into a numpy Array, with copy.

returns a numpy Array of shape (number of Points, 3)
Data is copied, the  numpy Array object owns its data.
Ownership is transfered to Python:
the numpy Array object and its data will be handled by the Python Garbage Collector

:return: numpy Array of shape (number of Points, 3)
:rtype: ndarray
)";

const char* ccPointCloudPy_translate_doc= R"(
translate the cloud of (x,y,z).

:param tuple translation: tuple: (x,y,z))";

#endif /* CCPOINTCLOUDPY_DOCSTRINGS_HPP_ */
