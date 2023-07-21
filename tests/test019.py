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

#---partialClone01-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))

refCloud = cc.CloudSamplingTools.noiseFilter(cloud, 0.04, 1.0) # selection on cloud
origCloud = refCloud.getAssociatedCloud()            # the original cloud ~ cloud
(noiseCloud, res) = origCloud.partialClone(refCloud) # ccPointCloud from selection, status
noiseCloud.setName("noiseCloud")

if refCloud.__class__ != cc.ReferenceCloud:
    raise RuntimeError
if refCloud.size() < 7470 or refCloud.size() > 7570:
    raise RuntimeError
if res != 0:
    raise RuntimeError
#---partialClone01-end

#---resampleCloudSpatially01-begin
refCloud = cc.CloudSamplingTools.resampleCloudSpatially(cloud, 0.05)
(spatialCloud, res) = cloud.partialClone(refCloud)
spatialCloud.setName("spatialCloud")

if refCloud.size() != 55465:
    raise RuntimeError
if res != 0:
    raise RuntimeError
#---resampleCloudSpatially01-end

#---SubsampleRandom01-begin
refCloud = cc.CloudSamplingTools.subsampleCloudRandomly(cloud, 50000)
(randomCloud, res) = cloud.partialClone(refCloud)
randomCloud.setName("randomCloud")

if refCloud.size() != 50000:
    raise RuntimeError
if res != 0:
    raise RuntimeError
#---SubsampleRandom01-end


#---resampleOctree01-begin
resOctrCloud = cc.CloudSamplingTools.resampleCloudWithOctree(randomCloud, newNumberOfPoints=5000,
                                         resamplingMethod=cc.RESAMPLING_CELL_METHOD.CELL_CENTER)
resOctrCloud.setName("resOctrCloud")

if resOctrCloud.size() < 1900 or resOctrCloud.size() > 8000:
    raise RuntimeError
#---resampleOctree01-end

#---resampleOctreeLevel01-begin
resOctrAlCloud  = cc.CloudSamplingTools.resampleCloudWithOctreeAtLevel(randomCloud, octreeLevel=5,
                                         resamplingMethod=cc.RESAMPLING_CELL_METHOD.CELL_CENTER)
resOctrAlCloud.setName("resOctrAlCloud")

if resOctrAlCloud.size() < 2050 or resOctrAlCloud.size() > 2100:
    raise RuntimeError
#---resampleOctreeLevel01-end

#---sorFilter01-begin
refCloud = cc.CloudSamplingTools.sorFilter(randomCloud)
(sorCloud, res) = randomCloud.partialClone(refCloud)
sorCloud.setName("sorCloud")

if refCloud.size() < 43000 or refCloud.size() > 45000:
    raise RuntimeError
if res != 0:
    raise RuntimeError
#---sorFilter01-end


#---subsampleOctree01-begin
refCloud = cc.CloudSamplingTools.subsampleCloudWithOctree(cloud, 25000,
                                 cc.SUBSAMPLING_CELL_METHOD.RANDOM_POINT)
(subOctreeCloud, res) = cloud.partialClone(refCloud)
subOctreeCloud.setName("subOctreeCloud")

if refCloud.size() != 36777:
    raise RuntimeError
if res != 0:
    raise RuntimeError
#---subsampleOctree01-end


#---subsampleOctreeLevel01-begin
refCloud = cc.CloudSamplingTools.subsampleCloudWithOctreeAtLevel(cloud, 5,
                    cc.SUBSAMPLING_CELL_METHOD.NEAREST_POINT_TO_CELL_CENTER)
(subOctLevCloud, res) = cloud.partialClone(refCloud)
subOctLevCloud.setName("subOctLevCloud")

if refCloud.size() != 2262:
    raise RuntimeError
if res != 0:
    raise RuntimeError
#---subsampleOctreeLevel01-end

cloud.fuse(randomCloud)   # fuse to clouds to have duplicated points
if cloud.size() != 1050000:
    raise RuntimeError

#---filterPoints01-begin
dupSFindex = cloud.addScalarField("DuplicateFlags")
cloud.setCurrentScalarField(dupSFindex)
ret = cc.GeometricalAnalysisTools.FlagDuplicatePoints(cloud) # identify duplicated points
if ret != cc.ErrorCode.NoError:
    raise RuntimeError
noDuplCloud = cloud.filterPointsByScalarValue(0., 0., outside=False) # remove duplicated pts
#---filterPoints01-end
if noDuplCloud.size() != 1000000:
    raise RuntimeError

entities = [noiseCloud, spatialCloud, randomCloud, resOctrCloud, resOctrAlCloud, sorCloud, subOctreeCloud, subOctLevCloud, cloud, noDuplCloud]
cc.SaveEntities(entities, os.path.join(dataDir, "samplings.bin"))
