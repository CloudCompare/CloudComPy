# CloudComPy: Introduction
Python wrapper for CloudCompare

## Project site 
This [site](https://www.simulation.openfields.fr/index.php/projects/cloudcompy) in development already contains reference material,
Linux and Windows 10 binaries for a Conda environment (see below).

## What is CloudComPy?
This project proposes a Python module to interface to CloudCompare, 
of equivalent level to the command mode of CloudCompare.

CloudComPy does not yet cover all the features of CloudCompare. 
Features are added according to the needs reported by users,
feel free to post issues, preferably on [GitHub](https://github.com/CloudCompare/CloudComPy/issues),
but it's also possible on [GitLab](https://gitlab.com/openfields1/CloudComPy/-/issues)
(Following a temporary problem of visibility of the CloudComPy developer's contributions on GitHub,
the CloudComPy repository has been cloned on GitLab to achieve redundancy).

Here is an example of a Python script:

```
import cloudComPy as cc                                                # import the CloudComPy module
cc.initCC()                                                            # to do once before dealing with plugins

cloud = cc.loadPointCloud("myCloud.xyz")                               # read a point cloud from a file
print("cloud name: %s"%cloud.getName())

res=cc.computeCurvature(cc.CurvatureType.GAUSSIAN_CURV, 0.05, [cloud]) # compute curvature as a scalar field
nsf = cloud.getNumberOfScalarFields()
sfCurv=cloud.getScalarField(nsf-1)
cloud.setCurrentOutScalarField(nsf-1)
filteredCloud=cc.filterBySFValue(0.01, sfCurv.getMax(), cloud)         # keep only the points above a given curvature

ok = filteredCloud.exportCoordToSF(False, False, True)                 # Z coordinate as a scalar Field
nsf = cloud.getNumberOfScalarFields()
sf1=filteredCloud.getScalarField(nsf-1)
mean, var = sf1.computeMeanAndVariance()

# using Numpy...

coordinates = filteredCloud.toNpArrayCopy()                            # coordinates as a numpy array
x=coordinates[:,0]                                                     # x column
y=coordinates[:,1]
z=coordinates[:,2]

f=(2*x-y)*(x+3*y)                                                      # elementwise operation on arrays

asf1=sf1.toNpArray()                                                   # scalar field as a numpy array
sf1.fromNpArrayCopy(f)                                                 # replace scalar field values by a numpy array

res=cc.SavePointCloud(filteredCloud,"myModifiedCloud.bin")             #save the point cloud to a file
```

As you can see, it is possible to read and write point clouds, 
access scalar fields from Numpy (with or without copy of data), call some CloudCompare functions to transform point clouds.

The [reference documentation](https://www.simulation.openfields.fr/documentation/CloudComPy/html/) gives the list of available methods.

From the Python interpreter, Docstrings provide some documentation on the  methods and their arguments.

# Release notes
Release notes starts with the October 18, 2021 CloudComPy release as reference.
Have a look [here](doc/ReleaseNotes.md).

# Experimental: PyPI wheels
One of the easiest ways to install a Python module is to use the `pip` command that comes with your Python environment.
For example, `pip install numpy`.

We are currently working on setting up this service for cloudComPy. Since cloudComPy is not a pure Python module but relies primarily on CloudCompare’s C++ code, we need to create wheels (binary packages that can be downloaded and installed via pip). Each combination of operating system (Windows, Linux, macOS) and Python version (3.10 through 3.14) requires a specific wheel, for a total of 15 wheels.

The Windows wheels run on Windows 11; the Linux wheels are compiled on Ubuntu 20.04 and should work on any Linux distribution with GLIBC >= 2.39 (Ubuntu 24.04); and the macOS version must be 13.0 or higher (Apple processor).

For the first step of our development process, you’ll need to download the wheels from the [CloudComPy website](https://www.simulation.openfields.fr/index.php/projects/cloudcompy): After downloading the wheel of your choice, install it with pip,
for example, `pip install cloudcompy-2.14.0-cp310-cp310-macosx_13_0_arm64.whl`.

Here, 2.14.0 corresponds to the CloudComPy version, cp310 refers to Python 3.10, and macosx_13_0_arm64 refers to macOS, arm64 architecture (Apple), version 13.0 or higher.

Next, we’ll upload the wheels to the PyPI servers once they’ve been sufficiently debugged. Feel free to test the available wheels and report any issues on [GitHub](https://github.com/CloudCompare/CloudComPy/issues) to help debug them!

To install and test a wheel, see [testing wheels](doc/testingWheels.md).

If you'd like to view the instructions for building wheels, go to the [Building Wheels](doc/buildingWheels.md) page.

# Production: non wheel binaries requiring a Conda or Python venv environment and a specific setup script

## CloudComPy: availables non wheel binaries

CloudComPy is available for Windows, Linux, and macOS. The package runs in a Python environment that meets its requirements, typically a Python venv or Conda environment. See the installation documentation for your platform below. 

The binary are available [here](https://www.simulation.openfields.fr/index.php/cloudcompy-downloads). See the instructions below.

 - To install and use a **Windows** binary, consult the [Windows installation documentation](doc/UseWindowsCondaBinary.md) in the doc directory.

 - To install and use a **Linux** binary, consult the [Linux installation documentation](doc/UseLinuxCondaBinary.md) in the doc directory.

 - To install and use a **macOS** binary, consult the [macOS installation documentation](doc/UseMacOSCondaBinary.md) in the doc directory.

 - to test and run a Linux Conda binary in a docker container, see the [docker configuration documentation](doc/UseDockerLinuxConda.md) in the doc directory.

## CloudComPy: building non wheel

Prerequisites for CloudComPy are Python3, PyBind11 and Numpy plus, of course, everything needed to build CloudCompare and its plugins.

With CloudComPy you build CloudCompare and the associated Python module.

Compilation is done with CMake, minimum version 3.13 or newer.

### **WARNING: the building instructions are not up to date!**

### prerequisites versions
The minimum required version of each prerequisite is not always precisely identified. Examples of constructions that work are given [here](doc/prerequisitesVersions.md).

### Windows 10 or 11, with Anaconda3 or miniconda3

To build CloudComPy on Windows 10 or 11 with conda packages, consult the [Windows build documentation](doc/BuildWindowsConda.md) in the doc directory.

### Linux64, with Anaconda3 or miniconda3

To build CloudComPy on Linux64 with conda packages, consult the [Linux Conda build documentation](doc/BuildLinuxConda.md) in the doc directory.

### Docker, Linux64 and Conda

To build CloudComPy using Docker, for Linux64 and conda3, see [building with Docker](doc/BuildLinuxCondaDocker.md) in the doc directory).

### Ubuntu 20.04, native packages

To build CloudComPy on Ubuntu 20.04, consult the [Ubuntu build documentation](doc/BuildUbuntuNative.md) in the doc directory.
