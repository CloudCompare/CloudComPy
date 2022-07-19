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
import psutil

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, getSamplePoly, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC.init

cloud = cc.loadPointCloud(getSampleCloud(5.0))

tr3 = cc.ccGLMatrix()
tr3.initFromParameters(0., (0., 0., 0.), (3.0, 0.0, 4.0))
cylinder = cc.ccCylinder(0.5, 3.0, tr3, 'aCylinder', 48)

nbCpu = psutil.cpu_count()
bestOctreeLevel = cc.DistanceComputationTools.determineBestOctreeLevel(cloud,cylinder)
params = cc.Cloud2MeshDistancesComputationParams()
params.maxThreadCount = nbCpu
params.octreeLevel = bestOctreeLevel
cc.DistanceComputationTools.computeCloud2MeshDistances(cloud, cylinder, params)

# --- save distances histogram, png file

#---histogram01-begin
sf = cloud.getScalarField(cloud.getScalarFieldDic()['C2M absolute distances'])
asf = sf.toNpArray()
#---histogram01-end

#---histogram02-begin
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib import colors
#---histogram02-end

#---histogram03-begin
matplotlib.use('agg') # png images

(n, bins, patches) = plt.hist(asf, bins=256, density=1) # histogram for matplotlib
fracs = bins / bins.max()
norm = colors.Normalize(fracs.min(), fracs.max())
for thisfrac, thispatch in zip(fracs, patches):
    color = plt.cm.rainbow(norm(thisfrac))
    thispatch.set_facecolor(color)

plt.savefig(os.path.join(dataDir, "histogram.png"))
#---histogram03-end

# --- save distances histogram, csv file

#---histogram04-begin
import numpy as np
import csv
#---histogram04-end

#---histogram05-begin
(n, bins) = np.histogram(asf, bins=256) # numpy histogram (without graphics)
with open(os.path.join(dataDir, "histogram.csv"), 'w') as f:
    writer = csv.writer(f, delimiter=';')
    writer.writerow(("Class", "Value", "Class start", "Class end"))
    for i in range(n.size):
        writer.writerow((i, n[i], bins[i], bins[i+1]))
#---histogram05-end

cc.SaveEntities([cloud, cylinder], os.path.join(dataDir, "cloudCylinder.bin"))
