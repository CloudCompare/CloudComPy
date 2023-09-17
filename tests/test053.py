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
import numpy as np
import multiprocessing

if not cc.isPluginPoissonRecon():
    print("Test skipped")
    sys.exit()

import cloudComPy.PoissonRecon

npts = 300000
x = np.float32(-5 +10*np.random.random((npts)))
y = np.float32(-5 +10*np.random.random((npts)))
f = np.sqrt(x*x + y*y)
z = np.sin(3*f)
coords = np.column_stack((x,y,z))
cloud = cc.ccPointCloud("cloud")
cloud.coordsFromNPArray_copy(coords)
cloud.exportCoordToSF(False, False, True)
cc.computeNormals([cloud])

#---PoissonRecon02-begin
mesh = cc.PoissonRecon.PR.PoissonReconstruction(pc=cloud, threads=multiprocessing.cpu_count(), density=True)
#---PoissonRecon02-end

cc.SaveEntities([cloud, mesh], os.path.join(dataDir, "PoissonRecon.bin"))
