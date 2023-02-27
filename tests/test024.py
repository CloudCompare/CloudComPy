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
import numpy as np
import time

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud2, getSamplePoly2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

cloud = cc.loadPointCloud(getSampleCloud2(5.0, 0, 0.02))

timestamps=[]
timestamps.append(time.time())

ret = cc.computeApproxLocalDensity(cc.Density.DENSITY_KNN, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration DENSITY_KNN:", timestamps[-1] -timestamps[-2])

ret = cc.computeApproxLocalDensity(cc.Density.DENSITY_2D, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration DENSITY_2D:", timestamps[-1] -timestamps[-2])

ret = cc.computeApproxLocalDensity(cc.Density.DENSITY_3D, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration DENSITY_3D:", timestamps[-1] -timestamps[-2])

ret = cc.computeCurvature(cc.CurvatureType.GAUSSIAN_CURV, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration GAUSSIAN_CURV:", timestamps[-1] -timestamps[-2])

ret = cc.computeCurvature(cc.CurvatureType.MEAN_CURV, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration MEAN_CURV:", timestamps[-1] -timestamps[-2])

ret = cc.computeCurvature(cc.CurvatureType.NORMAL_CHANGE_RATE, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration NORMAL_CHANGE_RATE:", timestamps[-1] -timestamps[-2])


ret = cc.computeFeature(cc.GeomFeature.Anisotropy, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration Anisotropy:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.EigenEntropy, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration EigenEntropy:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.EigenValue1, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration EigenValue1:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.EigenValue2, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration EigenValue2:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.EigenValuesSum, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration EigenValuesSum:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.Linearity, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration Linearity:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.Omnivariance, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration Omnivariance:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.PCA1, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration PCA1:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.PCA2, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration PCA2:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.Planarity, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration Planarity:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.Sphericity, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration Sphericity:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.SurfaceVariation, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration SurfaceVariation:", timestamps[-1] -timestamps[-2])

ret = cc.computeFeature(cc.GeomFeature.Verticality, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration Verticality:", timestamps[-1] -timestamps[-2])


ret = cc.computeLocalDensity(cc.Density.DENSITY_KNN, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration computeLocalDensity KNN:", timestamps[-1] -timestamps[-2])

ret = cc.computeLocalDensity(cc.Density.DENSITY_2D, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration computeLocalDensity 2D:", timestamps[-1] -timestamps[-2])

ret = cc.computeLocalDensity(cc.Density.DENSITY_3D, 0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration computeLocalDensity 3D:", timestamps[-1] -timestamps[-2])


ret = cc.computeMomentOrder1(0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration computeMomentOrder1:", timestamps[-1] -timestamps[-2])

ret = cc.computeRoughness(0.06, [cloud])
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration computeRoughness:", timestamps[-1] -timestamps[-2])
print("Total duration:", timestamps[-1] -timestamps[0])

ret = cc.computeRoughness(0.06, [cloud], (0., 1., 0.))
if not ret:
    raise RuntimeError
timestamps.append(time.time())
print("duration computeRoughness up dir:", timestamps[-1] -timestamps[-2])
print("Total duration:", timestamps[-1] -timestamps[0])

dic = cloud.getScalarFieldDic()
if len(dic) != 24:
    raise RuntimeError

cc.SaveEntities([cloud], os.path.join(dataDir, "geometrics.bin"))
