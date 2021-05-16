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
from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))
octree = cloud.computeOctree()
r=0.05
level = octree.findBestLevelForAGivenNeighbourhoodSizeExtraction(r)

neighbours = octree.getPointsInSphericalNeighbourhood((-1.500000, 2.000000, -0.026529), r, level)
if len(neighbours) != 37:
    raise RuntimeError

params = cc.CylindricalNeighbourhood_py()
params.center = (-1.5, 2.0, 0.0)
params.level = level
params.radius = r
params.maxHalfLength = 1

neighboursCyl = octree.getPointsInCylindricalNeighbourhood(params)
if len(neighboursCyl) != 81:
    raise RuntimeError

params = cc.BoxNeighbourhood_py()
params.center=(-1.500000, 2.000000, -0.026529)
params.level = level
params.dimensions = (0.05, 0.04, 0.03)

neighboursBox = octree.getPointsInBoxNeighbourhood(params)
if len(neighboursBox) != 9:
    raise RuntimeError

