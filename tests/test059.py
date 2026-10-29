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
#          Copyright 2020-2025 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

import os
import sys
import math
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

import cloudComPy as cc
from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual

tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0., (0., 0., 0.), (3.0, 0.0, 4.0))

cylinder = cc.ccCylinder(0.5, 3.0, tr1, 'aCylinder', 48)
cylinderRef = cylinder.cloneMesh()
cylinderRef.setName("cylinder_1")

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.5, (0., 1., 0.), (0.0, 0.0, 0.0))

cylinder.applyRigidTransformation(tr2)

m2 = tr2*tr1
hist = cylinder.getGLTransformationHistory()

if m2.toString() != hist.toString():
    raise RuntimeError

cylinder2Ref = cylinder.cloneMesh()
cylinder2Ref.setName("cylinder_2")

cylinder.translate((-3,0,-4))
cylinder3Ref = cylinder.cloneMesh()
cylinder3Ref.setName("cylinder_3")

cylinder.scale(1,2,3)
cylinder4Ref = cylinder.cloneMesh()
cylinder4Ref.setName("cylinder_4")

cc.SaveEntities([cylinderRef, cylinder2Ref, cylinder3Ref, cylinder4Ref, cylinder], os.path.join(dataDir, "cylinder.bin"))

