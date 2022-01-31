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
from scipy.optimize import minimize
import cloudComPy as cc

def dist2(a,b):
    """
    Euclidian square distance between two 3D points
    """
    d2 = np.inner(a-b, a-b)
    return d2

def isocahedron():
    """
    isocahedron: defines 12 nodes equally distributed on the sphere.
    For each node, we keep the neighbours.
    The sphere is normalised.
    
    :return: the 12 nodes (tuple) and a dictionary of the neighbours indices for each node index
    :rtype: tuple
    """
    phi = (1. + math.sqrt(5.))/2.
    s = []
    s.append((  phi,  1,  0)) # 0
    s.append(( -phi,  1,  0)) # 1
    s.append((  phi, -1,  0)) # 2
    s.append(( -phi, -1,  0)) # 3
    s.append((  1,  0,  phi)) # 4
    s.append(( -1,  0,  phi)) # 5
    s.append((  1,  0, -phi)) # 6
    s.append(( -1,  0, -phi)) # 7
    s.append((  0,  phi,  1)) # 8
    s.append((  0, -phi,  1)) # 9
    s.append((  0,  phi, -1)) # 10
    s.append((  0, -phi, -1)) # 11
    isoNodes = np.array(s)
    neighbours ={}
    for i in range(12):
        neighbours[i] = [j for j in range(12) if math.isclose(4., dist2(isoNodes[i], isoNodes[j]))]
    norm = np.linalg.norm(isoNodes[0])
    isoNodes = isoNodes/norm
    return isoNodes, neighbours

def zinv(x, i1, i2, i3, cl, isoNodes):
    """
    optimization function: maximize the z of object bounding box by adjusting rotation.
    3 neighbours points on the normalized isocahedron defines a starting point and 2 directions:
    starting point: rotation defined by i1,
    direction 1: geodesic defined by points i1, i2,
    direction 2: geodesic defined by points i1, i3,
    
    :param: tuple x: 2 test values for rotations along the 2 directions
    :param int i1: index of the starting point in isocahedron
    :param int i2: index of in isocahedron (i1,i2 define the geodesic of first direction)
    :param int i3: index of in isocahedron (i1,i3 define the geodesic of second direction)
    :param ccPointCloud cl: point cloud
    :param tuple isoNodes: the 12 points of the normalised isocahedron
    
    :return: opposite of the z dimension of the bounding box after rotation (value to minimise)
    :rtype: float
    """
    global rot0
    global rotation
    rot1 = cc.ccGLMatrixd.FromToRotation(isoNodes[i1].tolist(), (0,0,1))
    rot2 = cc.ccGLMatrixd.FromToRotation(isoNodes[i2].tolist(), (0,0,1))
    rot3 = cc.ccGLMatrixd.FromToRotation(isoNodes[i3].tolist(), (0,0,1))
    rota = cc.ccGLMatrixd.Interpolate(x[0], rot1, rot2)
    rotation = cc.ccGLMatrixd.Interpolate(x[1], rota, rot3)
    deltarot = rotation*rot0 # rot0 is the inverse rotation of the previous iteration
    cl.applyRigidTransformation(cc.ccGLMatrix.fromDouble(deltarot))
    rot0 = rotation.inverse()
    bb = cl.getOwnBB()
    zbox = bb.minCorner()[2] - bb.maxCorner()[2]
    print("x:", x, "zbox:", zbox)
    return zbox

def section(x, cl):
    """
    Optimise the object bounding box section by oz rotation
    
    :param tuple x: one test value for rotation along oz
    :param ccPointCloud cl: point cloud
    
    :return:  bounding box x*y section, to minimise
    :rtype: float
    """
    global rot0
    global rotz
    rotz = cc.ccGLMatrixd()
    rotz.initFromParameters(x[0], (0,0,1), (0,0,0)) # rotation oZ angle x
    deltarot = rotz*rot0 # rot0 is the inverse rotation of the previous iteration
    cl.applyRigidTransformation(cc.ccGLMatrix.fromDouble(deltarot))
    rot0 = rotz.inverse()
    bb = cl.getOwnBB()
    sectionxy = (bb.maxCorner()[0] - bb.minCorner()[0])*(bb.maxCorner()[1] - bb.minCorner()[1])
    print("x:", x, "section xy:", sectionxy)
    return sectionxy

