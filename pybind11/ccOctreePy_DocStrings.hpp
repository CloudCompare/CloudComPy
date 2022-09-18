//##########################################################################
//#                                                                        #
//#                              CloudComPy                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; either version 3 of the License, or     #
//#  any later version.                                                    #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#  You should have received a copy of the GNU General Public License     #
//#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
//#                                                                        #
//#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
//#                                                                        #
//##########################################################################

#ifndef CCOCTREEPY_DOCSTRINGS_HPP_
#define CCOCTREEPY_DOCSTRINGS_HPP_

const char* ccOctree_ccOctree_doc= R"(
Octree structure.

Extends the CCCoreLib::DgmOctree class.
The octree structure used throughout the library
Implements the GenericOctree interface.
Corresponds to the octree structure developed during Daniel
Girardeau-Montaut's PhD (see PhD manuscript, Chapter 4).)";

const char* DgmOctree_BoxNeighbourhood_doc= R"(
Input/output parameters structure for getPointsInBoxNeighbourhood.

Use findBestLevelForAGivenNeighbourhoodSizeExtraction to get the right
value for 'level' (only once as it only depends on the radius value ;).

:ivar tuple center: Box center (3 coordinates), default initialization (0, 0, 0)
:ivar list axes: 3 axes, optional, default initialization None
:ivar tuple dimensions: dimensions of the box, default initialization (0, 0, 0)
:ivar int level: subdivision level at which to apply the extraction process )";

const char* DgmOctree_CellDescriptor_doc= R"(
Structure used during nearest neighbour search.

:ivar tuple center: Cell center coordinates
:ivar int index: First point index in associated NeighboursSet)";

const char* DgmOctree_codeComp_doc= R"(
Compares two IndexAndCode instances based on their code.

:param IndexAndCode a: first IndexAndCode structure
:param IndexAndCode b: second IndexAndCode structure

:return: whether the code of 'a' is smaller than the code of 'b'
:rtype: bool )";

const char* DgmOctree_computeCellCenter_py_doc= R"(
Returns the cell center for a given level of subdivision of a cell designated by its code.

:param int code: the cell code
:param int level: the level of subdivision
:param bool,optional isCodeTruncated: (default `False`) indicates if the given code is truncated or not

:return: computed center coordinates
:rtype: tuple )";

const char* DgmOctree_computeCellCenter2_py_doc= R"(
Returns the cell center for a given level of subdivision of a cell designated by its position.

:param tuple cellPos: the cell position (3 int indices)
:param int level: the level of subdivision

:return: computed center coordinates
:rtype: tuple )";

const char* DgmOctree_CylindricalNeighbourhood_doc= R"(
Parameters structure for getPointsInCylindricalNeighbourhood.

Use `findBestLevelForAGivenNeighbourhoodSizeExtraction` to get the right
value for 'level' (only once as it only depends on the radius value).

:ivar tuple center: Cylinder center, default (0,0,0)
:ivar tuple dir: Cylinder axis (direction), default (0,0,1)
:ivar float radius: Cylinder radius, default 0
:ivar float maxHalfLength: Cylinder (half) length, default 0
:ivar int level: subdivision level at which to apply the extraction process, default 0
:ivar bool onlyPositiveDir: Whether to look in both directions
      or only along the positive direction (i.e. half cylinder), default  `False` )";

const char* DgmOctree_findBestLevelForAGivenCellNumber_doc= R"(
Determines the best subdivision level of the octree to match a given number of cells.

:param int indicativeNumberOfCells: 'desired' number of cells

:return: the 'best' level
:rtype: int
)";

const char* DgmOctree_findBestLevelForAGivenNeighbourhoodSizeExtraction_doc= R"(
Determines the best level of subdivision of the octree at which to apply the nearest neighbours search algorithm
(inside a sphere) depending on the sphere radius.

:param float radius: the sphere radius

:return: the 'best' level
:rtype: int
)";

const char* DgmOctree_findBestLevelForAGivenPopulationPerCell_doc= R"(
Determines the best subdivision level of the octree that gives the average population per cell closest to the input value.

:param int indicativeNumberOfPointsPerCell: 'desired' average number of points per cell

:return: the 'best' level
:rtype: int
)";

const char* DgmOctree_findPointNeighbourhood_doc= R"(
Finds the nearest neighbours around a query point.

This is the simplest form of the nearest neighbour search algorithm.
It should only be used for unique/few requests as it is not optimized
for repetitive search around points lying in the same octree cell (see
:py:meth:`findNearestNeighborsStartingFromCell` for example). Moreover,
distances between each neighbour and the query aren't stored in this
version of the algorithm.

