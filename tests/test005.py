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
cc.initCC.init()  # to do once before using plugins or dealing with numpy

thisProcess = psutil.Process()
startMem = thisProcess.memory_full_info().uss
maxMem = startMem
curMem = startMem
print("memory USS, before loading point cloud = REFERENCE: %s" % startMem)

cloud = cc.loadPointCloud(getSampleCloud(5.0))
npts = cloud.size()
if cc.initCC.getScalarType() == 'float32':
    sizePoint = 4
else:
    sizePoint = 8
sfMem = npts * sizePoint
coordMem = 3 * sfMem
print("memory size of the scalar field, bytes: %s" % sfMem)
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
print("=========== loop coords no copy =============")
for i in range(10):
    coords = cloud.toNpArray(False)
    if coords.shape != (cloud.size(), 3):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("coord no copy %s" % i)
    if (maxMem - refMem) > 0.15 * coordMem: # some memory not immediately released ?
        raise RuntimeError

print("=========== loop coords with copy =============")
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
    if (maxMem - refMem) > 2.0 * coordMem: # some memory not immediately released ?
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

print("=========== loop scalar field no copy =============")
for i in range(10):
    coords = sf.toNpArray(False)
    if coords.shape != (cloud.size(),):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("scalarField no copy %s" % i)
    if (maxMem - refMem) > 0.15 * sfMem: # some memory not immediately released ?
        raise RuntimeError

print("=========== loop scalar field with copy =============")
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
    if (maxMem - refMem) > 2.0 * sfMem: # some memory not immediately released ?
        raise RuntimeError
