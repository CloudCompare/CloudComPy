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

from gendata import getSampleCloud, getSampleCloud2, dataDir, dataExtDir, isCoordEqual
import cloudComPy as cc

#---render001-begin
cloud1 = cc.loadPointCloud(os.path.join(dataDir, "boule.bin"))
cloud1.setCurrentScalarField(0)
cloud1.setCurrentDisplayedScalarField(0)
cc.addToRenderScene(cloud1)

struct = cc.importFile(os.path.join(dataExtDir, "manitou.e57"))
for cloud in struct[1]:
    cc.computeNormals([cloud])
    cloud.showNormals(True)
    cc.addToRenderScene(cloud)

cc.render(os.path.join(dataDir, "rendera.png"), 2000,1500)
#---render001-end

#---render002-begin
cc.removeFromRenderScene(cloud1)
cloud1=None
#---render002-end

cc.setOrthoView()
cc.setGlobalZoom()
cc.render(os.path.join(dataDir, "renderb.png"), 2000,1500)

cc.setFrontView()
cc.render(os.path.join(dataDir, "renderFront.png"), 2000,1500)

cc.setBottomView()
cc.render(os.path.join(dataDir, "renderBottom.png"), 2000,1500)

cc.setTopView()
cc.render(os.path.join(dataDir, "renderTop.png"), 2000,1500)

cc.setBackView()
cc.render(os.path.join(dataDir, "renderBack.png"), 2000,1500)

cc.setLeftView()
cc.render(os.path.join(dataDir, "renderLeft.png"), 2000,1500)

cc.setRightView()
cc.render(os.path.join(dataDir, "renderRight.png"), 2000,1500)

cc.setIsoView1()
cc.render(os.path.join(dataDir, "renderIso1.png"), 2000,1500)

cc.setIsoView2()
cc.render(os.path.join(dataDir, "renderIso2.png"), 2000,1500)

cc.setCustomView((3., 5., 7.), (1., 2., 3.))
cc.render(os.path.join(dataDir, "renderCustom.png"), 2000,1500)


cc.setCameraPos((2., -15, 90.))
cc.render(os.path.join(dataDir, "renderCamPos1.png"), 2000,1500)


cc.setCameraPos((15., -15, 90.))
cc.render(os.path.join(dataDir, "renderCamPos2.png"), 2000,1500)

for cloud in struct[1]:
    cc.removeFromRenderScene(cloud)
struct = None

#---render003-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))
cloud.exportCoordToSF(False, False, True)
cloud.setCurrentScalarField(0)
cloud.setCurrentDisplayedScalarField(0)
cloud.showSFColorsScale(True)
cc.addToRenderScene(cloud)

cc.setOrthoView()
cc.setGlobalZoom()
cc.setBackgroundColor(False, 255, 255, 255)
cc.setTextDefaultCol(0, 0, 0)
cc.setColorScaleShowHistogram(True)
cc.render(os.path.join(dataDir, "render0.png"), 2000,1500)

cc.setCameraPos((0., 0., 20.))
cc.setCustomView((0., 1., 0.), (0., 0., 1.))
cc.render(os.path.join(dataDir, "renderRef.png"), 2000,1500)

cc.setCenteredPerspectiveView()
cc.render(os.path.join(dataDir, "renderRefCenterPers.png"), 2000,1500)

dist=20.
alphaRad=math.pi/20.
cc.setCenteredPerspectiveView()
for i in range(15):
    cc.setCustomView((0., math.cos(i*alphaRad), -math.sin(i*alphaRad)), (0., math.sin(i*alphaRad), math.cos(i*alphaRad)))
    cc.setGlobalZoom()
    cc.render(os.path.join(dataDir, "renderangle_%d.png"%i), 2000,1500, False)
#---render003-end
