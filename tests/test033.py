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
import sys
import math

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import getSampleCloud, dataDir
import cloudComPy as cc

#---HPR01-begin
if cc.isPluginHPR():
    import cloudComPy.HPR
    
    cloud = cc.loadPointCloud(getSampleCloud(5.0))
    
    cloudCut = cc.HPR.computeHPR(cloud, (0.,-15., 25.))
    #---HPR01-end
    
    nbPts = cloudCut.size()
    if not math.isclose(901000, nbPts, rel_tol = 1.e-2):
        raise RuntimeError
    
    cc.SaveEntities([cloudCut, cloudCut], os.path.join(dataDir, "HPR.bin"))
