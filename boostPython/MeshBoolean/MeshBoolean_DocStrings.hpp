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

#ifndef MeshBoolean_DOCSTRINGS_HPP_
#define MeshBoolean_DOCSTRINGS_HPP_


const char* MeshBoolean_doc= R"(
MeshBoolean is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginMeshBoolean function:

  isMeshBoolean_available = cc.isPluginMeshBoolean()

MeshBoolean is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginMeshBoolean():
      import cloudComPy.MeshBoolean
      cc.MeshBoolean.computeMeshBoolean(...)
 )";

const char* MeshBoolean_computeMeshBoolean_doc=R"(
Compute Ambient Occlusion for mesh or point cloud. (plugin MeshBoolean)

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


#endif /* MeshBoolean_DOCSTRINGS_HPP_ */
