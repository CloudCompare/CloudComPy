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
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init
cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))
poly = cc.loadPolyline(getSamplePoly("poly1"))

lg = poly.computeLength()
if not math.isclose(lg, 16.885, rel_tol=1e-04):
    raise RuntimeError

if poly.isClosed():
    raise RuntimeError

if poly.segmentCount() != 6:
    raise RuntimeError

poly.setClosed(True)
if not poly.isClosed():
    raise RuntimeError

if poly.segmentCount() != 7:
    raise RuntimeError

if poly.size() != 7:
    raise RuntimeError

poly.setName("myPoly")
if poly.getName() != "myPoly":
    raise RuntimeError

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

if poly.is2DMode():
    raise RuntimeError
poly.set2DMode(True)
if not poly.is2DMode():
    raise RuntimeError

p2 = poly.smoothChaikin(0.2, 4)
lg = p2.computeLength()
if not math.isclose(lg, 17.504, rel_tol=1e-04):
    raise RuntimeError
if p2.segmentCount() != 112:
    raise RuntimeError