:param tuple _queryPoint: the query point (coordinates)
:param ReferenceCloud Yk: RefefenceCloud for the nearest neighbours
:param int maxNumberOfNeighbors: the maximal number of points to find
:param int level: the subdivision level of the octree at which to perform the search
:param float,optional maxSearchDist: (default 0) the maximum search distance (ignored if <= 0)

:return: tuple

   - number of neighbours found,
   - final neighborhood (half)size,
   - square distance between the farthest "nearest neighbour" and the query point

:rtype: tuple )";

const char* DgmOctree_findNearestNeighborsStartingFromCell_doc= R"(
Advanced form of the nearest neighbours search algorithm (multiple neighbours).

This version is optimized for a multiple nearest neighbours search
that is applied around several query points included in the same octree
cell. See :py:class:`NearestNeighboursSearchStruct` for more details.

:param NearestNeighboursSearchStruct nNSS: search parameters
:param bool getOnlyPointsWithValidScalar: whether to ignore points having an invalid associated scalar value

:return: the number of neighbours found
:rtype: int
)";

const char* DgmOctree_findNeighborsInASphereStartingFromCell_doc= R"(
Advanced form of the nearest neighbours search algorithm (in a sphere).

This version is optimized for a spatially bounded search instead of
a search bounded by a number of neighbours.
**WARNING** the number of points in the output buffer (nNSS.pointsInNeighbourhood) may be greater
than the actual count of closest points inside the sphere! (which is returned by the method).
Only the 'k' first points are actually inside the sphere (the others are not removed for the sake
of performance).

:param NearestNeighboursSearchStruct nNSS: a pack of parameters
:param float radius: the sphere radius
:param bool sortValues: specifies if the neighbours needs to be sorted by their distance to the query point or not

:return: the number of neighbours found
:rtype: int )";

const char* DgmOctree_findTheNearestNeighborStartingFromCell_doc= R"(
Advanced form of the nearest neighbour search algorithm (unique neighbour).

This version is optimized for a unique nearest-neighbour search.
See :py:class:`NearestNeighboursSearchStruct` for more details.

:param NearestNeighboursSearchStruct nNSS: search parameters

:return: the square distance between the query point and its nearest neighbour
         (or -1 if none was found - i.e. maxSearchDist was reached)
:rtype: float )";

const char* DgmOctree_GenerateTruncatedCellCode_doc= R"(
Generates the truncated cell code of a cell given its position at a given level of subdivision.

For a given level of subdivision (lets call it N), the cell position
can be expressed as 3 integer coordinates between 0 and 2^N-1 (the
number of cells along each dimension). This method computes the
corresponding cell code, truncated at the level N (meaning that it
is only valid for the Nth level, not for other levels).

:param tuple cellPos: the cell position (3 int indices)
:param int level: the level of subdivision

:return: the truncated cell code
:rtype: int )";

const char* DgmOctree_getBoundingBox_doc= R"(
Returns the octree bounding box.

Method to request the octree bounding box limits.

:return: ((Xmin,Ymin,Zmin), (Xmax,Ymax,Zmax))
:rtype: tuple )";

const char* DgmOctree_getCellCode_doc= R"(
Returns the ith cell code.

**WARNING:** i is NOT the point index in cloud! Very specific use!
(the table giving index in cloud and CellCode is sorted by CellCodes)

:return: CellCode
:rtype: int)";

const char* DgmOctree_getCellCodes_doc= R"(
Returns the list of codes corresponding to the octree cells for a given level of subdivision.

Only the non empty cells are represented in the octree structure.

:param int level: the level of subdivision
:param bool,optional truncatedCodes: (default `False`) indicates if the resulting codes should be truncated or not

:return: the list of codes
:rtype: list)";

const char* DgmOctree_getCellCodesAndIndexes_doc= R"(
Returns the list of indexes and codes corresponding to the octree cells for a given level of subdivision.

Only the non empty cells are represented in the octree structure.
Cell indexes are expressed relatively to the DgmOctree structure. They correspond
to the indexes of the first points of each cell.

:param int level: the level of subdivision
:param bool truncatedCodes: indicates if the resulting codes should be truncated or not

:return: the list of :py:class:`IndexAndCode`
:rtype: list )";

const char* DgmOctree_getCellDistanceFromBorders_doc= R"(
Returns distance from a cell to the filled octree borders in all directions (3 int).

**WARNING:** distance values may be negative! (if cell is outside)

