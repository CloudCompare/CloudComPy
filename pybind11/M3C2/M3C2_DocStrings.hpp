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

#ifndef M3C2_DOCSTRINGS_HPP_
#define M3C2_DOCSTRINGS_HPP_


const char* M3C2_doc= R"(
M3C2 is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginM3C2 function:

  isM3C2_available = cc.isPluginM3C2()

M3C2 is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginM3C2():
      import cloudComPy.M3C2
      cc.M3C2.computeM3C2(...)
 )";

const char* M3C2_computeM3C2_doc=R"(
Compute Multiscale Model to Model Cloud Comparison (plugin M3C2)
The computation parameters are regrouped in a text file:
see :py:func:`M3C2guessParamsToFile` or the GUI to get a first version.

To compute the uncertainty with precision maps, 6 scalar fields are required 
(3 components for first cloud then 3 components for second cloud). Two scales are also required
(to take into account different units). When these fields and scales are given, 
the computation uses the precision maps and supersedes the options in the parameter file.

:param list clouds: two or three clouds to compare. If a 3rd cloud is present, it will be used as core points.
:param string paramFilename: full path of the parameter file
:param list,optional precisionMaps: list of 6 scalar fields (3 components for first cloud then 3 components for second cloud), default empty list
:param list,optional scales: list of two doubles (scale for first and second cloud), default empty list

:return: output cloud with computed scalar fields
:rtype: ccPointCloud
)";


const char* M3C2_initTrace_M3C2_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";

const char* M3C2_M3C2guessParamsToFile_doc=R"(
Guess the parameters for compute_M3C2 and save them to a file.

:param list clouds: two or three clouds to compare. If a 3rd cloud is present, it will be used as core points.
:param string paramFilename: full path of the parameter file
:param bool fastMode: True corresponds to the values of the GUI dialog before Guess params button, 
                      False corresponds to the values of the GUI dialog after Guess params button.

:return: success
:rtype: bool
)";

#endif /* M3C2_DOCSTRINGS_HPP_ */
