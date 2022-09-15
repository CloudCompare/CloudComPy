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

#ifndef CCPRIMITIVESPY_DOCSTRINGS_HPP_
#define CCPRIMITIVESPY_DOCSTRINGS_HPP_

const char* ccPrimitivesPy_ccGLMatrix_doc= R"(
A 4x4 'transformation' matrix (column major order), simple precision (float).

Transformation (M) is composed by a rotation (R) and a translation (T):
``M*X = R*X + T``. See OpenGL transformations.

Available operators: ``+= -= *= *``

Default constructor (no parameter) gives identity.

Alternate constructor 1:

:param tuple X: 3 first elements of the 1st column (last one is 0)
:param tuple Y: 3 first elements of the 2nd column (last one is 0)
:param tuple Z: 3 first elements of the 3rd column (last one is 0)
:param tuple Tr: 3 first elements of the last column (last one is 1)

Alternate constructor 2:
:param list data: 16 elements of the 4x4 matrix, stored in column major order (see :py:meth:`data`)
)";

const char* ccPrimitivesPy_ccGLMatrixd_doc= R"(
A 4x4 'transformation' matrix (column major order), double precision (double).

Transformation (M) is composed by a rotation (R) and a translation (T):
``M*X = R*X + T``. See OpenGL transformations.

Available operators: ``+= -= *= *``

Default constructor (no parameter) gives identity.

Alternate constructor 1:

:param tuple X: 3 first elements of the 1st column (last one is 0)
:param tuple Y: 3 first elements of the 2nd column (last one is 0)
:param tuple Z: 3 first elements of the 3rd column (last one is 0)
:param tuple Tr: 3 first elements of the last column (last one is 1)

Alternate constructor 2:
:param list data: 16 elements of the 4x4 matrix, stored in column major order (see :py:meth:`data`)
)";

const char* ccPrimitivesPy_ccGLMatrixParams1_doc= R"(
Equivalent parameters of a ccGLMatrix transformation: a rotation axis, an angle and a translation.

:ivar float alpha_rad: rotation angle in radians (in [0;pi])
:ivar tuple axis3D: unit rotation axis
:ivar tuple t3D: translation)";

const char* ccPrimitivesPy_ccGLMatrixParams2_doc= R"(
Equivalent parameters of a ccGLMatrix transformation: 3 rotation angles and a translation.

See `Euler angles <http://en.wikipedia.org/wiki/Euler_angles>`_ (Tait-Bryan Z1Y2X3)

:ivar float phi_rad: Phi angle (in radians)
:ivar float theta_rad: Theta angle (in radians)
:ivar float psi_rad: Psi angle (in radians)
:ivar tuple t3D: translation)";

const char* ccPrimitivesPy_fromDouble_doc= R"(
Converts a ccGLMatrixd (double precision) in a ccGLMatrix (simple precision).

:param ccGLMatrix_double dblmat: double precision ccGLMatrix

:return: simple precision ccGLMatrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_getParameters1_py_doc= R"(
Returns equivalent parameters: a rotation axis, an angle and a translation.

:return: equivalent parameters structure
:rtype: ccGLMatrixParams1_float or ccGLMatrixParams1_double)";

const char* ccPrimitivesPy_getParameters2_py_doc= R"(
Returns equivalent parameters: 3 rotation angles and a translation.

:return: equivalent parameters structure
:rtype: ccGLMatrixParams2_float or ccGLMatrixParams2_double)";

const char* ccPrimitivesPy_getTransformation_doc= R"(
Returns transformation matrix.

:return: transformation matrix
:rtype: ccGLMatrix
)";

const char* ccPrimitivesPy_getHeight_doc= R"(
Returns Height.

:return: Height
:rtype: double
)";

const char* ccPrimitivesPy_getBottomRadius_doc= R"(
Returns bottom radius.

:return: bottom radius
:rtype: double
)";

const char* ccPrimitivesPy_getTopRadius_doc= R"(
Returns top radius.

:return: top radius
:rtype: double
)";

const char* ccPrimitivesPy_getBottomCenter_doc= R"(
Returns axis bottom end point after applying transformation.

:return: axis bottom end point after applying transformation
:rtype: CCVector3
)";

const char* ccPrimitivesPy_getTopCenter_doc= R"(
Returns axis top end point after applying transformation.

:return: axis top end point after applying transformation
:rtype: CCVector3
)";

const char* ccPrimitivesPy_getSmallCenter_doc= R"(
Returns cone axis end point associated with whichever radii is smaller.

:return: cone axis end point associated with whichever radii is smaller
:rtype: CCVector3
)";

const char* ccPrimitivesPy_getLargeCenter_doc= R"(
Returns cone axis end point associated with whichever radii is larger.

