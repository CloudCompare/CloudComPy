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
import numpy as np

os.environ["_CCTRACE_"] = "ON"  # only if you want C++ debug traces

import cloudComPy as cc

# --- check deleteEntity on various ccHObject constructed from Python

coords = np.random.rand(1000,3)
cloud = cc.ccPointCloud('temp')
cloud.coordsFromNPArray_copy(coords.copy(order='C'))
print(cloud.size())
cc.deleteEntity(cloud)
cloud=None

coords = np.random.rand(10,3)
cloud = cc.ccPointCloud('temp')
cloud.coordsFromNPArray_copy(coords.copy(order='C'))
print(cloud.size())
poly = cc.ccPolyline(cloud)
cc.deleteEntity(poly)
poly = None
cc.deleteEntity(cloud)
cloud=None

box = cc.ccBox((1., 2., 3.))
print(box.size())
cc.deleteEntity(box)
box = None

cone = cc.ccCone(3., 1., 2.)
print(cone.size())
cc.deleteEntity(cone)
cone = None

cylinder = cc.ccCylinder(0.5, 6.0)
print(cylinder.size())
cc.deleteEntity(cylinder)
cylinder = None

plane = cc.ccPlane(3.0, 4.0)
print(plane.size())
cc.deleteEntity(plane)
plane = None

sphere = cc.ccSphere(1.5)
print(sphere.size())
cc.deleteEntity(sphere)
sphere = None

torus = cc.ccTorus(5.0, 7.0)
print(torus.size())
cc.deleteEntity(torus)
torus = None

quadric = cc.ccQuadric((-1., -1.), (1., 1.), (1., 2., 3., 4., 5., 6.))
print(quadric.size())
cc.deleteEntity(quadric)
quadric = None

dish = cc.ccDish(2.0, 1.0)
print(dish.size())
cc.deleteEntity(dish)
dish = None

coords = np.random.rand(1000,3)
cloud = cc.ccPointCloud('temp')
cloud.coordsFromNPArray_copy(coords.copy(order='C'))
print(cloud.size())
countRef = cloud.countRef()
print(countRef)
cloud.incRef()
if (cloud.countRef() != (countRef + 1)):
    raise RuntimeError
cloud.decRef()
if (cloud.countRef() != countRef):
    raise RuntimeError
cc.deleteEntity(cloud)
cloud=None


