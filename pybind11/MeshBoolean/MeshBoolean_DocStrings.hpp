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
Compute boolean operation on two meshes. (plugin MeshBoolean)

The boolean operation are UNION, INTERSECT, DIFF, SYM_DIFF.
Original meshes are not modified, a new mesh is built.

:param ccMesh MeshA: first mesh.
:param ccMesh MeshB: second mesh.
:param CSG_OPERATION operation: from cc.MeshBoolean.CSG_OPERATION (UNION, INTERSECT, DIFF, SYM_DIFF)

:return: new mesh or None
:rtype: ccMesh
)";

const char* MeshBoolean_initTrace_MeshBoolean_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* MeshBoolean_DOCSTRINGS_HPP_ */
