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

#ifndef PCV_DOCSTRINGS_HPP_
#define PCV_DOCSTRINGS_HPP_


const char* PCV_doc= R"(
PCV is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginPCV function:

  isPCV_available = cc.isPluginPCV()

PCV is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginPCV():
      import cloudComPy.PCV
      cc.PCV.computeShadeVIS(...)
 )";

const char* PCV_computeShadeVIS_doc=R"(
Compute Ambient Occlusion for mesh or point cloud. (plugin PCV / ShadeVIS)

The ambient occlusion is either computed with normals samples on a sphere or upper Z hemisphere,
or with normals provided with a cloud.
Result on a ScalarField.

:param list clouds: list of clouds or meshes on witch to compute the ShadeVIS.
:param ccpointCloud,optional cloudWithNormals: cloud with normals, default None
:param int,optional rayCount: number of normals to use if sampled on a sphere or hemisphere, default 256.
:param int,optional resolution: render context resolution, default 1024
:param bool,optional is360: use the whole sphere or not (default false)
:param bool,optional isClosedMesh: if the mesh is closed, accelerate the computation (default false)

:return: success
:rtype: bool
)";

const char* PCV_initTrace_PCV_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* PCV_DOCSTRINGS_HPP_ */
