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

#ifndef CLOUDSAMPLINGTOOLSPY_DOCSTRINGS_HPP_
#define CLOUDSAMPLINGTOOLSPY_DOCSTRINGS_HPP_

const char* CloudSamplingToolsPy_CloudSamplingTools_doc= R"(
Several point cloud resampling algorithms (octree-based, random, etc.))";

const char* CloudSamplingToolsPy_SFModulationParams_doc= R"(
Parameters for the scalar-field based modulation of a parameter)";

const char* CloudSamplingToolsPy_enabled_doc= R"(
Whether the modulation is enabled or not, default False)";

const char* CloudSamplingToolsPy_a_doc= R"(
Modulation scheme: y = a.sf + b, float a, default 0.0)";

const char* CloudSamplingToolsPy_b_doc= R"(
Modulation scheme: y = a.sf + b, float b, default 1.0)";

const char* CloudSamplingToolsPy_resampleCloudWithOctreeAtLevel_doc= R"(
Resamples a point cloud (process based on the octree)

A resampling algorithm is applied inside each cell of the octree. The
different resampling methods are represented as an enumerator
(see RESAMPLING_CELL_METHOD) and consist in simple processes
such as replacing all the points lying in a cell by the cell center or
by the points gravity center.

:param GenericIndexedCloudPersist cloud: the point cloud to resample
:param int octreeLevel: the octree level at which to perform the resampling process
:param RESAMPLING_CELL_METHOD resamplingMethod: resampling method (applied to each octree cell)
       value from RESAMPLING_CELL_METHOD.CELL_CENTER, RESAMPLING_CELL_METHOD.CELL_GRAVITY_CENTER
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)
:param DgmOctree,optional inputOctree: default None, if the octree has been already computed,
       it can be used by the process (avoid recomputation)

:return: the resampled cloud (new cloud)
:rtype: ccPointCloud
)";

const char* CloudSamplingToolsPy_resampleCloudWithOctree_doc= R"(
Resamples a point cloud (process based on the octree)

Same as :py:meth:`resampleCloudWithOctreeAtLevel` method, apart the fact that instead
of giving a specific octree subdivision level as input parameter, one can specify
an approximative number of points for the resulting cloud (algorithm will
automatically determine the corresponding octree level).

:param GenericIndexedCloudPersist cloud: the point cloud to resample
:param int newNumberOfPoints: desired number of points (approximative)
:param RESAMPLING_CELL_METHOD resamplingMethod: resampling method (applied to each octree cell)
       value from RESAMPLING_CELL_METHOD.CELL_CENTER, RESAMPLING_CELL_METHOD.CELL_GRAVITY_CENTER
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)
:param DgmOctree,optional inputOctree: default None, if the octree has been already computed,
       it can be used by the process (avoid recomputation)

:return: the resampled cloud (new cloud)
:rtype: ccPointCloud
)";

const char* CloudSamplingToolsPy_subsampleCloudWithOctreeAtLevel_doc= R"(
Subsamples a point cloud (process based on the octree)

A subsampling algorithm is applied inside each cell of the octree. The
different subsampling methods are represented as an enumerator
(see SUBSAMPLING_CELL_METHOD) and consist in simple processes
such as choosing a random point, or the one closest to the cell center.

:param GenericIndexedCloudPersist cloud: point cloud to subsample
:param int octreeLevel: octree level at which to perform the subsampling process
:param SUBSAMPLING_CELL_METHOD subsamplingMethod: subsampling method (applied to each octree cell)
       value from SUBSAMPLING_CELL_METHOD.RANDOM_POINT, SUBSAMPLING_CELL_METHOD.NEAREST_POINT_TO_CELL_CENTER
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)
:param DgmOctree,optional inputOctree: default None, if the octree has been already computed,
       it can be used by the process (avoid recomputation)

:return: a reference cloud corresponding to the subsampling 'selection'
:rtype: ReferenceCloud
)";

const char* CloudSamplingToolsPy_subsampleCloudWithOctree_doc= R"(
Subsamples a point cloud (process based on the octree)

