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
octree = cloud.getOctree() 
if octree is not None:
    raise RuntimeError
octree = cloud.computeOctree()

bb = octree.getBoundingBox()
if not isCoordEqual(bb[0], (-5.004994869232178, -5.004994869232178, -3.043078899383545)):
    raise RuntimeError
if not isCoordEqual(bb[1], (4.994994640350342, 4.994994640350342, 6.956910610198975)):
    raise RuntimeError
nb = octree.getNumberOfProjectedPoints()
if nb != 1000000:
    raise RuntimeError
