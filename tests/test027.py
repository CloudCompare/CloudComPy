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
from gendata import dataDir, createSymbolicLinks
import cloudComPy as cc
from minimalBoundingBox import findRotation

createSymbolicLinks() # required for tests on build, before cc.initCC.init
cc.initCC()  # to do once before using plugins or dealing with numpy

# --- a rotated object, with a non optimal bounding box

sphere = cc.ccSphere(1.0)
cloud = sphere.samplePoints(False, 100000)
cloud.scale(1.0, 3.0, 9.0)
boundingBox = cloud.getOwnBB()

transform1 = cc.ccGLMatrix()
transform1.initFromParameters(0.25, (1.5, 2.9, 6.3), (0,0,0))
cloud.applyRigidTransformation(transform1)

# --- find an optimal bounding box with the associated rotation

boundingBox, rotinv, poly, clbbox = findRotation(cloud)
res = cc.SaveEntities([cloud, poly], os.path.join(dataDir, "optimalBoundingBox.bin"))
