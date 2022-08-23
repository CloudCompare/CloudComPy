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

from gendata import dataDir, isCoordEqual
import cloudComPy as cc
import numpy as np

# --- generate a set of coords and a scalar field

npts = 20000
h = 2.
x = np.float32(-5. + 10.*np.random.random((npts)))
y = np.float32(-5. + 10.*np.random.random((npts)))
z = np.float32(np.sin(h * np.sqrt(x**2 + y**2)) / np.sqrt(x**2 + y**2))
coords = np.column_stack((x,y,z))

# --- create the pointCloud, add the scalar field, save the cloud on .las format

cl = cc.ccPointCloud("wave_%d"%h)
cl.coordsFromNPArray_copy(coords)
res = cl.exportCoordToSF(False, False, True)

res = cc.SavePointCloud(cl, os.path.join(dataDir, "cloud_2.las"))

# --- load the .las file, compute density, save

cloud = cc.loadPointCloud(os.path.join(dataDir, "cloud_2.las"))
isOk = cc.computeLocalDensity(cc.Density.DENSITY_3D, 0.16, [cloud])
if not isOk:
    raise RuntimeError

res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud_2s.las"))
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
