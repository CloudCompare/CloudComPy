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

from gendata import getSampleCloud, dataDir, isCoordEqual
import cloudComPy as cc

#---interpolSF_01-begin
cloud1 = cc.loadPointCloud(getSampleCloud(5.0))
cloud2 = cc.loadPointCloud(getSampleCloud(1.0))
cloud1.exportCoordToSF(True, True, True)
dic = cloud1.getScalarFieldDic()

params = cc.interpolatorParameters()
params.method = cc.INTERPOL_METHOD.RADIUS
params.algos = cc.INTERPOL_ALGO.NORMAL_DIST
params.radius = 0.15
params.sigma = 0.06

sfIndexes = [dic['Coord. X'], dic['Coord. Y'], dic['Coord. Z']]

ret = cc.interpolateScalarFieldsFrom(cloud2, cloud1, sfIndexes, params)
#---interpolSF_01-end
if not ret:
    raise RuntimeError

cc.SaveEntities([cloud1, cloud2], os.path.join(dataDir, "interpolSF.bin"))
