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
import numpy as np
import cloudComPy as cc

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))


refCloud = cc.CloudSamplingTools.noiseFilter(cloud, 0.04, 1.0)
if refCloud.__class__ != cc.ReferenceCloud:
    raise RuntimeError
if refCloud.size() != 7489:
    raise RuntimeError
origCloud = refCloud.getAssociatedCloud()
(noiseCloud, res) = origCloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError
noiseCloud.setName("noiseCloud")


params = cc.SFModulationParams()
refCloud = cc.CloudSamplingTools.resampleCloudSpatially(cloud, 0.05, params)
if refCloud.size() != 55465:
    raise RuntimeError
(spatialCloud, res) = cloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError
spatialCloud.setName("spatialCloud")


refCloud = cc.CloudSamplingTools.subsampleCloudRandomly(cloud, 50000)
if refCloud.size() != 50000:
    raise RuntimeError
(randomCloud, res) = cloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError
randomCloud.setName("randomCloud")


resOctrCloud = cc.CloudSamplingTools.resampleCloudWithOctree(randomCloud, 5000,
                                         cc.RESAMPLING_CELL_METHOD.CELL_CENTER)
if resOctrCloud.size() < 1900 or resOctrCloud.size() > 8000:
    raise RuntimeError
resOctrCloud.setName("resOctrCloud")

resOctrAlCloud  = cc.CloudSamplingTools.resampleCloudWithOctreeAtLevel(randomCloud, 5,
                                         cc.RESAMPLING_CELL_METHOD.CELL_CENTER)
if resOctrAlCloud.size() < 2050 or resOctrAlCloud.size() > 2100:
    raise RuntimeError
resOctrAlCloud.setName("resOctrAlCloud")


refCloud = cc.CloudSamplingTools.sorFilter(randomCloud)
if refCloud.size() < 43000 or refCloud.size() > 45000:
    raise RuntimeError
(sorCloud, res) = randomCloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError
sorCloud.setName("sorCloud")


refCloud = cc.CloudSamplingTools.subsampleCloudWithOctree(cloud, 25000,
                                                          cc.SUBSAMPLING_CELL_METHOD.RANDOM_POINT)
if refCloud.size() != 36777:
    raise RuntimeError
(subOctreeCloud, res) = cloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError
subOctreeCloud.setName("subOctreeCloud")


refCloud = cc.CloudSamplingTools.subsampleCloudWithOctreeAtLevel(cloud, 5,
                                                                 cc.SUBSAMPLING_CELL_METHOD.NEAREST_POINT_TO_CELL_CENTER)
if refCloud.size() != 2262:
    raise RuntimeError
(subOctLevCloud, res) = cloud.partialClone(refCloud)
if res != 0:
    raise RuntimeError
subOctLevCloud.setName("subOctLevCloud")

cloud.fuse(randomCloud)   # fuse to clouds to have duplicated points
if cloud.size() != 1050000:
    raise RuntimeError

dupSFindex = cloud.addScalarField("DuplicateFlags")
cloud.setCurrentScalarField(dupSFindex)
ret = cc.GeometricalAnalysisTools.FlagDuplicatePoints(cloud) # identify duplicated points
if ret != cc.ErrorCode.NoError:
    raise RuntimeError
noDuplicateCloud = cloud.filterPointsByScalarValue(0., 0., False) # remove duplicated points
if noDuplicateCloud.size() != 1000000:
    raise RuntimeError


entities = [noiseCloud, spatialCloud, randomCloud, resOctrCloud, resOctrAlCloud, sorCloud, subOctreeCloud, subOctLevCloud, cloud, noDuplicateCloud]
cc.SaveEntities(entities, os.path.join(dataDir, "samplings.bin"))
