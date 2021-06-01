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
import numpy as np

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

# --- generate a set of coords and a scalar field

npts = 10000000
phi = 2*np.pi*np.random.random((npts))
theta = 2*np.pi*np.random.random((npts))
r = 5 + 0.3*np.sin(2*2*np.pi*phi + 3*2*np.pi*theta)
x = np.float32(r*np.sin(phi)*np.cos(theta))
y = np.float32(r*np.sin(phi)*np.sin(theta))
z = np.float32(r*np.cos(phi))
coords = np.column_stack((x,y,z))
dr = np.float32(np.sqrt(x*x + y*y + z*z) -5)

# --- create the pointCloud, add the scalar field, save

cl = cc.ccPointCloud("boule")
cl.coordsFromNPArray_copy(coords)
cl.addScalarField("delta")
sf = cl.getScalarField(0)
sf.fromNpArrayCopy(dr)
res = cc.SavePointCloud(cl, os.path.join(dataDir, "boule.bin"))
