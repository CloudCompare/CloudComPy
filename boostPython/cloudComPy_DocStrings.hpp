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
Result values on ICP registration.

:ivar ccPointCloud aligned: the point cloud on which the transformation must be applied

:ivar ccGLMatrix transMat: the resulting transformation to apply

:ivar float finalScale: calculated scale if rescale required

:ivar float finalRMS: final error (RMS)

:ivar int finalPointCount: number of points used to compute the final RMS
)";

const char* cloudComPy_ICP_doc=R"(
Applies ICP registration on two entities.

:param ccHObject data: cloud to align.
:param ccHObject model: reference cloud.
:param float minRMSDecrease: The minimum error (RMS) reduction between two consecutive steps to continue process,
       if CONVERGENCE_TYPE == MAX_ERROR_CONVERGENCE.
:param int maxIterationCount: Stop after this number of iterations,
       if CONVERGENCE_TYPE == MAX_ITER_CONVERGENCE.
:param int randomSamplingLimit: Limit above which clouds should be randomly resampled.
:param bool removeFarthestPoints: If `True`, the algorithm will automatically ignore farthest points from the reference.
       This is a trick to improve registration for slightly different clouds.
:param CONVERGENCE_TYPE method: Mode of convergence, CONVERGENCE_TYPE.MAX_ITER_CONVERGENCE or CONVERGENCE_TYPE.MAX_ERROR_CONVERGENCE.
:param bool adjustScale: Whether to release the scale parameter during the registration procedure or not.
:param float,optional finalOverlapRatio: Theoretical overlap ratio (at each iteration, only this percentage (between 0 and 1).
       Will be used for registration (default 1.0).
:param bool,optional useDataSFAsWeights: Weights for data points (default `False`).
:param bool,optional useModelSFAsWeights: Weights for model points (default `False`).
:param int,optional transformationFilters: Filters to be applied on the resulting transformation at each step (default 0).

   - SKIP_NONE           = 0
   - SKIP_RXY            = 1
   - SKIP_RYZ            = 2
   - SKIP_RXZ            = 4
   - SKIP_ROTATION       = 7
   - SKIP_TX             = 8
   - SKIP_TY             = 16
   - SKIP_TZ             = 32
   - SKIP_TRANSLATION    = 56

:param int,optional maxThreadCount: Maximum number of threads to use (default 0 = max))";

const char* cloudComPy_initCC_doc= R"(
Should be done once before using plugins!)";

const char* cloudComPy_loadPointCloud_doc= R"(
Load a 3D cloud from a file.

:param filename:
:type filename: str
:param shiftMode: shift mode from (`CC_SHIFT_MODE.AUTO`, `CC_SHIFT_MODE.XYZ`),  optional, default `AUTO`.

  - `CC_SHIFT_MODE.AUTO`: automatic shift of coordinates
  - `CC_SHIFT_MODE.XYZ`:  coordinates shift given by x, y, z parameters
  
:type shiftMode: CC_SHIFT_MODE
:param skip: parameter not used yet! default 0
:type skip: int, optional
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

:param str filename: The polyline file.
:param CC_SHIFT_MODE,optional shift mode: from (AUTO, XYZ),  optional, default AUTO.

  - AUTO: automatic shift of coordinates
  - XYZ:  coordinates shift given by x, y, z parameters

:param int,optional skip: not used yet! default 0.
:param float,optional x: optional shift value for coordinates (mode XYZ), default 0
:param float,optional y: optional shift value for coordinates (mode XYZ), default 0
:param float,optional z: optional shift value for coordinates (mode XYZ), default 0

:return: a ccPolyline object.
:rtype: ccPolyline

Usage: see ccPolyline doc.)";

const char* cloudComPy_SavePointCloud_doc= R"(
Save a 3D cloud in a file.

:param ccPointCloud cloud: the cloud to save.
:param str filename: The cloud file.

:return: 0 or I/O error.
:rtype: CC_FILE_ERROR)";

const char* cloudComPy_SaveEntities_doc= R"(
Save a list of entities (cloud, meshes, primitives...) in a file: use bin format!

:param entities: list of entities
:type entities: list of :py:class:`ccHObject`
:param str filename: The entities file.

:return: 0 or I/O error.
:rtype: CC_FILE_ERROR)";

const char* cloudComPy_computeCurvature_doc= R"(
Compute the curvature on a list of point clouds (create a scalarField).

:param CurvatureType cvt: from CurvatureType.GAUSSIAN_CURV, CurvatureType.MEAN_CURV, CurvatureType.NORMAL_CHANGE_RATE.
:param float radius: try value obtained by GetPointCloudRadius.
:param clouds: list of clouds
:type clouds: list of :py:class:`ccHObject`)";

const char* cloudComPy_filterBySFValue_doc= R"(
Create a new point cloud by filtering points using the current out ScalarField (see cloud.setCurrentOutScalarField).
Keep the points whose ScalarField value is between the min and max parameters.

:param float min: minimum value to keep
:param float max: maximum value to keep
:param ccPointCloud cloud: the input cloud

:return: a ccPointCloud object.
:rtype: ccPointCloud )";

const char* cloudComPy_GetPointCloudRadius_doc= R"(
Compute an estimate radius to use in computeCurvature.

:param clouds: list of clouds
:type clouds: list of :py:class:`ccHObject`
:param int nodes: number of nodes wanted within the radius

:return: estimated radius
:rtype: float )";

const char* cloudComPy_getScalarType_doc= R"(
Get the scalar type used in cloudCompare under the form defined in Numpy: 'float32' or 'float64'

:return: scalar type
:rtype: str )";

const char* cloudComPy_computeNormals_doc= R"(
Compute normals on a list of clouds and meshes.

:param selectedEntities: list of entities (clouds, meshes)
:type selectedEntities: list of :py:class:`ccHObject`
:param LOCAL_MODEL_TYPES,optional model: default = LOCAL_MODEL_TYPES.LS (Least Square best fitting plane)
:param bool,optional useScanGridsForComputation: default `True`, whether to use ScanGrids when available
:param float,optional defaultRadius: default 0.0
:param float,optional minGridAngle_deg: default 1.0
:param bool,optional orientNormals: default `True`
:param bool,optional useScanGridsForOrientation: default `True`, when ScanGrids available
:param bool,optional useSensorsForOrientation: default `True`, when Sensors available
:param Orientation,optional preferredOrientation: default Orientation.UNDEFINED
:param bool,optional orientNormalsMST: default `True`, use Minimum Spanning Tree
:param int,optional mstNeighbors: default 6, for Minimum Spanning Tree
:param bool,optional computePerVertexNormals: default `True`, apply on mesh, if `True`, compute on vertices, if `False`, compute on triangles)";

#endif /* CLOUDCOMPY_DOCSTRINGS_HPP_ */
