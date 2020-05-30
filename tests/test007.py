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
from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
from PyQt5.QtWidgets import QApplication
import cloudCompare as cc

app = QApplication(sys.argv)
createSymbolicLinks() # required for tests on build, before cc.initCC.init
cc.initCC.init()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(2.0))
poly = cc.loadPolyline(getSamplePoly("poly1"))

cloudCropZ = cloud.crop2D(poly, 2, True)
cc.SavePointCloud(cloudCropZ, os.path.join(dataDir, "cloudCropZ.xyz"))
npts = cloudCropZ.size()
print("cloud.size %s" % npts)
if npts != 189981:
    raise RuntimeError

cloudCropX = cloud.crop2D(poly, 1, True)
cc.SavePointCloud(cloudCropX, os.path.join(dataDir, "cloudCropX.xyz"))
npts = cloudCropX.size()
print("cloud.size %s" % npts)
if npts != 458593:
    raise RuntimeError

cloudCropY = cloud.crop2D(poly, 0, True)
cc.SavePointCloud(cloudCropY, os.path.join(dataDir, "cloudCropY.xyz"))
npts = cloudCropY.size()
print("cloud.size %s" % npts)
if npts != 399968:
    raise RuntimeError
