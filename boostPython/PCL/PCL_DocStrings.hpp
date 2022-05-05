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

#ifndef PCL_DOCSTRINGS_HPP_
#define PCL_DOCSTRINGS_HPP_


const char* PCL_doc= R"(
PCL is a standard plugin of cloudComPy.

The availability of the plugin can be tested with the isPluginPCL function:

  isPCL_available = cc.isPluginPCL()

PCL is a submodule of cloudCompy:
::

  import cloudComPy as cc
  # ...
  if cc.isPluginPCL():
      import cloudComPy.PCL
      fgr = cc.PCL.FastGlobalRegistrationFilter()
      fgr.setParameters(...)
      fgr.compute()
)";

const char* PCL_initTrace_PCL_doc=R"(
Debug trace must be initialized for each Python module.

Done in module init, following the value of environment variable _CCTRACE_ ("ON" if debug traces wanted)
)";

const char* PCL_FastGlobalRegistrationFilter_doc= R"(
)";

const char* PCL_setParameters_doc= R"(
        )";

const char* PCL_compute_doc= R"(
        )";

#endif /* PCL_DOCSTRINGS_HPP_ */
