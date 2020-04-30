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

# --- Test of memory usage and ownership when exporting data to Numpy (copy or reference)

import os
import sys
import math
import psutil
from gendata import getSampleCloud, dataDir
from PyQt5.QtWidgets import QApplication
import cloudCompare as cc

app = QApplication(sys.argv)
cc.initNumpy.init()  # to do once before dealing with numpy

thisProcess = psutil.Process()
startMem = thisProcess.memory_full_info().uss
maxMem = startMem
curMem = startMem
print("memory USS, before loading point cloud = REFERENCE: %s" % startMem)

cloud = cc.loadPointCloud(getSampleCloud(5.0))
npts = cloud.size()
if cc.initNumpy.getScalarType() == 'float32':
    sizePoint = 4
else:
    sizePoint = 8
sfMem = npts * sizePoint
coordMem = 3 * sfMem
print("memory size of the coordinates, bytes: %s" % coordMem)

curMem = thisProcess.memory_full_info().uss
if curMem > maxMem:
    maxMem = curMem
print(
    "memory USS, after loading point cloud, delta from REFERENCE: %s" %
     (curMem - startMem))
refMem = curMem

# --- transfer cloud coordinates to Numpy Array, test memory without copy, and with copy

coords = None

for i in range(10):
    coords = cloud.toNpArray(False)
    if coords.shape != (cloud.size(), 3):
        raise RuntimeError
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("coord no copy %s" % i)
    if (maxMem - refMem) > 0.1 * coordMem:
        raise RuntimeError

for i in range(10):
    coords = cloud.toNpArray(True)
    if coords.shape != (cloud.size(), 3):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("coord copy %s" % i)
    if (maxMem - refMem) > 1.1 * coordMem:
        raise RuntimeError

coords = None  # clean memory

curMem = thisProcess.memory_full_info().uss
if curMem > maxMem:
    maxMem = curMem
print(
    "memory USS, after cleaning, delta from REFERENCE: %s" %
     (curMem - startMem))

# --- transfer scalarField to Numpy Array, test memory without copy, and with copy

res = cloud.exportCoordToSF((False, False, True))
sf = cloud.getScalarField(0)

curMem = thisProcess.memory_full_info().uss
print(
    "memory USS, after loading scalar field, delta from REFERENCE: %s" %
     (curMem - startMem))
refMem = curMem
maxMem = curMem

for i in range(10):
    coords = sf.toNpArray(False)
    if coords.shape != (cloud.size(),):
        raise RuntimeError
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("scalarField no copy %s" % i)
    if (maxMem - refMem) > 0.1 * sfMem:
        raise RuntimeError

for i in range(10):
    coords = sf.toNpArray(True)
    if coords.shape != (cloud.size(),):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("scalarField copy %s" % i)
    if (maxMem - refMem) > 1.1 * sfMem:
        raise RuntimeError