:param tuple cellPos: cell position
:param int level: level at which octree grid is considered

:return: cellDists output (3 int)
:rtype: tuple )";

const char* DgmOctree_getCellDistanceFromBordersN_doc=R"(
Returns distance from cell center to cell neighbourhood INSIDE filled octree (3 int).

**WARNING:** if cell neighbourhood is totally outside filled octree,
the method returns invalid cellDists (empty list).

:param tuple cellPos: center cell position
:param int level: level at which octree grid is considered
:param float neighbourhoodLength: cell neighbourhood "radius"

:return: cellDists output (3 int)
:rtype: tuple )";

const char* DgmOctree_getCellIndexes_doc= R"(
Returns the list of indexes corresponding to the octree cells for a given level of subdivision.

Only the non empty cells are represented in the octree structure.
Cell indexes are expressed relatively to the DgmOctree structure. They correspond
to the indexes of the first points of each cell.

:param int level: the level of subdivision

:return: the list of indexes
:rtype: list)";

const char* DgmOctree_getCellNumber_doc= R"(
Returns the number of cells for a given level of subdivision.

:param int level: the level of subdivision

:return: the number of cells at this level
:rtype: int )";

const char* DgmOctree_getCellPos_doc= R"(
Returns the cell position for a given level of subdivision of a cell designated by its code.

:param int code: the cell code
:param int level: the level of subdivision
:param bool isCodeTruncated: indicates if the given code is truncated or not

:return: the computed cell position
:rtype: tuple )";

const char* DgmOctree_getCellSize_doc= R"(
Returns the octree cells length for a given level of subdivision.

As the octree is cubical, cells are cubical.

:param int level: the level of subdivision (up to MAX_OCTREE_LEVEL+1 for convenience)

:return: the cell size
:rtype: float)";

const char* DgmOctree_getMaxFillIndexes_doc= R"(
Returns the highest cell positions in the octree along all dimensions and for a given level of subdivision.

For example, at a level n, the octree length is 2^n cells along each
dimension. The highest cell position along each dimension will be expressed
between 0 and 2^n-1.

:param int level: the level of subdivision

:return: the highest cell position along X,Y and Z for a given level of subdivision
:rtype: tuple )";

const char* DgmOctree_getMinFillIndexes_doc= R"(
Returns the lowest cell positions in the octree along all dimensions and for a given level of subdivision.

For example, at a level n, the octree length is 2^n cells along each
dimension. The lowest cell position along each dimension will be expressed
between 0 and 2^n-1.

:param int level: the level of subdivision

:return: the lowest cell position along X,Y and Z for a given level of subdivision
:rtype: tuple )";

const char* DgmOctree_getNumberOfProjectedPoints_doc= R"(
Returns the number of points projected into the octree.

:return: the number of projected points
:rtype: int )";

const char* DgmOctree_getOctreeMaxs_doc= R"(
Returns the higher boundaries of the octree.

:return: the higher coordinates along X,Y and Z
:rtype: tuple )";

const char* DgmOctree_getOctreeMins_doc= R"(
Returns the lower boundaries of the octree.

:return: the lower coordinates along X,Y and Z
:rtype: tuple )";

const char* DgmOctree_getPointsInBoxNeighbourhood_doc= R"(
Returns the points falling inside a box.

Use :py:meth:`findBestLevelForAGivenNeighbourhoodSizeExtraction` to get the right
value for 'level' (only once as it only depends on the radius or max dimension value).

**WARNING** the 'squareDistd' field of each neighbour returned is not used/set

:param BoxNeighbourhood neighbourhood: the box in which to search points

:return: list of :py:class:`PointDescriptor`
:rtype: list)";

const char* DgmOctree_getPointsInCell_doc= R"(
Returns the points lying in a specific cell.

:param int cellCode: the unique cell code
:param int level: the level of subdivision
:param ReferenceCloud,[in,out] subset: set of points lying in the cell (references, no duplication)
:param bool,optional isCodeTruncated: (default `False`) specifies if the code is given in a truncated form or not
:param bool,optional clearOutputCloud: (default `True`) whether to clear or not the output cloud (subest)
       if no points lie in the specified cell

:return: success
:rtype: bool )";

const char* DgmOctree_getPointsInCellByCellIndex_doc= R"(
Returns the points lying in a specific cell.

