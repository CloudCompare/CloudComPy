## Installing, testing and using a CloudComPy binary on Linux, with conda

The binary *CloudComPy_Conda39_Linux64_-date-.tgz* available [here](https://www.simulation.openfields.fr/index.php/download-binaries) is built with a Conda environment
(see [here](BuildWindowsConda.md) for the corresponding building instructions).

As CloudComPy is under development, these instructions and the link are subject to change from time to time...

**This binary works only on Linux 64, on recent distributions, and with a Conda environment as described below, not anywhere else!**

**Only tested un Ubuntu 20.04 (focal) and Debian 11 (bullseye), please report any problems on other distributions.**

GLIBC version should be 2.29 or more. To know your version of GLIBC:

```
ldd --version
```

You need a recent installation of Anaconda3 or miniconda3.

You need to create an environment for CloudComPy with conda, from the terminal
(here, I chose to activate conda environment on demand: please adapt the instructions to your installation):

```
conda activate
conda create --name CloudComPy39 python=3.9
# --- erase previous env if existing
conda activate CloudComPy39
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install boost cgal cmake eigen ffmpeg gdal jupyterlab matplotlib mysql numpy opencv openmp pcl pdal psutil "qhull=2019.1" qt scipy sphinx_rtd_theme spyder tbb tbb-devel xerces-c
```

**Remark:** Within a script bash, add the following instruction before conda commands:

```
. ~/anaconda3/etc/profile.d/conda.sh
```

Install the binary in the directory of your choice.

### Using CloudCompare and CloudComPy:

Before using CloudCompare or CloudComPy, you need to load the environment. 
The script `bin/condaCloud.sh` overrides the conda command for activation and deactivation, it must be sourced. 

From a new prompt (replace `<path install>` by its value): 

```
. <path install>/bin/condaCloud.sh activate CloudComPy39
```

Have a look on the script usage:
```
. <path install>/bin/condaCloud.sh
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
. <path install>/bin/condaCloud.sh activate CloudComPy39
cd  <path install>\CloudComPy39\doc\PythonAPI_test
```

To execute all the tests (about two minutes):

```
ctest
```

The files created with the tests are in your user space: %USERPROFILE%\CloudComPy\data

From the prompt, you can :

### In case of problem:

There may be differences in the versions of conda packages. When updating the conda configuration, the package versions may change slightly.This is usually not a problem, but since the CloudComPy binary is fixed, there may be a version difference on a package, which makes it incompatible with CloudComPy. For your information, here is the list of package versions when CloudComPy was built.

The result of ```conda list``` command is provided in the sources in [building directory](../building)
