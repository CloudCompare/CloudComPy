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
#          Copyright 2020-2025 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

import os
import sys
import math
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

import cloudComPy as cc
from gendata import getSampleCloud, dataDir

if not cc.isPluginHoughNormals():
    print("Test skipped")
    sys.exit()

#---HoughNormals01-begin
import cloudComPy.HoughNormals

cloud1 = cc.loadPointCloud(getSampleCloud(5.0))
cloudComPy.HoughNormals.computeHoughNormals(cloud1)

cc.SaveEntities([cloud1], os.path.join(dataDir, "houghNormals.bin"))
#---HoughNormals01-end

