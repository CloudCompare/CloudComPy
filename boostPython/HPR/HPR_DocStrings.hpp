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

#ifndef HPR_DOCSTRINGS_HPP_
#define HPR_DOCSTRINGS_HPP_


const char* HPR_doc= R"(
HPR is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginHPR function:

  isHPR_available = cc.isPluginHPR()

HPR is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginHPR():
      import cloudComPy.HPR
      cc.HPR.computeHPR(...)
 )";

const char* HPR_computeHPR_doc=R"(
Compute Hidden Point Removal for point cloud. (plugin HPR)

The HPR algorithm creates a new cloud in which the hidden points are removed.
The viewPoint coordinates are required.
(With the CloudCompare GUI, the viewPoint is deduced from the 3D view).

:param ccPointCloud cloud: input cloud, with hidden points.
:param ccVector3D viewPoint: view point coordinates
:param int,optional octreeLevel: octree level, default 7.

:return: point cloud without hidden points or None (if problem or no change)
:rtype: ccPointCloud
)";

const char* HPR_initTrace_HPR_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* HPR_DOCSTRINGS_HPP_ */
