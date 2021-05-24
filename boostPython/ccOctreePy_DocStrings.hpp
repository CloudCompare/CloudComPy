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

const char* ccOctree_ccOctree_doc= R"(
Octree structure
Extends the CCCoreLib::DgmOctree class.
The octree structure used throughout the library
Implements the GenericOctree interface.
Corresponds to the octree structure developed during Daniel
Girardeau-Montaut's PhD (see PhD manuscript, Chapter 4).)";

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
Determines the best level of subdivision of the octree at which to apply the nearest neighbours search algorithm
(inside a sphere) depending on the sphere radius
param radius the sphere radius
return the 'best' level)";

const char* DgmOctree_findBestLevelForAGivenPopulationPerCell_doc= R"(
Determines the best subdivision level of the octree that gives the average population per cell closest to the input value
param indicativeNumberOfPointsPerCell 'desired' average number of points per cell
return the 'best' level)";

const char* DgmOctree_findPointNeighbourhood_doc= R"(
Finds the nearest neighbours around a query point
This is the simplest form of the nearest neighbour search algorithm.
It should only be used for unique/few requests as it is not optimized
for repetitive search around points lying in the same octree cell (see
DgmOctree::findNearestNeighborsStartingFromCell for example). Moreover,
distances between each neighbour and the query aren't stored in this
version of the algorithm.
param _queryPoint the query point
param Yk RefefenceCloud for the nearest neighbours
param maxNumberOfNeighbors the maximal number of points to find
param level the subdivision level of the octree at which to perform the search
param maxSearchDist (optional default 0) the maximum search distance (ignored if <= 0)
return tuple( ref cloud completed,
              number of neighbours found, 
              final neighborhood (half)size,
              square distance between the farthest "nearest neighbour" and the query point
            ))";

const char* DgmOctree_findNearestNeighborsStartingFromCell_doc= R"(
Advanced form of the nearest neighbours search algorithm (multiple neighbours)
This version is optimized for a multiple nearest neighbours search
that is applied around several query points included in the same octree
cell. See DgmOctree::NearestNeighboursSearchStruct for more details.
param nNSS NearestNeighboursSearchStruct search parameters
param getOnlyPointsWithValidScalar whether to ignore points having an invalid associated scalar value
return the number of neighbours found)";

const char* DgmOctree_findNeighborsInASphereStartingFromCell_doc= R"(
Advanced form of the nearest neighbours search algorithm (in a sphere)
This version is optimized for a spatially bounded search instead of
a search bounded by a number of neighbours.
warning the number of points in the output buffer (nNSS.pointsInNeighbourhood) may be greater
than the actual count of closest points inside the sphere! (which is returned by the method).
Only the 'k' first points are actually inside the sphere (the others are not removed for the sake
of performance).
param nNSS NearestNeighboursSphericalSearchStruct a pack of parameters
param radius the sphere radius
param sortValues specifies if the neighbours needs to be sorted by their distance to the query point or not
return the number of neighbours found)";

const char* DgmOctree_findTheNearestNeighborStartingFromCell_doc= R"(
Advanced form of the nearest neighbour search algorithm (unique neighbour)
This version is optimized for a unique nearest-neighbour search.
See DgmOctree::NearestNeighboursSearchStruct for more details.
param nNSS NearestNeighboursSearchStruct search parameters
return the square distance between the query point and its nearest neighbour
       (or -1 if none was found - i.e. maxSearchDist was reached))";

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

const char* DgmOctree_getPointsInCell_doc= R"(
Returns the points lying in a specific cell
param cellCode the unique cell code
param level the level of subdivision
param[out] subset set of points lying in the cell (references, no duplication)
param isCodeTruncated (optional, default false) specifies if the code is given in a truncated form or not
param clearOutputCloud (optional, default true) whether to clear or not the output cloud (subest)
      if no points lie in the specified cell
return the subset set of points completed)";

const char* DgmOctree_getPointsInCellByCellIndex_doc= R"(
Returns the points lying in a specific cell
Each cell at a given level of subdivision can be recognized by the index
in the DgmOctree structure of the first point that lies inside it. By
construction, we are assured that every point lying in the same cell for
a given level of subdivision are next to each others in the octree
structure (which is the vector "m_thePointsAndTheirCellCodes" in practical).
This is the quickest way to access the points inside a given cell (but its
kind of hard to know directly what is the index of a given cell ;)
param cloud ReferenceCloud to store the points lying inside the cell
param cellIndex the cell index
param level the level of subdivision
param clearOutputCloud (optional, default true) whether to clear the input cloud
      prior to inserting the points or not
return the input cloud completed.)";

const char* DgmOctree_getPointsInCellsWithSortedCellCodes_doc= R"(
Returns the points lying in multiple cells
Cells are recognized here by their unique "code". They should be sorted
along by codes, with an ascendant order. See DgmOctree::getPointsInCellByCellIndex
for more information.
param cellCodes the cells codes
param level the level of subdivision
param[out] subset set of points lying in the cell (references, no duplication)
param areCodesTruncated (optional, default false) specifies if the codes are given in a truncated form or not
return the set of points lying in the cell (references, no duplication))";

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

