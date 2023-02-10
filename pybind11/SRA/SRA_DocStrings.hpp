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

#ifndef SRA_DOCSTRINGS_HPP_
#define SRA_DOCSTRINGS_HPP_


const char* SRA_doc= R"(
SRA is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginSRA function:

  isSRA_available = cc.isPluginSRA()

SRA is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginSRA():
      import cloudComPy.SRA
      cc.SRA.loadProfile(...)
 )";

const char* SRA_qSRA_doc = R"(
We only use the static method doComputeRadialDists of this class.
)";

const char* SRA_qSRA_doComputeRadialDists_doc = R"(
Computes the radial distances between the cloud and the surface of revolution.
A new scalarField named "Radial distance" is created.

:param ccPointCloud cloud: the point cloud
:param ccPoyline profile: the profile loaded with loadProfile
)";

const char* SRA_qSRA_loadProfile_doc = R"(
Load profile from a dedicated text file.
The format is given in CloudCompareGUI and in Python tests.

:param string filename: text file giving the revolution profile
:param int,optional axisDim: revolution axis, from (0, 1, 2) = (oX, oY, oZ), default 2 = oZ
:param bool,optional isAbsoluteHeightValues: default False

:return: a ccPolyline
:rtype: ccPolyline (None if problem)
)";

const char* SRA_exportMapAsCloud_doc=R"(
Export the map of radial distances as a point cloud.

:param ccPointCloud cloud: the point cloud used in radial distances computation
:param ccPoyline profile: the profile used in radial distances computation
:param ccScalarField sf: the "Radial distances" scalar field computed with doComputeRadialDists
:param double angStep_deg: the angle step of the map, in degrees
:param double yStep: the axial step of the map
:param double yMin: minimum axial value of the map
:param double yMax: maximum axial value of the map
:param bool,optional isConical: whether the projection is conical or cylindric, default False, i.e. cylindric projection
:param bool,optional ccw: whether the rotation is counter clockwise or not, default False i.e. clockwise
:param int,optional fillSt: fill strategy from cc.SRA.FillStrategyType, default cc.SRA.FILL_STRAT_AVG_DIST = average distance 
:param int,optional fillOpt: fill option from cc.SRA.EmptyCellFillOption, default cc.SRA.FILL_INTERPOLATE = interpolation
:param double,optional baseRadius: default 1.0. The dimension of the map along angular axis will be 2*pi*baseRadius

:return: a point cloud for the map
:rtype: ccPointCloud
)";

const char* SRA_exportMapAsMesh_doc=R"(
Export the map of radial distances as a mesh.

:param ccPointCloud cloud: the point cloud used in radial distances computation
:param ccPoyline profile: the profile used in radial distances computation
:param ccScalarField sf: the "Radial distances" scalar field computed with doComputeRadialDists
:param double angStep_deg: the angle step of the map, in degrees
:param double yStep: the axial step of the map
:param double yMin: minimum axial value of the map
:param double yMax: maximum axial value of the map
:param bool,optional isConical: whether the projection is conical or cylindric, default False, i.e. cylindric projection
:param bool,optional ccw: whether the rotation is counter clockwise or not, default False i.e. clockwise
:param int,optional fillSt: fill strategy from cc.SRA.FillStrategyType, default cc.SRA.FILL_STRAT_AVG_DIST = average distance 
:param int,optional fillOpt: fill option from cc.SRA.EmptyCellFillOption, default cc.SRA.FILL_INTERPOLATE = interpolation
:param double,optional baseRadius: default 1.0. The dimension of the map along angular axis will be 2*pi*baseRadius
:param int,optional colScale: from cc.SRA.colorScales default cc.SRA.DEFAULT_SCALES.BGYR
:param int,optional colScaleSteps: number of steps in the colorScale, default 256

:return: a mesh for the map
:rtype: ccMesh
)";

const char* SRA_initTrace_SRA_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* SRA_DOCSTRINGS_HPP_ */
