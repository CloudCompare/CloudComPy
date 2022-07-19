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

from gendata import dataDir, createSymbolicLinks
import cloudComPy as cc

#---optimalBB00-begin
from cloudComPy.minimalBoundingBox import findRotation
#---optimalBB00-end

createSymbolicLinks() # required for tests on build, before cc.initCC.init

# --- a rotated object, with a non optimal bounding box

#---optimalBB01-begin
sphere = cc.ccSphere(1.0)
cloud = sphere.samplePoints(False, 100000)
cloud.scale(1.0, 3.0, 9.0)

cloudBeforeRot = cloud.cloneThis()
cloudBeforeRot.setName("cloudBeforeRot")

transform1 = cc.ccGLMatrix()
transform1.initFromParameters(1., (1.5, 2.5, 2.), (0,0,0))
cloud.applyRigidTransformation(transform1)
cloud.setName("rotated object")
#---optimalBB01-end

# --- find an optimal bounding box with the associated rotation

#---optimalBB02-begin
boundingBox, rotinv, poly, clbbox = findRotation(cloud)
#---optimalBB02-end

#---optimalBB03-begin
rotation = (cc.ccGLMatrix.fromDouble(rotinv)).inverse()
axisObj = cloud.cloneThis()
axisObj.applyRigidTransformation(rotation)
axisObj.setName("object on axes")

clb = clbbox.cloneThis()
clb.applyRigidTransformation(rotation)
axisPoly = cc.ccPolyline(clb)
axisPoly.addChild(clb)
axisPoly.addPointIndex(0, clb.size())
axisPoly.setClosed(True)

res = cc.SaveEntities([cloudBeforeRot, cloud, axisObj, poly, axisPoly], os.path.join(dataDir, "optimalBoundingBox.bin"))
#---optimalBB03-end

