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
from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init
cc.initCC()  # to do once before using plugins or dealing with numpy
cloud = cc.loadPointCloud(getSampleCloud(5.0))

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0., (0., 0., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 3.0, tr3, 'aCylinder', 48)

params = cc.Cloud2MeshDistancesComputationParams()
params.maxThreadCount=12
params.octreeLevel=6
cc.DistanceComputationTools.computeCloud2MeshDistances(cloud, cylinder, params)

# --- save distances histogram, png file

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib import colors
matplotlib.use('agg') # png images

sf=cloud.getScalarField(0)
asf= sf.toNpArray()

(n, bins, patches) = plt.hist(asf, bins=256, density=1) # histogram for matplotlib
fracs = bins / bins.max()
norm = colors.Normalize(fracs.min(), fracs.max())
for thisfrac, thispatch in zip(fracs, patches):
    color = plt.cm.rainbow(norm(thisfrac))
    thispatch.set_facecolor(color)

plt.savefig(os.path.join(dataDir, "histogram.png"))

# --- save distances histogram, csv file

import csv
(n, bins) = np.histogram(asf, bins=256) # numpy histogram (without graphics)
with open(os.path.join(dataDir, "histogram.csv"), 'w') as f:
    writer = csv.writer(f, delimiter=';')
    writer.writerow(("Class", "Value", "Class start", "Class end"))
    for i in range(n.size):
        writer.writerow((i, n[i], bins[i], bins[i+1]))

cc.SaveEntities([cloud, cylinder], os.path.join(dataDir, "cloudCylinder.bin"))