Same as :py:meth:`subsampleCloudWithOctreeAtLevel` method, apart the fact that instead
of giving a specific octree subdivision level as input parameter, one can specify
an approximative number of points for the resulting cloud (algorithm will
automatically determine the corresponding octree level).

:param GenericIndexedCloudPersist cloud: point cloud to subsample
:param int newNumberOfPoints: desired number of points (approximative)
:param SUBSAMPLING_CELL_METHOD subsamplingMethod: resampling method (applied to each octree cell)
       value from SUBSAMPLING_CELL_METHOD.RANDOM_POINT, SUBSAMPLING_CELL_METHOD.NEAREST_POINT_TO_CELL_CENTER
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)
:param DgmOctree,optional inputOctree: default None, if the octree has been already computed,
       it can be used by the process (avoid recomputation)

:return: a reference cloud corresponding to the subsampling 'selection'
:rtype: ReferenceCloud
)";

const char* CloudSamplingToolsPy_subsampleCloudRandomly_doc= R"(
Subsamples a point cloud (process based on random selections)

A very simple subsampling algorithm that simply consists in selecting
"n" different points, in a random way.

:param GenericIndexedCloudPersist cloud: point cloud to subsample
:param int newNumberOfPoints: desired number of points (exact)
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)

:return: a reference cloud corresponding to the subsampling 'selection'
:rtype: ReferenceCloud
)";

const char* CloudSamplingToolsPy_resampleCloudSpatially_doc= R"(
Resamples a point cloud (process based on inter point distance)

The cloud is resampled so that there is no point nearer than a given distance to other points
It works by picking a reference point, removing all points which are to close to this point,
and repeating these two steps until the result is reached

:param GenericIndexedCloudPersist cloud: the point cloud to resample
:param float minDistance: the distance under which a point in the resulting cloud cannot have any neighbour
:param SFModulationParams modParams: parameters of the subsampling behavior modulation with a scalar field (optional)
:param DgmOctree,optional octree: default None, associated octree if available
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)

:return: a reference cloud corresponding to the resampling 'selection'
:rtype: ReferenceCloud
)";

const char* CloudSamplingToolsPy_sorFilter_doc= R"(
Statistical Outliers Removal (SOR) filter

This filter removes points based on their mean distance to their distance
(by comparing it to the average distance of all points to their neighbors).
It is equivalent to PCL StatisticalOutlierRemoval filter
(see `Removing outliers using a StatisticalOutlierRemoval filter <http://pointclouds.org/documentation/tutorials/statistical_outlier.html>`_)

:param GenericIndexedCloudPersist cloud: the point cloud to resample
:param int,optional knn: default 6,number of neighbors
:param float,optional nSigma: default 1.0, number of sigmas under which the points should be kept
:param DgmOctree,optional octree: default None, associated octree if available
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)

:return: a reference cloud corresponding to the filtered cloud
:rtype: ReferenceCloud
)";

const char* CloudSamplingToolsPy_noiseFilter_doc= R"(
Noise filter based on the distance to the approximate local surface

This filter removes points based on their distance relatively to the best fit plane computed on their neighbors.

:param GenericIndexedCloudPersist cloud: the point cloud to resample
:param float kernelRadius: neighborhood radius
:param float nSigma: number of sigmas under which the points should be kept
:param bool,optional removeIsolatedPoints: default False, whether to remove isolated points
       (i.e. with 3 points or less in the neighborhood)
:param bool,optional useKnn: default False, whether to use a constant number of neighbors instead of a radius
:param int,optional knn: default 6, number of neighbors (if useKnn is true)
:param bool,optional useAbsoluteError: default True, whether to use an absolute error instead of 'n' sigmas
:param float,optional absoluteError: default 0.0, absolute error (if useAbsoluteError is true)
:param DgmOctree,optional octree: default None, associated octree if available
:param GenericProgressCallback,optional progressCb: default None,
       the client application can get some notification of the process progress through this callback mechanism
       (not available yet)

:return: a reference cloud corresponding to the filtered cloud
:rtype: ReferenceCloud
)";

#endif /* CLOUDSAMPLINGTOOLSPY_DOCSTRINGS_HPP_ */
