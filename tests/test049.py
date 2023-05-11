#!/usr/bin/env python3

##########################################################################
#                                                                        #
#                              CloudComPy                                #
#                                                                        #
#  This program is free software; you can redistribute it and/or modify  #
#  it under the terms of the GNU General Public License as published by  #
#  the Free Software Foundation; either version 3 of the License, or     #
#  any later version.                                                    #
#                                                                        #
#  This program is distributed in the hope that it will be useful,       #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#  GNU General Public License for more details.                          #
#                                                                        #
#  You should have received a copy of the GNU General Public License     #
#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
#                                                                        #
#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
#                                                                        #
##########################################################################

import os

os.environ["_CCTRACE_"] = "ON"  # only if you want C++ debug traces

from gendata import getSampleCloud
import cloudComPy as cc

cloud = cc.loadPointCloud(getSampleCloud(5.0))

n_ccs = cc.LabelConnectedComponents(clouds=[cloud], octreeLevel=10)
if n_ccs != 15684:
    raise RuntimeError

cc_sf = cloud.getScalarField("CC labels")
cc_sf.computeMinAndMax()
if cc_sf.getMin() != 1:
    raise RuntimeError

if cc_sf.getMax() != 15684:
    raise RuntimeError
