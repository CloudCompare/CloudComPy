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
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init

cloud = cc.loadPointCloud(getSampleCloud(5.0))

tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.1, 0.2, 0.3, (8.0, 0.0, 0.0))
box = cc.ccBox((1., 2., 3.), tr1, "aBox")
if box.getName() != 'aBox':
    raise RuntimeError
if box.size() != 12:
    raise RuntimeError

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.5, (0., 1., 0.), (5.0, 6.0, 3.0))
cone = cc.ccCone(3., 1., 2., 0., 0., tr2, "aCone", 12)
if cone.getName() != 'aCone':
    raise RuntimeError
if cone.size() != 48:
    raise RuntimeError

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0., (0., 0., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 3.0, tr3, 'aCylinder', 48)
if cylinder.getName() != 'aCylinder':
    raise RuntimeError
if cylinder.size() != 192:
    raise RuntimeError

tr4 = cc.ccGLMatrix()
tr4.initFromParameters(0.5, (0., 1., 0.), (-3.0, 0.0, 4.0))
plane = cc.ccPlane(3.0, 4.0, tr4, "aPlane")
if plane.getName() != 'aPlane':
    raise RuntimeError
if plane.size() != 2:
    raise RuntimeError

tr5 = cc.ccGLMatrix()
tr5.initFromParameters(0.0, (0., 0., 0.), (-7.0, 5.0, 1.0))
sphere = cc.ccSphere(1.5, tr5, "aSphere", 72)
if sphere.getName() != 'aSphere':
    raise RuntimeError
if sphere.size() != 10224:
    raise RuntimeError

tr6 = cc.ccGLMatrix()
tr6.initFromParameters(0.2, (1., 2., 0.), (-0.0, -5.0, -9.0))
torus = cc.ccTorus(5.0, 7.0, math.pi, True, 3.0, tr6, "aTorus", 60)
if torus.getName() != 'aTorus':
    raise RuntimeError
if torus.size() != 964:
    raise RuntimeError

tr7 = cc.ccGLMatrix()
tr7.initFromParameters(0.5*math.pi, (1., 0., 0.), (-5.0, -2.0, -2.0))
quadric = cc.ccQuadric((-1., -1.), (1., 1.), (1., 2., 1., 1., 2., 2.), None, tr7, "aQuadric", 60)
if quadric.getName() != 'aQuadric':
    raise RuntimeError
if quadric.size() != 6962:
    raise RuntimeError

tr8 = cc.ccGLMatrix()
tr8.initFromParameters(0.5*math.pi, (0., 1., 0.), (-0.0, -0.0, -4.0))
dish = cc.ccDish(2.0, 1.0, 3.0, tr8, "aDish", 72)
if dish.getName() != 'aDish':
    raise RuntimeError
if dish.size() != 2520:
    raise RuntimeError

stats = cc.DistanceComputationTools.computeApproxCloud2MeshDistance(cloud, cylinder)
print(stats) # min, max, mean, variance, error max

nbCpu = psutil.cpu_count()
bestOctreeLevel = cc.DistanceComputationTools.determineBestOctreeLevel(cloud,cylinder)
params = cc.Cloud2MeshDistancesComputationParams()
params.maxThreadCount = nbCpu
params.octreeLevel = bestOctreeLevel
cc.DistanceComputationTools.computeCloud2MeshDistances(cloud, cylinder, params)

bestOctreeLevel = cc.DistanceComputationTools.determineBestOctreeLevel(cone.getAssociatedCloud(), sphere)
params.octreeLevel = bestOctreeLevel
cc.DistanceComputationTools.computeCloud2MeshDistances(cone.getAssociatedCloud(), sphere, params)

bestOctreeLevel = cc.DistanceComputationTools.determineBestOctreeLevel(dish.getAssociatedCloud(),None, box.getAssociatedCloud())
params2 = cc.Cloud2CloudDistancesComputationParams()
params2.maxThreadCount = nbCpu
params2.octreeLevel = bestOctreeLevel
cc.DistanceComputationTools.computeCloud2CloudDistances(dish.getAssociatedCloud(), box.getAssociatedCloud(), params2)

cc.SaveEntities([cloud, box, cone, cylinder, plane, sphere, torus, quadric, dish], os.path.join(dataDir, "entities2.bin"))

