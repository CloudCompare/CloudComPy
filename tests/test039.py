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

from gendata import getSampleCloud2, getSamplePoly, dataDir
import cloudComPy as cc

#---crop2Dmesh01-begin
cloud1 = cc.loadPointCloud(getSampleCloud2(3.0,0, 0.1))
cloud1.setName("cloud1")
cloud1.exportCoordToSF(True, True, True)

mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh1.setName("mesh1")
cc.computeNormals([mesh1], computePerVertexNormals=False)

poly = cc.loadPolyline(getSamplePoly("poly1"))
meshCropZ = mesh1.crop2D(poly, 2, True)
#---crop2Dmesh01-end

res = cc.SaveEntities([cloud1, mesh1, meshCropZ], os.path.join(dataDir, "cropMesh.bin"))
