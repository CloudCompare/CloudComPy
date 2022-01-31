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
from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init
cc.initCC()  # to do once before using plugins or dealing with numpy

cloud1 = cc.loadPointCloud(getSampleCloud(5.0))
cloud1.setName("cloud1")

cloud2ref = cc.loadPointCloud(getSampleCloud(5.0, 9.0))
cloud2ref.setName("cloud2_reference")

cloud2 = cloud2ref.cloneThis()
tr1 = cc.ccGLMatrix()
# -------------------- z -- y -- x
tr1.initFromParameters(0.0, 0.0, 0.1, (0.0, 0.0, 0.3))
cloud2.applyRigidTransformation(tr1)
cloud2.setName("cloud2_transformed")

cc.SaveEntities([cloud1, cloud2ref, cloud2], os.path.join(dataDir, "clouds2.bin"))

res=cc.ICP(cloud2, cloud1, 1.e-5, 20, 50000, False, cc.CONVERGENCE_TYPE.MAX_ITER_CONVERGENCE, False, 0.1)
tr2 = res.transMat
cloud3 = res.aligned
cloud3.applyRigidTransformation(tr2)
cloud3.setName("cloud2_transformed_afterICP")

params = cc.Cloud2CloudDistancesComputationParams()
params.maxThreadCount=12
params.octreeLevel=6
cc.DistanceComputationTools.computeCloud2CloudDistances(cloud2ref, cloud3, params)

sf = cloud2ref.getScalarField(cloud2ref.getNumberOfScalarFields()-1)
mindist = sf.getMin()
maxdist = sf.getMax()
if mindist < 0:
    raise RuntimeError
if maxdist > 0.03:
    raise RuntimeError

cc.SaveEntities([cloud1, cloud2, cloud2ref, cloud3], os.path.join(dataDir, "clouds3.bin"))




