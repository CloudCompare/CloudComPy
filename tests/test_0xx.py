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
