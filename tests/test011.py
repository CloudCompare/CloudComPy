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
from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud1 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
cloud1.setName("cloud1")
if cloud1.size() != 10000:
    raise RuntimeError

mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh1.setName("mesh1")
if not math.isclose(mesh1.size(), 19602, rel_tol=5e-02):
    raise RuntimeError

mesh2 = mesh1.cloneMesh()
if mesh2.getName() != "mesh1.clone":
    raise RuntimeError

mesh3 = mesh2.subdivide(0.001)
if not math.isclose(mesh3.size(), 335696, rel_tol=5e-02):
    raise RuntimeError

mesh3.laplacianSmooth()

cc.SaveEntities([cloud1, mesh1, mesh2, mesh3], os.path.join(dataDir, "clouds1.bin"))




