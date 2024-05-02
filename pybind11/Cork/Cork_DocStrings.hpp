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

#ifndef Cork_DOCSTRINGS_HPP_
#define Cork_DOCSTRINGS_HPP_


const char* Cork_doc= R"(
Cork is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginCork function:

  isCork_available = cc.isPluginCork()

Cork is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginCork():
      import cloudComPy.Cork
      mesh = cc.Cork.Cork.compute(...)
)";

const char* Cork_Cork_doc = R"(
We only use the static method compute of this class.
)";

const char* Cork_compute_doc = R"(
Compute Boolean operation between mesh A and mesh B.

:param ccMesh meshA: first mesh
:param ccMesh meshB: second mesh
:param CSG_OPERATION operation: from UNION, INTERSECT, DIFF, SYM_DIFF

:return: result of the Boolean operation
:rtype: ccMesh
)";

const char* Cork_initTrace_Cork_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* Cork_DOCSTRINGS_HPP_ */
