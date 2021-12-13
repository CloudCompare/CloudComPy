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

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

# --- generate a set of coords and a scalar field

npts = 1000000
h = 2.
x = np.float32(-5. + 10.*np.random.random((npts)))
y = np.float32(-5. + 10.*np.random.random((npts)))
z = np.float32(np.sin(h * np.sqrt(x**2 + y**2)) / np.sqrt(x**2 + y**2))
coords = np.column_stack((x,y,z))

# --- create the pointCloud, add the scalar field, save

cloud = cc.ccPointCloud("wave_%d"%h)
cloud.coordsFromNPArray_copy(coords)
res = cloud.exportCoordToSF(False, False, True)

rcloud = cc.RasterizeToCloud(cloud, 0.01)

rmesh = cc.RasterizeToMesh(cloud, 0.03)

rcloud2 = cc.RasterizeToCloud(cloud, 0.01, cc.CC_DIRECTION.Z, True, True, False, dataDir,
                              False, cc.ProjectionType.PROJ_AVERAGE_VALUE, cc.ProjectionType.PROJ_AVERAGE_VALUE, 
                              cc.EmptyCellFillOption.FILL_MAXIMUM_HEIGHT)

cc.RasterizeGeoTiffOnly( cloud, 0.01, cc.CC_DIRECTION.Z, True, True, False, dataDir,
                         False, cc.ProjectionType.PROJ_AVERAGE_VALUE, cc.ProjectionType.PROJ_AVERAGE_VALUE, 
                         cc.EmptyCellFillOption.FILL_MAXIMUM_HEIGHT)
# cc.EmptyCellFillOption.FILL_MAXIMUM_HEIGHT behave like EmptyCellFillOption.INTERPOLATE

cc.SaveEntities([cloud, rcloud, rcloud2, rmesh], os.path.join(dataDir, "wave%d.bin"%h))

