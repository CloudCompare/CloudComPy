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
import requests

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, getSampleCloud2, dataDir, dataExtDir, isCoordEqual, createSymbolicLinks, getSamplePoly
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

cloud = cc.loadPointCloud(getSampleCloud2(3.0, 0, 0.1))
cloud.setName("cloud")
if cloud.size() != 10000:
    raise RuntimeError

mesh = cc.ccMesh.triangulate(cloud, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh.setName("mesh")
if not math.isclose(mesh.size(), 19602, rel_tol=5e-02):
    raise RuntimeError

meshSize = mesh.size()

res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.asc")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.xyz")) # OK (idem .asc)
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.las")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.laz")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud14.las"), version="1.4") # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud14.laz"), version="1.4") # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.E57")) # OK  FileIO::setWriterInfo has not been called
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.sbf")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.ply")) # OK  FileIO::setWriterInfo has not been called
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.vtk")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.dxf")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.pcd")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.shp")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.pn"))  # NOK cloudComPy.CC_FILE_ERROR.CC_FERR_BAD_ENTITY_TYPE
if res != cc.CC_FILE_ERROR.CC_FERR_BAD_ENTITY_TYPE:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.pv"))  # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.bin")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
if cc.isPluginDraco():
    res = cc.SavePointCloud(cloud, os.path.join(dataDir, "cloud.drc")) # OK
    if res != cc.CC_FERR_NO_ERROR:
        raise RuntimeError

cloudasc = cc.loadPointCloud(os.path.join(dataDir, "cloud.asc"))
if cloudasc.size() != 10000:
    raise RuntimeError

cloudxyz = cc.loadPointCloud(os.path.join(dataDir, "cloud.xyz"))
if cloudxyz.size() != 10000:
    raise RuntimeError

cloudlas = cc.loadPointCloud(os.path.join(dataDir, "cloud.las"))
if cloudlas.size() != 10000:
    raise RuntimeError

cloudlaz = cc.loadPointCloud(os.path.join(dataDir, "cloud.laz"))
if cloudlaz.size() != 10000:
    raise RuntimeError

cloud14las = cc.loadPointCloud(os.path.join(dataDir, "cloud14.las"))
if cloud14las.size() != 10000:
    raise RuntimeError

cloud14laz = cc.loadPointCloud(os.path.join(dataDir, "cloud14.laz"))
if cloud14laz.size() != 10000:
    raise RuntimeError

cloud14laz.exportCoordToSF(False, False, True)
res = cc.SavePointCloud(cloud14laz, os.path.join(dataDir, "cloud14SF.las"), version="1.4") # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SavePointCloud(cloud14laz, os.path.join(dataDir, "cloud14SF.laz"), version="1.4", pointFormat=6) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError

cloud14SFlas = cc.loadPointCloud(os.path.join(dataDir, "cloud14SF.las"))
if cloud14SFlas.size() != 10000:
    raise RuntimeError
dic = cloud14SFlas.getScalarFieldDic()
sf = cloud14SFlas.getScalarField(dic['Coord. Z'])
sfmin = sf.getMin()             # -0.6516870
sfmax = sf.getMax()             #  3.0
if not math.isclose(sfmin, -0.652, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(sfmax, 3.0, rel_tol=1e-06):
    raise RuntimeError

cloud14SFlaz = cc.loadPointCloud(os.path.join(dataDir, "cloud14SF.laz"))
if cloud14SFlaz.size() != 10000:
    raise RuntimeError
dic = cloud14SFlaz.getScalarFieldDic()
sf = cloud14SFlaz.getScalarField(dic['Coord. Z'])
sfmin = sf.getMin()             # -0.6516870
sfmax = sf.getMax()             #  3.0
if not math.isclose(sfmin, -0.652, rel_tol=1e-03):
    raise RuntimeError
if not math.isclose(sfmax, 3.0, rel_tol=1e-06):
    raise RuntimeError

cloudE57 = cc.loadPointCloud(os.path.join(dataDir, "cloud.E57"))
if cloudE57.size() != 10000:
    raise RuntimeError

cloudsbf = cc.loadPointCloud(os.path.join(dataDir, "cloud.sbf"))
if cloudsbf.size() != 10000:
    raise RuntimeError

cloudply = cc.loadPointCloud(os.path.join(dataDir, "cloud.ply"))
if cloudply.size() != 10000:
    raise RuntimeError

cloudvtk = cc.loadPointCloud(os.path.join(dataDir, "cloud.vtk"))
if cloudvtk.size() != 10000:
    raise RuntimeError

clouddxf = cc.loadPointCloud(os.path.join(dataDir, "cloud.dxf"))
if clouddxf.size() != 10000:
    raise RuntimeError

cloudpcd = cc.loadPointCloud(os.path.join(dataDir, "cloud.pcd"))
if cloudpcd.size() != 10000:
    raise RuntimeError

cloudshp = cc.loadPointCloud(os.path.join(dataDir, "cloud.shp"))
if cloudshp.size() != 10000:
    raise RuntimeError

cloudpv = cc.loadPointCloud(os.path.join(dataDir, "cloud.pv"))
if cloudpv.size() != 10000:
    raise RuntimeError

cloudbin = cc.loadPointCloud(os.path.join(dataDir, "cloud.bin"))
if cloudbin.size() != 10000:
    raise RuntimeError
if cc.isPluginDraco():
    clouddrc = cc.loadPointCloud(os.path.join(dataDir, "cloud.drc"))
    if clouddrc.size() != 10000:
        raise RuntimeError

res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.ma"))  # NOK cloudComPy.CC_FILE_ERROR.CC_FERR_BAD_ENTITY_TYPE
if res != cc.CC_FILE_ERROR.CC_FERR_BAD_ENTITY_TYPE:
    raise RuntimeError
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.dxf")) # OK  FileIO::setWriterInfo has not been called
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.off")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.stl")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.vtk")) # OK
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.obj")) # OK  FileIO::setWriterInfo has not been called
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.ply")) # OK  FileIO::setWriterInfo has not been called
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
mesh.addChild(cloud)
res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.bin")) # OK with cloud as child
if res != cc.CC_FERR_NO_ERROR:
    raise RuntimeError
