## Installing, testing and using a CloudComPy binary on Windows 10, with Conda

The binary *CloudComPy\*_-date-.7z* available [here](https://www.simulation.openfields.fr/index.php/cloudcompy-downloads) is built in a Conda environment.
(see [here](BuildWindowsConda.md) for the corresponding building instructions).

As CloudComPy is under development, these instructions and the link are subject to change from time to time...

**This binary works only on Windows 10, and with a Conda environment as described below, not anywhere else!**

You need a recent installation of Anaconda3 or miniconda3.

You need to create a conda environment for CloudComPy: for instance, in Anaconda3, use the Anaconda3 prompt console:

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
conda install "boost=1.74" "cgal=5.4" cmake draco ffmpeg "gdal=3.5" jupyterlab laszip "matplotlib=3.5" "mysql=8.0" "numpy=1.22" "opencv=4.5" "openmp=8.0" "pcl=1.12" "pdal=2.4" "psutil=5.9" pybind11 "qhull=2020.2" "qt=5.15.4" "scipy=1.8" sphinx_rtd_theme spyder tbb tbb-devel "xerces-c=3.2"
```

Install the binary in the directory of your choice.

### Using CloudCompare and CloudComPy:

From the conda prompt, you have to set the Path and Pythonpath once :

```
conda activate CloudComPy310
cd <path install>\CloudComPy310
envCloudComPy.bat
```

To run CloudCompare:

```
CloudCompare
```

To execute a Python script using CloudComPy:

```
Python myscript.py
```

The IDE [Spyder](https://www.spyder-ide.org/) and [Jupyter](https://jupyter.org/) can be launched in this environment:

```
spyder
```
The first time you run Spyder, you may need to add the paths to CloudCompare and the test scripts to the PYTHONPATH,
using the menu Tools / PYTHONPATH Manager. These paths are the same as those defined in the envCloudComPy.bat script.

```
jupyter notebook
```

An example of notebook is provided in ```doc/samples/histogramOnDistanceComputation.ipynb```.

### Execute all the Python tests:

```
conda activate CloudComPy310
cd  <path install>\CloudComPy310\doc\PythonAPI_test
```
NB: ```envCloudComPy.bat``` is OK but not mandatory here, ```ctest``` resets the necessary paths

To execute all the tests (takes about three minutes, creates about 1.3GB of data files):

```
ctest
```

The files created with the tests are in your user space: %USERPROFILE%\CloudComPy\data

### In case of problem:

There may be differences in the versions of conda packages. When updating the conda configuration, the package versions may change slightly.This is usually not a problem, but since the CloudComPy binary is fixed, there may be a version difference on a package, which makes it incompatible with CloudComPy. For your information, here is the list of package versions when CloudComPy was built.

The result of ```conda list``` command is provided in the sources in [building directory](../building)
