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

#endif /* RANSAC_SD_DOCSTRINGS_HPP_ */
