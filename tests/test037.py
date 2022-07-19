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

#---extractCC01-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))
bbox = cc.ccBBox((-5.0, -5.0, 0.), (5., 5., 1.), True)
res=cc.ExtractSlicesAndContours(entities=[cloud], bbox=bbox)
clouds = res[0] # result = [one cloud]
#---extractCC01-end

#---extractCC02-begin
res2 = cc.ExtractConnectedComponents(clouds=clouds, octreeLevel=6, randomColors=True)
components = res2[1]
for comp in components:
    comp.showColors(True)
#---extractCC02-end
if res2[0] != 1: # the number of clouds in input
    raise RuntimeError
if len(components) != 12:
    raise RuntimeError

cc.SaveEntities(components, os.path.join(dataDir, "components.bin"))

