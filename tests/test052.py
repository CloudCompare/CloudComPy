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

os.environ["_CCTRACE_"]="ON" # only if you want C++ debug traces

from gendata import  dataDir, dataExtDir, isCoordEqual
import cloudComPy as cc

if not os.path.isfile(os.path.join(dataExtDir,"garonne_L93.xyz")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/garonne_L93.xyz"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"garonne_L93.xyz"), 'wb') as f:
        f.write(r.content)

if not os.path.isfile(os.path.join(dataExtDir,"ligne_eau.poly")):
    if not os.path.exists(dataExtDir):
        os.makedirs(dataExtDir)
    url = "https://www.simulation.openfields.fr/phocadownload/ligne_eau.poly"
    r = requests.get(url)
    with open(os.path.join(dataExtDir,"ligne_eau.poly"), 'wb') as f:
        f.write(r.content)

#---extractSections01-begin
cloud=cc.loadPointCloud(os.path.join(dataExtDir, "garonne_L93.xyz"))
shift=cloud.getGlobalShift()
poly=cc.loadPolyline(os.path.join(dataExtDir, "ligne_eau.poly"), cc.CC_SHIFT_MODE.XYZ, 0, shift[0], shift[1], shift[2] )
orthoPolys=poly.generateOrthoSections(1000, 600, 2)
#---extractSections01-end

if len(orthoPolys) != 49:
    raise RuntimeError

res = cloud.exportCoordToSF(False, False, True)
cloud.setCurrentScalarField(0)
cloud.setCurrentDisplayedScalarField(0)

#---extractSections02-begin
sections = cc.extractPointsAlongSections([cloud], [poly])
orthoSections = cc.extractPointsAlongSections([cloud], orthoPolys)
cloudSections = cc.extractPointsAlongSections([cloud], [poly], extractSectionsAsClouds=True, extractSectionsAsEnvelopes=False)
unfoldedClouds = cc.unfoldPointsAlongPolylines([cloud], [poly], 1000)
#---extractSections02-end

if sections[0].size() != 653:
    raise RuntimeError
if len(orthoSections) != 49:
    raise RuntimeError
if orthoSections[48].size() != 10:
    raise RuntimeError
if cloudSections[0].size() != 660:
    raise RuntimeError
if unfoldedClouds[0].size() != 8851:
    raise RuntimeError

entities = [cloud, poly]
entities += orthoPolys
entities += sections
entities += orthoSections
entities += cloudSections
entities += unfoldedClouds

cc.SaveEntities(entities, os.path.join(dataDir, "sections.bin"))

