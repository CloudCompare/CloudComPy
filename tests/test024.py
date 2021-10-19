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

from gendata import getSampleCloud, getSamplePoly2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))

ret = cc.computeApproxLocalDensity(cc.Density.DENSITY_KNN, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeApproxLocalDensity(cc.Density.DENSITY_2D, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeApproxLocalDensity(cc.Density.DENSITY_3D, 0.03, [cloud])
if not ret:
    raise RuntimeError

ret = cc.computeCurvature(cc.CurvatureType.GAUSSIAN_CURV, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeCurvature(cc.CurvatureType.MEAN_CURV, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeCurvature(cc.CurvatureType.NORMAL_CHANGE_RATE, 0.03, [cloud])
if not ret:
    raise RuntimeError

ret = cc.computeFeature(cc.GeomFeature.Anisotropy, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.EigenEntropy, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.EigenValue1, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.EigenValue2, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.EigenValuesSum, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.Linearity, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.Omnivariance, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.PCA1, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.PCA2, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.Planarity, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.Sphericity, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.SurfaceVariation, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeFeature(cc.GeomFeature.Verticality, 0.03, [cloud])
if not ret:
    raise RuntimeError

ret = cc.computeLocalDensity(cc.Density.DENSITY_KNN, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeLocalDensity(cc.Density.DENSITY_2D, 0.03, [cloud])
if not ret:
    raise RuntimeError
ret = cc.computeLocalDensity(cc.Density.DENSITY_3D, 0.03, [cloud])
if not ret:
    raise RuntimeError

ret = cc.computeMomentOrder1(0.03, [cloud])
if not ret:
    raise RuntimeError

ret = cc.computeRoughness(0.03, [cloud])
if not ret:
    raise RuntimeError

dic = cloud.getScalarFieldDic()
if len(dic) != 24:
    raise RuntimeError

cc.SaveEntities([cloud], os.path.join(dataDir, "geometrics.bin"))
