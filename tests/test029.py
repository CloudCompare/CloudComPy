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

from gendata import getSampleCloud, dataDir, isCoordEqual
import cloudComPy as cc
import numpy as np

cloud = cc.loadPointCloud(getSampleCloud(5.0))
if cloud.hasColors():
    raise RuntimeError

#---colors01-begin
cloud.colorize(0.2, 0.3, 0.4, 1.0) 
if not cloud.hasColors():
    raise RuntimeError

col = cc.QColor.fromRgb(32,48,64) 
if not cloud.setColor(col):
    raise RuntimeError

c1=cc.QColor.fromRgbF(1.0, 0., 0.)
c2=cc.QColor.fromRgbF(0., 0., 1.0)
if not cloud.setColorGradient(0, c1, c2):
    raise RuntimeError

if not cloud.setColorGradientDefault(2):
    raise RuntimeError

if not cloud.setColorGradientBanded(1, 3.0):
    raise RuntimeError

if not cloud.changeColorLevels(60, 180, 20, 220, 1, 1, 1):
    raise RuntimeError

if not cloud.convertRGBToGreyScale():
    raise RuntimeError
#---colors01-end

#---colors02-begin
cloud.exportCoordToSF(True, True, True)
cloud.setCurrentDisplayedScalarField(0)

if not cloud.convertCurrentScalarFieldToColors(mixWithExistingColor=False):
    raise RuntimeError

if not cloud.enhanceRGBWithIntensitySF(sfIdx=1):
    raise RuntimeError

n1 = cloud.getNumberOfScalarFields()

cloud.sfFromColor(True, True, True, False, False) # export R, G, B, not alpha, not composite

n2 = cloud.getNumberOfScalarFields()
if (n2-n1) != 3:
    raise RuntimeError
#---colors02-end

#---colors03-begin
cloud.unallocateColors()
if cloud.hasColors():
    raise RuntimeError
#---colors03-end


cloud.exportCoordToSF(True, True, True)
cloud.setCurrentDisplayedScalarField(0)
if not cloud.convertCurrentScalarFieldToColors():
    raise RuntimeError

#---colors04-begin
cloud1 = cc.loadPointCloud(getSampleCloud(1.0))

if not cloud1.interpolateColorsFrom(cloud):
    raise RuntimeError
#---colors04-end

#---colorsArray01-begin
cola = cloud.colorsToNpArray()
if not cola.shape == (1000000, 4):
    raise RuntimeError
if not cola.dtype == 'uint8':
    raise RuntimeError

colaCopy = cloud.colorsToNpArrayCopy()
if not colaCopy.shape == (1000000, 4):
    raise RuntimeError
if not colaCopy.dtype == 'uint8':
    raise RuntimeError
#---colorsArray01-end

diff = cola - colaCopy
if diff.mean() != 0:
    raise RuntimeError

cloud.setCurrentDisplayedScalarField(2)
cloud.convertCurrentScalarFieldToColors()
diff2 = cola - colaCopy
if diff2.mean() == 0:
    raise RuntimeError

diff2[:,3]=128
cloud1.colorsFromNPArray_copy(diff2)

cc.SaveEntities([cloud, cloud1], os.path.join(dataDir, "colors.bin"))
