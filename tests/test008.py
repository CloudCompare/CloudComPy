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

cloud = cc.loadPointCloud(getSampleCloud(5.0))

box = cc.ccBox((1., 2., 3.))
if box.getTypeName() != 'Box':
    raise RuntimeError
if box.size() != 12:
    raise RuntimeError

cone = cc.ccCone(3., 1., 2.)
if cone.getTypeName() != 'Cone':
    raise RuntimeError
if cone.size() != 96:
    raise RuntimeError

#---primitiveTypeName01-begin
cylinder = cc.ccCylinder(0.5, 6.0)
if cylinder.getTypeName() != 'Cylinder':
    raise RuntimeError
if cylinder.size() != 96:
    raise RuntimeError
#---primitiveTypeName01-end

plane = cc.ccPlane(3.0, 4.0)
if plane.getTypeName() != 'Plane':
    raise RuntimeError
if plane.size() != 2:
    raise RuntimeError

sphere = cc.ccSphere(1.5)
if sphere.getTypeName() != 'Sphere':
    raise RuntimeError
if sphere.size() != 1104:
    raise RuntimeError

torus = cc.ccTorus(5.0, 7.0)
if torus.getTypeName() != 'Torus':
    raise RuntimeError
if torus.size() != 4608:
    raise RuntimeError

quadric = cc.ccQuadric((-1., -1.), (1., 1.), (1., 2., 3., 4., 5., 6.))
if quadric.getTypeName() != 'Quadric':
    raise RuntimeError
if quadric.size() != 1058:
    raise RuntimeError

dish = cc.ccDish(2.0, 1.0)
if dish.getTypeName() != 'Dish':
    raise RuntimeError
if dish.size() != 168:
    raise RuntimeError

cc.SaveEntities([cloud, box, cone, cylinder, plane, sphere, torus, quadric, dish], os.path.join(dataDir, "entities.bin"))

