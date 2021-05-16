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
from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud1 = cc.loadPointCloud(getSampleCloud2(3.0,0, 0.1))
cloud1.setName("cloud1")
tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.2*math.pi, (1., 1., 1.), (0.0, 0.0, 10.0))
cloud1.applyRigidTransformation(tr1)

plane = cc.ccPlane.Fit(cloud1)
equation = plane.getEquation()
eqRef = [0.4032580554485321, -0.2757962644100189, 0.8725361824035645, 8.782577514648438]
for i in range(4):
    if not math.isclose(equation[i], eqRef[i], rel_tol=1.e-3):
        raise RuntimeError

cc.SaveEntities([cloud1, plane], os.path.join(dataDir, "cloudsFit.bin"))




