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

from gendata import getSampleCloud, getSamplePoly2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

#---computeVol25D01-begin
cloud = cc.loadPointCloud(getSampleCloud(5.0))

report = cc.ReportInfoVol()
isOk = cc.ComputeVolume25D(report, ground=None, ceil=cloud, 
                           vertDim=2, gridStep=0.05, groundHeight=0, ceilHeight=0)
#---computeVol25D01-end

#---computeVol25D02-begin
if not isOk:
    raise RuntimeError
if not math.isclose(report.volume, 0.995, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.surface, 101.002, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.addedVolume, 11.726, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.removedVolume, 10.731, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.matchingPercent, 100., rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.ceilNonMatchingPercent, 0., abs_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.groundNonMatchingPercent, 0., abs_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.averageNeighborsPerCell, 8., abs_tol=1e-03):
    raise RuntimeError
#---computeVol25D02-end

#---computeVol25D03-begin
cloud2 = cc.loadPointCloud(getSampleCloud(2.0))
cloud2.translate((1,2, -3)) # creates a translated floor, 
                            # with a non matching part with the ceil 

report = cc.ReportInfoVol()
isOk = cc.ComputeVolume25D(report, ground=cloud2, ceil=cloud,
                           vertDim=2, gridStep=0.05, groundHeight=0, ceilHeight=0,
                           projectionType=cc.PROJ_MINIMUM_VALUE,
                           groundEmptyCellFillStrategy=cc.INTERPOLATE_DELAUNAY,
                           groundMaxEdgeLength=0,
                           ceilEmptyCellFillStrategy=cc.INTERPOLATE_DELAUNAY,
                           ceilMaxEdgeLength=0) 
#---computeVol25D03-end

#---computeVol25D04-begin
if not isOk:
    raise RuntimeError
if not math.isclose(report.volume, 299.07, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.surface, 133.152, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.addedVolume, 301.305, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.removedVolume, 2.237, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.matchingPercent, 100., rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.ceilNonMatchingPercent, 0, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.groundNonMatchingPercent, 0., rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.averageNeighborsPerCell, 8.0, rel_tol=1e-03):
    raise RuntimeError
#---computeVol25D04-end


