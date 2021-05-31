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

# --- search points in neighbourhood, within a given radius r

r=0.05
level = octree.findBestLevelForAGivenNeighbourhoodSizeExtraction(r)
level3 = octree.findBestLevelForAGivenPopulationPerCell(3)

# --- search in a sphere

neighbours = octree.getPointsInSphericalNeighbourhood((-1.500000, 2.000000, -0.026529), r, level)
if len(neighbours) != 37:
    raise RuntimeError

# --- search in a cylinder

params = cc.CylindricalNeighbourhood()
params.center = (-1.5, 2.0, 0.0)
params.level = level
params.radius = r
params.maxHalfLength = 1

neighboursCyl = octree.getPointsInCylindricalNeighbourhood(params)
if len(neighboursCyl) != 81:
    raise RuntimeError

# --- search in a box

params = cc.BoxNeighbourhood()
params.center=(-1.500000, 2.000000, -0.026529)
params.level = level
params.dimensions = (0.05, 0.04, 0.03)

neighboursBox = octree.getPointsInBoxNeighbourhood(params)
if len(neighboursBox) != 9:
    raise RuntimeError

# --- get information on some points

pt = neighbours[0].point
id = neighbours[0].pointIndex
sd = neighbours[0].squareDistd
if (math.sqrt(sd) > r):
    raise RuntimeError

# --- exploring the octree:
#     the octree is a tridimensionnal matrix of cubical cells
#     level 0 is one box, at each successive level, cells are divided by 2 in each direction. 
#     maximum level is 21.

# --- octree min and max coordinates, bounding box of the cloud

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

# --- indexes of cells at a given level of the octree: min an max values
#     a cell at a given level has 3 integer coordinates (CellPos)

if octree.getMinFillIndexes(2) != [0, 0, 0] or octree.getMaxFillIndexes(2) != [3, 3, 3]:
    raise RuntimeError

minIndexes = octree.getMinFillIndexes(level)
maxIndexes = octree.getMaxFillIndexes(level)
if minIndexes != [2, 2, 101] or maxIndexes != [509, 509, 410]:
    raise RuntimeError

# --- cell size in the octree at a given level

cellSize0 = octree.getCellSize(0)
if not math.isclose(cellSize0 , 10.09, rel_tol=1.e-4):
    raise RuntimeError

cellSize9 = octree.getCellSize(9)
if not math.isclose(cellSize9 , cellSize0/2**9):
    raise RuntimeError

# --- find the cell coordinates containing a point
#     a cell at a given level has 3 integer coordinates (CellPos)

pt = neighbours[5].point       # point coordinates
id = neighbours[5].pointIndex  # index of point in the cloud
cellPosMax = octree.getTheCellPosWhichIncludesThePoint(pt)     # level max=21
cellPos = octree.getTheCellPosWhichIncludesThePoint(pt, level)

# --- idem with a check if the point is inside the BoundingBox or not

res= octree.getTheCellPosWhichIncludesThePointInbBounds((0,0,0),9)
if not res[1]:
    raise RuntimeError

res= octree.getTheCellPosWhichIncludesThePointInbBounds((0,0,10),9)
if res[1]:
    raise RuntimeError

# --- CellCode is a unique id of the cell generated with CellPos and level

cellCodeT = octree.GenerateTruncatedCellCode(cellPos, level)
#cellCode =octree.getCellCode(id) # do not work here!
cellCode = octree.GenerateTruncatedCellCode(cellPosMax, 21) # 21 = level max (64bits)

# --- get points in a given cell, at a given level, using CellCode
#     the ReferenceCloud receive the points found

rc = cc.ReferenceCloud(cloud)
octree.getPointsInCell(cellCodeT, level, rc, True, True)
if rc.size() < 1:
    raise RuntimeError
found = False
for i in range(rc.size()):
    if rc.getCurrentPointGlobalIndex() == id:
        found = True
        break
if not found:
    raise RuntimeError

