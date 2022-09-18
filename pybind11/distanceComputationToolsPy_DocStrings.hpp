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

#ifndef DISTANCECOMPUTATIONTOOLSPY_DOCSTRINGS_HPP_
#define DISTANCECOMPUTATIONTOOLSPY_DOCSTRINGS_HPP_

const char* distanceComputationToolsPy_Cloud2CloudDistancesComputationParams_doc= R"(
Cloud-to-cloud "Hausdorff" distance computation parameters)";

const char* distanceComputationToolsPy_octreeLevel_doc= R"(
Level of subdivision of the octree at witch to apply the distance computation algorithm.

TODO: If set to 0 (default) the algorithm will try to guess the best level automatically.)";

const char* distanceComputationToolsPy_maxSearchDist_doc= R"(
Maximum search distance (true distance won't be computed if greater).

Set to -1 to deactivate (default).
**WARNING** Not compatible with closest point set determination (see CPSet))";

const char* distanceComputationToolsPy_multiThread_doc= R"(
Whether to use multi-thread (default) or single thread mode.

If maxSearchDist > 0, single thread mode will be forced.)";

const char* distanceComputationToolsPy_maxThreadCount_doc= R"(
Maximum number of threads to use (default 0 = max).)";

const char* distanceComputationToolsPy_localModel_doc= R"(
Type of local 3D modeling to use.

Default: NO_MODEL. Otherwise see CC_LOCAL_MODEL_TYPES.)";

const char* distanceComputationToolsPy_useSphericalSearchForLocalModel_doc= R"(
Whether to use a fixed number of neighbors or a (sphere) radius for nearest neighbours search.

For local models only (i.e. ignored if localModel = NO_MODEL). Default `False`)";

const char* distanceComputationToolsPy_kNNForLocalModel_doc= R"(
Number of neighbours for nearest neighbours search (local model).

For local models only (i.e. ignored if localModel = NO_MODEL).
Ignored if useSphericalSearchForLocalModel is `True`. Default 0)";

const char* distanceComputationToolsPy_radiusForLocalModel_doc= R"(
Radius for nearest neighbours search (local model).

For local models only (i.e. ignored if localModel = NO_MODEL).
Ignored if useSphericalSearchForLocalModel is `True`. Default 0)";

const char* distanceComputationToolsPy_reuseExistingLocalModels_doc= R"(
Whether to use an approximation for local model computation.

For local models only (i.e. ignored if localModel = NO_MODEL).
Computation is much faster but less "controlled". Default `False`)";

const char* distanceComputationToolsPy_CPSet_doc= R"(
Container of (references to) points to store the 'Closest Point Set'.

The Closest Point Set corresponds to (the reference to) each compared point's closest neighbour.
warning Not compatible with max search distance (see maxSearchDist). Default None)";

const char* distanceComputationToolsPy_setSplitDistances_doc= R"(
Split distances (one scalar field per dimension: X, Y and Z).

To activate split distance, fill this with the cloud size, it will create 3 scalar fields
to store the X, Y and Z distances. These scalar fields are not associated to the cloud.
(that can be done explicitely later).

Default None)";

const char* distanceComputationToolsPy_getSplitDistance_doc= R"(
Split distances (one scalar field per dimension: X, Y and Z).

Default None)";

const char* distanceComputationToolsPy_resetFormerDistances_doc= R"(
Whether to keep the existing distances as is (if any) or not.

By default, any previous distances/scalar values stored in the 'enabled' scalar field will be
reset before computing them again.)";

const char* distanceComputationToolsPy_Cloud2MeshDistancesComputationParams_doc= R"(
Cloud-to-mesh distances computation parameters.)";

const char* distanceComputationToolsPy_useDistanceMap_doc= R"(
Use distance map (acceleration).

If `True` the distances will be approximated by a Distance Transform.
**WARNING** Incompatible with signed distances or Closest Point Set. Default `False`)";

const char* distanceComputationToolsPy_signedDistances_doc= R"(
Whether to compute signed distances or not.

If `True`, the computed distances will be signed (in this case, the Distance Transform can't be used
and therefore useDistanceMap will be ignored). Default `False`)";

const char* distanceComputationToolsPy_flipNormals_doc= R"(
Whether triangle normals should be computed in the 'direct' order (`True`) or 'indirect' (`False`).

Default `False`)";

const char* distanceComputationToolsPy_DistanceComputationTools_doc= R"(
Several entity-to-entity distances computation algorithms (cloud-cloud, cloud-mesh, etc.).)";

const char* distanceComputationToolsPy_computeCloud2CloudDistances_doc= R"(
Computes the "nearest neighbour distance" between two point clouds (formerly named "Hausdorff distance").

The main algorithm and its different versions (with or without local modeling) are described in
Daniel Girardeau-Montaut's PhD manuscript (Chapter 2, section 2.3). It is the standard way to compare
directly two dense (and globally close) point clouds.

