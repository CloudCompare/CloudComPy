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

os.environ["_CCTRACE_"] = "ON"  # only if you want C++ debug traces

from gendata import dataDir, dataExtDir, isCoordEqual

import cloudComPy as cc

#---sensor001-begin
# example data available here: http://sourceforge.net/projects/e57-3d-imgfmt/files/E57Example-data/manitouNoInvalidPoints.e57/download
if not os.path.isfile(os.path.join(dataExtDir,"manitouNoInvalidPoints.e57")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/manitouNoInvalidPoints.e57"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"manitouNoInvalidPoints.e57"), 'wb') as f:
        f.write(r.content)
        
entities = cc.importFile(os.path.join(dataExtDir,"manitouNoInvalidPoints.e57"))
#---sensor001-end

#---sensor002-begin
for entity in entities[1]:
    entity.getName()
    sensors = entity.getSensors()
    if len(sensors) < 1:
        raise RuntimeError
    sensor= sensors[0]
    if sensor.getType() != cc.CC_SENSOR_TYPE.GROUND_BASED_LIDAR:
        raise RuntimeError
    if sensor.getClassID() != cc.CC_TYPES.GBL_SENSOR:
        raise RuntimeError
    grscale = sensor.getGraphicScale()
    print("graphic scale:", grscale)
    tr=sensor.getRigidTransformation()
    ret1=tr.getParameters1()
    alpha = ret1.alpha_rad*180/math.pi
    print("alpha:", alpha)
    axis = ret1.axis3D
    print("axis", axis)
    trans = ret1.t3D
    print("translation:", trans)
    uncertainty = sensor.getUncertainty()
    print("uncertainty", uncertainty)
#---sensor002-end

#---sensor003-begin
    structure = entities[-1]
#---sensor003-end
    if len(structure)!= 343:
        raise RuntimeError
    for str in structure:
        print(str)

#---sensor004-begin
cc.computeNormals(entities[1])
for entity in entities[1]:
    sensors = entity.getSensors()
    if len(sensors) < 1:
        raise RuntimeError
    sensor= sensors[0]
    sensor.ComputeScatteringAngles()
#---sensor004-end

res = cc.SaveEntities(entities[1], os.path.join(dataDir, "manitouNoInvalidPoints.bin"))

#--- check files with invalid points (invalid points are discarded and scalar fields should be of correct size)
# example data available here: http://sourceforge.net/projects/e57-3d-imgfmt/files/E57Example-data/manitou.e57/download
if not os.path.isfile(os.path.join(dataExtDir,"manitou.e57")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/manitou.e57"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"manitou.e57"), 'wb') as f:
        f.write(r.content)
        
entities = cc.importFile(os.path.join(dataExtDir,"manitou.e57"))
for entity in entities[1]:
    print(entity.getName(), "size", entity.size())
    sf = entity.getScalarField(0)
    print("sf.currentSize()", sf.currentSize())
    if (sf.currentSize() > entity.size()):
        raise RuntimeError
res = cc.SaveEntities(entities[1], os.path.join(dataDir, "manitou.bin"))

#---E57filter-begin
entities = cc.importFile(os.path.join(dataExtDir,"manitouNoInvalidPoints.e57"), extraData="sp2.*_1")
#---E57filter-end
if len(entities[1]) != 2:
    raise RuntimeError
res = cc.SaveEntities(entities[1], os.path.join(dataDir, "manitou_filtered.bin"))

