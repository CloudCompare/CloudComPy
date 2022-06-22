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

from gendata import getSampleCloud2, getSamplePoly, dataDir
import cloudComPy as cc

# --- create 2 ply files, a mesh and a cloud, with normals, color, scalar fields

cloud1 = cc.loadPointCloud(getSampleCloud2(3.0,0, 0.1))
cloud1.setName("cloud1")
cloud1.exportCoordToSF(True, True, True)
cc.computeNormals([cloud1])
cloud1.convertNormalToRGB()

mesh1 = cc.ccMesh.triangulate(cloud1, cc.TRIANGULATION_TYPES.DELAUNAY_2D_AXIS_ALIGNED)
mesh1.setName("mesh1")
cc.computeNormals([mesh1], computePerVertexNormals=True)
vertices = mesh1.getAssociatedCloud()
vertices.exportCoordToSF(True, True, False)
vertices.convertNormalToRGB()

cc.SavePointCloud(cloud1, os.path.join(dataDir, "cloud01.ply"))
cc.SaveMesh(mesh1, os.path.join(dataDir, "mesh01.ply"))

# --- replace the name of the scalar fields in the header:
#     the prefix scalar_ is set and recognized in CloudCompare to read the scalar fields
#     remove the prefix, to test the ability to read the extra scalar fields with other names

with open(os.path.join(dataDir, "cloud01.ply"), "rb") as f:
    fileData= f.read()

newData=fileData.replace(b'scalar_Coord._', b'coord')

with open(os.path.join(dataDir, "cloud01b.ply"), 'wb') as f:
    f.write(newData)


with open(os.path.join(dataDir, "mesh01.ply"), "rb") as f:
    fileData= f.read()

newData=fileData.replace(b'scalar_Coord._', b'coord')

with open(os.path.join(dataDir, "mesh01b.ply"), 'wb') as f:
    f.write(newData)

# --- read all the scalar fields, by default

cloud2 = cc.loadPointCloud(os.path.join(dataDir, "cloud01b.ply"))
dic = cloud2.getScalarFieldDic()
if dic != {'coordX': 0, 'coordY': 1, 'coordZ': 2}:
    raise RuntimeError

mesh2 = cc.loadMesh(os.path.join(dataDir, "mesh01b.ply"))
vertices2 = mesh2.getAssociatedCloud()
dic = vertices2.getScalarFieldDic()
if dic != {'coordX': 0, 'coordY': 1, 'coordZ': 2}:
    raise RuntimeError

# -- read only the scalar fields whose name ends in dX or dZ (regular expression, see Qt, QRegExp)
#    the letters should be given in uppercase (CloudCompare filter works in uppercase)

nameFilter = ".+D[XZ]" # meaning one or more character followed by DX or DZ
cloud2 = cc.loadPointCloud(os.path.join(dataDir, "cloud01b.ply"), extraData=nameFilter)
dic = cloud2.getScalarFieldDic()
if dic != {'coordX': 0, 'coordZ': 1}:
    raise RuntimeError

ret = cc.importFile(os.path.join(dataDir, "mesh01b.ply"), extraData=nameFilter)
mesh2 = ret[0][0]
vertices2 = mesh2.getAssociatedCloud()
dic = vertices2.getScalarFieldDic()
if dic != {'coordX': 0, 'coordZ': 1}:
    raise RuntimeError

mesh2 = cc.loadMesh(os.path.join(dataDir, "mesh01b.ply"), extraData=nameFilter)
vertices2 = mesh2.getAssociatedCloud()
dic = vertices2.getScalarFieldDic()
if dic != {'coordX': 0, 'coordZ': 1}:
    raise RuntimeError

