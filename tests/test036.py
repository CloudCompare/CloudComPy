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

from gendata import dataDir, isCoordEqual
import cloudComPy as cc

# --- define material to slice

tr1 = cc.ccGLMatrix()
tr1.initFromParameters(0.0, (0., 0., 0.), (-2.0, 0.0, 0.0))
sphere1 = cc.ccSphere(3.0, tr1)
c1 = sphere1.samplePoints(True, 1000)

tr2 = cc.ccGLMatrix()
tr2.initFromParameters(0.0, (0., 0., 0.), (+2.0, 0.0, 0.0))
sphere2 = cc.ccSphere(3.0, tr2)
c2 = sphere2.samplePoints(True, 1000)

cloud = c1.cloneThis()
cloud.fuse(c2)

toslice = [cloud, sphere1, sphere2] # one cloud, two meshes


# --- bounding box: sclice tool

bbox = cc.ccBBox((-5.0, -5.0, 0.), (5., 5., 0.5))

# --- single slice with the bounding box

res=cc.ExtractSlicesAndContours(entities=toslice, bbox=bbox)
if len(res[0]) !=1:
    raise RuntimeError
shapes = toslice + res[0]
cc.SaveEntities(shapes, os.path.join(dataDir, "slice0.bin"))

# --- repeat slices along Z with gap

res=cc.ExtractSlicesAndContours(entities=toslice, bbox=bbox, singleSliceMode=False, gap=0.5, generateRandomColors=True)
if len(res[0]) !=18:
    raise RuntimeError
shapes = res[0]
cc.SaveEntities(shapes, os.path.join(dataDir, "slices1.bin"))

# --- repeat slices along Z with gap, extract envelopes

res=cc.ExtractSlicesAndContours(entities=toslice, bbox=bbox, singleSliceMode=False, gap=0.5, generateRandomColors=True,
                                extractEnvelopes=True, maxEdgeLength=0.1, envelopeType=2)
if (len(res[0]) !=18) or (len(res[1]) !=6):
    raise RuntimeError
shapes = res[0] + res[1]
cc.SaveEntities(shapes, os.path.join(dataDir, "slices2.bin"))

# --- add a rotation of the bounding box

tr0 = cc.ccGLMatrix()
tr0.initFromParameters(math.pi/12., (0., 1., 0.), (0.0, 0.0, 0.0))
res=cc.ExtractSlicesAndContours(entities=toslice, bbox=bbox, bboxTrans=tr0, singleSliceMode=False, gap=0.5, generateRandomColors=True,
                                extractEnvelopes=True, maxEdgeLength=0.1, envelopeType=2)
if (len(res[0]) !=21) or (len(res[1]) !=8):
    raise RuntimeError
shapes = res[0] + res[1]
cc.SaveEntities(shapes, os.path.join(dataDir, "slices3.bin"))

# --- extract also contours

res=cc.ExtractSlicesAndContours(entities=toslice, bbox=bbox, bboxTrans=tr0, singleSliceMode=False, gap=0.5, generateRandomColors=True,
                                extractEnvelopes=True, maxEdgeLength=0.1, envelopeType=2,
                                extractLevelSet=True, levelSetGridStep=0.05, levelSetMinVertCount=100)
if (len(res[0]) !=21) or (len(res[1]) !=8) or (len(res[2]) !=20):
    raise RuntimeError
shapes = res[0] + res[1] +res[2]
cc.SaveEntities(shapes, os.path.join(dataDir, "slices4.bin"))

