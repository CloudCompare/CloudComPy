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

from gendata import getSampleCloud, dataDir
import numpy as np
import cloudComPy as cc

cloud1 = cc.loadPointCloud(getSampleCloud(2.0))

ok = cloud1.exportCoordToSF(False, False, True)
if not ok:
    raise RuntimeError

res = cloud1.hasScalarFields()
print("hasScalarField: %s" % res)
if not res:
    raise RuntimeError

n = cloud1.getNumberOfScalarFields()
print("number of saclar fields: %s" % n)
if n != 1:
    raise RuntimeError

csfname0 = cloud1.getScalarFieldName(0)
print("cloud scalar field name: %s" % csfname0)

sf1 = cloud1.getScalarField(0)
sfname = sf1.getName()
print("scalar field name: %s" % sfname)
if sfname != "Coord. Z":
    raise RuntimeError

mean, var = sf1.computeMeanAndVariance()
print("mean: %14.7e, var: %14.7e" % (mean, var))
if not math.isclose(mean, 3.2965615e-02, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(var, 1.1403269e-01, rel_tol=1e-06):
    raise RuntimeError

sf1.computeMinAndMax()
sfmin = sf1.getMin()
sfmax = sf1.getMax()
print("min: %14.7e" % sfmin)
print("max: %14.7e" % sfmax)
if not math.isclose(sfmin, -4.3446699e-01, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(sfmax, 2.0000000e+00, rel_tol=1e-06):
    raise RuntimeError

asf1 = sf1.toNpArray()
print(asf1.size)
if asf1.size != 1000000:
    raise RuntimeError

cloud2 = cc.loadPointCloud(getSampleCloud(1.9))
res = cloud2.exportCoordToSF(False, False, True)
sf2 = cloud2.getScalarField(0)
asf2 = sf2.toNpArray()

sf2.fromNpArrayCopy(asf1)

res = cc.SavePointCloud(cloud2, os.path.join(dataDir, "res2.xyz"))
cloud2 = cc.loadPointCloud(os.path.join(dataDir, "res2.xyz"))
sf2 = cloud2.getScalarField(0)
sfname = sf2.getName()
print("scalar field name: %s" % sfname)
if sfname != "Scalar field":
    raise RuntimeError
asf2 = sf2.toNpArray()
ok = np.allclose(asf1, asf2, rtol=1.e-6)
if not ok:
    raise RuntimeError

#---sfNumpy01-begin
dic = cloud1.getScalarFieldDic()
sf1 = cloud1.getScalarField(dic['Coord. Z'])
max1 = sf1.getMax()
asf1 = sf1.toNpArray()   # access to Numpy array, without copy
asf1[0] = 2*max1         # modification in place
sf1.computeMinAndMax()
#---sfNumpy01-end

if not math.isclose(sf1.getMax(), 2*max1):
    raise RuntimeError

#---sfNumpy02-begin
sfname = sf1.getName()
sfmin = sf1.getMin()
sfmax = sf1.getMax()
mean, var = sf1.computeMeanAndVariance()
val = sf1.getValue(23)
#---sfNumpy02-end

#---sfNumpy03-begin
sf1.setName("aNewName")
sf1.fill(0.5)
sf1.setValue(3, 2.25) # index, value
#---sfNumpy03-end

