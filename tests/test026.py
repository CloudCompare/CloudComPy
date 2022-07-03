#!/usr/bin/env python3

##########################################################################
#                                                                        #
#                              CloudComPy                                #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation; either version 3 of the License, or     #
#  any later version.                                                    #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
#                                                                        #
#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

import os
import sys
import math
import numpy as np

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

#---transformations-begin
# --- a transformation from a rotation angle and axis, plus a translation 

#---transformations01-begin
tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.47, (1.5, 2.9, 6.3), (5,3,-2))
#---transformations01-end

# --- check the access to transformation matrix data (4x4 matrix stored in major column order)

#---transformations02-begin
m_data_str = tr1.toString()
m_data = tr1.data()
tr1b = cc.ccGLMatrix.fromString(m_data_str)
m_datab = tr1b.data()
for i in range(16):
    if m_data[i] != m_datab[i]:
        raise RuntimeError
#---transformations02-end

# --- transformations can be stored in double precision
#     (useful if you iterate on transformations)


#---transformations03-begin
tr2 = cc.ccGLMatrixd()
tr2.initFromParameters(0.47, (1.5, 2.9, 6.3), (5,3,-2))
#---transformations03-end

m_data2_str = tr2.toString()
m_data2 = tr2.data()
tr2b = cc.ccGLMatrixd.fromString(m_data2_str)
m_data2b = tr2b.data()
for i in range(16):
    if not math.isclose(m_data2[i], m_data2b[i]):
        raise RuntimeError

# --- create a cloud from a primitive object (ellipsoid built from a sphere)

#---transformations04-begin
sphere = cc.ccSphere(1.0)
cloud = sphere.samplePoints(False, 100000)
cloud.scale(1.0, 3.0, 9.0)
#---transformations04-end

boundingBox = cloud.getOwnBB()
if not isCoordEqual(boundingBox.minCorner(), (-1., -3., -9.), 1.e-2):
    raise RuntimeError
if not isCoordEqual(boundingBox.maxCorner(), (1., 3., 9.), 1.e-2):
    raise RuntimeError

# --- rotate the cloud: define the transformation, then apply it

#---transformations05-begin
transform1 = cc.ccGLMatrix()
transform1.initFromParameters(0.25, (1.5, 2.9, 6.3), (0,0,0))
cloud.applyRigidTransformation(transform1)
#---transformations05-end

boundingBox = cloud.getOwnBB()
if not isCoordEqual(boundingBox.minCorner(), (-1.510, -2.944, -8.937), 1.e-2):
    raise RuntimeError
if not isCoordEqual(boundingBox.maxCorner(), (1.510, 2.944, 8.937), 1.e-2):
    raise RuntimeError

# --- get the transformation parameters (rotation angle and vector) 

#---transformations06-begin
matrixParams1 = transform1.getParameters1()