Each cell at a given level of subdivision can be recognized by the index
in the DgmOctree structure of the first point that lies inside it. By
construction, we are assured that every point lying in the same cell for
a given level of subdivision are next to each others in the octree
structure (which is the vector "m_thePointsAndTheirCellCodes" in practical).
This is the quickest way to access the points inside a given cell (but its
kind of hard to know directly what is the index of a given cell)
See :py:meth:`getCellIndexes`, :py:meth:`getCellCodesAndIndexes`.

:param ReferenceCloud,[in,out] cloud: ReferenceCloud to store the points lying inside the cell
:param int cellIndex: the cell index
:param int level: the level of subdivision
:param bool,optional clearOutputCloud: (default `True`) whether to clear the input cloud
       prior to inserting the points or not

:return: success
:rtype: bool )";

const char* DgmOctree_getPointsInCellsWithSortedCellCodes_doc= R"(
Returns the points lying in multiple cells.

Cells are recognized here by their unique "code". They should be sorted
along by codes, with an ascendant order. See :py:meth:`getPointsInCellByCellIndex`
for more information.

param list cellCodes: the cells codes
param int level: the level of subdivision
param ReferenceCloud,[in,out] subset: set of points lying in the cell (references, no duplication)
param bool,optional areCodesTruncated: (default `False`) specifies if the codes are given in a truncated form or not
return the set of points lying in the cell (references, no duplication)

:return: the set of points lying in the cell
:rtype: ReferenceCloud )";

const char* DgmOctree_getPointsInCylindricalNeighbourhood_doc= R"(
Returns the points falling inside a cylinder.

Use :py:meth:`findBestLevelForAGivenNeighbourhoodSizeExtraction` to get the right
value for 'level' (only once as it only depends on the radius value).

**WARNING** the 'squareDistd' field of each neighbour in the NeighboursSet
structure is in fact the signed distance (not squared) of the point
relatively to the cylinder's center and projected along its axis.

:param CylindricalNeighbourhood neighbourhood: parameters structure

:return: the extracted points: list of :py:class:`PointDescriptor`
:rtype: list )";

const char* DgmOctree_getPointsInCylindricalNeighbourhoodProgressive_doc= R"(
Same as getPointsInCylindricalNeighbourhood with progressive approach.

Can be called multiple times (the 'currentHalfLength' parameter will increase
each time until 'maxHalfLength' is reached).

**WARNING** the 'squareDistd' field of each neighbour in the NeighboursSet
structure is in fact the signed distance (not squared) of the point
relatively to the cylinder's center and projected along its axis.

:param CylindricalNeighbourhood neighbourhood: parameters structure

:return: the extracted points (list of :py:class:`PointDescriptor`)
:rtype: list )";

const char* DgmOctree_getPointsInSphericalNeighbourhood_doc= R"(
Returns the points falling inside a sphere.

Use :py:meth:`findBestLevelForAGivenNeighbourhoodSizeExtraction` to get the right
value for 'level' (only once as it only depends on the radius value).

:param tuple sphereCenter: center coordinates
:param float radius: radius
:param int level: subdivision level at which to apply the extraction process

:return: neighbours points falling inside the sphere (list of :py:class:`PointDescriptor`)
:rtype: list )";

const char* DgmOctree_getTheCellPosWhichIncludesThePoint_doc= R"(
Returns the position **FOR THE DEEPEST LEVEL OF SUBDIVISION** of the cell that includes a given point.

The cell coordinates can be negative or greater than 2^MAX_OCTREE_LEVEL-1
as the point can lie outside the octree bounding-box.

:param tuple thePoint: the query point coordinates

:return: cellPos the computed position (3 int indices)
:rtype: tuple )";

const char* DgmOctree_getTheCellPosWhichIncludesThePointL_doc= R"(
Returns the position **for a given level of subdivision** of the cell that includes a given point.

The cell coordinates can be negative or greater than 2^N-1  (where N
is the level of subdivision) as the point can lie outside the octree
bounding-box.

:param tuple thePoint: the query point coordinates
:param int level: the level of subdivision

:return: cellPos the computed position (3 int indices)
:rtype: tuple )";

const char* DgmOctree_getTheCellPosWhichIncludesThePointLI_doc= R"(
Returns the position for a **given level of subdivision** of the cell that includes a given point.

The cell coordinates can be negative or greater than 2^N-1  (where N
is the level of subdivision) as the point can lie outside the octree
bounding-box. In this version, method indicates if the query point
is inside ("inbounds") or outside the octree bounding-box.

:param tuple thePoint: the query point coordinates
:param int level: the level of subdivision

:return: tuple (cellPos the computed position, 
                inBounds indicates if the query point is inside or outside the octree bounding-box)
