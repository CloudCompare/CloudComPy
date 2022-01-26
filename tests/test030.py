import os
import sys
import math
from gendata import getSampleCloud, dataDir, isCoordEqual
import cloudComPy as cc
import numpy as np
from builtins import RuntimeError
import multiprocessing

cc.initCC()  # to do once before using plugins or dealing with numpy

m3c2_params_dic={}
m3c2_params_dic["ExportDensityAtProjScale"] = "false"
m3c2_params_dic["ExportStdDevInfo"] = "false"
m3c2_params_dic["M3C2VER"] = 1
m3c2_params_dic["MaxThreadCount"] = multiprocessing.cpu_count()
m3c2_params_dic["MinPoints4Stat"] = 5
m3c2_params_dic["NormalMaxScale"] = 0.283607
m3c2_params_dic["NormalMinScale"] = 0.070902
m3c2_params_dic["NormalMode"] = 0
m3c2_params_dic["NormalPreferedOri"] = 4
m3c2_params_dic["NormalScale"] = 0.141803
m3c2_params_dic["NormalStep"] = 0.070902
m3c2_params_dic["NormalUseCorePoints"] = "false"
m3c2_params_dic["PM1Scale"] = 1
m3c2_params_dic["PM2Scale"] = 1
m3c2_params_dic["PositiveSearchOnly"] = "false"
m3c2_params_dic["ProjDestIndex"] = 1
m3c2_params_dic["RegistrationError"] = 0
m3c2_params_dic["RegistrationErrorEnabled"] = "false"
m3c2_params_dic["SearchDepth"] = 0.709017
m3c2_params_dic["SearchScale"] = 0.141803
m3c2_params_dic["SubsampleEnabled"] = "true"
m3c2_params_dic["SubsampleRadius"] = 0.070902
m3c2_params_dic["UseMedian"] = "false"
m3c2_params_dic["UseMinPoints4Stat"] = "false"
m3c2_params_dic["UseOriginalCloud"] = "false"
m3c2_params_dic["UsePrecisionMaps"] = "false"
m3c2_params_dic["UseSinglePass4Depth"] = "false"

paramFilename = os.path.join(dataDir, "m3c2_params.txt")
with open(paramFilename, 'w') as f:
    f.write("[General]\n")
    for k,v in m3c2_params_dic.items():
        f.write("%s=%s\n"%(k,v))

if cc.isPluginM3C2():
    cloud = cc.loadPointCloud(getSampleCloud(5.0))
    cloud1 = cc.loadPointCloud(getSampleCloud(1.0))
    cloud2 = cc.computeM3C2([cloud,cloud1], paramFilename)
    
    if cloud2 is None:
        raise RuntimeError
    if cloud2.getNumberOfScalarFields() < 3:
        raise RuntimeError
    dic= cloud2.getScalarFieldDic()
    sf = cloud2.getScalarField(dic['M3C2 distance'])
    if sf is None:
        raise RuntimeError
    if not math.isclose(sf.getMax(), 0.71, rel_tol=0.01):
        raise RuntimeError
    if not math.isclose(sf.getMin(), -0.70, rel_tol=0.01):
        raise RuntimeError
    
    cc.SaveEntities([cloud, cloud1, cloud2], os.path.join(dataDir, "M3C2.bin"))
