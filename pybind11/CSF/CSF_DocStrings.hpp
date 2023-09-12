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

#ifndef CSF_DOCSTRINGS_HPP_
#define CSF_DOCSTRINGS_HPP_


const char* CSF_doc= R"(
CSF is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginCSF function:

  isCSF_available = cc.isPluginCSF()

CSF is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginCSF():
      import cloudComPy.CSF
      cc.CSF.computeCSF(...)
 )";

const char* CSF_computeCSF_doc=R"(
Compute Cloth Simulation Filter (CSF).

See `CSF plugin <https://www.cloudcompare.org/doc/wiki/index.php/CSF_(plugin)>`_.

:param ccPointCloud pc: the point cloud on which the filter is applied.
:param int,optional csfRigidness: from (1:steep slope, 2:relief 3:flat), default 2
:param int,optional maxIteration: maximum iterations, default 500
:param double,optional clothResolution: default 2.0
:param double,optional classThreshold: default 0.5
:param bool,optional csfPostprocessing: default false
:param bool,optional computeMesh: default false, compute the cloth mesh

:return: a list of clouds and mesh (ground, offground, optional cloth mesh)
:rtype: list
)";

const char* CSF_initTrace_CSF_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* CSF_DOCSTRINGS_HPP_ */
