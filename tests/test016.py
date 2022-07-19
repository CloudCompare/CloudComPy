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

#---octree01-begin
# --- get a sample cloud, build the octree

cloud = cc.loadPointCloud(getSampleCloud(5.0))
octree = cloud.computeOctree(progressCb=None, autoAddChild=True)
if (octree.getNumberOfProjectedPoints() != 1000000):
    raise RuntimeError

# --- search points in neighbourhood, within a given radius r

r=0.05
level = octree.findBestLevelForAGivenNeighbourhoodSizeExtraction(r)
level12 = octree.findBestLevelForAGivenPopulationPerCell(12)
#---octree01-end

#---octree02-begin
# --- search in a sphere

neighbours = octree.getPointsInSphericalNeighbourhood((-1.5, 2.0, -0.026529), r, level)
if len(neighbours) != 37:
    raise RuntimeError
#---octree02-end

#---octree03-begin
# --- search in a cylinder

params = cc.CylindricalNeighbourhood()
params.center = (-1.5, 2.0, 0.0)
params.level = level
params.radius = r
params.maxHalfLength = 1

neighboursCyl = octree.getPointsInCylindricalNeighbourhood(params)
if len(neighboursCyl) != 81:
    raise RuntimeError
#---octree03-end

#---octree04-begin
# --- search in a box

params = cc.BoxNeighbourhood()
params.center=(-1.500000, 2.000000, -0.026529)
params.level = level
params.dimensions = (0.05, 0.04, 0.03)

neighboursBox = octree.getPointsInBoxNeighbourhood(params)
if len(neighboursBox) != 9:
    raise RuntimeError
#---octree04-end

#---octree05-begin
# --- get information on some points

pt = neighbours[0].point
id = neighbours[0].pointIndex
sd = neighbours[0].squareDistd
if (math.sqrt(sd) > r):
    raise RuntimeError
#---octree05-end

# --- exploring the octree:
#     the octree is a tridimensionnal matrix of cubical cells
#     level 0 is one box, at each successive level, cells are divided by 2 in each direction. 
#     maximum level is 21.

# --- octree min and max coordinates, bounding box of the cloud

mins = octree.getOctreeMins()
maxs = octree.getOctreeMaxs()
dmin = np.array([-5.00499, -5.00499, -3.04308]) - mins
dmax = np.array([ 4.99499,  4.99499,  6.95691]) - maxs
if np.dot(dmin,dmin) > 1.e-5 or np.dot(dmax,dmax) > 1.e-5:
    raise RuntimeError

bbox= octree.getBoundingBox()
dmin = np.array([-5.00499, -5.00499, -3.04308]) - bbox[0]
dmax = np.array([ 4.99499,  4.99499,  6.95691]) - bbox[1]
if np.dot(dmin,dmin) > 1.e-5 or np.dot(dmax,dmax) > 1.e-5:
    raise RuntimeError

# --- indexes of cells at a given level of the octree: min an max values
#     a cell at a given level has 3 integer coordinates (CellPos)

if octree.getMinFillIndexes(2) != [0, 0, 0] or octree.getMaxFillIndexes(2) != [3, 3, 3]:
    raise RuntimeError

minIndexes = octree.getMinFillIndexes(level)
maxIndexes = octree.getMaxFillIndexes(level)
if minIndexes != [0, 0, 100] or maxIndexes != [511, 511, 411]:
    raise RuntimeError

# --- number of cells at a given level

nbCells = octree.getCellNumber(level)
if nbCells != 429425:
    raise RuntimeError

# --- cell size in the octree at a given level

cellSize0 = octree.getCellSize(0)
if not math.isclose(cellSize0 , 10.000, rel_tol=1.e-4):
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

# --- cell distance from border cells

cellDist1 = octree.getCellDistanceFromBorders(cellPos,level)
if (cellDist1[0] + cellDist1[1]) != (maxIndexes[0] - minIndexes[0]): # distances along x
    raise RuntimeError
if (cellDist1[3] + cellDist1[2]) != (maxIndexes[1] - minIndexes[1]): # distances along y
    raise RuntimeError
if (cellDist1[5] + cellDist1[4]) != (maxIndexes[2] - minIndexes[2]): # distances along z
    raise RuntimeError

cellDist2 = octree.getCellDistanceFromBorders(cellPos,level, 200)   # limit to a radius of 200 cells
for i in range(6):
    if cellDist2[i] > 200:
        raise RuntimeError

# --- CellCode is a unique id of the cell generated with CellPos and level

cellCodeT = octree.GenerateTruncatedCellCode(cellPos, level)
#cellCode =octree.getCellCode(id) # do not work this way, see getCellCodes below
cellCode = octree.GenerateTruncatedCellCode(cellPosMax, 21) # 21 = level max (64bits)

if octree.getCellPos(cellCodeT, level, True) != cellPos:
    raise RuntimeError

# --- cell center, from CellCode or CellPos

center1 = octree.computeCellCenter(cellCodeT, level, isCodeTruncated=True)
center2 = octree.computeCellCenter(cellPos, level)
if center1 != center2:
    raise RuntimeError

# --- get points in a given cell, at a given level, using CellCode
#     the ReferenceCloud receive the points found

rc = cc.ReferenceCloud(cloud)
octree.getPointsInCell(cellCodeT, level, subset=rc, isCodeTruncated=True, clearOutputCloud=True)
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

level2 = octree.findBestLevelForAGivenCellNumber(5000)
if level2 != 5:
    raise RuntimeError
codes = octree.getCellCodes(level=5, truncatedCodes=True)
indexes = octree.getCellIndexes(5)
if len(indexes) != 2262: 
    raise RuntimeError
if len(indexes) != len(codes):
    raise RuntimeError
codesAndIndexes=octree.getCellCodesAndIndexes(level=5, truncatedCodes=True)
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
    octree.getPointsInCellByCellIndex(rc, indexes[i], level=5)
    # do something with the points (see ReferenceCloud methods, below)
    currentIndex += rc.size()

# --- find a given number of points in neighbourhood (not optimized for repetitive calls)

point = (2.81, -0.24, 0.354)
rc = cc.ReferenceCloud(cloud) # start with an empty ReferenceCloud
res = octree.findPointNeighbourhood(point, rc, maxNumberOfNeighbors=1000, level=7)
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
res = octree.findPointNeighbourhood(point, rc, maxNumberOfNeighbors=1000, level=7, maxSearchDist=dmax1/2.)
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

# TODO: test coverage incomplete:
# findNearestNeighborsStartingFromCell
# findNeighborsInASphereStartingFromCell
# findTheNearestNeighborStartingFromCell
# getCellCode
# getPointsInCellsWithSortedCellCodes
# getPointsInCylindricalNeighbourhoodProgressive

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

