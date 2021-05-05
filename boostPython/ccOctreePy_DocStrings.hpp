//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#ifndef CCOCTREEPY_DOCSTRINGS_HPP_
#define CCOCTREEPY_DOCSTRINGS_HPP_

const char* DgmOctree_BoxNeighbourhood_doc= R"(
Input/output parameters structure for getPointsInBoxNeighbourhood
Use findBestLevelForAGivenNeighbourhoodSizeExtraction to get the right
value for 'level' (only once as it only depends on the radius value ;).
data members:
-------------
center: Box center
axes: optional axes
dimensions: dimensions of the box
level: subdivision level at which to apply the extraction process )";

const char* DgmOctree_CylindricalNeighbourhood_doc= R"(
parameters structure for getPointsInCylindricalNeighbourhood
Use findBestLevelForAGivenNeighbourhoodSizeExtraction to get the right
value for 'level' (only once as it only depends on the radius value ;).
data members:
-------------
center: Cylinder center
dir: Cylinder axis (direction)
radius: Cylinder radius
maxHalfLength: Cylinder (half) length
level: subdivision level at which to apply the extraction process
onlyPositiveDir: Whether to look in both directions or only along the positive direction (i.e. half cylinder))";

const char* DgmOctree_findBestLevelForAGivenNeighbourhoodSizeExtraction_doc= R"(
Determines the best level of subdivision of the octree at which to apply the nearest neighbours search algorithm (inside a sphere) depending on the sphere radius
param radius the sphere radius
return the 'best' level)";

const char* DgmOctree_getBoundingBox_doc= R"(
Returns the octree bounding box
Method to request the octree bounding box limits
return tuple ((Xmin,Ymin,Zmin), (Xmax,Ymax,Zmax)))";

const char* DgmOctree_getNumberOfProjectedPoints_doc= R"(
Returns the number of points projected into the octree
return the number of projected points)";

const char* DgmOctree_getOctreeMaxs_doc= R"(
Returns the higher boundaries of the octree
return the higher coordinates along X,Y and Z)";

const char* DgmOctree_getOctreeMins_doc= R"(
Returns the lower boundaries of the octree
return the lower coordinates along X,Y and Z)";

const char* DgmOctree_getPointsInBoxNeighbourhood_doc= R"(
Returns the points falling inside a box
Use findBestLevelForAGivenNeighbourhoodSizeExtraction to get the right
value for 'level' (only once as it only depends on the radius or max dimension value ;).
warning the 'squareDistd' field of each neighbour returned is not used/set)";

const char* DgmOctree_getPointsInCylindricalNeighbourhood_doc= R"(
Returns the points falling inside a cylinder
Use findBestLevelForAGivenNeighbourhoodSizeExtraction to get the right
value for 'level' (only once as it only depends on the radius value ;).
warning the 'squareDistd' field of each neighbour in the NeighboursSet
structure is in fact the signed distance (not squared) of the point
relatively to the cylinder's center and projected along its axis.
parameters:
params: parameters structure
return the extracted points)";

const char* DgmOctree_getPointsInSphericalNeighbourhood_doc= R"(
Returns the points falling inside a sphere
Use findBestLevelForAGivenNeighbourhoodSizeExtraction to get the right
value for 'level' (only once as it only depends on the radius value ;).
param sphereCenter center
param radius radius
param level subdivision level at which to apply the extraction process
return neighbours points falling inside the sphere (list of PointDescriptors)";

const char* DgmOctree_PointDescriptor_doc= R"(
Structure used during nearest neighbour search
Association between a point, its index and its square distance to the query point.
The structure is made persistent, by copying the coordinates.)";

const char* ccOctree_ccOctree_doc= R"(
Octree structure
Extends the CCCoreLib::DgmOctree class.
The octree structure used throughout the library
Implements the GenericOctree interface.
Corresponds to the octree structure developed during Daniel
Girardeau-Montaut's PhD (see PhD manuscript, Chapter 4).)";

#endif /* CCOCTREEPY_DOCSTRINGS_HPP_ */
