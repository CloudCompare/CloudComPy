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

from gendata import getSampleCloud, getSampleCloud2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

cloud = cc.loadPointCloud(getSampleCloud(5.0))
if cloud.hasNormals():
    raise RuntimeError
cc.computeNormals([cloud])
if not cloud.hasNormals():
    raise RuntimeError

cloud.unallocateNorms()
if cloud.hasNormals():
    raise RuntimeError
cc.computeNormals([cloud])
if not cloud.hasNormals():
    raise RuntimeError

octree = cloud.getOctree() 
if octree is None:
    raise RuntimeError
nb = octree.getNumberOfProjectedPoints()
if nb != 1000000:
    raise RuntimeError

#---normals01-begin
cloud.exportNormalToSF(True, True, True)

sf=cloud.getScalarField(2)
if sf.getName() != 'Nz':
    raise RuntimeError
#---normals01-end

sfmin = sf.getMin()
sfmax = sf.getMax()
if not math.isclose(sfmin, 0.06670579, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(sfmax, 0.9999990, rel_tol=1e-06):
    raise RuntimeError
meanvar=sf.computeMeanAndVariance()
if not math.isclose(meanvar[0], 0.74157232, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(meanvar[1], 0.04167303, rel_tol=1e-06):
    raise RuntimeError

if cloud.hasColors():
    raise RuntimeError

#---normals02-begin
cloud.convertNormalToRGB()

if not cloud.hasColors():
    raise RuntimeError
#---normals02-end

#---normals03-begin
cloud.convertNormalToDipDirSFs()

dicsf = cloud.getScalarFieldDic()
sfdip = cloud.getScalarField(dicsf['Dip (degrees)'])
sfdipd = cloud.getScalarField(dicsf['Dip direction (degrees)'])
#---normals03-end

sfmin = sfdip.getMin()
sfmax = sfdip.getMax()
if not math.isclose(sfmin, 0.0791, rel_tol=1e-02):
    raise RuntimeError
if not math.isclose(sfmax, 86.175, rel_tol=1e-02):
    raise RuntimeError

sfmin = sfdipd.getMin()
sfmax = sfdipd.getMax()
if not math.isclose(sfmin, 0., abs_tol=1e-01):
    raise RuntimeError
if not math.isclose(sfmax, 360., abs_tol=1e-01):
    raise RuntimeError

#---normals04-begin
if not cloud.orientNormalsWithFM():
    raise RuntimeError

if not cloud.orientNormalsWithMST():
    raise RuntimeError

if not cc.invertNormals([cloud]):
    raise RuntimeError
#---normals04-end

cloud.exportNormalToSF(True, True, True)
sf=cloud.getScalarField(2)
if sf.getName() != 'Nz':
    raise RuntimeError
sfmin = sf.getMin()
sfmax = sf.getMax()
if not math.isclose(sfmax, -0.06670579, rel_tol=1e-06):
    raise RuntimeError
if not math.isclose(sfmin, -0.9999990, rel_tol=1e-06):
    raise RuntimeError

#---meshNormals01-begin
cloud1 = cc.loadPointCloud(getSampleCloud2(3.0,0, 0.1))
cloud1.setName("cloud1")
plane = cc.ccPlane.Fit(cloud1)
mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh1.setName("mesh1")
cc.computeNormals([mesh1], computePerVertexNormals=False)
#---meshNormals01-end
res = cc.SaveEntities([cloud, cloud1, mesh1], os.path.join(dataDir, "cloudmesh.bin"))
if res != cc.CC_FILE_ERROR.CC_FERR_NO_ERROR:
    raise RuntimeError

