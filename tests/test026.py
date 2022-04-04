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

# --- a rotated object

sphere = cc.ccSphere(1.0)
cloud = sphere.samplePoints(False, 100000)
cloud.scale(1.0, 3.0, 9.0)
boundingBox = cloud.getOwnBB()

if not isCoordEqual(boundingBox.minCorner(), (-1., -3., -9.), 1.e-2):
    raise RuntimeError
if not isCoordEqual(boundingBox.maxCorner(), (1., 3., 9.), 1.e-2):
    raise RuntimeError

transform1 = cc.ccGLMatrix()
transform1.initFromParameters(0.25, (1.5, 2.9, 6.3), (0,0,0))
cloud.applyRigidTransformation(transform1)
boundingBox = cloud.getOwnBB()

if not isCoordEqual(boundingBox.minCorner(), (-1.510, -2.944, -8.937), 1.e-2):
    raise RuntimeError
if not isCoordEqual(boundingBox.maxCorner(), (1.510, 2.944, 8.937), 1.e-2):
    raise RuntimeError

matrixParams1 = transform1.getParameters1()

if not math.isclose(matrixParams1.alpha_rad, 0.25, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(matrixParams1.axis3D, (0.211393, 0.408694, 0.887852), 1.e-5):
    raise RuntimeError

matrixParams2 = transform1.getParameters2()
if not math.isclose(matrixParams2.phi_rad, 0.225260, rel_tol=1.e-5):
    raise RuntimeError
if not math.isclose(matrixParams2.psi_rad, 0.0639141, rel_tol=1.e-5):
    raise RuntimeError
if not math.isclose(matrixParams2.theta_rad, 0.0954226, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(matrixParams2.t3D, (0., 0., 0.)):
    raise RuntimeError

x =transform1*transform1
mpx = x.getParameters1()
if not math.isclose(mpx.alpha_rad, 0.5, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(mpx.axis3D, (0.211393, 0.408694, 0.887852), 1.e-5):
    raise RuntimeError

rotinv = transform1.inverse()
mpi = rotinv.getParameters1()
if not math.isclose(mpi.alpha_rad, 0.25, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(mpi.axis3D, (-0.211393, -0.408694, -0.887852), 1.e-5):
    raise RuntimeError

xi = x*rotinv
mpxi = xi.getParameters1()
if not math.isclose(mpxi.alpha_rad, 0.25, rel_tol=1.e-5):
    raise RuntimeError
if not isCoordEqual(mpxi.axis3D, (0.211393, 0.408694, 0.887852), 1.e-5):
    raise RuntimeError

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

mat = (rotTaitBryan.transposed()).transposed()
v= np.array(rotTaitBryan.data()) - np.array(mat.data())
d2 = np.inner(v, v)
if d2>1e-14:
    raise RuntimeError

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

rot1 = cc.ccGLMatrix.FromToRotation((1., 0., 0.),(0., 0., 1.))
a = rot1.getParameters1()
if not isCoordEqual(a.axis3D, (0., -1., 0.)):
    raise RuntimeError
if not math.isclose(a.alpha_rad, math.pi/2., rel_tol=1.e-7):
    raise RuntimeError

r2 = math.sqrt(2.)
rot1 = cc.ccGLMatrix.FromViewDirAndUpDir((0., r2, r2), (0., -r2, r2))
a = rot1.getParameters1()
if not isCoordEqual(a.axis3D, (-1., 0., 0.)):
    raise RuntimeError
if not math.isclose(a.alpha_rad, 3*math.pi/4., rel_tol=1.e-7):
    raise RuntimeError

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

# ---delete a C++ entity

cl = cloud.cloneThis()
cc.deleteEntity(cl) # delete the cloud copy
cl = None

# --- build a polyline from a bounding box

sphere = cc.ccSphere(1.0)
cloud = sphere.samplePoints(False, 100000)
cloud.scale(1.0, 3.0, 9.0)
boundingBox = cloud.getOwnBB()

transform1 = cc.ccGLMatrix()
transform1.initFromParameters(0.25, (1.5, 2.9, 6.3), (0,0,0))
cloud.applyRigidTransformation(transform1)

cmin = boundingBox.minCorner()
cmax = boundingBox.maxCorner()

x = np.float32(np.array((cmin[0], cmax[0], cmax[0], cmin[0], cmin[0], cmax[0], cmax[0], cmin[0])))
y = np.float32(np.array((cmin[1], cmin[1], cmin[1], cmin[1], cmax[1], cmax[1], cmax[1], cmax[1])))
z = np.float32(np.array((cmin[2], cmin[2], cmax[2], cmax[2], cmax[2], cmax[2], cmin[2], cmin[2])))
coords = np.column_stack((x,y,z))

cloud1 = cc.ccPointCloud("boundingBox1")
cloud1.coordsFromNPArray_copy(coords)
cloud1.applyRigidTransformation(transform1)

poly1 = cc.ccPolyline(cloud1)
poly1.addChild(cloud1)
poly1.addPointIndex(0, cloud1.size())
poly1.setClosed(True)

# --- another way do define the polyline 

cloud2 = cc.ccPointCloud("boundingBox2")
poly2 = cc.ccPolyline(cloud2)
poly2.addChild(child=cloud2, dependencyFlags = cc.DEPENDENCY_FLAGS.DP_NONE, insertIndex=-1)
cloud2.reserve(8)
for i in range(8):
    cloud2.addPoint(cloud1.getPoint(i))
cloud2.shrinkToFit()
poly2.addPointIndex(0, cloud2.size())
poly2.setClosed(True)
res = cc.SaveEntities([cloud, poly1, poly2], os.path.join(dataDir, "boundingBoxes.bin"))


