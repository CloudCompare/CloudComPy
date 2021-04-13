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
import cloudComPy as cc

#app = QApplication(sys.argv)
createSymbolicLinks() # required for tests on build, before cc.initCC.init
cc.initCC()  # to do once before using plugins or dealing with numpy
cloud = cc.loadPointCloud(getSampleCloud(5.0))

tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.1, 0.2, 0.3, (8.0, 0.0, 0.0))
box = cc.ccBox((1., 2., 3.), tr1, "aBox")
if box.getTypeName() != 'Box':
    raise RuntimeError
if box.size() != 12:
    raise RuntimeError

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.5, (0., 1., 0.), (5.0, 6.0, 3.0))
cone = cc.ccCone(3., 1., 2., 0., 0., tr2, "aCone", 12)
if cone.getTypeName() != 'Cone':
    raise RuntimeError
if cone.size() != 48:
    raise RuntimeError

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0., (0., 0., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 3.0, tr3, 'aCylinder', 48)
if cylinder.getTypeName() != 'Cylinder':
    raise RuntimeError
if cylinder.size() != 192:
    raise RuntimeError

tr4 = cc.ccGLMatrix()
tr4.initFromParameters(0.5, (0., 1., 0.), (-3.0, 0.0, 4.0))
plane = cc.ccPlane(3.0, 4.0, tr4, "aPlane")
if plane.getTypeName() != 'Plane':
    raise RuntimeError
if plane.size() != 2:
    raise RuntimeError

tr5 = cc.ccGLMatrix()
tr5.initFromParameters(0.0, (0., 0., 0.), (-7.0, 5.0, 1.0))
sphere = cc.ccSphere(1.5, tr5, "aSphere", 72)
if sphere.getTypeName() != 'Sphere':
    raise RuntimeError
if sphere.size() != 10224:
    raise RuntimeError

tr6 = cc.ccGLMatrix()
tr6.initFromParameters(0.2, (1., 2., 0.), (-0.0, -5.0, -9.0))
torus = cc.ccTorus(5.0, 7.0, math.pi, True, 3.0, tr6, "aTorus", 60)
if torus.getTypeName() != 'Torus':
    raise RuntimeError
if torus.size() != 964:
    raise RuntimeError

tr7 = cc.ccGLMatrix()
tr7.initFromParameters(0.5*math.pi, (1., 0., 0.), (-5.0, -2.0, -2.0))
quadric = cc.ccQuadric((-1., -1.), (1., 1.), (1., 2., 1., 1., 2., 2.), None, tr7, "aQuadric", 60)
if quadric.getTypeName() != 'Quadric':
    raise RuntimeError
if quadric.size() != 6962:
    raise RuntimeError

tr8 = cc.ccGLMatrix()
tr8.initFromParameters(0.5*math.pi, (0., 1., 0.), (-0.0, -0.0, -4.0))
dish = cc.ccDish(2.0, 1.0, 3.0, tr8, "aDish", 72)
if dish.getTypeName() != 'Dish':
    raise RuntimeError
if dish.size() != 2520:
    raise RuntimeError

cc.SaveEntities([cloud, box, cone, cylinder, plane, sphere, torus, quadric, dish], os.path.join(dataDir, "entities2.bin"))

