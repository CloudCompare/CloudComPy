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

#---render001-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))
res = cloud.exportCoordToSF(True, True, True)
sfx=cloud.getScalarField(0)
sfy=cloud.getScalarField(1)
sfz=cloud.getScalarField(2)

colorScalesManager = cc.ccColorScalesManager.GetUniqueInstance()
scale=colorScalesManager.getDefaultScale(cc.DEFAULT_SCALES.VIRIDIS)
sfx.setColorScale(scale)
#---render001-end

#---render002-begin
newScale=cc.ccColorScale.Create("myNewScale")
newScale.insert(0, cc.QColor("blue"))
newScale.insert(1, cc.QColor("red"))
newScale.insert(0.5, cc.QColor("green"))
sfy.setColorScale(newScale)
#---render002-end

aNewScale=cc.ccColorScale.Create("aNewScale", "aNewScale")
aNewScale.insert(0, cc.QColor.fromRgb(128,64,0))
aNewScale.insert(1, cc.QColor.fromRgb(0,255,64))
aNewScale.insert(0.3, cc.QColor.fromRgb(0,128,128))
aNewScale.insert(0.7, cc.QColor.fromRgb(255,64,128))
sfz.setColorScale(aNewScale)

res = cc.SaveEntities([cloud], os.path.join(dataDir, "scales.bin"))

cloud.setCurrentScalarField(1)
cloud.setCurrentDisplayedScalarField(1)
cc.addToRenderScene(cloud)
cc.setIsoView1()
cc.setGlobalZoom()
cc.setBackgroundColor(False, 255, 255, 255)
cc.render(os.path.join(dataDir, "renderScale.png"), 2000,1500)

# --- save a colorScale manager with new scales in persistent settings does not work properly yet from Python.
#colorScalesManager.addScale(newScale)
#colorScalesManager.addScale(aNewScale)
#colorScalesManager.toPersistentSettings()