:rtype: tuple
)";

const char* DgmOctree_IndexAndCode_doc= R"(
Association between an index and the code of an octree cell
Index could be the index of a point, in which case the code
would correspond to the octree cell where the point lies.

:ivar int index: the index
:ivar int code: theCode )";

const char* DgmOctree_indexComp_doc= R"(
Compares two IndexAndCode instances based on their index.

:param IndexAndCode a: first structure
:param IndexAndCode b: second structure

:return: whether the index of 'a' is smaller than the index of 'b'
:rtype: bool )";

const char* DgmOctree_NearestNeighboursSearchStruct_doc= R"(
Container of in/out parameters for nearest neighbour(s) search.

This structure is generic and can be used in multiple cases.
It is particularly useful when searching nearest neighbours around points
that lie in the same octree cell. In this case, several informations about
this cell should be given to the search algorithm through this structure, but only
once,before the first search. Then the search algorithm can be called multiple times,
and only few informations need to be updated (the query point, etc.).

**Information to set before search**

:ivar tuple queryPoint: Query point coordinates. Should be updated each time.
:ivar int level: Level of subdivision of the octree at which to start the search,
    Should be set once and for all.
:ivar int minNumberOfNeighbors: Minimal number of neighbours to find
    used only during multiple neighbours search (see findNearestNeighborsStartingFromCell).
    This is only indicative and not guaranteed.
:ivar tuple cellPos: Cell position in the octree of the cell including the query point.
    The position is expressed for the level of subdivision at which the search will
    be processed. Use see :py:meth:`getCellPos` to determine this position.
    This information should only be updated if the cell changes.
:ivar tuple cellCenter: Coordinates of the center of the cell including the query point.
    Use DgmOctree::computeCellCenter to determine these coordinates.
    This information should only be updated if the cell changes.
:ivar float maxSearchSquareDistd: Maximum neihgbours distance.
    The NN search process will stop if it reaches this radius even if it
    hasn't find any neighbour (acceleration). To disable this behavior,
    set the maxSearchSquareDistd to something <= 0).

**Information to set to 0 before search**

:ivar list minimalCellsSetToVisit: List of indexes of the cells that
    have been already visited by the algorithm.
    This field is updated by the search algorithm. It should only be emptied
    if the cell that includes the query points change. Only used by the
    "unique nearest point" search algorithm.
:ivar list pointsInNeighbourhood: List of :py:class:`PointDescriptor`.
    All the points that belong to the cubical neighbourhood of the current cell.
    This structure is only used by the "multiple nearest neighbours" search algorithms.
    The nearest points (relatively to the query point) are stored at the beginning of
    the vector. They are associated to their square distance to the query point.
:ivar int alreadyVisitedNeighbourhoodSize: Size of the cell neighbourhood
    that has been already visited by the algorithm.
    This field is updated by the search algorithm. It should only be reset to 0
    when the cell that includes the query point changes. A value of 0 means that
    no cell has been visited yet, 1 means that only the cell that includes the
    query point has been visited, 2 means that this cell and its 27 neighbourhing
    cells have been visited, etc.

**Result**

:ivar int theNearestPointIndex: The nearest point.
    This field is only used by the "unique nearest neighbour" search algorithm
    (see :py:meth:`findTheNearestNeighborStartingFromCell`).
)";

const char* DgmOctree_PointDescriptor_doc= R"(
Structure used during nearest neighbour search.

Association between a point, its index and its square distance to the query point.
The structure is made persistent, by copying the coordinates.

:ivar tuple point: point coordinates
:ivar int index: point index
:ivar float squareDistd: point associated distance value
)";

const char* DgmOctree_ProgressiveCylindricalNeighbourhood_doc= R"(
Input/output parameters structure for getPointsInCylindricalNeighbourhoodProgressive.

Derived from CylindricalNeighbourhood.
See CylindricalNeighbourhood for generic data members.

**Specific data members**

:ivar float currentHalfLength: Current search depth
:ivar list potentialCandidates: list of :py:class:`PointDescriptor` to store
    potential candidates for the next pass.
    Candidates are points close enough to the cylinder's axis but too far
    from its actual center.
:ivar tuple prevMinCornerPos: Previous search box (min corner)
:ivar tuple prevMaxCornerPos: Previous search box (max corner))";

const char* NearestNeighboursSearchStruct_prepare_doc= R"(
Initialize ready status (`False`), Updates maxD2 and minD2 with search radius and cellSize.)";

#endif /* CCOCTREEPY_DOCSTRINGS_HPP_ */
