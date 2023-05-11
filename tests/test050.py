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

import os, sys

os.environ["_CCTRACE_"] = "ON"  # only if you want C++ debug traces

from gendata import dataDir, dataExtDir
import cloudComPy as cc

if not os.path.isfile(os.path.join(dataExtDir, "testIssue100.e57")):
    print("Test skipped")
    sys.exit()
    
cloud = cc.loadPointCloud(os.path.join(dataExtDir, "testIssue100.e57"))
cc.SavePointCloud(cloud, os.path.join(dataExtDir, "testIssue100.pcd"))
