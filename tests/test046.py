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
import requests
import urllib.request

os.environ["_CCTRACE_"] = "ON"  # only if you want C++ debug traces

from gendata import dataDir, dataExtDir, isCoordEqual

import cloudComPy as cc

#---Canupo001-begin
# example data available here: https://nicolas.brodu.net/common/recherche/canupo/benchmark.tar.gz
# classifier are available here : http://www.cloudcompare.org/forum/viewtopic.php?f=17&t=808&start=90#p11588

if not os.path.isfile(os.path.join(dataExtDir,"recombi_10.txt")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/recombi_10.txt"
    with open(os.path.join(dataExtDir,"recombi_10.txt"), 'wb') as f:
        for line in urllib.request.urlopen(url):
            f.write(line)
        
if not os.path.isfile(os.path.join(dataExtDir,"vegetTidal.prm")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/vegetTidal.prm"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"vegetTidal.prm"), 'wb') as f:
        f.write(r.content)
        
cloud=cc.loadPointCloud(os.path.join(dataExtDir,"recombi_10.txt"))
#---Canupo001-end

#---Canupo002-begin
if cc.isPluginCanupo():
    import cloudComPy.Canupo
    res = cc.Canupo.Classify(cloud, os.path.join(dataExtDir,"vegetTidal.prm"))
    
    dic = cloud.getScalarFieldDic()
    sf1 = cloud.getScalarField(dic['CANUPO.class'])
    if sf1 is None:
        raise RuntimeError
    sf2 = cloud.getScalarField(dic['CANUPO.confidence'])
    if sf2 is None:
        raise RuntimeError
    
    res = cc.SaveEntities([cloud], os.path.join(dataDir, "cloudCanupo.bin"))
#---Canupo002-end

