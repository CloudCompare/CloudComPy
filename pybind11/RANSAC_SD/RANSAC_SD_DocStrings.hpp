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

#ifndef RANSAC_SD_DOCSTRINGS_HPP_
#define RANSAC_SD_DOCSTRINGS_HPP_


const char* RANSAC_SD_doc= R"(
RANSAC_SD is a standard plugin of cloudComPy.

Wrapper to Schnabel et al. library for automatic shape detection in point cloud
"Efficient RANSAC for Point-Cloud Shape Detection", Ruwen Schnabel, Roland Wahl, 
and Reinhard Klein, in Computer Graphics Forum (June 2007), 26:2(214-226)
http://cg.cs.uni-bonn.de/en/publications/paper-details/schnabel-2007-efficient/

The availability of the plugin can be tested with the isPluginRANSAC_SD function:

  isRANSAC_SD_available = cc.isPluginRANSAC_SD()

RANSAC_SD is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginRANSAC_SD():
      import cloudComPy.RANSAC_SD
      params = cc.RANSAC_SD.RansacParams()
      results = cc.RANSAC_SD.computeRANSAC_SD(cloud, params)
)";

const char* RANSAC_SD_computeRANSAC_SD_doc=R"(
RANSAC Shape Detection is a simple interface to the automatic shape detection algorithm
proposed by Ruwen Schnabel et al. of Bonn university
(Efficient RANSAC for Point-Cloud Shape Detection).
)";

const char* RANSAC_SD_initTrace_RANSAC_SD_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";

const char* RANSAC_SD_RansacParams_doc=R"(
RANSAC_SD parameters
)";

const char* RANSAC_SD_RansacParams_epsilon_doc=R"(
Maximum distance if the samples to the ideal shape, default 0.005.)";

const char* RANSAC_SD_RansacParams_bitmapEpsilon_doc=R"(
Sampling resolution, should correspond to the average distance of neighbors points in data, default 0.001.)";

const char* RANSAC_SD_RansacParams_supportPoints_doc=R"(
Minimal number of points required to define a primitive, default 500.)";

const char* RANSAC_SD_RansacParams_maxNormalDev_deg_doc=R"(
Maximum deviation from the ideal shape normal vector (in degrees), default 25.)";

const char* RANSAC_SD_RansacParams_probability_doc=R"(
Probability that no better candidate was overlooked during the sampling,
the lower the better, default 0.01)";

const char* RANSAC_SD_RansacParams_randomColor_doc=R"(
Set random color for each shape found, default True.)";

const char* RANSAC_SD_RansacParams_createCloudFromLeftOverPoints_doc=R"(
Save leftover points into a new cloud, default True.)";

const char* RANSAC_SD_RansacParams_allowSimplification_doc=R"(
Attempt to simplify shapes.
Will attempt to convert torus, cones, spheres, cylinders into simpler shapes.
For instance, cones may be simplified into cylinder, sphere or plane. Default True.)";

const char* RANSAC_SD_RansacParams_allowFitting_doc=R"(
Use Least Squares fitting on found shapes, default True.)";

const char* RANSAC_SD_RansacParams_minSphereRadius_doc=R"(
Minimum sphere radius, default -infinity.)";

const char* RANSAC_SD_RansacParams_maxSphereRadius_doc=R"(
Maximum sphere radius, default +infinity.)";

const char* RANSAC_SD_RansacParams_minCylinderRadius_doc=R"(
Minimum cylinder radius, default -infinity.)";

const char* RANSAC_SD_RansacParams_maxCylinderRadius_doc=R"(
Maximum cylinder radius, default +infinity.)";

const char* RANSAC_SD_RansacParams_maxCylinderLength_doc=R"(
Maximum cylinder length, default +infinity.)";

const char* RANSAC_SD_RansacParams_maxConeRadius_doc=R"(
Maximum cone radius, default +infinity.)";

const char* RANSAC_SD_RansacParams_maxConeAngle_deg_doc=R"(
Maximum cone angle, default +infinity.)";

const char* RANSAC_SD_RansacParams_maxConeLength_doc=R"(
Maximum cone length, default +infinity.)";

const char* RANSAC_SD_RansacParams_minTorusMinorRadius_doc=R"(
Minimum tore minor radius, default -infinity.)";

const char* RANSAC_SD_RansacParams_minTorusMajorRadius_doc=R"(
Minimum tore major radius, default -infinity.)";

const char* RANSAC_SD_RansacParams_maxTorusMinorRadius_doc=R"(
Maximum tore minor radius, default +infinity.)";

const char* RANSAC_SD_RansacParams_maxTorusMajorRadius_doc=R"(
Maximum tore major radius, default +infinity.)";

const char* RANSAC_SD_RansacParams_setPrimEnabled_doc=R"(
Define the type of primitive (plane, sphere, cylinder, cone, torus) enabled.
By default, plane, sphere, cylinder are enabled.
Types of primitive are from:

 - RANSAC_PRIMITIVE_TYPES.RPT_PLANE
 - RANSAC_PRIMITIVE_TYPES.RPT_SPHERE
 - RANSAC_PRIMITIVE_TYPES.RPT_CYLINDER
 - RANSAC_PRIMITIVE_TYPES.RPT_CONE
 - RANSAC_PRIMITIVE_TYPES.RPT_TORUS

:param RANSAC_PRIMITIVE_TYPES rpt: type of primitive
:param bool isEnabled: whether the type is enabled or not)";

const char* RANSAC_SD_RansacParams_getPrimEnabled_doc=R"(
Check the type of primitive (plane, sphere, cylinder, cone, torus) enabled.
By default, plane, sphere, cylinder are enabled.
Types of primitive are from:

 - RANSAC_PRIMITIVE_TYPES.RPT_PLANE
 - RANSAC_PRIMITIVE_TYPES.RPT_SPHERE
 - RANSAC_PRIMITIVE_TYPES.RPT_CYLINDER
 - RANSAC_PRIMITIVE_TYPES.RPT_CONE
 - RANSAC_PRIMITIVE_TYPES.RPT_TORUS

:param RANSAC_PRIMITIVE_TYPES rpt: type of primitive

:return: whether the type is enabled or not
:rtype: bool)";

const char* RANSAC_SD_RansacParams_optimizeForCloud_doc=R"(
Sets values for epsilon and bitmapEpsilon based on the cloud bounding box, maximum dimension.
epsilon is set to 0.005 * maxDim, bitmapEpsilon is set to 0.01 * maxDim.

:param ccPointCloud cloud: the cloud to be analyzed.)";

#endif /* RANSAC_SD_DOCSTRINGS_HPP_ */
