## Experimental: Installing, testing and using a CloudComPy binary on MacOS, with conda

The binary *CloudComPy_Conda310_MacOS-date-.zip* available [here](https://www.simulation.openfields.fr/index.php/cloudcompy-downloads)
 is built with a Conda environment.

**This binary works only on macOS Apple arm architecture (not on Intel processors), on recent macOS versions, not anywhere else!**

**Built and tested on macOS VENTURA 13.4.1. Please post issues on CloudComPy GitHub in case of problem**

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
If your environment CloudComPy310 does not exist:
```
conda create --name CloudComPy310 python=3.10
   # --- erase previous env with the same name if existing
```
Add or update the packages:
```
conda activate CloudComPy310
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install "boost" "cgal" cmake draco ffmpeg "gdal" jupyterlab laszip "matplotlib" "mysql=8.0" "numpy" "opencv" "openssl=3.0.8" "pcl" "pdal" "psutil" pybind11 "qhull=2020.2" "qt=5.15.8" "scipy" sphinx_rtd_theme spyder tbb tbb-devel "xerces-c=3.2" xorg-libx11 || error_exit "conda environment ${CONDA_ENV} cannot be completed"
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
. <path install>/bin/condaCloud.sh
```

If you use another Python configuration (without conda), you need to set the PYTHONPATH:

```
export PYTHONPATH=${CLOUDCOMPY_ROOT}:${PYTHONPATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/CloudCompare/CloudCompare.app/Contents/Frameworks:${PYTHONPATH}
export PYTHONPATH=${CLOUDCOMPY_ROOT}/doc/PythonAPI_test:${PYTHONPATH}
```

where `CLOUDCOMPY_ROOT` is the path of CloudComPy310.

To execute a Python script using CloudComPy:

```
python myscript.py
```

### Execute all the Python tests:

```
. <path install>/bin/condaCloud.zsh activate CloudComPy310
cd  <path install>/doc/PythonAPI_test
```

To execute all the tests (takes about three minutes, creates about 1.3GB of data files):

```
ctest
```

The files created with the tests are in your user space: `${HOME}/CloudComPy/Data`

### Current status and problems:

 - The RANSAC-SD plugin does not work (infinite loop).
 - The PoissonRecon plugin aborts.
 - The plugins QRDB, QSTEP, QTREEISO are not available.
 - Jupyter and Spyder provided with conda are not yet working.
 - The signature and notarization of CloudComPy and CloudCompare should be OK but it will be confirmed by your feedback
   (execution of the code on your Mac is possible and does not require to modify protections).
 