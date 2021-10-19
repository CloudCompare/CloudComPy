#!/usr/bin/env python3

##########################################################################
#                                                                        #
#                                PYCC                                    #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU Library General Public License as       #
#  published by the Free Software Foundation; version 2 or later of the  #
#  License.                                                              #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
#                                                                        #
##########################################################################

import os
import sys
import math
import numpy as np

from gendata import getSampleCloud, getSamplePoly2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc
createSymbolicLinks() # required for tests on build, before cc.initCC

cc.initCC()  # to do once before using plugins or dealing with numpy

cloud = cc.loadPointCloud(getSampleCloud(5.0))

report = cc.ReportInfoVol()

isOk = cc.ComputeVolume25D(report, None, cloud, 2, 0.05, 0, 0)
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

cloud2 = cc.loadPointCloud(getSampleCloud(2.0))
cloud2.translate((1,2, -3))

report = cc.ReportInfoVol()

isOk = cc.ComputeVolume25D(report, cloud2, cloud, 2, 0.05, 0, 0)
if not isOk:
    raise RuntimeError
if not math.isclose(report.volume, 216.609, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.surface, 72.852, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.addedVolume, 216.609, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.removedVolume, 0., rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.matchingPercent, 56.4, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.ceilNonMatchingPercent, 21.8, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.groundNonMatchingPercent, 21.8, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(report.averageNeighborsPerCell, 7.93, rel_tol=1e-03):
    raise RuntimeError