**WARNING** The current scalar field of the compared cloud should be enabled. By default it will be reset to
NAN_VALUE but one can avoid this by defining the Cloud2CloudDistanceComputationParams::resetFormerDistances
parameters to `False`. But even in this case, only values above Cloud2CloudDistanceComputationParams::maxSearchDist
will remain untouched.

**WARNING** Max search distance (Cloud2CloudDistanceComputationParams::maxSearchDist > 0) is not compatible with the
determination of the Closest Point Set (Cloud2CloudDistanceComputationParams::CPSet)

:param GenericIndexedCloudPersist comparedCloud: the compared cloud
       (the distances will be computed on these points)
:param GenericIndexedCloudPersist referenceCloud: the reference cloud
       (the distances will be computed relatively to these points)
:param Cloud2CloudDistancesComputationParams params: distance computation parameters
:param GenericProgressCallback,optional progressCb: the client application can get some notification
       of the process progress through this callback mechanism (see GenericProgressCallback)
       default None, not available yet.
:param DgmOctree,optional compOctree: the pre-computed octree of the compared cloud
       (warning: both octrees must have the same cubical bounding-box - it is automatically computed if 0)
:param DgmOctree,optional refOctree: the pre-computed octree of the reference cloud
       (warning: both octrees must have the same cubical bounding-box - it is automatically computed if 0)

:return: >0 if ok, a negative value otherwise
:rtype: int )";

const char* distanceComputationToolsPy_computeCloud2MeshDistances_doc= R"(
Computes the distance between a point cloud and a mesh.

The algorithm, inspired from METRO by Cignoni et al., is described
in Daniel Girardeau-Montaut's PhD manuscript (Chapter 2, section 2.2).
It is the general way to compare a point cloud with a triangular mesh.

:param GenericIndexedCloudPersist pointCloud: the compared cloud
       (the distances will be computed on these points)
:param GenericIndexedMesh mesh: the reference mesh
       (the distances will be computed relatively to its triangles)
:param Cloud2MeshDistancesComputationParams params: parameters
:param GenericProgressCallback,optional progressCb: the client application can get some notification
       of the process progress through this callback mechanism (see GenericProgressCallback)
       default None, not available yet.
:param DgmOctree,optional cloudOctree: the pre-computed octree of the compared cloud
       (warning: its bounding box should be equal to the union of both point cloud
       and mesh bbs and it should be cubical - it is automatically computed if 0)

:return: >0 if ok, a negative value otherwise
:rtype: int )";

const char* distanceComputationToolsPy_computeApproxCloud2CloudDistance_doc= R"(
Computes approximate distances between two point clouds.

This methods uses an exact Distance Transform to approximate the real distances.
Therefore, the greater the octree level is (it is used to determine the grid step), the finer
the result will be (but more memory and time will be needed).

:param GenericIndexedCloudPersist comparedCloud: the compared cloud
:param GenericIndexedCloudPersist referenceCloud: the reference cloud
:param int,optional octreeLevel: the octree level at which to compute the Distance Transform, default 7
:param double,optional maxSearchDist: Maximum search distance, default 0.
:param GenericProgressCallback,optional progressCb: the client application can get some notification
       of the process progress through this callback mechanism (see GenericProgressCallback)
       default None, not available yet.
:param DgmOctree,optional compOctree: the pre-computed octree of the compared cloud
       (warning: both octrees must have the same cubical bounding-box - it is automatically computed if 0)
:param DgmOctree,optional refOctree: the pre-computed octree of the reference cloud
       (warning: both octrees must have the same cubical bounding-box - it is automatically computed if 0)

:return: a list of statistics (min, max, mean, variance, max error) or an empty list if problem
:rtype: list )";

const char* distanceComputationToolsPy_computeApproxCloud2MeshDistance_doc= R"(
Computes approximate distances between a point cloud and a mesh.

:param GenericIndexedCloudPersist pointCloud: the compared cloud
       (the distances will be computed on these points)
:param GenericIndexedMesh mesh: the reference mesh

:return: a list of statistics (min, max, mean, variance, max error) or an empty list if problem
:rtype: list )";

const char* distanceComputationToolsPy_determineBestOctreeLevel_doc= R"(
compute the best octree level to use for cloud to mesh or cloud distance calculation (C2M or C2C).

**WARNING** if a scalar field "Approx. distances" is present in the compared cloud, it will be used
for the determination: in case of several distance computations with different clouds or meshes,
remove it before the call.

:param ccPointCloud compCloud: the compared cloud
       (the distances will be computed on these points)
:param GenericIndexedMesh,optional refMesh: the reference mesh, default None
       (if None, referenceCloud must be provided).
       (the distances will be computed relatively to its triangles)
:param ccPointCloud,optional refCloud: the reference cloud, default None
       (if None, mesh must be provided).
       (the distances will be computed relatively to these points)
:param double,optional maxSearchDist: Maximum search distance, default 0.

:return: the best octree level to use for C2M or C2C.
:rtype: int)";


#endif /* DISTANCECOMPUTATIONTOOLSPY_DOCSTRINGS_HPP_ */
