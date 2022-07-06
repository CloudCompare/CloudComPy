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

#---PCV01-begin
if cc.isPluginPCV():
    import cloudComPy.PCV
#---PCV01-end
    
    cloud = cc.loadPointCloud(getSampleCloud(5.0))
    
#---PCV02-begin
    tr1 = cc.ccGLMatrix()
    tr1.initFromParameters(0.3*math.pi, (0., 1., 0.), (0.0, 0.0, 0.0))
    dish = cc.ccDish(2.0, 0.5, 0.0, tr1)
    cln =dish.getAssociatedCloud()
    cc.computeNormals([cln])
#---PCV02-end

#---PCV03-begin
    cc.PCV.computeShadeVIS([cloud],cln)
    dic = cloud.getScalarFieldDic()
    cloud.renameScalarField(dic["Illuminance (PCV)"], "IlluminanceDish")
#---PCV03-end
    if cloud.getNumberOfScalarFields() != 1:
        raise RuntimeError
   
#---PCV04-begin
    cc.PCV.computeShadeVIS([cloud])
#---PCV04-end
    if cloud.getNumberOfScalarFields() != 2:
        raise RuntimeError

    cc.SaveEntities([cloud, dish, cln], os.path.join(dataDir, "PCV.bin"))
