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
from gendata import getSampleCloud, dataDir
import numpy as np
import cloudComPy as cc

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))

refCloud = cc.CloudSamplingTools.subsampleCloudRandomly(cloud, 50000)
if refCloud.size() != 50000:
    raise RuntimeError
if refCloud.__class__ != cc.ReferenceCloud:
    raise RuntimeError

origCloud = refCloud.getAssociatedCloud()
(randomCloud, res) = origCloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError


refCloud = cc.CloudSamplingTools.subsampleCloudWithOctreeAtLevel(cloud, 5,
                                                                 cc.SUBSAMPLING_CELL_METHOD.NEAREST_POINT_TO_CELL_CENTER)
if refCloud.size() != 2223:
    raise RuntimeError
(subOctLevCloud, res) = cloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError


refCloud = cc.CloudSamplingTools.subsampleCloudWithOctree(cloud, 25000,
                                                          cc.SUBSAMPLING_CELL_METHOD.RANDOM_POINT)
if refCloud.size() != 36450:
    raise RuntimeError
(subOctreeCloud, res) = cloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError


resOctrCloud = cc.CloudSamplingTools.resampleCloudWithOctree(randomCloud, 5000,
                                         cc.RESAMPLING_CELL_METHOD.CELL_CENTER)
if resOctrCloud.size() < 7800 or resOctrCloud.size() > 7900:
    raise RuntimeError

resOctrAlCloud  = cc.CloudSamplingTools.resampleCloudWithOctreeAtLevel(randomCloud, 5,
                                         cc.RESAMPLING_CELL_METHOD.CELL_CENTER)
if resOctrAlCloud.size() < 2050 or resOctrAlCloud.size() > 2100:
    raise RuntimeError


cc.SaveEntities([randomCloud, subOctLevCloud, subOctreeCloud, resOctrCloud, resOctrAlCloud], os.path.join(dataDir, "samplings.bin"))
