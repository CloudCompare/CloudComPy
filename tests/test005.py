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

# --- Test of memory usage and ownership when exporting data to Numpy (copy or reference)

import os
import sys
import math
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, dataDir
import cloudComPy as cc

thisProcess = psutil.Process()
startMem = thisProcess.memory_full_info().uss
maxMem = startMem
curMem = startMem
print("memory USS, before loading point cloud = REFERENCE: %s" % startMem)

cloud = cc.loadPointCloud(getSampleCloud(5.0))
npts = cloud.size()
if cc.getScalarType() == 'float32':
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
    coords = cloud.toNpArray()
    if coords.shape != (cloud.size(), 3):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("coord no copy %s" % i)
    if (maxMem - refMem) > 0.1 * coordMem: # some memory not immediately released ?
        raise RuntimeError

coords = None
print("=========== loop coords with copy =============")
for i in range(10):
    coords = cloud.toNpArrayCopy()
    if coords.shape != (cloud.size(), 3):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("coord copy %s" % i)
    if (maxMem - refMem) > 2.1 * coordMem: # some memory not immediately released ?
        raise RuntimeError

coords = None  # clean memory

curMem = thisProcess.memory_full_info().uss
if curMem > maxMem:
    maxMem = curMem
print(
    "memory USS, after cleaning, delta from REFERENCE: %s" %
     (curMem - startMem))

# --- transfer scalarField to Numpy Array, test memory without copy, and with copy

res = cloud.exportCoordToSF(False, False, True)
sf = cloud.getScalarField(0)

curMem = thisProcess.memory_full_info().uss
print(
    "memory USS, after loading scalar field, delta from REFERENCE: %s" %
     (curMem - startMem))
refMem = curMem
maxMem = curMem

asf = None
print("=========== loop scalar field no copy =============")
for i in range(10):
    asf = sf.toNpArray()
    if asf.shape != (cloud.size(),):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("scalarField no copy %s" % i)
    if (maxMem - refMem) > 0.1 * sfMem: # some memory not immediately released ?
        raise RuntimeError

asf = None
print("=========== loop scalar field with copy =============")
for i in range(10):
    asf = sf.toNpArrayCopy()
    if asf.shape != (cloud.size(),):
        raise RuntimeError
    curMem = thisProcess.memory_full_info().uss
    if curMem > maxMem:
        maxMem = curMem
    print("memory USS, delta current: %s max: %s" %
          (curMem - refMem, maxMem - refMem))
    print("scalarField copy %s" % i)
    if (maxMem - refMem) > 2.0 * sfMem: # some memory not immediately released ?
        raise RuntimeError

asf = None
curMem = thisProcess.memory_full_info().uss
if curMem > maxMem:
    maxMem = curMem
print(
    "memory USS, after cleaning, delta from REFERENCE: %s" %
     (curMem - startMem))
