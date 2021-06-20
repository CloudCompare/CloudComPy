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

#ifndef CLOUDCOMPY_DOCSTRINGS_HPP_
#define CLOUDCOMPY_DOCSTRINGS_HPP_

const char* cloudComPy_doc= R"(
cloudComPy is the Python module interfacing cloudCompare library.
Python3 access to cloudCompare objects is done like this:
::

  import cloudComPy as cc 
  cc.initCC()  # to do once before using plugins
  cloud = cc.loadPointCloud("/home/paul/CloudComPy/Data/boule.bin")
 
 )";

const char* cloudComPy_ICPres_doc=R"(
result parameters on ICP registration

:property aligned: the point cloud on which the transformation must be applied

:property transMat: the resulting transformation to apply

:property finalScale: calculated scale if rescale required

:property finalRMS: final error (RMS)

:property finalPointCount: number of points used to compute the final RMS
)";

const char* cloudComPy_ICP_doc=R"(
Applies ICP registration on two entities.

parameters:

- data: cloud to align
- model: reference cloud
- minRMSDecrease: The minimum error (RMS) reduction between two consecutive steps to continue process,
                  if CONVERGENCE_TYPE == MAX_ERROR_CONVERGENCE
- maxIterationCount: Stop after this number of iterations,
                     if CONVERGENCE_TYPE == MAX_ITER_CONVERGENCE
- randomSamplingLimit: Limit above which clouds should be randomly resampled
- removeFarthestPoints: If `True`, the algorithm will automatically ignore farthest points from the reference.
                        This is a trick to improve registration for slightly different clouds.
- method: Mode of convergence, CONVERGENCE_TYPE.MAX_ITER_CONVERGENCE or CONVERGENCE_TYPE.MAX_ERROR_CONVERGENCE
- adjustScale: Whether to release the scale parameter during the registration procedure or not
- finalOverlapRatio: Theoretical overlap ratio (at each iteration, only this percentage (between 0 and 1).
                     Will be used for registration (optional, default 1.0)
- useDataSFAsWeights: Weights for data points (optional, default `False`) = `False`,
- useModelSFAsWeights: Weights for model points (optional, default `False`)
- transformationFilters: Filters to be applied on the resulting transformation at each step.
                         Integer (optional, default 0)
   - SKIP_NONE           = 0
   - SKIP_RXY            = 1
   - SKIP_RYZ            = 2
   - SKIP_RXZ            = 4
   - SKIP_ROTATION       = 7
   - SKIP_TX             = 8
   - SKIP_TY             = 16
   - SKIP_TZ             = 32
   - SKIP_TRANSLATION    = 56
- maxThreadCount: Maximum number of threads to use (optional, default 0 = max))";

const char* cloudComPy_initCC_doc= R"(
Should be done once before using plugins!)";

const char* cloudComPy_loadPointCloud_doc= R"(
Load a 3D cloud from a file.

:param filename:
:type filename: string
:param shiftMode: shift mode from (`CC_SHIFT_MODE.AUTO`, `CC_SHIFT_MODE.XYZ`),  optional, default `AUTO`.

  - `CC_SHIFT_MODE.AUTO`: automatic shift of coordinates
  - `CC_SHIFT_MODE.XYZ`:  coordinates shift given by x, y, z parameters
  
:type shiftMode: CC_SHIFT_MODE
:param skip: optional parameter not used yet! default 0
:type skip: int
:param x: shift value for coordinates (mode XYZ),  default 0
:type x: float, optional
:param y: shift value for coordinates (mode XYZ),  default 0
:type y: float, optional
:param z: shift value for coordinates (mode XYZ),  default 0
:type z: float, optional

:return: a `ccPointCloud` object. Usage: see ccPointCloud doc.
:rtype: ccPointCloud)";

const char* cloudComPy_loadPolyline_doc= R"(
Load a polyline from a file.

parameters:

- filename
- shift mode from (AUTO, XYZ),  optional, default AUTO
  - AUTO: automatic shift of coordinates
  - XYZ:  coordinates shift given by x, y, z parameters
- skip: optional parameter not used yet! default 0
- x, y, z: optional shift values for coordinates (mode XYZ),  default 0

return a ccPolyline object.

Usage: see ccPolyline doc.)";

const char* cloudComPy_SavePointCloud_doc= R"(
Save a 3D cloud in a file.

parameters:

- cloud: ccPointCloud
- filename

return 0 or I/O error.)";

const char* cloudComPy_SaveEntities_doc= R"(
Save a list of entities (cloud, meshes, primitives...) in a file: use bin format!

parameters:

- list of entities
- filename

return 0 or I/O error.)";

const char* cloudComPy_computeCurvature_doc= R"("
Compute the curvature on a list of point clouds (create a scalarField).

parameters:

- CurvatureType from cloudCompare.GAUSSIAN_CURV, cloudCompare.MEAN_CURV, cloudCompare.NORMAL_CHANGE_RATE
- radius: try value obtained by GetPointCloudRadius
- list of clouds)";

const char* cloudComPy_filterBySFValue_doc= R"("
Create a new point cloud by filtering points using the current out ScalarField (see cloud.setCurrentOutScalarField).
Keep the points whose ScalarField value is between the min and max parameters.

parameters:

- minimum value
- maximum value
- cloud: ccPointCloud

return a ccPointCloud object.)";

const char* cloudComPy_GetPointCloudRadius_doc= R"("
Compute an estimate radius to use in computeCurvature.

parameters:

- list of clouds
- number of nodes wanted within the radius

return estimated radius)";

const char* cloudComPy_getScalarType_doc= R"(
Get the scalar type used in cloudCompare under the form defined in Numpy: 'float32' or 'float64')";

const char* cloudComPy_computeNormals_doc= R"(
Compute normals on a list of clouds and meshes.

parameters:

- selectedEntities: list of entities (clouds, meshes)

optional parameters:

- model: default = LOCAL_MODEL_TYPES.LS (Least Square best fitting plane)
- useScanGridsForComputation: default `True`, whether to use ScanGrids when available
- defaultRadius: default 0.0,
- minGridAngle_deg: default 1.0,
- orientNormals: default `True`
- useScanGridsForOrientation: default `True`, when ScanGrids available
- useSensorsForOrientation: default `True`, when Sensors available
- preferredOrientation: default ccNormalVectors::UNDEFINED
- orientNormalsMST: default `True`, use Minimum Spanning Tree
- mstNeighbors: default 6, for Minimum Spanning Tree
- computePerVertexNormals: default `True`, apply on mesh, if `True`, compute on vertices, if `False`, compute on triangles)";

#endif /* CLOUDCOMPY_DOCSTRINGS_HPP_ */
