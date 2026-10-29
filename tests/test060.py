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
#          Copyright 2020-2025 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

import os
import sys
import math
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

import cloudComPy as cc
from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual

cyls = []

tr0 = cc.ccGLMatrix()
tr0.initFromParameters(math.pi/2., (1., 0., 0.), (0.0, 0.0, 0.0))
cyl = cc.ccCylinder(0.5, 3.0, tr0, 'axe', 48)
cyls.append(cyl)

for i in range(8):
    tr0 = cc.ccGLMatrix()
    tr0.initFromParameters(0., (0., 0., 0.), (0.0, 0.0, 2.0))
    cyl = cc.ccCylinder(0.5, 3.0, tr0, 'Cylinder%d'%i, 48)
    tr1 = cc.ccGLMatrix()
    tr1.initFromParameters(i*math.pi/4., (0., 1., 0.), (0.0, 0.0, 0.0))
    cyl.applyRigidTransformation(tr1)
    cyls.append(cyl)

mesh = cc.MergeEntities(cyls, createSFcloudIndex=True, createSubMeshes=True)
cyls.append(mesh)

cc.SaveEntities(cyls, os.path.join(dataDir, "cylinders.bin"))

res=cc.importFile(os.path.join(dataDir, "cylinders.bin"))
print(res)
mesh=res[0][0]
names=res[4]
print(len(names))
#---subMesh01-begin
nbChildren=mesh.getChildrenNumber()
subMeshes=[]
for i in range(nbChildren):
    child = mesh.getChild(i)
    if child.isA(cc.CC_TYPES.SUB_MESH):
        subMeshes.append(mesh.getChild(i))
#---subMesh01-end

#---subMesh02-begin
for i in range(len(subMeshes)):
    child = subMeshes[i]
    print("child", i , child.getName())
    print("  size:", child.size())
    print("  cloud.size:", child.getAssociatedCloud().size())
    print("  ", child.getAssociatedCloud().getName())
    print("  first triangle vertex indexes:", child.getTriangleVertIndexes(0))
    #---subMesh02-end
    if child.size() != 192:
        raise RuntimeError
    if child.getAssociatedCloud().size() != 882:
        raise RuntimeError


