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

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual
import cloudComPy as cc

#---triangleEdges01-begin
sphere = cc.ccSphere(radius=2, precision=128)
area = sphere.computeMeshArea()
res = sphere.computeMeshVolume()
if res[1]:                # there are edges not shared or shared by more than 2 triangles
    raise RuntimeError
vol = res[0]
sphereArea = 16*math.pi            # geometric values 
sphereVolume = (4/3.)*math.pi*8
if not math.isclose(area, sphereArea, rel_tol=1.e-3):
    raise RuntimeError
if not math.isclose(vol, sphereVolume, rel_tol=1.e-3):
    raise RuntimeError
#---triangleEdges01-end
stats = res[2]
if stats.edgesCount != 48768:
    raise runtimeError

#---triangleEdges02-begin
dish = cc.ccDish(radius=2, height=1, precision=128)
area = dish.computeMeshArea()
res = dish.computeMeshVolume()
if not res[1]:                # there should be edges not shared
    raise RuntimeError
stats = res[2]
if stats.edgesNotShared != 128:
    raise runtimeError
#---triangleEdges02-end

dish.flagVerticesByType()
cloud = dish.getAssociatedCloud()
nsf = cloud.getNumberOfScalarFields()
sfc = cloud.getScalarField(nsf - 1)
cloud.setCurrentOutScalarField(nsf - 1)
fcloud = cc.filterBySFValue(0.01, sfc.getMax(), cloud)

poly = cc.ccPolyline(fcloud)
poly.addChild(fcloud)
poly.addPointIndex(0, fcloud.size())
poly.setClosed(True)

cloud2 = cloud.cloneThis()
#---normals01-begin
cloud2.shiftPointsAlongNormals(0.5)
#---normals01-end
mesh2 = cc.ccMesh.triangulate(cloud2, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED, dim=2)
mesh2.flagVerticesByType()
nsf2 = cloud2.getNumberOfScalarFields()
sfc2 = cloud2.getScalarField(nsf2 - 1)
cloud2.setCurrentOutScalarField(nsf2 - 1)
fcloud2 = cc.filterBySFValue(0.01, sfc2.getMax(), cloud2)

poly2 = cc.ccPolyline(fcloud2)
poly2.addChild(fcloud2)
poly2.addPointIndex(0, fcloud2.size())
poly2.setClosed(True)
poly2.samplePoints(False, 1000)

#---meshTriangles01-begin
dish.flipTriangles()
#---meshTriangles01-end

cc.SaveEntities([sphere, dish, cloud2, mesh2, fcloud, fcloud2, poly, poly2], os.path.join(dataDir, "entities3.bin"))


