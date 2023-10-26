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

const char* ccPointCloudPy_changeColorLevels_doc= R"(
Linear transformation of color components levels between boundaries:
the input levels are defined by a lower and an upper bound, and similarly at the output.
The transformation can be applied on one or more color component (red, green, blue)

:param int in0: input lower bound in range [0..255]
:param int in1: input upper bound in range [0..255]
:param int out0: output lower bound in range [0..255]
:param int out1: output upper bound in range [0..255]
:param bool onRed: whether to apply the transformation on the red component
:param bool onGreen: whether to apply the transformation on the green component
:param bool onBlue: whether to apply the transformation on the blue component

:return: success
:rtype: bool)";


const char* ccPointCloudPy_colorize_doc= R"(
Multiplies all color components of all points by coefficients.

If the cloud has no color, all points are considered white and
the color array is automatically allocated.

:param float r: red component (normalized value in [0., 1.])
:param float g: green component (normalized value in [0., 1.])
:param float b: blue component (normalized value in [0., 1.])
:param float,optional a: alpha component default 1.0, (normalized value in [0., 1.])

:return: success
:rtype: bool)";

const char* ccPointCloudPy_computeGravityCenter_doc= R"(
Return a tuple of the 3 coordinates of the gravity center of the cloud.

:return: gravity center coordinates
:rtype: tuple of float )";

const char* ccPointCloudPy_computeScalarFieldGradient_doc= R"(
Computes the geometrical gradient of a scalar field associated to a point cloud.

The gradient is a new scalar field.
See Daniel Girardeau-Montaut's PhD manuscript (Chapter 3, section 3.3.2) for more
information. As explained in this section, if the scalar field corresponds to
(euclidean) distances between the points and another entity, then it is possible
to filter out aberrant values.

:param int SFindex: The scalar Field index in the Point Cloud.
:param double radius: spherical neighborhood size (or 0 for automatic size)
:param bool euclideanDistances: indicates if the scalar values are euclidean distances
:param DgmOctree,optional theOctree: the octree, if it has already been computed

:return: success
:rtype: bool)";

const char* ccPointCloudPy_convertCurrentScalarFieldToColors_doc= R"(
Converts current scalar field to RGB colors.

Requires a current "displayed" scalar field: see :py:meth:`setCurrentDisplayedScalarField`.

:param bool,optional mixWithExistingColor: whether to mix with an existing color, default False

:return: success
:rtype: bool)";

const char* ccPointCloudPy_convertNormalToDipDirSFs_doc= R"(
Converts normals to two scalar fields: 'dip' and 'dip direction'.

See `Strike and dip <https://en.wikipedia.org/wiki/Strike_and_dip>`_. Angles are in degrees.
   
:return: success
:rtype: bool)";

const char* ccPointCloudPy_convertNormalToRGB_doc= R"(
Converts normals to color.

Converts the normals of a cloud to an HSV color field.
H = dip direction, S = dip and V = 1. 

:return: success
:rtype: bool)";

const char* ccPointCloudPy_convertRGBToGreyScale_doc= R"(
Converts RGB to grey scale colors.

:return: success
:rtype: bool)";

const char* ccPointCloudPy_colorsFromNPArray_copy_doc= R"(
Set cloud color from a Numpy array (nbPoints,4).

Each row is (r,g,b,alpha) of type uint8 (unsigned char).
Color memory is reserved automatically in the cloud.

:param ndarray array: a Numpy array (nbPoints,4).
)";

const char* ccPointCloudPy_coordsFromNPArray_copy_doc= R"(
Set cloud coordinates from a Numpy array (nbPoints,3).

**WARNING** Be sure to have an array data in C-style contiguous order,
for instance, a transpose operation do not reorder the data in memory.
Check the boolean ``array.flags['C_CONTIGUOUS']`` and, if ``False``,
reorder with ``array.copy(order='C')``.

Cloud memory is reserved /resized automatically

:param ndarray array: a Numpy array (nbPoints,3).
)";

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

const char* ccPointCloudPy_enhanceRGBWithIntensitySF_doc= R"(
Enhances the RGB colors with a scalar field (assuming it's intensities)

:param int sfIdx: scalarField index
:param bool,optional useCustomIntensityRange: whether to set a custom range for intensity, default False
:param float,optional minI: min value for the custom range, default 0.
:param float,optional maxI: max value for the custom range, default 1.

:return: status
:rtype: bool
)";

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

const char* ccPointCloudPy_getSensors_doc= R"(
Returns a list of sensors if any, otherwise empty list.

:return: list of :py:class:`ccSensor`
:rtype: list
)";

const char* ccPointCloudPy_hasColors_doc= R"(
Return whether the cloud has Colors.

:return: `True` or `False`
:rtype: bool
)";

const char* ccPointCloudPy_hasNormals_doc= R"(
Return whether the cloud has Normals.

:return: `True` or `False`
:rtype: bool
)";

const char* ccPointCloudPy_hasScalarFields_doc= R"(
Return whether the cloud has ScalarFields.

:return: `True` or `False`
:rtype: bool
)";

const char* ccPointCloudPy_interpolateColorsFrom_doc= R"(
Interpolate colors from another cloud (nearest neighbor only).

:param ccGenericPointCloud other: source cloud with color
:param int,optional octreeLevel: octreeLevel, default 0

:return: success
:rtype: bool
)";

const char* ccPointCloudPy_normalsFromNpArrayCopy_doc= R"(
Set cloud normals from a Numpy array (nbPoints,3).

