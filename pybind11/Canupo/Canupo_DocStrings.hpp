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

#ifndef Canupo_DOCSTRINGS_HPP_
#define Canupo_DOCSTRINGS_HPP_


const char* Canupo_doc= R"(
Canupo is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginCanupo function:

  isCanupo_available = cc.isPluginCanupo()

Canupo is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginCanupo():
      import cloudComPy.Canupo
      cc.Canupo.Classify(...)
 )";

const char* Canupo_Classify_doc=R"(
Classify a point cloud using an existing Canupo classifier.

See `Canupo plugin <https://www.cloudcompare.org/doc/wiki/index.php/CANUPO_(plugin)>`_.

:param ccPointCloud cloud: the point cloud to classify.
:param string classifierFilename: the path of the Canupo classifier file.
:param ccPointCloud,optional corePointCloud: core PointCloud to use if coreSource=OTHER, default None
:param int,optional coreSource: type of Core source from [ORIGINAL, OTHER, SUBSAMPLED, MSC_FILE], default ORIGINAL
:param string,optional MscFilename: path of the Msc file, default ""
:param double,optional confidenceThreshold: threshold to use for classification, default 0.
:param bool,optional generateAdditionalSF: default False
:param bool,optional generateRoughnessSF: default False
:param int,optional maxThreadCount: number of threads used for parallel computation, default 0 meaning automatic
:param bool,optional useActiveSFForConfidence: use the active scalarField as confidence, default False
:param double,optional samplingDist: default 0., to use if coreSource=SUBSAMPLED, must be >0 in that case.

:return: whether the classification is successful or not
:rtype: bool
)";

const char* Canupo_initTrace_Canupo_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";


#endif /* Canupo_DOCSTRINGS_HPP_ */