if not math.isclose(matrixParams1.alpha_rad, 0.25, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(matrixParams1.axis3D, (0.211393, 0.408694, 0.887852), 1.e-5):
    raise RuntimeError
#---transformations06-end

# --- get the transformation parameters (the 3 angles phi, psi, theta)

#---transformations07-begin
matrixParams2 = transform1.getParameters2()

if not math.isclose(matrixParams2.phi_rad, 0.225260, rel_tol=1.e-5):
    raise RuntimeError
if not math.isclose(matrixParams2.psi_rad, 0.0639141, rel_tol=1.e-5):
    raise RuntimeError
if not math.isclose(matrixParams2.theta_rad, 0.0954226, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(matrixParams2.t3D, (0., 0., 0.)):
    raise RuntimeError
#---transformations07-end

# --- transformation product, inverse

#---transformations08-begin
x =transform1*transform1

mpx = x.getParameters1()
if not math.isclose(mpx.alpha_rad, 0.5, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(mpx.axis3D, (0.211393, 0.408694, 0.887852), 1.e-5):
    raise RuntimeError
#---transformations08-end

#---transformations09-begin
rotinv = transform1.inverse()

mpi = rotinv.getParameters1()
if not math.isclose(mpi.alpha_rad, 0.25, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(mpi.axis3D, (-0.211393, -0.408694, -0.887852), 1.e-5):
    raise RuntimeError
#---transformations09-end

xi = x*rotinv
mpxi = xi.getParameters1()
if not math.isclose(mpxi.alpha_rad, 0.25, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(mpxi.axis3D, (0.211393, 0.408694, 0.887852), 1.e-5):
    raise RuntimeError

# --- manipulations with angles phi theta and psi

#---transformations10-begin
rotphi = cc.ccGLMatrix()
rotphi.initFromParameters(0.1, (0., 0., 1.), (0,0,0))

a1 = rotphi.getParameters2()
if not math.isclose(a1.phi_rad, 0.1, rel_tol=1.e-7):
    raise RuntimeError

rotpsi = cc.ccGLMatrix()
rotpsi.initFromParameters(0.2, (1., 0., 0.), (0,0,0))

a2 = rotpsi.getParameters2()
if not math.isclose(a2.psi_rad, 0.2, rel_tol=1.e-7):
    raise RuntimeError

rottheta = cc.ccGLMatrix()
rottheta.initFromParameters(0.3, (0., 1., 0.), (0,0,0))

a3 = rottheta.getParameters2()
if not math.isclose(a3.theta_rad, 0.3, rel_tol=1.e-7):
    raise RuntimeError

rotTaitBryan = rotphi*rottheta*rotpsi

a = rotTaitBryan.getParameters2()
if not isCoordEqual((a.phi_rad, a.psi_rad, a.theta_rad), (0.1, 0.2, 0.3), 1.e-7):
    raise RuntimeError
#---transformations10-end

#---transformations11-begin
mat1 = rotTaitBryan.transposed()
p1 = rotTaitBryan.getParameters1()
pt1 = mat1.getParameters1()
if not math.isclose(p1.alpha_rad, pt1.alpha_rad, rel_tol=1.e-7):
    raise RuntimeError
angle1 = p1.axis3D
angle2 = pt1.axis3D
for i in range(3):
    if not math.isclose(angle1[i], - angle2[i], rel_tol=1.e-7):
        raise RuntimeError
#---transformations11-end

#---transformations12-begin
mat = (rotTaitBryan.transposed()).inverse()

# --- access to transformation data from numpy

v= np.array(rotTaitBryan.data()) - np.array(mat.data()) # should be null

d2 = np.inner(v, v)
if d2>1e-12:
    raise RuntimeError
#---transformations12-end

tr1 = cc.ccGLMatrix(rotphi) # copy
tr1+= rotpsi
v= np.array(tr1.data()) -(np.array(rotphi.data()) + np.array(rotpsi.data()))
d2 = np.inner(v, v)
if d2>1e-14:
    raise RuntimeError

tr2 = cc.ccGLMatrix(rotphi) # copy
tr2-= rotpsi
v= np.array(tr2.data()) + np.array(rotpsi.data()) -np.array(rotphi.data())
d2 = np.inner(v, v)
if d2>1e-14:
    raise RuntimeError

# --- interpolation between transformations

#---transformations13-begin
rot1 = cc.ccGLMatrix()
rot1.initFromParameters(math.pi/3., (0., 0., 1.), (0,0,0))
rot2 = cc.ccGLMatrix()
rot2.initFromParameters(2*math.pi/3., (0., 0., 1.), (0,0,0))
rotation = cc.ccGLMatrix.Interpolate(0.5, rot1, rot2)

a = rotation.getParameters1()
if not isCoordEqual(a.axis3D, (0., 0., 1.)):
    raise RuntimeError
if not math.isclose(a.alpha_rad, math.pi/2., rel_tol=1.e-7):
    raise RuntimeError
#---transformations13-end

# --- create a transformation matrix that rotates a vector to another

#---transformations14-begin
rot1 = cc.ccGLMatrix.FromToRotation((1., 0., 0.),(0., 0., 1.))

a = rot1.getParameters1()
if not isCoordEqual(a.axis3D, (0., -1., 0.)):
    raise RuntimeError
if not math.isclose(a.alpha_rad, math.pi/2., rel_tol=1.e-7):
    raise RuntimeError
#---transformations14-end

# --- generate a ‘viewing’ matrix from a looking vector and a ‘up’ direction

#---transformations15-begin
r2 = math.sqrt(2.)
rot1 = cc.ccGLMatrix.FromViewDirAndUpDir((0., r2, r2), (0., -r2, r2))

a = rot1.getParameters1()
if not isCoordEqual(a.axis3D, (-1., 0., 0.)):
    raise RuntimeError
if not math.isclose(a.alpha_rad, 3*math.pi/4., rel_tol=1.e-7):
    raise RuntimeError
#---transformations15-end

# --- angles manipulation

rotx = cc.ccGLMatrix.xRotation(rotTaitBryan)
ax=rotx.getParameters1()
roty = cc.ccGLMatrix.yRotation(rotTaitBryan)
ay=roty.getParameters1()
rotz = cc.ccGLMatrix.zRotation(rotTaitBryan)
az=rotz.getParameters1()
if not isCoordEqual(ax.axis3D, (-1., 0., 0.)):
    raise RuntimeError
if not isCoordEqual(ay.axis3D, (0., 1., 0.)):
    raise RuntimeError
if not isCoordEqual(az.axis3D, (0., 0., 1.)):
    raise RuntimeError
#---transformations-end

# ---delete a C++ entity

#---cloneThis01-begin
cloned = cloud.cloneThis()
#---cloneThis01-end

#---deleteEntity01-begin
cc.deleteEntity(cloned) # delete the cloud copy
cloned = None
#---deleteEntity01-end

# --- build a polyline from a bounding box

sphere = cc.ccSphere(1.0)
cloud = sphere.samplePoints(False, 100000)
cloud.scale(1.0, 3.0, 9.0)
boundingBox = cloud.getOwnBB()

transform1 = cc.ccGLMatrix()
transform1.initFromParameters(0.25, (1.5, 2.9, 6.3), (0,0,0))
cloud.applyRigidTransformation(transform1)

#---coordsBoundinGBox-begin
cmin = boundingBox.minCorner()
cmax = boundingBox.maxCorner()

x = np.float32(np.array((cmin[0], cmax[0], cmax[0], cmin[0], cmin[0], cmax[0], cmax[0], cmin[0])))
y = np.float32(np.array((cmin[1], cmin[1], cmin[1], cmin[1], cmax[1], cmax[1], cmax[1], cmax[1])))
z = np.float32(np.array((cmin[2], cmin[2], cmax[2], cmax[2], cmax[2], cmax[2], cmin[2], cmin[2])))
coords = np.column_stack((x,y,z))
#---coordsBoundinGBox-end

#---polyFromCloud01-begin
cloud1 = cc.ccPointCloud("boundingBox1")
cloud1.coordsFromNPArray_copy(coords)
cloud1.applyRigidTransformation(transform1)

poly1 = cc.ccPolyline(cloud1)
poly1.addChild(cloud1)
poly1.addPointIndex(0, cloud1.size())
poly1.setClosed(True)
#---polyFromCloud01-end

# --- another way do define the polyline 

#---polyFromCloud02-begin
cloud2 = cc.ccPointCloud("boundingBox2")
poly2 = cc.ccPolyline(cloud2)
poly2.addChild(child=cloud2, dependencyFlags = cc.DEPENDENCY_FLAGS.DP_NONE, insertIndex=-1)
cloud2.reserve(cloud1.size())   # fill the cloud with ordered points
for i in range(cloud1.size()):
    cloud2.addPoint(cloud1.getPoint(i))
cloud2.shrinkToFit()
poly2.addPointIndex(0, cloud2.size())
poly2.setClosed(True)
#---polyFromCloud02-end

res = cc.SaveEntities([cloud, poly1, poly2], os.path.join(dataDir, "boundingBoxes.bin"))


