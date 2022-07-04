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

from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init

#---ICP01-begin
cloud1 = cc.loadPointCloud(getSampleCloud(5.0))
cloud1.setName("cloud1")

cloud2ref = cc.loadPointCloud(getSampleCloud(5.0, 9.0))
cloud2ref.setName("cloud2_reference")

cloud2 = cloud2ref.cloneThis()
tr1 = cc.ccGLMatrix()
# --------------------  z -- y -- x  (rotation x 0.1, translation z 0.3)
tr1.initFromParameters(0.0, 0.0, 0.1, (0.0, 0.0, 0.3))
cloud2.applyRigidTransformation(tr1)
cloud2.setName("cloud2_transformed")

cc.SaveEntities([cloud1, cloud2ref, cloud2], os.path.join(dataDir, "clouds2.bin"))
#---ICP01-end

#---ICP02-begin
res=cc.ICP(data=cloud2, model=cloud1, minRMSDecrease=1.e-5,
           maxIterationCount=20, randomSamplingLimit=50000, removeFarthestPoints=False,
           method=cc.CONVERGENCE_TYPE.MAX_ITER_CONVERGENCE,
           adjustScale=False, finalOverlapRatio=0.1)
tr2 = res.transMat
cloud3 = res.aligned
cloud3.applyRigidTransformation(tr2)
cloud3.setName("cloud2_transformed_afterICP")
#---ICP02-end

#---C2C01-begin
stats = cc.DistanceComputationTools.computeApproxCloud2CloudDistance(cloud2ref, cloud3)
print(stats) # min, max, mean, variance, error max
#---C2C01-end

#---C2C02-begin
nbCpu = psutil.cpu_count()
bestOctreeLevel = cc.DistanceComputationTools.determineBestOctreeLevel(cloud2ref, None, cloud3)
params = cc.Cloud2CloudDistancesComputationParams()
params.maxThreadCount = nbCpu
params.octreeLevel = bestOctreeLevel
cc.DistanceComputationTools.computeCloud2CloudDistances(cloud2ref, cloud3, params)
#---C2C02-end

sf = cloud2ref.getScalarField(cloud2ref.getNumberOfScalarFields()-1)
mindist = sf.getMin()
maxdist = sf.getMax()
if mindist < 0:
    raise RuntimeError
if maxdist > 0.04:
    raise RuntimeError

cc.SaveEntities([cloud1, cloud2, cloud2ref, cloud3], os.path.join(dataDir, "clouds3.bin"))




