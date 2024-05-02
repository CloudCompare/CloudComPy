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

from gendata import getSampleCloud, dataDir, dataExtDir
import cloudComPy as cc

cloud = cc.loadPointCloud(getSampleCloud(5.0))
res = cloud.exportCoordToSF(False, True, True)
n = cloud.getNumberOfScalarFields()
cloud.setCurrentInScalarField(0)
cloud.setCurrentOutScalarField(1)

sfi = cloud.getCurrentInScalarField()
if sfi.getName() != cloud.getScalarFieldName(0):
    raise RuntimeError

sfo = cloud.getCurrentOutScalarField()
if sfo.getName() != cloud.getScalarFieldName(1):
    raise RuntimeError

radius = cc.GetPointCloudRadius([cloud])
print("radius: %s" % radius)
if not math.isclose(radius, 0.0293, rel_tol=1e-02):
    raise RuntimeError

radius = cc.GetPointCloudRadius(clouds=[cloud], nodes=6)
if not math.isclose(radius, 0.0207, rel_tol=1e-02):
    raise RuntimeError

#---curvature01-begin
radius = 0.03
res = cc.computeCurvature(cc.CurvatureType.GAUSSIAN_CURV, radius, [cloud])
nsf = cloud.getNumberOfScalarFields()
sfc = cloud.getScalarField(nsf - 1)

if sfc.getName() != "Gaussian curvature (0.03)":
    raise RuntimeError
#---curvature01-end

#---filterSFValue01-begin
cloud.setCurrentOutScalarField(nsf - 1)
fcloud = cc.filterBySFValue(0.01, sfc.getMax(), cloud)
filteredSize = fcloud.size()
#---filterSFValue01-end

print("filtered cloud size: %s" % filteredSize)
if not math.isclose(filteredSize, 113325, rel_tol=1e-03):
    raise RuntimeError

res = cc.SavePointCloud(fcloud, os.path.join(dataDir, "res3.xyz"))

#---cloudsf01-begin
radius = cc.GetPointCloudRadius([cloud], 12) # number of nodes wanted within the radius
if not cloud.computeScalarFieldGradient(1, radius, True):
    raise RuntimeError
#---cloudsf01-end

res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud03.bin"))

# example data available here: https://github.com/CloudCompare/CloudComPy/issues/94
if not os.path.isfile(os.path.join(dataExtDir,"PTS_LAMB93_IGN69_extract.las")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/PTS_LAMB93_IGN69_extract.las"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"PTS_LAMB93_IGN69_extract.las"), 'wb') as f:
        f.write(r.content)

#---cloudsf02-begin
cloud=cc.loadPointCloud(os.path.join(dataExtDir,"PTS_LAMB93_IGN69_extract.las"))
dic = cloud.getScalarFieldDic()
ret=cloud.applyScalarFieldGaussianFilter(dic["Intensity"])
sf=cloud.getScalarField(cloud.getNumberOfScalarFields()-1)
#---cloudsf02-end
sfName = sf.getName()
if "Intensity.smooth" not in sfName:
    raise RuntimeError

#---cloudsf03-begin
ret=cloud.sfBilateralFilter(dic["Intensity"])
sf=cloud.getScalarField(cloud.getNumberOfScalarFields()-1)
#---cloudsf03-end
sfName = sf.getName()
if "Intensity.bilsmooth" not in sfName:
    raise RuntimeError

res = cc.SavePointCloud(cloud, os.path.join(dataDir, "PTS_LAMB93_IGN69_extract.bin"))

#---cloudsf04-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))
res = cloud.exportCoordToSF(False, True, True)
l = cloud.sfSplitCloud(cloud.getNumberOfScalarFields()-1)
#---cloudsf04-end
if len(l) != 7:
    raise RuntimeError
res = cc.SaveEntities(l, os.path.join(dataDir, "sfsplit.bin"))

