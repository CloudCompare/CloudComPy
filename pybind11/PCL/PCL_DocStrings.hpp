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

#ifndef PCL_DOCSTRINGS_HPP_
#define PCL_DOCSTRINGS_HPP_


const char* PCL_doc= R"(
PCL is a standard plugin of cloudComPy.
The plugin implements some methods of the Point Cloud Library (PCL): https://pointclouds.org.

The availability of the plugin can be tested with the isPluginPCL function:

  isPCL_available = cc.isPluginPCL()

PCL is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginPCL():
      import cloudComPy.PCL
      fgr = cc.PCL.FastGlobalRegistrationFilter()
      fgr.setParameters(...)
      fgr.compute()
)";

const char* PCL_initTrace_PCL_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";

const char* PCL_FastGlobalRegistrationFilter_doc= R"(
Filter based on "Q.-Y. Zhou, J. Park, and V. Koltun, Fast Global Registration, ECCV, 2016."
See https://github.com/isl-org/FastGlobalRegistration
)";

const char* PCL_FGRFsetParameters_doc= R"(
Defines the reference PointCloud, the list of PointCloud to align and the radius to use for registration.

:param ccPointCloud refCloud: the reference cloud
:param list alignClouds: the list of ccPointCloud to align.
:param float,optional radius: radius to use for computation, default 0 (automatic estimation of radius)
)";

const char* PCL_FGRFcompute_doc= R"(
Compute the PCL fast registration, using the PCL library.
Call :py:meth:`FastGlobalRegistrationFilter.setParameters` first!
The cloud is moved (no copy of the original).

:return: success
:rtype: bool
)";

const char* PCL_FGRFgetTransformation_doc= R"(
get the transformation matrix after the PCL fast registration.

:return: the transformation matrix
:rtype: ccGLMatrix
)";

const char* PCL_FGRFgetAutoRadius_doc= R"(
Get the radius value proposed by default for PCL registration.
:py:meth:`FastGlobalRegistrationFilter.setParameters` with radius 0 should be called first to compute the default radius value.

:return: default radius value
:rtype: float
)";

const char* PCL_NormalEstimation_doc= R"(
A tool to compute normals and optionally curvature of a cloud, using the PCL library.
See :py:meth:`NormalEstimation.setParameters` and :py:meth:`NormalEstimation.compute`.
)";

const char* PCL_NEcompute_doc= R"(
compute normals and optionally curvature of a cloud, using the PCL library.

:return: success
:rtype: bool
)";

const char* PCL_NLsetParameters_doc= R"(
Defines the parameters to use for normals and curvature calculation, using the PCL library.

:param ccPointCloud cloud: the point cloud
:param bool,optional useKnn: default False, whether to use a constant number of neighbors instead of a radius.
:param bool,optional ovewriteCurvature: default True, compute curvature, overwrite the existing curvature if present.
:param int,optional knn: default 10, number of neighbors (if useKnn is true)
:param double,optional radius: default 0. meaning an automatic calculation of radius.
)";

const char* PCL_MLSSmoothingUpsampling_doc= R"(
Smoothing and normal estimation based on polynomial reconstruction, using the PCL library.
See :py:meth:`MLSSmoothingUpsampling.setParameters` :py:meth:`MLSSmoothingUpsampling.compute` 
)";

const char* PCL_MLScompute_doc= R"(
Compute the smoothing and normals of the cloud, using the PCL library.
Call first :py:meth:`MLSSmoothingUpsampling.setParameters`
Get the result cloud with :py:meth:`MLSSmoothingUpsampling.getOutputCloud`

:return: success
:rtype: bool
)";

const char* PCL_MLSsetParameters_doc= R"(
Defines the parameters to use for smoothing and normals calculation, using the PCL library.

:param ccPointCloud cloud: the point cloud
:param bool,optional computeNormals: default True
:param float,optional searchRadius: default 0.01, radius to use for the computation.
:param bool,optional polyfit: default True, whether to fit a polynom to smooth the cloud.
:param int,optional order: default 2, polynom order.
:param float,optional sqrGauss: default 0.0001, squared Gauss parameter.
:param int,optional upsampleMethod: default 0, value from 0=NONE, 1=SAMPLE_LOCAL_PLANE, 2=RANDOM_UNIFORM_DENSITY, 3=VOXEL_GRID_DILATION
:param float,optional upsampingRadius: default 0.01, radius to use for upsampling (SAMPLE_LOCAL_PLANE method).
:param float,optional upsamplingStep: default 0.01, radius to use for upsampling step size (SAMPLE_LOCAL_PLANE method).
:param int,optional stepPointDensity: default 4, step point density (RANDOM_UNIFORM_DENSITY).
:param float,optional dilationVoxelSize: default 0.01, dilation voxel size (VOXEL_GRID_DILATION).
:param int,optional dilationIterations: default 0 (VOXEL_GRID_DILATION).
)";

const char* PCL_MLSgetOutputCloud_doc= R"(
Get the resulting cloud after :py:meth:`MLSSmoothingUpsampling.compute`.

:return: computed cloud
:rtype: ccPointCloud
)";

#endif /* PCL_DOCSTRINGS_HPP_ */
