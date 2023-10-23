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
import requests

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import dataDir, dataExtDir
import cloudComPy as cc

# example data available here: http://sourceforge.net/projects/e57-3d-imgfmt/files/E57Example-data/
if not os.path.isfile(os.path.join(dataExtDir,"pumpARowColumnIndexNoInvalidPoints.e57")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/pumpARowColumnIndexNoInvalidPoints.e57"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"pumpARowColumnIndexNoInvalidPoints.e57"), 'wb') as f:
        f.write(r.content)

#---extractCC01-begin
cloud = cc.loadPointCloud(os.path.join(dataExtDir, "pumpARowColumnIndexNoInvalidPoints.e57"))
res = cc.ExtractConnectedComponents(clouds=[cloud], randomColors=True)
cloud01 = cc.MergeEntities(res[1], createSFcloudIndex=True)
cloud01.setName("pump_Extract_Components")
cloud02 = cc.MergeEntities(res[2], createSFcloudIndex=True)
cloud02.setName("pump_residual_Components")
res2 = res[1] + res[2] # connected components plus regrouped residual components
cloud2 = cc.MergeEntities(res2, createSFcloudIndex=True)
cloud2.setName("pump_extract_Residual_Components")
cloud3 = cc.MergeEntities(res2, deleteOriginalClouds=True)
cloud3.setName("pump_extract_Residual_Components")
res = None
#---extractCC01-end


tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.1, 0.2, 0.3, (8.0, 0.0, 0.0))
box = cc.ccBox((1., 2., 3.), tr1, "aBox")

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.5, (0., 1., 0.), (5.0, 6.0, 3.0))
cone = cc.ccCone(3., 1., 2., 0., 0., tr2, "aCone", 12)

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0., (0., 0., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 3.0, tr3, 'aCylinder', 48)

#---merge02-begin
mesh1 = cc.MergeEntities([box, cone, cylinder])
mesh2 = cc.MergeEntities([box, cone, cylinder], createSubMeshes=True)
#---merge02-end

cc.SaveEntities([cloud, cloud01, cloud02, cloud2, cloud3, box, cone, cylinder, mesh1, mesh2], os.path.join(dataDir, "merge.bin"))
