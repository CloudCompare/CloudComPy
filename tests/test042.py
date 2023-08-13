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
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import dataDir, getSampleCloud
import cloudComPy as cc

#---extractCC01-begin
process = psutil.Process(os.getpid())
mem_start = process.memory_full_info().rss
cloud = cc.loadPointCloud(getSampleCloud(5.0))
bbox = cc.ccBBox((-5.0, -5.0, 0.), (5., 5., 1.), True)
res=cc.ExtractSlicesAndContours(entities=[cloud], bbox=bbox)
clouds = res[0]
print('input data memory usage:', process.memory_full_info().rss - mem_start)
#---extractCC01-end

#---extractCC02-begin
for i in range(10):
    mem_start = process.memory_full_info().rss
    res2 = cc.ExtractConnectedComponents(clouds=clouds, octreeLevel=6, randomColors=True)
    components = res2[1]
    for comp in components:
        comp.showColors(True)
    #---extractCC02-end
    if res2[0] != 1: # the number of clouds in input
        raise RuntimeError
    if len(components) != 12:
        raise RuntimeError
    #---extractCC03-begin
    for comp in components:
        cc.deleteEntity(comp)
    # be sure to have no more Python variable referencing the deleted items
    components = None
    res2 = None
    addedMemory = process.memory_full_info().rss - mem_start
    print(f'iteration {i}, ExtractConnectedComponents added memory: ', addedMemory)
    #---extractCC03-end
    if i > 1 and addedMemory >20000:
        raise RuntimeError