:return: cone axis end point associated with whichever radii is larger
:rtype: CCVector3
)";

const char* ccPrimitivesPy_getSmallRadius_doc= R"(
Returns whichever cone radii is smaller.

:return: whichever cone radii is smaller
:rtype: double
)";

const char* ccPrimitivesPy_getLargeRadius_doc= R"(
Returns whichever cone radii is larger.

:return: whichever cone radii is larger
:rtype: double
)";

const char* ccPrimitivesPy_Interpolate_doc= R"(
Interpolates two matrices at relative position 'coef'

:param float coef: interpolation position (should be between 0 and 1).
:param ccGLMatrix glMat1: 'left' matrix
:param ccGLMatrix glMat2: 'right' matrix

:return: interpolated matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_FromToRotation_doc= R"(
Creates a transformation matrix that rotates a vector to another.

Adapted from  "Efficiently Building a Matrix to Rotate One Vector to Another"
By Tomas Möller, John Hughes, Journal of Graphics Tools, 4(4):1-4, 1999

:param tuple from: normalized non-zero source vector
:param tuple to: normalized non-zero destination vector

:return: transformation matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_FromViewDirAndUpDir_doc= R"(
Generates a 'viewing' matrix from a looking vector and a 'up' direction.

*warning* No translation is applied (pure rotation matrix).

:param tuple forward: forward 'view' vector
:param tuple up: up vector

:return: rotation matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_xRotation_doc= R"(
Returns the rotation component around X only.

:return: rotation matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_yRotation_doc= R"(
Returns the rotation component around Y only.

:return: rotation matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_zRotation_doc= R"(
Returns the rotation component around Z only.

:return: rotation matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_clearTranslation_doc= R"(
Clears translation.

Translation is set to (0,0,0).)";

const char* ccPrimitivesPy_invert_doc= R"(
Inverts transformation (in place).)";

const char* ccPrimitivesPy_inverse_doc= R"(
Returns inverse transformation.

:return: inverse transformation matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_transpose_doc= R"(
Transposes matrix (in place))";

const char* ccPrimitivesPy_transposed_doc= R"(
Returns transposed matrix.

:return: transposed matrix
:rtype: ccGLMatrix)";

const char* ccPrimitivesPy_data_doc= R"(
Returns internal data of the ccGLMatrix

:return: one dim. data array of 16 float in the order col. 1, col. 2, col. 3, col. 4
:rtype: tuple)";

const char* ccPrimitivesPy_getColumn_doc= R"(
Returns a column of the transformation, given its index.

:param int index: column index (between 0 and 3)

:return: column
:rtype: tuple)";

const char* ccPrimitivesPy_initFromParameters1_doc= R"(
Inits transformation from a rotation axis, an angle and a translation.

:param float alpha_rad: rotation angle (in radians)
:param tuple axis3D: rotation axis
:param tuple t3D: translation (x, y, z)
)";

const char* ccPrimitivesPy_initFromParameters2_doc= R"(
Inits transformation from 3 rotation angles and a translation.

See `Euler angles <http://en.wikipedia.org/wiki/Euler_angles>`_ (Tait-Bryan Z1Y2X3)

:param float phi_rad: Phi angle (in radians)
:param float theta_rad: Theta angle (in radians)
:param float psi_rad: Psi angle (in radians)
:param tuple t3D: translation (x, y, z))";


const char* ccPrimitivesPy_getTypeName_doc= R"(
Returns type name (sphere, cylinder, etc.)

:return: type name
:rtype: str )";

const char* ccPrimitivesPy_ccBox_doc= R"(
Box dimensions axis along each dimension are defined in a single 3D vector.

A box is in fact composed of 6 planes (ccPlane).

:param tuple dims: box dimensions
:param ccGLMatrix,optional transMat: 3D transformation, default 0
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: name, default 'Box'
)";

const char* ccPrimitivesPy_ccBox_getDimensions_doc= R"(
Get the box dimensions.

:return: the box dimensions
:rtype: tuple dimensions
)";

const char* ccPrimitivesPy_ccCone_doc= R"(
Cone axis corresponds to the 'Z' dimension by default.

:param float bottomRadius: cone bottom radius
:param float topRadius: cone top radius
:param float height: cone height (transformation should point to the axis center)
:param float,optional xOff: (default 0) displacement of axes along X-axis (Snout mode)
:param float,optional yOff: (default 0) displacement of axes along Y-axis (Snout mode)
:param ccGLMatrix,optional transMat: (default 0) 3D transformation
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: name, default 'Cone'
:param int,optional precision: drawing precision (angular step = 360/precision), default 24
:param int,optional uniqueID:  unique ID (handle with care))";

