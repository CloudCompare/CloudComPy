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

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

#---createCloudNumpy01-begin
# --- generate a set of coords and a scalar field

npts = 10000000
theta = 2*np.pi*np.random.random((npts))
z = 10*np.random.random((npts))
dr = 0.1*np.sin(2*2*np.pi*z + theta)
r = 5+np.cos(z*np.pi/10) + dr
x = np.float32(r*np.cos(theta))
y = np.float32(r*np.sin(theta))
coords = np.column_stack((x,y,z))

# --- create the pointCloud, add the scalar field

cl = cc.ccPointCloud("revol")
cl.coordsFromNPArray_copy(coords)
cl.addScalarField("delta")
sf = cl.getScalarField(0)
sf.fromNpArrayCopy(dr)

# --- save the point cloud

res = cc.SavePointCloud(cl, os.path.join(dataDir, "revol.bin"))
#---createCloudNumpy01-end

with open(os.path.join(dataDir, "profil.txt"), 'w') as f:
    f.write("Xc        Yc         Zc      (profile origin)\n")
    f.write("%f %f %f\n" % (0, 0, 0))
    f.write("R        H    (radius and height of profile vertices)\n")
    for i in range(101):
        z = i/10.
        r = 5+np.cos(z*np.pi/10)
        f.write("%f %f\n" % (r, z))


#---SRA01-begin
if cc.isPluginSRA():
    import cloudComPy.SRA
    poly = cc.SRA.loadProfile(os.path.join(dataDir, "profil.txt"), 2, False)
    sra = cc.SRA.qSRA()
    res=sra.doComputeRadialDists(cl, poly)
    dic = cl.getScalarFieldDic()
    sf = cl.getScalarField(dic['Radial distance'])
#---SRA01-end
    if not math.isclose(sf.getMin(), -0.1, rel_tol = 0.01):
        raise RuntimeError
    if not math.isclose(sf.getMax(), 0.1, rel_tol = 0.01):
        raise RuntimeError
    
#---SRA02-begin
    clmap = cc.SRA.exportMapAsCloud(cl, poly, sf, 0.5, 0.01, 0., 10., baseRadius=2)
#---SRA02-end
    if clmap.size() != 720000:
        raise RuntimeError

#---SRA03-begin
    meshmap = cc.SRA.exportMapAsMesh(cl, poly, sf, 0.5, 0.01, 0., 10., colScale=cc.SRA.DEFAULT_SCALES.YELLOW_BROWN)
#---SRA03-end
    if meshmap.size() != 144000:
        raise RuntimeError
    
    res = cc.SaveEntities([cl,poly, clmap, meshmap], os.path.join(dataDir, "revol2.bin"))

