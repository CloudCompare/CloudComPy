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

#ifndef CCMESHPY_DOCSTRINGS_HPP_
#define CCMESHPY_DOCSTRINGS_HPP_

const char* ccGenericMeshPy_computeMeshArea_doc= R"(
Compute the area of the mesh (sum of the triangles areas).

:return: mesh area
:rtype: float
)";

const char* ccGenericMeshPy_flagVerticesByType_doc= R"(
Flag the vertices of a mesh by type (normal, border, non manifold)

A new scalar field 'Vertex type' is created on the mesh associated cloud.

:return: success
:rtype: bool
)";

const char* ccGenericMeshPy_samplePoints_doc= R"(
Samples points on a mesh.

:param bool densityBased: whether the sampling method is based on a surface density or a number of points
:param float samplingParameter: either the surface density or the number of points
:param bool,optional withNormals: default `True`
:param bool,optional withRGB: default `True`
:param bool,optional withTexture: default `True`
:param progressDialog,optional progdiag: progress dialog, default None, use None!)";

const char* ccMeshPy_ccMesh_doc= R"(
A triangular mesh based on a cloud of vertices.)";

const char* ccMeshPy_clearTriNormals_doc= R"(
Removes per-triangle normals.
)";

const char* ccMeshPy_cloneMesh_doc= R"(
Clones this entity.

All the main features of the entity are cloned, except from the octree)";

const char* ccMeshPy_computeMeshVolume_doc= R"(
Compute the volume inside the mesh.

If the mesh is not closed, with holes or non manifold, a warning is issued.
Stats are available to get:

edgesCount: Total number of edges

edgesNotShared: Edges not shared (i.e. used by only one triangle) ==> hole or non closed

edgesSharedByTwo: Edges shared by exactly two triangles           ==> non manifold

edgesSharedByMore: Edges shared by more than two triangles        ==> non manifold

:return: tuple (volume, isWarning, stats)
:rtype: (float, bool, EdgeConnectivityStats)
)";

const char* ccMeshPy_crop2D_doc= R"(
Crop the mesh using a 2D polyline.

:param ccPolyline poly: polyline object
:param int orthoDim: normal plane, value in (0, 1, 2) 0 = oY, 1 = oX, 2 = oZ
:param bool inside: boolean

:return: the cropped cloud. Points are copied, the original cloud is not modified.
:rtype: ccPointCloud
)";

const char* ccMeshPy_EdgeConnectivityStats_doc= R"(
Statistics on edge connectivity for the mesh.

:ivar int edgesCount: Total number of edges
:ivar int edgesNotShared: Edges not shared (i.e. used by only one triangle) ==> hole or non closed
:ivar int edgesSharedByTwo: Edges shared by exactly two triangles           ==> non manifold
:ivar int edgesSharedByMore: Edges shared by more than two triangles        ==> non manifold
)";

const char* ccMeshPy_flipTriangles_doc=R"(
Flip the triangles of the mesh (reverse the node order).
)";

const char* ccMeshPy_getAssociatedCloud_doc= R"(
Returns the cloud corresponding to the mesh vertices.

:return: the associated cloud
:rtype: ccGenericPointCloud)";

const char* ccMeshPy_getTriangleVertIndexes_doc= R"(
Returns the 3 indexes of the nodes of a given triangle index.

:param int index: triangle index

:return: a tuple with 3 nodes indexes
:rtype: tuple )";

const char* ccMeshPy_IndexesToNpArray_doc= R"(
Wrap the indexes of nodes per triangle in the mesh into a numpy Array, without copy.

Returns a numpy Array of shape (number of triangles, 3).
Data is not copied, the numpy Array object does not own the data.

**WARNING** No automatic action on the Python side on the variables referencing the C++ object in case of destruction!

:return: numpy Array of shape (number of triangles, 3)
:rtype: ndarray
 )";

const char* ccMeshPy_IndexesToNpArray_copy_doc= R"(
Wrap the indexes of nodes per triangle in the mesh into a numpy Array, with copy.

Returns a numpy Array of shape (number of triangles, 3).
Data is copied, the  numpy Array object owns its data.
Ownership is transfered to Python:
the numpy Array object and its data will be handled by the Python Garbage Collector.

:return: numpy Array of shape (number of triangles, 3)
:rtype: ndarray
 )";

const char* ccMeshPy_subdivide_doc= R"(
Subdivides mesh (so as to ensure that all triangles are falls below 'maxArea'.

:param float maxArea: maximal triangle area to obtain afer subdivision

:return: subdivided mesh (if successful)
:rtype: ccMesh )";

const char* ccMeshPy_laplacianSmooth_doc= R"(
Laplacian smoothing.

:param int,optional nbIteration: smoothing iterations, default 20
:param float,optional factor: smoothing 'force', default 0.2

:return: success
:rtype: bool )";

const char* ccMeshPy_size_doc= R"(
Returns the number of triangles in the mesh.

:return:  number of triangles
:rtype: int)";

const char* ccMeshPy_triangulate_doc= R"(
Creates a Delaunay 2.5D mesh from a point cloud.

The triangulation can be either computed on the points projected
in the XY plane, or projected on the best least-square
fitting plane. The triangulation is in 2D (in the plane) but the
3D points are connected, so it's a kind of 2.5D triangulation (that
may present however several topological aberrations ;).

:param ccGenericPointCloud cloud: a point cloud
:param TRIANGULATION_TYPES type: the triangulation strategy
:param bool,optional updateNormals: (default `False`)
:param float,optional maxEdgeLength: max edge length for output triangles (default 0 = ignored)
:param int,optional dim: projection dimension (for axis-aligned meshes) (default 2)

:return: triangles mesh (if successful)
:rtype: ccMesh )";

const char* ccMeshPy_triangulateTwoPolylines_doc= R"(
Creates a Delaunay 2.5D mesh from two polylines.

:param ccPolyline p1: first polyline
:param ccPolyline p2: second polyline
:param CCVector3,optional projectionDir: projection direction, default None (use the best fit plane (normal)) 

:return: triangles mesh (if successful)
:rtype: ccMesh )";

const char* ccMeshPy_TRIANGULATION_TYPES_doc= R"("
Triangulation types.

- DELAUNAY_2D_AXIS_ALIGNED  Delaunay 2D triangulation in an axis-aligned plane.
- DELAUNAY_2D_BEST_LS_PLANE Delaunay 2D with points projected on the best least square fitting plane
)";

#endif /* CCMESHPY_DOCSTRINGS_HPP_ */
