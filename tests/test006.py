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

from gendata import getSampleCloud, dataDir, isCoordEqual
import cloudComPy as cc

cloud = cc.loadPointCloud(getSampleCloud(5.0))
cloud.translate((1000, 2000, 3000))
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "res1.xyz"))
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError

cloud = cc.loadPointCloud(os.path.join(dataDir, "res1.xyz"))
coords = cloud.toNpArray()
if not isCoordEqual(coords[0], (995., 1995., 2999.8987)):
    raise RuntimeError

cloud1 = cc.loadPointCloud(os.path.join(dataDir, "res1.xyz"), cc.CC_SHIFT_MODE.XYZ, 0, -1000, -2000, -3000)
coords1 = cloud1.toNpArray()
if not isCoordEqual(coords1[0], (-5., -5., -0.10131836)):
    raise RuntimeError

cloud = cc.loadPointCloud(getSampleCloud(5.0))
cloud.translate((100000, 200000, 300000))
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "shifted.xyz"))
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError

cloud = cc.loadPointCloud(os.path.join(dataDir, "shifted.xyz"))
if not cloud.isShifted():
    raise RuntimeError
shift = cloud.getGlobalShift()
if not isCoordEqual(shift, (-100000, -200000, -300000), tol=1.e-2):
    raise RuntimeError
    
