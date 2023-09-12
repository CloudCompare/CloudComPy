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

from gendata import dataDir, getSampleCloud
import cloudComPy as cc

#---CSF01-begin
if cc.isPluginCSF():
    import cloudComPy.CSF
    cloud = cc.loadPointCloud(getSampleCloud(5.0))
    clouds = cc.CSF.computeCSF(cloud)
    #---CSF01-end
    if len(clouds) != 2:
        raise RuntimeError
    if not math.isclose(clouds[0].size(), 935000, rel_tol=1.e-2):
        raise RuntimeError
    if clouds[1].size() != 1000000 - clouds[0].size():
        raise RuntimeError
    #---CSF02-begin
    clouds2 = cc.CSF.computeCSF(cloud, csfRigidness=1, clothResolution=1.0, classThreshold=0.3, computeMesh=True)
    #---CSF02-end

for cloud in clouds2:
    clouds.append(cloud)
res = cc.SaveEntities(clouds, os.path.join(dataDir, "CSF.bin"))
