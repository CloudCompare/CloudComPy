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

#---triangulate01-begin
cloud1 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
cloud1.setName("cloud1")

mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED, dim=2)
mesh1.setName("mesh1")
#---triangulate01-end

if cloud1.size() != 10000:
    raise RuntimeError
if not math.isclose(mesh1.size(), 19602, rel_tol=5e-02):
    raise RuntimeError

#---cloneMesh01-begin
mesh2 = mesh1.cloneMesh()
if mesh2.getName() != "mesh1.clone":
    raise RuntimeError
#---cloneMesh01-end

#---subdivide01-begin
mesh3 = mesh2.subdivide(0.001)
mesh3.setName("mesh3")
#---subdivide01-end
if not math.isclose(mesh3.size(), 335696, rel_tol=5e-02):
    raise RuntimeError

#---laplacianSmooth01-begin
mesh3.laplacianSmooth(nbIteration=20, factor=0.2)
#---laplacianSmooth01-end

#---triangleVertices01-begin
# --- access to triangle nodes, per triangle indice
cloud = mesh1.getAssociatedCloud()
indexes = mesh1.getTriangleVertIndexes(453)
p0 = cloud.getPoint(indexes[0])
p1 = cloud.getPoint(indexes[1])
p2 = cloud.getPoint(indexes[2])
#---triangleVertices01-end

#---triangleIndexes01-begin
# --- access to the numpy array of node indexes (one row per triangle)
d = mesh1.IndexesToNpArray()
if d.shape != (19602, 3):
    raise RuntimeError
if d.dtype != np.dtype('uint32'):
    raise RuntimeError

d2 = mesh1.IndexesToNpArray_copy()
if d2.shape != (19602, 3):
    raise RuntimeError
if d2.dtype != np.dtype('uint32'):
    raise RuntimeError
#---triangleIndexes01-end

#---triangulate02-begin
cloud2 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
cloud2.setName("cloud2")
tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.5, (0.7, 0.7, 0.7), (0.,0.,0.))
cloud2.applyRigidTransformation(tr1)

mesh4 = cc.ccMesh.triangulate(cloud2, cc.TRIANGULATION_TYPES.DELAUNAY_2D_BEST_LS_PLANE)
mesh4.setName("mesh4")
#---triangulate02-end

cc.SaveEntities([cloud1, cloud2, mesh1, mesh2, mesh3, mesh4], os.path.join(dataDir, "clouds1.bin"))

#---deleteEntity02-begin
cc.deleteEntity(mesh3)
mesh3=None
#---deleteEntity02-end


