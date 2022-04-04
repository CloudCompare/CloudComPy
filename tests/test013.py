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

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

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
