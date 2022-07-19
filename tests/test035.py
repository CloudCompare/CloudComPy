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

from gendata import dataDir, isCoordEqual
import cloudComPy as cc

#---RANSACSD01-begin
tr0 = cc.ccGLMatrix()
tr0.initFromParameters(math.pi/3., (0., 1., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 2.0, tr0)
c0 = cylinder.samplePoints(True, 1000)

tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.0, (0., 0., 0.), (-2.0, 5.0, 1.0))
sphere1 = cc.ccSphere(1.5, tr1)
c1 = sphere1.samplePoints(True, 1000)

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.0, (0., 0., 0.), (6.0, -3.0, -2.0))
sphere2 = cc.ccSphere(2.0, tr2)
c2 = sphere2.samplePoints(True, 1000)

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0.0, (0., 0., 0.), (0.0, 1.0, 2.0))
sphere3 = cc.ccSphere(1.0, tr3)
c3 = sphere3.samplePoints(True, 1000)

cloud = c0.cloneThis()
cloud.fuse(c1)
cloud.fuse(c2)
cloud.fuse(c3)
#---RANSACSD01-end

#---RANSACSD02-begin
if cc.isPluginRANSAC_SD():
    import cloudComPy.RANSAC_SD
    params = cc.RANSAC_SD.RansacParams()
    params.optimizeForCloud(cloud)
    print(params.epsilon, params.bitmapEpsilon)
    meshes, clouds = cc.RANSAC_SD.computeRANSAC_SD(cloud, params)
    #---RANSACSD02-end
    
    sortmeshes = [m for m in meshes if m is not None]
    sortmeshes.sort(key= lambda m: m.getName())
    for i in range(len(sortmeshes)):
        print("mesh %d: %s"%(i,sortmeshes[i].getName()))
    
    if not sortmeshes[0].isA(cc.CC_TYPES.CYLINDER):
        raise RuntimeError
    if not sortmeshes[1].isA(cc.CC_TYPES.PLANE):
        raise RuntimeError
    if not sortmeshes[2].isA(cc.CC_TYPES.PLANE):
        raise RuntimeError
    if not sortmeshes[3].isA(cc.CC_TYPES.SPHERE):
        raise RuntimeError
    if not sortmeshes[4].isA(cc.CC_TYPES.SPHERE):
        raise RuntimeError
    if not sortmeshes[5].isA(cc.CC_TYPES.SPHERE):
        raise RuntimeError
    
    if not math.isclose(sortmeshes[3].getRadius(), 1.0, rel_tol=3.e-2):
        raise RuntimeError
    if not math.isclose(sortmeshes[4].getRadius(), 1.5, rel_tol=3.e-2):
        raise RuntimeError
    if not math.isclose(sortmeshes[5].getRadius(), 2.0, rel_tol=3.e-2):
        raise RuntimeError
    
    tr_0 = sortmeshes[0].getTransformation()
    t3D0 = tr_0.getParameters1().t3D
    if not isCoordEqual(t3D0, (3.0, 0.0, 4.0), 0, 3.e-2):
        print(t3D0)
        raise RuntimeError
    tr_3 = sortmeshes[3].getTransformation()
    t3D3 = tr_3.getParameters1().t3D
    if not isCoordEqual(t3D3, (0.0, 1.0, 2.0), 0, 3.e-2):
        print(t3D3)
        raise RuntimeError
    tr_4 = sortmeshes[4].getTransformation()
    t3D4 = tr_4.getParameters1().t3D
    if not isCoordEqual(t3D4, (-2.0, 5.0, 1.0), 3.e-2):
        print(t3D4)
        raise RuntimeError
    tr_5 = sortmeshes[5].getTransformation()
    t3D5 = tr_5.getParameters1().t3D
    if not isCoordEqual(t3D5, (6.0, -3.0, -2.0), 3.e-2):
        print(t3D5)
        raise RuntimeError
    
    
    shapes = [cylinder, sphere1, sphere2, sphere3, cloud]
    for mesh in meshes:
        if mesh is not None:
            print("mesh:", mesh.getName())
            shapes.append(mesh)
    for cl in clouds:
        if cl is not None:
            print("cloud:", cl.getName())
            shapes.append(cl)
    
    cc.SaveEntities(shapes, os.path.join(dataDir, "ransac.bin"))