const char* ccPrimitivesPy_ccCylinder_doc= R"(
Cylinder axis corresponds to the 'Z' dimension.

Internally represented by a cone with the same top and bottom radius.

:param float radius: cylinder radius
:param float height: cylinder height (transformation should point to the axis center)
:param ccGLMatrix,optional transMat: (default 0) 3D transformation
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: name, default 'Cylinder'
:param int,optional precision: drawing precision (angular step = 360/precision), default 24
:param int,optional uniqueID: unique ID (handle with care))";

const char* ccPrimitivesPy_ccPlane_doc= R"(
Plane normal corresponds to 'Z' dimension.

:param float xWidth: plane width along 'X' dimension
:param float yWidth: plane width along 'Y' dimension
:param ccGLMatrix,optional transMat: (default 0) 3D transformation
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: name, default 'Plane')";

const char* ccPrimitivesPy_ccPlane_Fit_doc= R"(
Fits a plane primitive on a cloud.

The cloud can be either a pointCloud or a Polyline.

:param GenericIndexedCloudPersist cloud: input cloud

:return: plane primitive (if successful)
:rtype: ccPlane
)";

const char* ccPrimitivesPy_ccPlane_getEquation_doc= R"(
Returns the equation of the plane.

:return: Plane equation : [a, b, c, d] as ``ax+by+cz=d``
:rtype: tuple)";

const char* ccPrimitivesPy_ccPlane_getCenter_doc= R"(
Returns the center of the plane.

:return: plane center
:rtype: CCVector3)";

const char* ccPrimitivesPy_ccPlane_getNormal_doc= R"(
Returns the normal of the plane.

:return: plane normal
:rtype: CCVector3)";

const char* ccPrimitivesPy_ccQuadric_doc= R"(
Quadric orthogonal dimension is 'Z' by default.

:param tuple minCorner: min corner of the 'representation' base area
:param tuple maxCorner: max corner of the 'representation' base area
:param tuple eq: equation coefficients ( Z = a + b.X + c.Y + d.X^2 + e.X.Y + f.Y^2)
:param tuple,optional dims: (default 0) dimension indexes
:param ccGLMatrix,optional transMat: (default 0) 3D transformation
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: optional name, default 'Quadric'
:param int,optional precision: optional drawing precision, default 24)";

const char* ccPrimitivesPy_ccSphere_doc= R"(
:param float radius: sphere radius.
:param ccGLMatrix,optional transMat: (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: optional name, default 'Sphere'
:param int,optional precision: optional drawing precision (angular step = 360/precision), default 24
:param int,optional uniqueID: optional unique ID (handle with care))";

const char* ccPrimitivesPy_ccSphere_getRadius_doc= R"(
Get the sphere radius.

:return: radius
:rtype: float
)";

const char* ccPrimitivesPy_ccTorus_doc= R"(
Torus is defined in the XY plane by default.

:param float insideRadius: inside radius
:param float outsideRadius: outside radius
:param float,optional angle: rad (default 2*pi) subtended angle (in radians)
:param bool,optional rectangularSection: (default `False`) whether section is rectangular or round
:param float,optional rectSectionHeight: section height (if rectangular torus)
:param ccGLMatrix,optional transMat: (default 0) 3D transformation
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: name, default 'Torus'
:param int,optional precision: drawing precision
       (main loop angular step = 360/(4*precision),
       circular section angular step = 360/precision), default 24
:param int,optional uniqueID: unique ID (handle with care))";

const char* ccPrimitivesPy_ccDish_doc= R"(
Either a section of a sphere, or half of an ellipsoid!

The origin of the Dish is at the centre of the base, and the Z-axis lies along the normal to the base.

:param float radius: base radius
:param float height: maximum height of dished surface above base
:param float,optional radius2: (default 0) If radius2 is zero, dish is drawn as a section of sphere.
       If radius2 is >0, dish is defined as half of an ellipsoid.
:param ccGLMatrix,optional transMat: (default 0) 3D transformation
       (can be set afterwards with ccDrawableObject::setGLTransformation)
:param str,optional name: name, default 'Dish'
:param int,optional precision: drawing precision (angular step = 360/precision), default 24)";

const char* ccPrimitivesPy_toString_doc= R"(
String version of the transformer coefficients

:return: String version of the transformer coefficients
:rtype: string)";

const char* ccPrimitivesPy_fromString_doc= R"(
Static method returning a ccGLMatrix from the string version of the transformer coefficients.

If the creation does not succeed, returns identity transformation.

:param string matText: string version of the transformer coefficients.

:return: ccGLMatrix transformation
:rtype: ccGLMatrix)";


#endif /* CCPRIMITIVESPY_DOCSTRINGS_HPP_ */
