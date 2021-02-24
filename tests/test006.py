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
from gendata import getSampleCloud, dataDir, isCoordEqual
from PyQt5.QtWidgets import QApplication
import cloudCompare as cc

app = QApplication(sys.argv)
cc.initCC.init()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))
cloud.translate((1000, 2000, 3000))
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "res1.xyz"))
if res:
    raise RuntimeError

cloud = cc.loadPointCloud(os.path.join(dataDir, "res1.xyz"))
coords = cloud.toNpArray(True)
if not isCoordEqual(coords[0], (995., 1995., 2999.8987)):
    raise RuntimeError

cloud1 = cc.loadPointCloud(os.path.join(dataDir, "res1.xyz"), cc.XYZ, 0, -1000, -2000, -3000)
coords1 = cloud1.toNpArray(True)
if not isCoordEqual(coords1[0], (-5., -5., -0.10131836)):
    raise RuntimeError
