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
cc.computeNormals([cloud])

cloud.exportNormalToSF(True, True, True)
sf=cloud.getScalarField(2)
if sf.getName() != 'Nz':
    raise RuntimeError
sfmin = sf.getMin()
sfmax = sf.getMax()
if not math.isclose(sfmin, 0.06670579, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(sfmax, 0.9999990, rel_tol=1e-06):
    raise RuntimeError
meanvar=sf.computeMeanAndVariance()
if not math.isclose(meanvar[0], 0.74157232, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(meanvar[1], 0.04167303, rel_tol=1e-06):
    raise RuntimeError


cloud1 = cc.loadPointCloud(getSampleCloud2(3.0,0, 0.1))
cloud1.setName("cloud1")
plane = cc.ccPlane.Fit(cloud1)
mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh1.setName("mesh1")
cc.computeNormals([mesh1])
res = cc.SaveEntities([cloud, cloud1, mesh1], os.path.join(dataDir, "cloudmesh.bin"))
if res != cc.CC_FILE_ERROR.CC_FERR_NO_ERROR:
    raise RuntimeError

