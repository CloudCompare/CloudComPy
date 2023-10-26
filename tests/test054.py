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
import numpy as np

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual
import cloudComPy as cc

#---normals01-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))
cc.computeNormals([cloud])

cloud.exportNormalToSF(True, True, True)
dic = cloud.getScalarFieldDic()
sfx = cloud.getScalarField(dic['Nx'])
sfy = cloud.getScalarField(dic['Ny'])
sfz = cloud.getScalarField(dic['Nz'])
asfx = sfx.toNpArray()
asfy = sfy.toNpArray()
asfz = sfz.toNpArray()

normals=cloud.normalsToNpArrayCopy()

dx = normals[:,0] -asfx
dy = normals[:,1] -asfy
dz = normals[:,2] -asfz

if dx.max() != 0 or dx.min() !=0:
    raise RuntimeError

if dy.max() != 0 or dy.min() !=0:
    raise RuntimeError

if dz.max() != 0 or dz.min() !=0:
    raise RuntimeError
#---normals01-end

#---normals02-begin
normals *=-0.5 # an example of modification of the normals: invert and denormalize
cloud.normalsFromNpArrayCopy(normals)
#---normals02-end

res = cc.SaveEntities([cloud], os.path.join(dataDir, "cloudnormals.bin"))
if res != cc.CC_FILE_ERROR.CC_FERR_NO_ERROR:
    raise RuntimeError