**WARNING** Be sure to have an array data in C-style contiguous order,
for instance, a transpose operation do not reorder the data in memory.
Check the boolean ``array.flags['C_CONTIGUOUS']`` and, if ``False``,
reorder with ``array.copy(order='C')``.

Normals array in cloud is created/rewrited automatically.
Each normal is normalized automatically.
Note: normals are stored in a compressed form inside the cloud.

:param ndarray array: a Numpy array (nbPoints,3).
)";

const char* ccPointCloudPy_normalsToNpArrayCopy_doc= R"(
Export the PointCloud normals into a numpy Array.

returns a numpy Array of shape (number of Points, 3).
Because the normals are stored in compressed way inside the cloud,
the exported normals are decompressed in a new array: the numpy Array object owns its data.
Ownership is transfered to Python:
the numpy Array object and its data will be handled by the Python Garbage Collector.
The normals must have been computed before.

:return: numpy Array of shape (number of Points, 3)
:rtype: ndarray
)";

const char* ccPointCloudPy_orientNormalsWithFM_doc= R"(
Orient normals with Fast Marching method.

See `Fast marching method <https://en.wikipedia.org/wiki/Fast_marching_method>`_.

:param int,optional octreeLevel: octree level, default 6

:return: success
:rtype: bool
)";

const char* ccPointCloudPy_orientNormalsWithMST_doc= R"(
Orient normals with Minimum Spanning Tree method.

See `Minimum spanning tree <https://en.wikipedia.org/wiki/Minimum_spanning_tree>`_.

:param int,optional octreeLevel: octree level, default 6

:return: success
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

:param float fx: scale x
:param float fy: scale y
:param float fz: scale z
:param tuple,optional center: (xc, yc, zc), default (0,0,0))";

const char* ccPointCloudPy_setColor_doc= R"(
Set a unique color for the whole cloud (RGBA).
Color array is automatically allocated if necessary.

:param QColor color: a unique color, alpha is taken into account.

:return: success
:rtype: bool
)";

const char* ccPointCloudPy_setColorGradient_doc= R"(
Assigns color to points proportionally to their 'height'.

Height is defined with regard to to the specified dimension (heightDim).
Color array is defined by a two colors (QColor).

:param int heightDim: ramp dimension (0:X, 1:Y, 2:Z)
:param QColor first: first color of the array
:param QColor second: second color of the array

:return: success
:rtype: bool)";

const char* ccPointCloudPy_setColorGradientBanded_doc= R"(
Assigns color to points by 'banding'.
Banding is performed along the specified dimension.
Color array is automatically defined.

:param int heightDim: banding dimension (0:X, 1:Y, 2:Z)
:param double freq: banding frequency (size of the bands along the axis)

:return: success
:rtype: bool)";

const char* ccPointCloudPy_setColorGradientDefault_doc= R"(
Assigns color to points proportionally to their 'height'.

Height is defined with regard to to the specified dimension (heightDim).
Color array is defined by default.

:param int heightDim: ramp dimension (0:X, 1:Y, 2:Z)

:return: success
:rtype: bool)";

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

const char* ccPointCloudPy_showSFColorsScale_doc= R"(
Display or not the Color Scale ramp when render to a file.

:param bool state: whether to display the Color Scale Ramp when render to a file)";

const char* ccPointCloudPy_shrinkToFit_doc= R"(
Removes unused capacity)";

const char* ccPointCloudPy_sfFromColor_doc= R"(
Creates ScalarFields from color components.

:param bool exportR: whether to create a scalarField from red component
:param bool exportG: whether to create a scalarField from green component
:param bool exportB: whether to create a scalarField from blue component
:param bool exportAlpha: whether to create a scalarField from Alpha
:param bool exportComposite: whether to create a scalarField from (r+g+b)/3.

:return: success
:rtype: bool
)";

const char* ccPointCloudPy_size_doc= R"(
Return the number of points in the cloud.

:return: number of points in the cloud
:rtype: int)";

const char* ccPointCloudPy_colorsToNpArray_doc= R"(
Wrap the PointCloud colors into a numpy Array, without copy.

Returns a numpy Array of shape (number of Points, 4) (r, g ,b, a).
Data type np.uint8.
Data is not copied, the numpy Array object does not own the data.

**WARNING** No automatic action on the Python side on the variables referencing the C++ object in case of destruction!

:return: numpy Array of shape (number of Points, 4) dtype uint8
:rtype: ndarray
)";

const char* ccPointCloudPy_colorsToNpArrayCopy_doc= R"(
Wrap the PointCloud colors into a numpy Array, with copy.

Returns a numpy Array of shape (number of Points, 4) (r, g ,b, a).
Data type np.uint8.
Data is copied, the numpy Array object  owns its data.
Ownership is transfered to Python:
the numpy Array object and its data will be handled by the Python Garbage Collector

:return: numpy Array of shape (number of Points, 4) dtype uint8
:rtype: ndarray
)";

const char* ccPointCloudPy_toNpArray_doc= R"(
Wrap the PointCloud coordinates into a numpy Array, without copy.

Returns a numpy Array of shape (number of Points, 3).
Data is not copied, the numpy Array object does not own the data.

**WARNING** No automatic action on the Python side on the variables referencing the C++ object in case of destruction!

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

const char* ccPointCloudPy_unallocateColors_doc= R"(
Erases the cloud colors.
)";

const char* ccPointCloudPy_unallocateNorms_doc= R"(
Erases the cloud normals.
)";

#endif /* CCPOINTCLOUDPY_DOCSTRINGS_HPP_ */
