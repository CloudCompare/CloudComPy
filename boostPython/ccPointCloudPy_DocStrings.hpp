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

const char* ccHObject_setName_doc= R"(
Set the name of the entity)";

const char* ccHObject_getName_doc= R"(
get the name of the entity)";

const char* ccGenericPointCloud_computeOctree_doc= R"(
Computes the cloud octree
The octree bounding-box is automatically defined as the smallest
3D cube that totally encloses the cloud.
WARNING: any previously attached octree will be deleted,
         even if the new octree computation failed.
param progressCb (default None), use None
param autoAddChild (default True) whether to automatically add the computed octree as child of this cloud or not
return the computed octree
)";

const char* ccPointCloudPy_ccPointCloud_doc= R"(
A 3D cloud and its associated features ( normals, scalar fields, etc.)
A point cloud can have multiple features:
- normals (compressed) (TODO)
- scalar fields
- an octree structure
- other children objects (meshes, calibrated pictures, etc.) (TODO) )";

const char* ccPointCloudPy_addScalarField_doc= R"(
Creates a new scalar field and registers it
Warnings:
- the name must be unique (the method will fail if a SF with the same name already exists)
- this method DOES resize the scalar field to match the current cloud size
param uniqueName scalar field name (must be unique)
return index of this new scalar field (or -1 if an error occurred)
)";

const char* ccPointCloudPy_applyRigidTransformation_doc= R"(
Applies a GL transformation to the entity
this = rotMat*(this-rotCenter)+(rotCenter+trans)
param trans a ccGLMatrix structure)";

const char* ccPointCloudPy_cloneThis_doc= R"(
Clones this entity
All the main features of the entity are cloned, except from the octree and
the points visibility information.
param destCloud [optional] the destination cloud can be provided here
param ignoreChildren [optional] whether to ignore the cloud's children or not (in which case they will be cloned as well)
return a copy of this entity)";

const char* ccPointCloudPy_computeGravityCenter_doc= R"(
Return a tuple of the 3 coordinates of the gravity center of the cloud)";

const char* ccPointCloudPy_crop2D_doc= R"(
Crop the point cloud using a 2D polyline.
parameters:
- polyline object
- orthoDim : normal plane, value in (0, 1, 2) 0 = oY, 1 = oX, 2 = oZ
- inside : boolean
return the cropped cloud. Points are copied, the original cloud is not modified.
)";

const char* ccPointCloudPy_deleteAllScalarFields_doc= R"(
Deletes all scalar fields associated to this cloud)";

const char* ccPointCloudPy_deleteScalarField_doc= R"(
Deletes a specific scalar field.
WARNING: this operation may modify the scalar fields order
(especially if the deleted SF is not the last one). 
However current IN & OUT scalar fields will stay up-to-date
(while their index may change).
param index index of scalar field to be deleted)";

const char* ccPointCloudPy_exportCoordToSF_doc= R"(
export coordinates to ScalarFields.
parameters:
- isXexported boolean
- isYexported boolean
- isZexported boolean
return status
)";

const char* ccPointCloudPy_exportNormalToSF_doc= R"(
export normals to ScalarFields.
parameters:
- isXexported boolean
- isYexported boolean
- isZexported boolean
return status
)";

const char* ccPointCloudPy_getCurrentInScalarField_doc= R"(
Return the current 'in' ScalarField if there is one, otherwise None.)";

const char* ccPointCloudPy_getCurrentOutScalarField_doc= R"(
Return the current 'out' ScalarField if there is one, otherwise None.)";

const char* ccPointCloudPy_getNumberOfScalarFields_doc= R"(
return the number of scalar fields associated to the cloud)";

const char* ccPointCloudPy_getScalarField_doc= R"(
Return a ScalarField if index is valid, otherwise None.
param index)";

const char* ccPointCloudPy_getScalarFieldName_doc= R"(
Return the ScalarField name if index is valid, otherwise None.
param index)";

const char* ccPointCloudPy_hasScalarFields_doc= R"(
Return True or False.)";

const char* ccPointCloudPy_renameScalarField_doc= R"(
Rename the ScalarField if index is valid.
Warning: Name should not be already used by other ScalarFields of the cloud!
TODO: check the names in use.
parameters
- index of the ScalarField
- new name
return True in case of success)";

const char* ccPointCloudPy_reserve_doc= R"(
Reserves memory for all the active features
This method is meant to be called before increasing the cloud population.
Only the already allocated features will be re-reserved.
return true if ok, false if there's not enough memory)";

const char* ccPointCloudPy_resize_doc= R"(
Resizes all the active features arrays
This method is meant to be called after having increased the cloud population
(if the final number of insterted point is lower than the reserved size).
Otherwise, it fills all new elements with blank values.
return true if ok, false if there's not enough memory)";

const char* ccPointCloudPy_scale_doc= R"(
Scale the cloud with separate factors along the 3 directions x,y,z and an optional center (default: (0,0,0)).
parameters:
- x,
- y,
- z,
- center tuple: (xc, yc, zc), default (0,0,0))";

const char* ccPointCloudPy_setCurrentInScalarField_doc= R"(
Set the current 'in' ScalarField index. No validity check. Use -1 to set None.
param index)";

const char* ccPointCloudPy_setCurrentOutScalarField_doc= R"(
Set the current 'out' ScalarField index. No validity check. Use -1 to set None.
param index)";

const char* ccPointCloudPy_size_doc= R"(
Return the number of points in the cloud)";

const char* ccPointCloudPy_toNpArray_doc= R"(
Wrap the PointCloud coordinates into a numpy Array, without copy.
returns PyArray of shape (number of Points, 3)
Data is not copied, the PyArray object does not own the data.)";

const char* ccPointCloudPy_toNpArrayCopy_doc= R"(
Wrap the PointCloud coordinates into a numpy Array, with copy.
returns PyArray of shape (number of Points, 3)
Data is copied, the PyArray object owns its data.
Ownership is transfered to Python:
  the PyArray object and its data will be handled by the Python Garbage Collector)";

const char* ccPointCloudPy_translate_doc= R"(
translate the cloud of (x,y,z)
parameters:
translation tuple: (x,y,z))";

#endif /* CCPOINTCLOUDPY_DOCSTRINGS_HPP_ */