if cc.isPluginFbx():
    res = cc.SaveMesh(mesh, os.path.join(dataDir, "mesh.fbx")) # OK
    if res != cc.CC_FERR_NO_ERROR:
        raise RuntimeError

meshdxf = cc.loadMesh(os.path.join(dataDir, "mesh.dxf"))
if meshdxf.size() != meshSize:
    raise RuntimeError

meshoff = cc.loadMesh(os.path.join(dataDir, "mesh.off"))
if meshoff.size() != meshSize:
    raise RuntimeError

meshstl = cc.loadMesh(os.path.join(dataDir, "mesh.stl"))
if meshstl.size() != meshSize:
    raise RuntimeError

meshvtk = cc.loadMesh(os.path.join(dataDir, "mesh.vtk"))
if meshvtk.size() != meshSize:
    raise RuntimeError

meshobj = cc.loadMesh(os.path.join(dataDir, "mesh.obj"))
if meshobj.size() != meshSize:
    raise RuntimeError

#---meshcloud01-begin
meshply = cc.loadMesh(os.path.join(dataDir, "mesh.ply"))
cloud1 = meshply.getAssociatedCloud()
#---meshcloud01-end
if meshply.size() != meshSize:
    raise RuntimeError
if cloud1.size() != 10000:
    raise RuntimeError

meshbin = cc.loadMesh(os.path.join(dataDir, "mesh.bin"))
if meshbin.size() != meshSize:
    raise RuntimeError

if cc.isPluginFbx():
    meshfbx = cc.loadMesh(os.path.join(dataDir, "mesh.fbx"))
    if meshfbx.size() != meshSize:
        raise RuntimeError

cc.SaveEntities([cloud,mesh], os.path.join(dataDir, "meshCloud.bin"))
res = cc.importFile(os.path.join(dataDir, "meshCloud.bin"))
aMesh=res[0][0]
if aMesh.size() != meshSize:
    raise RuntimeError
aCloud=aMesh.getAssociatedCloud()
if aCloud.size() != 10000:
    raise RuntimeError
g= aCloud.computeGravityCenter()
if not isCoordEqual(g, (-0.050, -0.050, 0.057), tol= 1.e-1):
    raise RuntimeError

# --- warning, non default values for keyword paramaters are not taken into account for all types of files!

res = cc.importFile(filename=getSampleCloud2(3.0, 0, 0.1), mode=cc.CC_SHIFT_MODE.XYZ, z=10)
aCloud=res[1][0]
if aCloud.size() != 10000:
    raise RuntimeError
g= aCloud.computeGravityCenter()
if not isCoordEqual(g, (-0.050, -0.050, 10.057), tol= 1.e-1):
    raise RuntimeError

res = cc.importFile(filename=os.path.join(dataDir, "meshCloud.bin"), mode=cc.CC_SHIFT_MODE.XYZ, z=10)
aCloud=res[1][0]
if aCloud.size() != 10000:
    raise RuntimeError
g= aCloud.computeGravityCenter()
if not isCoordEqual(g, (-0.050, -0.050, 0.057), tol= 1.e-1):
    raise RuntimeError

poly = cc.loadPolyline(getSamplePoly("poly1"))
cc.SaveEntities([poly], os.path.join(dataDir, "poly01.poly"))
cc.SaveEntities([poly], os.path.join(dataDir, "poly01.dxf"))
cc.SaveEntities([poly], os.path.join(dataDir, "poly01.sx"))
cc.SaveEntities([poly], os.path.join(dataDir, "poly01.shp"))

p_poly=cc.loadPolyline(os.path.join(dataDir, "poly01.poly"))
if p_poly.size() != 7:
    raise RuntimeError
p_dxf=cc.loadPolyline(os.path.join(dataDir, "poly01.dxf"))
if p_dxf.size() != 7:
    raise RuntimeError
p_sx=cc.loadPolyline(os.path.join(dataDir, "poly01.sx"))
if p_sx.size() != 7:
    raise RuntimeError
p_shp=cc.loadPolyline(os.path.join(dataDir, "poly01.shp"))
if p_shp.size() != 7:
    raise RuntimeError

#---LASTimeShift001-begin
# example data available here: https://github.com/CloudCompare/CloudComPy/issues/94

if not os.path.isfile(os.path.join(dataExtDir,"PTS_LAMB93_IGN69_extract.las")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/PTS_LAMB93_IGN69_extract.las"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"PTS_LAMB93_IGN69_extract.las"), 'wb') as f:
        f.write(r.content)
#---LASTimeShift001-end

#---LASTimeShift002-begin
cloud=cc.loadPointCloud(os.path.join(dataExtDir,"PTS_LAMB93_IGN69_extract.las"))
dic = cloud.getScalarFieldDic()
sf = cloud.getScalarField(dic['Gps Time'])
timeShift = sf.getGlobalShift()
#---LASTimeShift002-end
if not math.isclose(timeShift, 120000):
    raise RuntimeError

