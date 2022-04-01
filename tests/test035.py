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

os.environ["_CCTRACE_"]="ON"

from gendata import dataDir
import cloudComPy as cc

tr0 = cc.ccGLMatrix()
tr0.initFromParameters(math.pi/3., (0., 1., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 2.0, tr0)
c0 = cylinder.samplePoints(True, 1000)

tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.0, (0., 0., 0.), (-2.0, 5.0, 1.0))
sphere1 = cc.ccSphere(1.5, tr1)
c1 = sphere1.samplePoints(True, 1000)

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.0, (0., 0., 0.), (6.0, -3.0, -2.0))
sphere2 = cc.ccSphere(2.0, tr2)
c2 = sphere2.samplePoints(True, 1000)

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0.0, (0., 0., 0.), (-0.0, 1.0, 2.0))
sphere3 = cc.ccSphere(1.0, tr3)
c3 = sphere3.samplePoints(True, 1000)

cloud = c0.cloneThis()
cloud.fuse(c1)
cloud.fuse(c2)
cloud.fuse(c3)

import cloudComPy.RANSAC_SD
params = cc.RANSAC_SD.RansacParams()
params.optimizeForCloud(cloud)
print(params.epsilon, params.bitmapEpsilon)
meshes, clouds = cc.RANSAC_SD.computeRANSAC_SD(cloud, params)

if not meshes[0].isA(cc.CC_TYPES.SPHERE):
    raise RuntimeError
if not meshes[1].isA(cc.CC_TYPES.SPHERE):
    raise RuntimeError
if not meshes[2].isA(cc.CC_TYPES.SPHERE):
    raise RuntimeError
if not meshes[3].isA(cc.CC_TYPES.CYLINDER):
    raise RuntimeError
if not meshes[4].isA(cc.CC_TYPES.PLANE):
    raise RuntimeError
if not meshes[5].isA(cc.CC_TYPES.PLANE):
    raise RuntimeError
if not math.isclose(meshes[0].getRadius(), 2.0, rel_tol=1.e-2):
    raise RuntimeError
if not math.isclose(meshes[1].getRadius(), 1.5, rel_tol=1.e-2):
    raise RuntimeError
if not math.isclose(meshes[2].getRadius(), 1.0, rel_tol=1.e-2):
    raise RuntimeError


shapes = [cylinder, sphere1, sphere2, sphere3, cloud]
for mesh in meshes:
    if mesh is not None:
        print("mesh:", mesh.getName())
        shapes.append(mesh)
for cl in clouds:
    if cl is not None:
        print("cloud:", cl.getName())
        shapes.append(cl)

cc.SaveEntities(shapes, os.path.join(dataDir, "ransac.bin"))


