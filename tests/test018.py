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

from gendata import getSampleCloud, dataDir
import numpy as np
import cloudComPy as cc

cloud = cc.loadPointCloud(getSampleCloud(2.0))
ok = cloud.exportCoordToSF(True, True, True)

# --- access to ScalarField by name

dic= cloud.getScalarFieldDic()
dic  # {'Coord. X': 0, 'Coord. Y': 1, 'Coord. Z': 2}
sf0 = cloud.getScalarField(dic['Coord. X'])
if sf0.getName() != 'Coord. X':
    raise RuntimeError
sf1 = cloud.getScalarField('Coord. Y')
if sf1.getName() != 'Coord. Y':
    raise RuntimeError

# --- check write and read ply format

cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.ply"))

cloud2 = cc.loadPointCloud(os.path.join(dataDir, "cloud.ply"))
if cloud2.size() != cloud.size():
    raise RuntimeError

dic2= cloud.getScalarFieldDic()
if dic2 != dic:
    raise RuntimeError
    