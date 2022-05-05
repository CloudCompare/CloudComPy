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

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual
import cloudComPy as cc

cloud1 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
cloud2 = cloud1.cloneThis()
tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.1, (0., 0.1, 0.9), (0.,0.,0.))
cloud2.applyRigidTransformation(tr1)
cc.computeNormals([cloud1, cloud2])
cloud2ref = cloud2.cloneThis()

if cc.isPluginPCL():
    import cloudComPy.PCL
    fpcl = cc.PCL.FastGlobalRegistrationFilter()
    fpcl.setParameters(cloud1, [cloud2], 0.15)
    res=fpcl.compute()

    bb1=cloud1.getOwnBB()
    bb2 = cloud2ref.getOwnBB()
    bb3 = cloud2.getOwnBB()

    if isCoordEqual(bb1.minCorner(), bb2.minCorner(), 1.e-2) or isCoordEqual(bb1.maxCorner(), bb2.maxCorner(), 1.e-2):
        raise RuntimeError

    if not isCoordEqual(bb1.minCorner(), bb3.minCorner(), 1.e-2) or not isCoordEqual(bb1.maxCorner(), bb3.maxCorner(), 1.e-2):
        raise RuntimeError

    shapes = [cloud1, cloud2ref, cloud2]
    cc.SaveEntities(shapes, os.path.join(dataDir, "cloudsPCL.bin"))
