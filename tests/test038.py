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

#---PCLregistration01-begin
cloud1 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))

cloud2 = cloud1.cloneThis()
tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.1, (0., 0.1, 0.9), (0.,0.,0.))
cloud2.applyRigidTransformation(tr1)

cc.computeNormals([cloud1, cloud2])

cloud2ref = cloud2.cloneThis()
cloud2bis = cloud2ref.cloneThis()
#---PCLregistration01-end

#---PCLregistration02-begin
if cc.isPluginPCL():
    import cloudComPy.PCL
    #---PCLregistration02-end
    
    # --- pCL registration
    
    #---PCLregistration03-begin
    fpcl = cc.PCL.FastGlobalRegistrationFilter()
    fpcl.setParameters(cloud1, [cloud2])
    res=fpcl.compute()
    #---PCLregistration03-end
    
    tr2 = fpcl.getTransformation()             # get the transformation applied to cloud2
    cloud2bis.applyRigidTransformation(tr2)    # apply it to the clone of cloud2 before registration, to check

    bb1=cloud1.getOwnBB()
    bb2 = cloud2ref.getOwnBB()
    bb3 = cloud2.getOwnBB()
    bb4 = cloud2bis.getOwnBB()
    
    if isCoordEqual(bb1.minCorner(), bb2.minCorner(), 1.e-2) or isCoordEqual(bb1.maxCorner(), bb2.maxCorner(), 1.e-2):
        raise RuntimeError

    if not isCoordEqual(bb1.minCorner(), bb3.minCorner(), 3.e-2) or not isCoordEqual(bb1.maxCorner(), bb3.maxCorner(), 3.e-2):
        raise RuntimeError
    if not isCoordEqual(bb4.minCorner(), bb3.minCorner(), 1.e-6) or not isCoordEqual(bb4.maxCorner(), bb3.maxCorner(), 1.e-6):
        raise RuntimeError
    
    # --- PCL normals and curvature
    
    cloud3 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
    ne = cc.PCL.NormalEstimation()
    ne.setParameters(cloud3)
    res=ne.compute()
    if not cloud3.hasNormals():
        raise RuntimeError
    if not cloud3.hasScalarFields():
        raise RuntimeError
    dic = cloud3.getScalarFieldDic()
    if not 'curvature' in dic.keys():
        raise RuntimeError
   
    # --- PCL smoothing and normals
    
    cloud4 = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
    su=cc.PCL.MLSSmoothingUpsampling()
    su.setParameters(cloud4, searchRadius=0.3, upsampleMethod=1)
    res=su.compute()
    cloud5=su.getOutputCloud()
    if cloud5.getName() != 'dataSample2_3.0_0_0.1 - Cloud_smoothed':
        raiseRuntimeError
    if cloud5.size() != 50000:
        raiseRuntimeError
        
    shapes = [cloud1, cloud2ref, cloud2, cloud2bis, cloud3, cloud4, cloud5]
    cc.SaveEntities(shapes, os.path.join(dataDir, "cloudsPCL.bin"))
