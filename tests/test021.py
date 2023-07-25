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

from gendata import getSampleCloud, getSamplePoly2, dataDir, isCoordEqual, createSymbolicLinks
import cloudComPy as cc

createSymbolicLinks() # required for tests on build, before cc.initCC

cloud = cc.loadPointCloud(getSampleCloud(5.0))
poly = cc.loadPolyline(getSamplePoly2("poly2"))
poly.setClosed(True)
cloudCropZ = cloud.crop2D(poly, 2, True)


polygon1 = cc.ccFacet.Create(cloudCropZ)
center1 = polygon1.getCenter()
normal1 = polygon1.getNormal()
eq1 = polygon1.getPlaneEquation()
contour1 = polygon1.getContour()
vert1 = polygon1.getContourVertices()
surface1 = polygon1.getSurface()
if not math.isclose(surface1, 64, rel_tol=1e-02):
    raise RuntimeError

polygon2 = cc.ccFacet.Create(cloudCropZ, 0.05)
center2 = polygon2.getCenter()
normal2 = polygon2.getNormal()
eq2 = polygon2.getPlaneEquation()
contour2 = polygon2.getContour()
vert2 = polygon2.getContourVertices()
surface2 = polygon2.getSurface()
if not math.isclose(surface2, 56, rel_tol=1e-02):
    raise RuntimeError

#---facetCreate01-begin
polygon3 = cc.ccFacet.Create(cloudCropZ, maxEdgeLength=0.5)
center3 = polygon3.getCenter()
normal3 = polygon3.getNormal()
eq3 = polygon3.getPlaneEquation()
contour3 = polygon3.getContour()
vert3 = polygon3.getContourVertices()
surface3 = polygon3.getSurface()
if not math.isclose(surface3, 56, rel_tol=2e-02):
    raise RuntimeError
#---facetCreate01-end

#---facetCreate02-begin
polygon4 = cc.ccFacet.Create(cloud=cloudCropZ, maxEdgeLength=0.05, planeEquation=(0., 0., 1., 0.))
center4 = polygon4.getCenter()
normal4 = polygon4.getNormal()
eq4 = polygon4.getPlaneEquation()
contour4 = polygon4.getContour()
vert4 = polygon4.getContourVertices()
surface4 = polygon4.getSurface()
if not math.isclose(surface4, 56, rel_tol=1e-02):
    raise RuntimeError
#---facetCreate02-end

cc.SaveEntities([cloud, cloudCropZ, poly, polygon1, polygon2, polygon3, polygon4, vert1, vert2, vert3, vert4], os.path.join(dataDir, "polygons2D.bin"))

mesh1 = polygon1.getPolygon()
cc.SaveMesh(mesh1, os.path.join(dataDir, "mesh1.ply"))

entities = cc.importFile(os.path.join(dataDir, "polygons2D.bin"))
meshes = entities[0]
clouds = entities[1]
facets = entities[2]
polylines = entities[3]
structure = entities[4]

if len(meshes) != 4:
    raise RuntimeError
if len(clouds) != 2:
    raise RuntimeError
if len(facets) != 4:
    raise RuntimeError
if len(polylines) != 1:
    raise RuntimeError
if len(structure) != 24:
    raise RuntimeError
                  

