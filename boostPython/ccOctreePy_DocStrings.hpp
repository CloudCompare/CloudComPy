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

const char* DgmOctree_CellDescriptor_doc= R"(
Structure used during nearest neighbour search
center: Cell center
index: First point index in associated NeighboursSet)";

const char* DgmOctree_codeComp_doc= R"(
Compares two IndexAndCode instances based on their code
param a first IndexAndCode structure
param b second IndexAndCode structure
return whether the code of 'a' is smaller than the code of 'b')";

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

const char* DgmOctree_findBestLevelForAGivenCellNumber_doc= R"(
Determines the best subdivision level of the octree to match a given number of cells
param indicativeNumberOfCells 'desired' number of cells
return the 'best' level
)";

const char* DgmOctree_findBestLevelForAGivenNeighbourhoodSizeExtraction_doc= R"(
Determines the best level of subdivision of the octree at which to apply the nearest neighbours search algorithm (inside a sphere) depending on the sphere radius
param radius the sphere radius
return the 'best' level)";

const char* DgmOctree_findBestLevelForAGivenPopulationPerCell_doc= R"(
Determines the best subdivision level of the octree that gives the average population per cell closest to the input value
param indicativeNumberOfPointsPerCell 'desired' average number of points per cell
return the 'best' level)";

const char* DgmOctree_getBoundingBox_doc= R"(
Returns the octree bounding box
Method to request the octree bounding box limits
return tuple ((Xmin,Ymin,Zmin), (Xmax,Ymax,Zmax)))";

const char* DgmOctree_getCellCode_doc= R"(
Returns the ith cell code)";

const char* DgmOctree_getCellCodes_doc= R"(
Returns the list of codes corresponding to the octree cells for a given level of subdivision
Only the non empty cells are represented in the octree structure.
param level the level of subdivision
param optional truncatedCodes indicates if the resulting codes should be truncated or not
return the list of codes)";

const char* DgmOctree_getCellCodesAndIndexes_doc= R"(
Returns the list of indexes and codes corresponding to the octree cells for a given level of subdivision
Only the non empty cells are represented in the octree structure.
Cell indexes are expressed relatively to the DgmOctree structure. They correspond
to the indexes of the first points of each cell.
param level the level of subdivision
param truncatedCodes indicates if the resulting codes should be truncated or not
return the list of codes & indexes)";

const char* DgmOctree_getCellDistanceFromBorders_doc= R"(
Returns distance from a cell to the filled octree borders in all directions (3 int).
WARNING: distance values may be negative! (if cell is outside)
param cellPos cell position
param level level at which octree grid is considered
return cellDists output (3 int))";

const char* DgmOctree_getCellDistanceFromBordersN_doc=R"(
Returns distance from cell center to cell neighbourhood INSIDE filled octree (3 int).
WARNING: if cell neighbourhood is totally outside filled octree,
the method returns invalid cellDists (empty list).
param cellPos center cell position
param level level at which octree grid is considered
param neighbourhoodLength cell neighbourhood "radius"
return cellDists output (3 int))";

const char* DgmOctree_getCellIndexes_doc= R"(
Returns the list of indexes corresponding to the octree cells for a given level of subdivision
Only the non empty cells are represented in the octree structure.
Cell indexes are expressed relatively to the DgmOctree structure. They correspond
to the indexes of the first points of each cell.
param level the level of subdivision
return vec the list of indexes)";

const char* DgmOctree_getCellNumber_doc= R"(
Returns the number of cells for a given level of subdivision
param level)";

const char* DgmOctree_getCellSize_doc= R"(
Returns the octree cells length for a given level of subdivision
As the octree is cubical, cells are cubical.
param level the level of subdivision (up to MAX_OCTREE_LEVEL+1 for convenience)
return the cell size)";

const char* DgmOctree_getMaxFillIndexes_doc= R"(
Returns the highest cell positions in the octree along all dimensions and for a given level of subdivision
For example, at a level n, the octree length is 2^n cells along each
dimension. The highest cell position along each dimension will be expressed
between 0 and 2^n-1.
param level the level of subdivision
return the highest cell position along X,Y and Z for a given level of subdivision)";

const char* DgmOctree_getMinFillIndexes_doc= R"(
Returns the lowest cell positions in the octree along all dimensions and for a given level of subdivision
For example, at a level n, the octree length is 2^n cells along each
dimension. The lowest cell position along each dimension will be expressed
between 0 and 2^n-1.
param level the level of subdivision
return the lowest cell position along X,Y and Z for a given level of subdivision)";

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

const char* DgmOctree_IndexAndCode_doc= R"(
Association between an index and the code of an octree cell
Index could be the index of a point, in which case the code
would correspond to the octree cell where the point lies.
the index: index
theCode: code)";

const char* DgmOctree_indexComp_doc= R"(
Compares two IndexAndCode instances based on their index
param a first IndexAndCode structure
param b second IndexAndCode structure
return whether the index of 'a' is smaller than the index of 'b')";

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
