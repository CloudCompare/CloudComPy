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

#ifndef CCPRIMITIVESPY_DOCSTRINGS_HPP_
#define CCPRIMITIVESPY_DOCSTRINGS_HPP_

const char* ccPrimitivesPy_ccGLMatrix_doc= R"(
A 4x4 'transformation' matrix (column major order)
Transformation (M) is composed by a rotation (R) and a translation (T):
M*X = R*X + T)";

const char* ccPrimitivesPy_ccGLMatrixd_doc= R"(
A 4x4 'transformation' matrix (column major order)
Transformation (M) is composed by a rotation (R) and a translation (T):
M*X = R*X + T)";

const char* ccPrimitivesPy_initFromParameters_doc= R"(
Several forms:
 1 Inits transformation from a rotation axis, an angle and a translation
    param[in] alpha_rad rotation angle (in radians)
    param[in] axis3D rotation axis
    param[in] t3D translation (x, y, z)
 2 Inits transformation from 3 rotation angles and a translation
    See http://en.wikipedia.org/wiki/Euler_angles (Tait-Bryan Z1Y2X3)
    param[in] phi_rad Phi angle (in radians)
    param[in] theta_rad Theta angle (in radians)
    param[in] psi_rad Psi angle (in radians)
    param[in] t3D translation (x, y, z))";

const char* ccPrimitivesPy_getTypeName_doc= R"(
Returns type name (sphere, cylinder, etc.))";

const char* ccPrimitivesPy_ccBox_doc= R"(
Box dimensions axis along each dimension are defined in a single 3D vector.
A box is in fact composed of 6 planes (ccPlane).
param dims box dimensions
param transMat optional 3D transformation, default 0  (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Box')";

const char* ccPrimitivesPy_ccCone_doc= R"(
Cone axis corresponds to the 'Z' dimension by default
param bottomRadius cone bottom radius
param topRadius cone top radius
param height cone height (transformation should point to the axis center)
param xOff optional (default 0) displacement of axes along X-axis (Snout mode)
param yOff optional (default 0) displacement of axes along Y-axis (Snout mode)
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Cone'
param precision optional drawing precision (angular step = 360/precision), default 24
param uniqueID optional unique ID (handle with care))";

const char* ccPrimitivesPy_ccCylinder_doc= R"(
Cylinder axis corresponds to the 'Z' dimension.
Internally represented by a cone with the same top and bottom radius.
param radius cylinder radius
param height cylinder height (transformation should point to the axis center)
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Cylinder'
param precision optional drawing precision (angular step = 360/precision), default 24
param uniqueID optional unique ID (handle with care))";

const char* ccPrimitivesPy_ccPlane_doc= R"(
Plane normal corresponds to 'Z' dimension
param xWidth plane width along 'X' dimension
param yWidth plane width along 'Y' dimension
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Plane')";

const char* ccPrimitivesPy_ccPlane_Fit_doc= R"(
Fits a plane primitive on a cloud
The cloud can be either a pointCloud or a Polyline
param cloud input cloud
return plane primitive (if successful)
)";

const char* ccPrimitivesPy_ccPlane_getEquation_doc= R"(
Returns the equation of the plane
Equation:
planeEquation plane equation : [a, b, c, d] as 'ax+by+cz=d')";

const char* ccPrimitivesPy_ccQuadric_doc= R"(
Quadric orthogonal dimension is 'Z' by default
param minCorner min corner of the 'representation' base area
param maxCorner max corner of the 'representation' base area
param eq equation coefficients ( Z = a + b.X + c.Y + d.X^2 + e.X.Y + f.Y^2)
param dims optional (default 0) dimension indexes
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Quadric'
param precision optional drawing precision, default 24)";

const char* ccPrimitivesPy_ccSphere_doc= R"(
param radius sphere radius
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Sphere'
param precision optional drawing precision (angular step = 360/precision), default 24
param uniqueID optional unique ID (handle with care))";

const char* ccPrimitivesPy_ccTorus_doc= R"(
Torus is defined in the XY plane by default
param insideRadius inside radius
param outsideRadius outside radius
param angle rad optional (default 2*pi) subtended angle (in radians)
param rectangularSection optional (default false) whether section is rectangular or round
param rectSectionHeight optional section height (if rectangular torus)
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Torus'
param precision optional drawing precision (main loop angular step = 360/(4*precision), circular section angular step = 360/precision), default 24
param uniqueID optional unique ID (handle with care))";

const char* ccPrimitivesPy_ccDish_doc= R"(
Either a section of a sphere, or half of an ellipsoid!
The origin of the Dish is at the centre of the base, and the Z-axis lies along the normal to the base.
param radius base radius
param height maximum height of dished surface above base
param radius2 optional (default 0) If radius2 is zero, dish is drawn as a section of sphere. If radius2 is >0, dish is defined as half of an ellipsoid.
param transMat optional (default 0) 3D transformation (can be set afterwards with ccDrawableObject::setGLTransformation)
param name optional name, default 'Dish'
param precision optional drawing precision (angular step = 360/precision), default 24)";

const char* ccPrimitivesPy_toString_doc= R"(
String version of the transformer coefficients)";

#endif /* CCPRIMITIVESPY_DOCSTRINGS_HPP_ */
