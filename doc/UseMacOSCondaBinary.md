## Installing, testing and using a CloudComPy binary on MacOS, with conda

The binary *CloudComPy_Conda310_MacOS-date-.zip* available [here](https://www.simulation.openfields.fr/index.php/cloudcompy-downloads)
 is built with a Conda environment.

**This binary works only on macOS Apple arm64 architecture (not on Intel processors), on recent macOS versions, not anywhere else!**

**Built and tested on macOS Sonoma 14.7.
Please post issues on CloudComPy [GitHub](https://github.com/CloudCompare/CloudComPy/issues)
or [GitLab](https://gitlab.com/openfields1/CloudComPy/-/issues) in case of problem**

The macOS binary provides **CloudCompare** and **CloudCompy** (same as binaries for Windows and Linux).

As CloudComPy is under development, these instructions and the link are subject to change from time to time...

**CloudCompare** works as it is (no specific environment).
It is located in CloudComPy310/CloudCompare/CloudCompare.app and can be launched from the Finder.

**CloudComPy** needs a Python 3.10 configuration with at least the following packages, either with conda or not:

```
numpy
scipy
requests
psutils
matplotlib
```

You can create an environment for CloudComPy with conda, from the terminal
(here, I chose to activate conda environment on demand: please adapt the instructions to your installation)
The following package list corresponds to the building environment, but you can adjust the list
(at least the above list):

```
conda activate
conda update -y -n base -c defaults conda
```
If your environment CloudComPy310 does not exist or to recreate it from scratch:
(**note:** for 2.13.2, it's better to recreate the environment, because there are a lot of changes)
```
conda create --name CloudComPy310 python=3.10
   # --- erase previous env with the same name if existing
```
Add or update the packages:
```
conda activate CloudComPy310
conda config --add channels conda-forge
conda config --set channel_priority flexible
conda install -y boost cgal cmake draco "ffmpeg=6.1" gdal jupyterlab laszip matplotlib "mysql=8" notebook numpy opencv "openssl=3.1" pcl pdal psutil pybind11 quaternion "qhull=2020.2" "qt=5.15.8" scipy sphinx_rtd_theme spyder tbb tbb-devel "xerces-c=3.2" xorg-libx11
```

Unzip the binary tarfile in the directory of your choice.

### Using CloudCompare and CloudComPy:

CloudCompare is located in `CloudComPy310/CloudCompare/CloudCompare.app` and can be launched from the Finder.

CloudcomPy requires to set the Python environment and the PYTHONPATH.

If you have configured the conda CloudComPy310 environment, use the script `bin/condaCloud.zsh`.
It overrides the conda command for activation and deactivation, it must be sourced. 

From a new prompt (replace `<path install>` by its value): 

```
. <path install>/bin/condaCloud.zsh activate CloudComPy310
```

if conda is unknown, execute the following instruction before:

```
. <conda_dir>/etc/profile.d/conda.sh
```
where `<conda_dir>` is the installation directory of conda (often `~/anaconda3` or `~/miniconda3`)

Have a look on the script usage:
```
. <path install>/bin/condaCloud.zsh
```

If you use another Python configuration (without conda), you need to set the PYTHONPATH:

```
export PYTHONPATH=${CLOUDCOMPY_ROOT}:${PYTHONPATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/CloudCompare/CloudCompare.app/Contents/Frameworks:${PYTHONPATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/doc/PythonAPI_test:${PYTHONPATH}
```

where `CLOUDCOMPY_ROOT` is the path of CloudComPy310.

To execute a Python script (for instance myscript.py) using CloudComPy:

```
python myscript.py
```
The IDE [Spyder](https://www.spyder-ide.org/) and [Jupyter](https://jupyter.org/) can be launched in this environment:

```
spyder
```
The first time you run Spyder, you may need to add the paths to CloudCompare and the test scripts to the PYTHONPATH,
using the menu Tools / PYTHONPATH Manager. These paths are the same as those defined in the condaCloud.zsh script.

```
jupyter notebook
```

An example of notebook is provided in ```doc/samples/histogramOnDistanceComputation.ipynb```.

### Execute all the Python tests:

```
. <path install>/bin/condaCloud.zsh activate CloudComPy310
cd  <path install>/doc/PythonAPI_test
```

To execute all the tests (takes about five minutes, creates about 2GB of data files):

```
ctest
```

The files created with the tests are in your user space: `${HOME}/CloudComPy/Data`

### Current status and problems:

 - The plugins QRDB, QSTEP, QTREEISO are not available.
 - test042.py fails on macOS

### In case of problem:

There may be differences in the versions of conda packages. When updating the conda configuration, the package versions may change slightly.
This is usually not a problem, but since the CloudComPy binary is fixed, there may be a version difference on a package, 
which makes it incompatible with CloudComPy. For your information, here is the list of package versions when CloudComPy was built.

The result of ```conda list``` command is provided in the sources in [building directory](../building)

 