# --- get points in a given cell, at a max level, using CellCode

octree.getPointsInCell(cellCode, level, rc)
if rc.size() < 1:
    raise RuntimeError
found = False
for i in range(rc.size()):
    if rc.getCurrentPointGlobalIndex() == id:
        found = True
        break
if not found:
    raise RuntimeError

# --- iterate through the nodes and cells at a given level
#     WARNING, the methods getCellCodes, getCellIndexes, getCellCodesAndIndexes
#     return heavy vectors by copy (TODO: try with numpy, wihout copy)

codes = octree.getCellCodes(5, True)
indexes = octree.getCellIndexes(5)
if len(indexes) != 2223: 
    raise RuntimeError
if len(indexes) != len(codes):
    raise RuntimeError
codesAndIndexes=octree.getCellCodesAndIndexes(5, True)
if len(codesAndIndexes) != len(codes):
    raise RuntimeError
cai1=codesAndIndexes[1000]
cai2=codesAndIndexes[1001]
if cai2 < cai1:
    raise RuntimeError
if cai1.theCode != codes[1000]:
    raise RuntimeError
if cai1.theIndex != indexes[1000]:
    raise RuntimeError

# --- Each cell at a given level of subdivision can be found by the index of a the first point inside the cell
#     See getCellIndexes

currentIndex = 0
for i in range(len(indexes)):
    if indexes[i] != currentIndex:
        raise RuntimeError
    octree.getPointsInCellByCellIndex(rc, indexes[i], 5)
    # do something with the points (see ReferenceCloud methods, below)
    currentIndex += rc.size()

# --- find a given number of points in neighbourhood (not optimized for repetitive calls)

point = (2.81, -0.24, 0.354)
rc = cc.ReferenceCloud(cloud) # start with an empty ReferenceCloud
res = octree.findPointNeighbourhood(point, rc, 1000, 7)
if rc.size() != 1000:
    raise RuntimeError
if rc.size() != res[0]:
    raise RuntimeError
maxdist2 = res[2]
print("maxdist2", maxdist2)

# --- ReferenceCloud methods

rc.placeIteratorAtBeginning()
rc.enableScalarField()
if not rc.isScalarFieldEnabled():
    raise RuntimeError
dmax1 = 0
for i in range(rc.size()):
    p = rc.getPoint(i)
    d = math.sqrt((p[0] - point[0])**2 + (p[1] - point[1])**2 + (p[2] - point[2])**2)
    if d > dmax1:
        dmax1 =d
    #print(i, d)
    rc.setPointScalarValue(i,d)
print("dmax1", dmax1)
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "resoctree.bin"))

#--- the same with a maximum distance
 
rc = cc.ReferenceCloud(cloud) # start with an empty ReferenceCloud
res = octree.findPointNeighbourhood(point, rc, 1000, 7, dmax1/2.)
if rc.size() >= 1000:
    raise RuntimeError
if rc.size() != res[0]:
    raise RuntimeError
maxdist2 = res[2]
print("maxdist2", maxdist2)

rc.placeIteratorAtBeginning()
rc.enableScalarField()
dmax2 = 0
for i in range(rc.size()):
    p = rc.getPoint(i)
    d = math.sqrt((p[0] - point[0])**2 + (p[1] - point[1])**2 + (p[2] - point[2])**2)
    if d > dmax2:
        dmax2 = d
    #print(i, d)
    rc.setPointScalarValue(i,d)
print("dmax2", dmax2)

# --- other ReferenceCloud methods

cloud2 = rc.getAssociatedCloud()
 
bb = rc.getBoundingBox()
 
rc.getCurrentPointCoordinates()
rc.getCurrentPointGlobalIndex()
rc.getCurrentPointScalarValue()
rc.placeIteratorAtBeginning()
rc.getNextPoint()
rc.getNextPoint()
rc.getPointGlobalIndex(3)
rc.getCurrentPointGlobalIndex()
rc.setCurrentPointScalarValue(0.24)

