## Installing, testing and using a CloudComPy binary on Windows 10, with Conda

The binary *CloudComPy\*_-date-.7z* available [here](https://www.simulation.openfields.fr/index.php/download-binaries) is built in a Conda environment.
(see [here](BuildWindowsConda.md) for the corresponding building instructions).

As CloudComPy is under development, these instructions and the link are subject to change from time to time...

**This binary works only on Windows 10, and with a Conda environment as described below, not anywhere else!**

You need a recent installation of Anaconda3 or miniconda3.

You need to create a conda environment for CloudComPy: for instance, in Anaconda3, use the Anaconda3 prompt console:

```
conda activate
conda create --name CloudComPy39 python=3.9
   # --- erase previous env if existing
conda activate CloudComPy39
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install boost cgal cmake ffmpeg gdal jupyterlab matplotlib mysql numpy opencv openmp pcl pdal psutil qt scipy sphinx_rtd_theme spyder tbb tbb-devel xerces-c
```

Install the binary in the directory of your choice.

### Using CloudCompare and CloudComPy:

From the conda prompt, you have to set the Path and Pythonpath once :

```
conda activate CloudComPy39
cd <path install>\CloudComPy39
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

```
jupyter notebook
```

An example of notebook is provided in ```doc/samples/histogramOnDistanceComputation.ipynb```.

### Execute all the Python tests:

```
conda activate CloudComPy39
cd  <path install>\CloudComPy39\doc\PythonAPI_test
```
NB: ```envCloudComPy.bat``` is not mandatory here, ```ctest``` resets the necessary paths

To execute all the tests (about two minutes):

```
ctest
```

The files created with the tests are in your user space: %USERPROFILE%\CloudComPy\data

### In case of problem:

There may be differences in the versions of conda packages. When updating the conda configuration, the package versions may change slightly.This is usually not a problem, but since the CloudComPy binary is fixed, there may be a version difference on a package, which makes it incompatible with CloudComPy. For your information, here is the list of package versions when CloudComPy was built.

The result of ```conda list``` command is provided in the sources in [building directory](../building)
