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

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual
import cloudComPy as cc

cloud = cc.loadPointCloud(getSampleCloud(5.0))
namecloud = cloud.getName()
print("cloud name: %s" % namecloud)
if namecloud != "dataSample_5.0 - Cloud":
    raise RuntimeError

npts = cloud.size()
print("cloud.size %s" % npts)
if npts != 1000000:
    raise RuntimeError

res = cloud.hasScalarFields()
print("hasScalarField: %s" % res)
if res:
    raise RuntimeError

sf = cloud.getScalarField(0)
if sf is not None:
    raise RuntimeError

nsf = cloud.getNumberOfScalarFields()
if nsf != 0:
    raise RuntimeError

sfname = cloud.getScalarFieldName(0)
if sfname is not None:
    raise RuntimeError

sfname = cloud.getScalarFieldName(-1)
if sfname is not None:
    raise RuntimeError

sfname = cloud.getScalarFieldName(25)
if sfname is not None:
    raise RuntimeError

#---scale-translate-begin
# --- get a reference before transformation
g = cloud.computeGravityCenter()

print("gravityCenter: (%14.7e, %14.7e, %14.7e)" % (g[0], g[1], g[2]))
if not isCoordEqual(g, (-4.9999999e-03, -4.9999999e-03, 9.6193114e-03)):
    raise RuntimeError

# --- rescale the cloud along Z direction, center at origin

cloud.scale(1.0, 1.0, 2.0, (0., 0., 0.))

g = cloud.computeGravityCenter()
print("gravityCenter: (%14.7e, %14.7e, %14.7e)" % (g[0], g[1], g[2]))
if not isCoordEqual(g, (-4.9999999e-03, -4.9999999e-03, 1.9238623e-02)):
    raise RuntimeError

# --- inverse scaling

cloud.scale(1, 1, 0.5)

# --- a simple translation

cloud.translate((-1, -2, -3))

g = cloud.computeGravityCenter()
print("gravityCenter: (%14.7e, %14.7e, %14.7e)" % (g[0], g[1], g[2]))
if not isCoordEqual(g, (-1.0050000e+00, -2.0050001e+00, -2.9903808e+00)):
    raise RuntimeError

# --- inverse translation

cloud.translate((1, 2, 3))
#---scale-translate-end

g = cloud.computeGravityCenter()
print("gravityCenter: (%14.7e, %14.7e, %14.7e)" % (g[0], g[1], g[2]))
if not isCoordEqual(g, (-4.9999999e-03, -4.9999999e-03, 9.6193114e-03)):
    raise RuntimeError

res = cc.SavePointCloud(cloud, os.path.join(dataDir, "res1.bin"))
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError

cloud = cc.loadPointCloud(os.path.join(dataDir, "res1.bin"))
namecloud = cloud.getName()
print("cloud name: %s" % namecloud)
if namecloud != "dataSample_5.0 - Cloud":
    raise RuntimeError

npts = cloud.size()
print("cloud.size %s" % npts)
if npts != 1000000:
    raise RuntimeError

g = cloud.computeGravityCenter()
print("gravityCenter: (%14.7e, %14.7e, %14.7e)" % (g[0], g[1], g[2]))
if not isCoordEqual(g, (-4.9999999e-03, -4.9999999e-03, 9.6193114e-03)):
    raise RuntimeError

cloud = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
g= cloud.computeGravityCenter()
if not isCoordEqual(g, (-0.050, -0.050, 0.057), tol= 1.e-1):
    raise RuntimeError

cloud2 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1), mode=cc.CC_SHIFT_MODE.XYZ, z=10)
g= cloud2.computeGravityCenter()
if not isCoordEqual(g, (-0.050, -0.050, 10.057), tol= 1.e-1):
    raise RuntimeError