const char* DgmOctree_getPointsInCylindricalNeighbourhoodProgressive_doc= R"(
Same as getPointsInCylindricalNeighbourhood with progressive approach
Can be called multiple times (the 'currentHalfLength' parameter will increase
each time until 'maxHalfLength' is reached).)";

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

#define dgm_nnss_0 R"(
Container of in/out parameters for nearest neighbour(s) search
This structure is generic and can be used in multiple cases.
It is particularly useful when searching nearest neighbours around points
that lie in the same octree cell. In this case, several informations about
this cell should be given to the search algorithm through this structure, but only
once,before the first search. Then the search algorithm can be called multiple times,
and only few informations need to be updated (the query point, etc.).

*** Information to set before search ***

- Query point
    Should be updated each time.
- level
    Level of subdivision of the octree at which to start the search
    Should be set once and for all.
- minNumberOfNeighbors
    Minimal number of neighbours to find
    used only during multiple neighbours search (see findNearestNeighborsStartingFromCell).
    This is only indicative and not guaranteed.
- cellPos
    Position in the octree of the cell including the query point
    The position is expressed for the level of subdivision at which the search will
    be processed. Use see DgmOctree::getCellPos to determine this position.
    This information should only be updated if the cell changes.
- cellCenter
    Coordinates of the center of the cell including the query point
    Use DgmOctree::computeCellCenter to determine these coordinates.
    This information should only be updated if the cell changes.
- maxSearchSquareDistd 
    Maximum neihgbours distance
    The NN search process will stop if it reaches this radius even if it
    hasn't find any neighbour (acceleration). To disable this behavior,
    set the maxSearchSquareDistd to something <= 0).

*** Information to set to 0 before search ***

- minimalCellsSetToVisit
    List of indexes of the cells that have been already visited by the algorithm
    This field is updated by the search algorithm. It should only be emptied
    if the cell that includes the query points change. Only used by the
    "unique nearest point" search algorithm.
- pointsInNeighbourhood
    All the points that belong to the cubical neighbourhood of the current cell
    This structure is only used by the "multiple nearest neighbours" search algorithms.
    The nearest points (relatively to the query point) are stored at the beginning of
    the vector. They are associated to their square distance to the query point.
- alreadyVisitedNeighbourhoodSize 
    Size of the cell neighbourhood that has been already visited by the algorithm
    This field is updated by the search algorithm. It should only be reset to 0
    when the cell that includes the query point changes. A value of 0 means that
    no cell has been visited yet, 1 means that only the cell that includes the
    query point has been visited, 2 means that this cell and its 27 neighbourhing
    cells have been visited, etc.

*** Result ***

- theNearestPointIndex
    The nearest point
    This field is only used by the "unique nearest neighbour" search algorithm
    (see DgmOctree::findTheNearestNeighborStartingFromCell).)"

#define dgm_nnsss_1 R"(
Container of in/out parameters for nearest neighbour(s) Spherical search,
derived from NearestNeighboursSearchStruct:
--- generic part ---
)"

#define dgm_nnsss_2 R"(
--- spherical part ----
- ready
    Whether pointsInSphericalNeighbourhood is ready or not (see prepare method for init)
)"

#define dgm_nnsss_3 R"(
- pointsInSphericalNeighbourhood
    All the points that belong to the spherical neighbourhood of the current cell
- cellsInNeighbourhood
    Meta data describing cells neighbourhood (associated to pointsInNeighbourhoodUnsorted)
- maxInD2
    max SQUARE distance from query point to cell center (to be sure of total inclusion)
- minOutD2
    min SQUARE distance from query point to cell center (to be sure of total exclusion)
)"

const char* DgmOctree_NearestNeighboursSearchStruct_doc= dgm_nnss_0;

#ifdef TEST_CELLS_FOR_SPHERICAL_NN
const char* DgmOctree_NearestNeighboursSphericalSearchStruct_doc = dgm_nnsss_1 dgm_nnss_0 dgm_nnsss_2 dgm_nnsss_3;
#else
const char* DgmOctree_NearestNeighboursSphericalSearchStruct_doc = dgm_nnsss_1 dgm_nnss_0 dgm_nnsss_2;
#endif

const char* DgmOctree_PointDescriptor_doc= R"(
Structure used during nearest neighbour search
Association between a point, its index and its square distance to the query point.
The structure is made persistent, by copying the coordinates.)";

const char* DgmOctree_ProgressiveCylindricalNeighbourhood_doc= R"(
Input/output parameters structure for getPointsInCylindricalNeighbourhoodProgressive
derived from CylindricalNeighbourhood.
See CylindricalNeighbourhood for generic data members.
Specific data members:
----------------------
- currentHalfLength
    Current search depth
- potentialCandidates
    Vector to store potential candidates for the next pass
    Candidates are points close enough to the cylinder's axis but too far
    from its actual center.
- prevMinCornerPos
    Previous search box (min corner)
- prevMaxCornerPos
    Previous search box (max corner))";

const char* NearestNeighboursSphericalSearchStruct_prepare_doc= R"(
initialize ready status (false), Updates maxD2 and minD2 with search radius and cellSize)";

#endif /* CCOCTREEPY_DOCSTRINGS_HPP_ */
