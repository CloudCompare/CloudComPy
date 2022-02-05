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
from gendata import getSampleCloud, dataDir
import cloudComPy as cc

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))
res = cloud.exportCoordToSF(False, True, True)
n = cloud.getNumberOfScalarFields()
cloud.setCurrentInScalarField(0)
cloud.setCurrentOutScalarField(1)

sfi = cloud.getCurrentInScalarField()
if sfi.getName() != cloud.getScalarFieldName(0):
    raise RuntimeError

sfo = cloud.getCurrentOutScalarField()
if sfo.getName() != cloud.getScalarFieldName(1):
    raise RuntimeError

radius = cc.GetPointCloudRadius([cloud])
print("radius: %s" % radius)
if not math.isclose(radius, 0.0293, rel_tol=1e-02):
    raise RuntimeError

radius = cc.GetPointCloudRadius(clouds=[cloud], nodes=6)
if not math.isclose(radius, 0.0207, rel_tol=1e-02):
    raise RuntimeError

radius = 0.03

res = cc.computeCurvature(cc.CurvatureType.GAUSSIAN_CURV, radius, [cloud])
nsf = cloud.getNumberOfScalarFields()
sfc = cloud.getScalarField(nsf - 1)
if sfc.getName() != "Gaussian curvature (0.03)":
    raise RuntimeError

cloud.setCurrentOutScalarField(nsf - 1)
fcloud = cc.filterBySFValue(0.01, sfc.getMax(), cloud)
filteredSize = fcloud.size()
print("filtered cloud size: %s" % filteredSize)
if not math.isclose(filteredSize, 113325, rel_tol=1e-03):
    raise RuntimeError

res = cc.SavePointCloud(fcloud, os.path.join(dataDir, "res3.xyz"))