def findRotation(cloud):
    """
    Finds the point cloud rotation that minimize the bounding box (built on Ox, Oy, Oz).
    
    **warning** the cloud is supposed centered (bounding box center near the origin)
    TODO: test for robustness, add parameters to control the accuracy...
    
    :param ccPointCloud cloud: the point cloud
    
    :return: bounding box after rotation, inverse rotation, polyline defining the optimal bounding box of the point cloud, polyline cloud
    :rtype: tuple of 4 objects
    """
    global rot0
    global rotation
    global rotz
    # --- the 12 nodes of an isocahedron are equally distributed on the sphere
    #     try rotation of the object on the 12 directions defined by the nodes,
    #     to find a direction which maximize Z on the bounding box
    #     (two opposite directions are relevant, the algorithm peeks one)
    isoNodes, neighbours = isocahedron()

    cl = cloud.cloneThis() # work on a copy (mutiple test rotations)
    imax = -1
    maxz = 0
    bz = []
    rot0 = cc.ccGLMatrix() # identity
    for i, sommet in enumerate(isoNodes):
        rot1 = cc.ccGLMatrix.FromToRotation(sommet.tolist(), (0,0,1))
        rotation = rot1*rot0 # rot0 is the inverse rotation of the previous step
        cl.applyRigidTransformation(rotation)
        bb = cl.getOwnBB()
        zbox = bb.maxCorner()[2] - bb.minCorner()[2]
        bz.append(zbox)
        print('i, zbox:', i, zbox)
        if zbox >maxz:
            maxz = zbox
            imax =i
        rot0 = rot1.inverse()

    v = neighbours[imax]
    d = [(bz[i], i) for i in v]
    d.sort()
    imax2 = d[0][1]
    imax3 = d[1][1]
    print("imax", imax)
    print("imax2", imax2)
    print("imax3", imax3)

    # --- we have a rotation vector near an optimum in Z,
    #     and 2 directions with a good gradient
    #     starting point: vector given by imax
    #     directions for optimisation: the 2 geodesics defined by (imax, imax2) and (imax, imax3)

    cc.deleteEntity(cl) # delete the cloud copy and create a new copy
    cl = None
    cl = cloud.cloneThis()
    x0 = np.array([0., 0.])
    params = (imax, imax2, imax3, cl, isoNodes)
    rot0 = cc.ccGLMatrixd() # identity
    rotation = cc.ccGLMatrixd()

    res = minimize(zinv, x0, method='BFGS', args=params, tol=1e-5, options={'eps':1.e-5, 'disp': True})

    # --- optimise second rotation on Oz
    
    x0 = np.array([0.])
    params = (cl)
    rot0 = cc.ccGLMatrixd() # identity
    rotz = cc.ccGLMatrixd()

    res = minimize(section, x0, method='nelder-mead', args=params, tol=1e-4, options={'eps':1.e-5,'disp': True})

    p2 = rotz.getParameters1()
    print("****** rotz *******")
    print("alpha:", p2.alpha_rad)
    print("axis:", p2.axis3D)
    print("translation", p2.t3D)
    p1 = rotation.getParameters1()
    print("****** rotation *******")
    print("alpha:", p1.alpha_rad)
    print("axis:", p1.axis3D)
    print("translation", p1.t3D)

    # --- compute the inverse rotation, to go from the optimised position to the original position
    
    rotglob = rotz*rotation
    rotinv = rotglob.inverse()
    p3 = rotinv.getParameters1()
    print("****** rotinv *******")
    print("alpha:", p3.alpha_rad)
    print("axis:", p3.axis3D)
    print("translation", p3.t3D)

    # --- draw a 3D polyline representing the bounding box, rotated 
    
    bb = cl.getOwnBB()
    cmin = bb.minCorner()
    cmax = bb.maxCorner()
    x = np.float32(np.array((cmin[0], cmax[0], cmax[0], cmin[0], cmin[0], cmax[0], cmax[0], cmin[0])))
    y = np.float32(np.array((cmin[1], cmin[1], cmin[1], cmin[1], cmax[1], cmax[1], cmax[1], cmax[1])))
    z = np.float32(np.array((cmin[2], cmin[2], cmax[2], cmax[2], cmax[2], cmax[2], cmin[2], cmin[2])))
    coords = np.column_stack((x,y,z))
    clb = cc.ccPointCloud("bbox")
    clb.coordsFromNPArray_copy(coords)
    clb.applyRigidTransformation(cc.ccGLMatrix.fromDouble(rotinv))

    bbpo = cc.ccPolyline(clb)
    bbpo.addChild(clb)
    bbpo.addPointIndex(0, clb.size())
    bbpo.setClosed(True)
    cc.deleteEntity(cl) # delete the cloud copy
    cl = None

    # --- return the Oxyz optimised bounding box, the inverse rotation, the rotated bounding box (polyline and cloud)
    
    return bb, rotinv, bbpo, clb
