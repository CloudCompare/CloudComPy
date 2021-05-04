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

#ifndef CCMESHPY_DOCSTRINGS_HPP_
#define CCMESHPY_DOCSTRINGS_HPP_

const char* ccGenericMeshPy_samplePoints_doc= R"(
Samples points on a mesh
parameters:
- densityBased: boolean, the sampling method is based on a surface density or a number of points
- samplingParameter: double, either the surface density or the number of points
optional: 
- withNormals: boolean, default True
- withRGB: boolean, default True
- withTexture: boolean, default True
- pDl: progress dialog, default None)";

const char* ccMeshPy_ccMesh_doc= R"(
a triangular mesh based on a cloud of vertices.)";

const char* ccMeshPy_cloneMesh_doc= R"(
Clones this entity
All the main features of the entity are cloned, except from the octree)";

const char* ccMeshPy_getAssociatedCloud_doc= R"(
return the cloud corresponding to the mesh vertices.)";

const char* ccMeshPy_subdivide_doc= R"(
Subdivides mesh (so as to ensure that all triangles are falls below 'maxArea'
param maxArea
return subdivided mesh (if successful))";

const char* ccMeshPy_laplacianSmooth_doc= R"(
Laplacian smoothing
param nbIteration smoothing iterations, optional, default 20
param factor smoothing 'force', optional, default 0.2)";

const char* ccMeshPy_triangulate_doc= R"(
Creates a Delaunay 2.5D mesh from a point cloud
The triangulation can be either computed on the points projected
in the XY plane, or projected on the best least-square
fitting plane. The triangulation is in 2D (in the plane) but the
3D points are connected, so it's a kind of 2.5D triangulation (that
may present however several topological aberrations ;).
param cloud a point cloud
param type the triangulation strategy
param updateNormals boolean (default false)
param maxEdgeLength max edge length for output triangles (default 0 = ignored)
param dim projection dimension (for axis-aligned meshes) (default 2))";

const char* ccMeshPy_TRIANGULATION_TYPES_doc= R"("
Triangulation types
DELAUNAY_2D_AXIS_ALIGNED  Delaunay 2D triangulation in an axis-aligned plane
DELAUNAY_2D_BEST_LS_PLANE Delaunay 2D with points projected on the best least square fitting plane)";

#endif /* CCMESHPY_DOCSTRINGS_HPP_ */
