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

cloud1 = cc.loadPointCloud(getSampleCloud2(3.0,0, 0.1))
cloud1.setName("cloud1")

mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh1.setName("mesh1")

cloud2=mesh1.samplePoints(True, 50)
cloud2.setName("cloud2")
if not math.isclose(cloud2.size(), 6489, rel_tol=0.05):
    raise RuntimeError

cloud3=mesh1.samplePoints(True, 500)
cloud3.setName("cloud3")
if not math.isclose(cloud3.size(), 64641, rel_tol=0.05):
    raise RuntimeError

cloud4=mesh1.samplePoints(False, 1000)
cloud4.setName("cloud4")
if not math.isclose(cloud4.size(), 1000, rel_tol=0.05):
    raise RuntimeError

cloud5=mesh1.samplePoints(False, 100000)
cloud5.setName("cloud5")
if not math.isclose(cloud5.size(), 100000, rel_tol=0.05):
    raise RuntimeError

cc.SaveEntities([cloud1, mesh1, cloud2, cloud3, cloud4, cloud5], os.path.join(dataDir, "samplemesh.bin"))

