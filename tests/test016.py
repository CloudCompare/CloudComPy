#!/usr/bin/env python3

##########################################################################
#                                                                        #
#                                PYCC                                    #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU Library General Public License as       #
#  published by the Free Software Foundation; version 2 or later of the  #
#  License.                                                              #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
#                                                                        #
##########################################################################

import os
import sys
import math
import numpy as np

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))
octree = cloud.computeOctree()
if (octree.getNumberOfProjectedPoints() != 1000000):
    raise RuntimeError

r=0.05
level = octree.findBestLevelForAGivenNeighbourhoodSizeExtraction(r)
level3 = octree.findBestLevelForAGivenPopulationPerCell(3)

neighbours = octree.getPointsInSphericalNeighbourhood((-1.500000, 2.000000, -0.026529), r, level)
if len(neighbours) != 37:
    raise RuntimeError

params = cc.CylindricalNeighbourhood()
params.center = (-1.5, 2.0, 0.0)
params.level = level
params.radius = r
params.maxHalfLength = 1

neighboursCyl = octree.getPointsInCylindricalNeighbourhood(params)
if len(neighboursCyl) != 81:
    raise RuntimeError

params = cc.BoxNeighbourhood()
params.center=(-1.500000, 2.000000, -0.026529)
params.level = level
params.dimensions = (0.05, 0.04, 0.03)

neighboursBox = octree.getPointsInBoxNeighbourhood(params)
if len(neighboursBox) != 9:
    raise RuntimeError

pt = neighbours[0].point
id = neighbours[0].pointIndex
sd = neighbours[0].squareDistd
if (math.sqrt(sd) > r):
    raise RuntimeError

mins = octree.getOctreeMins()
maxs = octree.getOctreeMaxs()
dmin = np.array([-5.05, -5.05, -3.09]) - mins
dmax = np.array([ 5.04,  5.04,  7.00]) - maxs
if np.dot(dmin,dmin) > 1.e-5 or np.dot(dmax,dmax) > 1.e-5:
    raise RuntimeError

bbox= octree.getBoundingBox()
dmin = np.array([-5.05, -5.05, -3.09]) - bbox[0]
dmax = np.array([ 5.04,  5.04,  7.00]) - bbox[1]
if np.dot(dmin,dmin) > 1.e-5 or np.dot(dmax,dmax) > 1.e-5:
    raise RuntimeError

if octree.getMinFillIndexes(2) != [0, 0, 0] or octree.getMaxFillIndexes(2) != [3, 3, 3]:
    raise RuntimeError

minIndexes = octree.getMinFillIndexes(level)
maxIndexes = octree.getMaxFillIndexes(level)
if minIndexes != [2, 2, 101] or maxIndexes != [509, 509, 410]:
    raise RuntimeError

cellSize0 = octree.getCellSize(0)
if not math.isclose(cellSize0 , 10.09, rel_tol=1.e-4):
    raise RuntimeError

cellSize9 = octree.getCellSize(9)
if not math.isclose(cellSize9 , cellSize0/2**9):
    raise RuntimeError

pt = neighbours[0].point
id = neighbours[0].pointIndex
cellPosMax = octree.getTheCellPosWhichIncludesThePoint(pt)
cellPos = octree.getTheCellPosWhichIncludesThePoint(pt, level)

cellCodeT = octree.GenerateTruncatedCellCode(cellPos, level)
rc = cc.ReferenceCloud(cloud)
rc2 = octree.getPointsInCell(cellCodeT, level, rc, True, True)
if rc2.size() != 1:
    raise RuntimeError
if rc2.getCurrentPointGlobalIndex() != id:
    raise RuntimeError

#cellCode =octree.getCellCode(id) # do not work here!
cellCode = octree.GenerateTruncatedCellCode(cellPosMax, 21) # 21 = level max (64bits)
rc3 = octree.getPointsInCell(cellCode, level, rc)
if rc3.size() != 1:
    raise RuntimeError
if rc3.getCurrentPointGlobalIndex() != id:
    raise RuntimeError

rc4=octree.getPointsInCellByCellIndex(rc2, id, 7)
if rc4.size() != 36:
    raise RuntimeError

point = (2.81, -0.24, 0.354)
rc = cc.ReferenceCloud(cloud)
res = octree.findPointNeighbourhood(point, rc, 100, 7)
rc = res[0]
if rc.size() != res[1]:
    raise RuntimeError
maxdist = res[3]
if not math.isclose(maxdist, 0.0250912908):
    raise RuntimeError


cloud2 = rc.getAssociatedCloud()
rc.enableScalarField()
if not rc.isScalarFieldEnabled():
    raise RuntimeError

bb = rc.getBoundingBox()

rc.getCurrentPointCoordinates()
rc.getCurrentPointGlobalIndex()
rc.getCurrentPointScalarValue()
rc.placeIteratorAtBeginning()
rc.getNextPoint()
rc.getNextPoint()
rc.getPoint(5)
rc.setPointScalarValue(3,1.23)
rc.getPointGlobalIndex(3)
rc.getCurrentPointGlobalIndex()
rc.setCurrentPointScalarValue(0.24)

pt = rc.getPoint(50)
cellPos = octree.getTheCellPosWhichIncludesThePoint(pt, level)
cellCode =octree.GenerateTruncatedCellCode(cellPos, level)

res= octree.getTheCellPosWhichIncludesThePointInbBounds((0,0,0),9)
if not res[1]:
    raise RuntimeError

res= octree.getTheCellPosWhichIncludesThePointInbBounds((0,0,10),9)
if res[1]:
    raise RuntimeError










