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

from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init

#---cloudCrop2D01-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))
poly = cc.loadPolyline(getSamplePoly("poly1"))
#---cloudCrop2D01-end
if poly.getBoundingBox() != [(-2.0, -3.0, 0.0), (3.0, 4.0, 0.0)]:
    raise RuntimeError

poly2 = cc.loadPolyline(filename=getSamplePoly("poly1"), mode=cc.CC_SHIFT_MODE.XYZ, z=10)
if poly2.getBoundingBox() != [(-2.0, -3.0, 10.0), (3.0, 4.0, 10.0)]:
    raise RuntimeError

lg = poly.computeLength()
if not math.isclose(lg, 16.885, rel_tol=1e-04):
    raise RuntimeError

if poly.isClosed():
    raise RuntimeError

if poly.segmentCount() != 6:
    raise RuntimeError

#---cloudCrop2D02-begin
poly.setClosed(True)
#---cloudCrop2D02-end
if not poly.isClosed():
    raise RuntimeError

if poly.segmentCount() != 7:
    raise RuntimeError

if poly.size() != 7:
    raise RuntimeError

poly.setName("myPoly")
if poly.getName() != "myPoly":
    raise RuntimeError

#---cloudCrop2D03-begin
cloudCropZ = cloud.crop2D(poly, 2, True)
#---cloudCrop2D03-end
cc.SavePointCloud(cloudCropZ, os.path.join(dataDir, "cloudCropZ.xyz"))
npts = cloudCropZ.size()
print("cloud.size %s" % npts)
if npts < 189950 or npts > 189999:
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

#---smoothChaikin01-begin
p2 = poly.smoothChaikin(0.2, 4)
#---smoothChaikin01-end
lg = p2.computeLength()
if not math.isclose(lg, 17.504, rel_tol=1e-04):
    raise RuntimeError
if p2.segmentCount() != 112:
    raise RuntimeError

