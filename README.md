# CloudComPy
Python wrapper for CloudCompare

## What is CloudComPy?
This project is a draft of what could be a Python module to interface to CloudCompare, of equivalent level to the command mode of CloudCompare.

There are still few features available in this prototype, the idea is to collect feedback from interested users to guide future developments.

Here is an example of a Python script:

```
import sys

from PyQt5.QtWidgets import QApplication                       # create a Qt application, 
app = QApplication(sys.argv)                                   # CloudCompare relies on Qt
import cloudCompare as cc                                      # import the CloudCompare module
cc.initNumpy.init()                                            # to do once before dealing with numpy

cloud = cc.loadPointCloud("myCloud.xyz")                       # read a point cloud from a file
print("cloud name: %s"%cloud.getName())

res=cc.computeCurvature(cc.GAUSSIAN_CURV, 0.05, [cloud])       # compute curvature as a scalar field
nsf = cloud.getNumberOfScalarFields()
sfCurv=cloud.getScalarField(nsf-1)
cloud.setCurrentOutScalarField(nsf-1)
filteredCloud=cc.filterBySFValue(0.01, sfCurv.getMax(), cloud) # keep only the points above a given curvature

ok = filteredCloud.exportCoordToSF((False, False, True))       # Z coordinate as a scalar Field
nsf = cloud.getNumberOfScalarFields()
sf1=filteredCloud.getScalarField(nsf-1)
mean, var = sf1.computeMeanAndVariance()

# using Numpy...

coordinates = filteredCloud.toNpArray(True)                    # coordinates as a numpy array
x=coordinates[:,0]                                             # x column
y=coordinates[:,1]
z=coordinates[:,2]

f=(2*x-y)*(x+3*y)                                              # elementwise operation on arrays

asf1=sf1.toNpArray()                                           # scalar field as a numpy array
sf1.fromNpArray(f)                                             # replace scalar field values by a numpy array

res=cc.SavePointCloud(filteredCloud,"myModifiedCloud.bin")     #save the point cloud to a file
```

As you can see, it is possible to read and write point clouds, access scalar fields from Numpy (with or without copy of data), call some CloudCompare functions to transform point clouds.

The list of available functions should quickly grow.

From the Python interpreter, Docstrings provide some documentation on the available methods, the arguments.

## how to build CloudComPy?

Prerequisites for CloudComPy are Python3, PyQt, sip and Numpy plus, of course, everything needed to build CloudCompare.

With CloudComPy you build CloudCompare and the associated Python module.

Compilation is done with CMake, minimum version 3.10, recommended version 3.13 or newer (a lot of false warnings with 3.10).

### prerequisites versions
The minimum required version of each prerequisite is not always precisely identified. Examples of constructions that work are given here.

First example: Linux, Ubuntu 18.4, all native packages plus CMake 3.13 rebuilt (native version is 3.10).

Second example: Windows 10, Visual Studio 2017, Anaconda3 to get all the prerequisites plus a lot more...


| Platform | Linux Ubuntu 18.4 gcc 7.5 | Windows 10 Visual Studio 2017 | minimum |
| -------- | ------------------------- | ------------------------------| ------- |
| Qt       | 5.9.5                     | 5.x                           | 5.x     |
| Python   | 3.6.7                     | 3.7                           | 3.6     |
| PyQt5    | 5.10.1                    |                               |         |
| sip      | 4.19.7                    |                               |         |
| Numpy    | 1.13.3                    |                               |         |



### Ubuntu 18.4

On Ubuntu 18.4, you can install the development versions of the prerequisites with:

```
sudo apt-get intall qtbase5-dev python3 libpython3-dev python3-pyqt5 python3-sip-dev python3-numpy
```
Download and install CMake 3.13.5 (plus ccmake, cmake-gui, ctest...). CMake 3.13.5 is is easy to build from sources using the native CMake 3.10 and cmake-gui.

Here are my options used with CMake 3.13.5: depending on the plugins and options you choose, you may need additional prerequisites (use the corresponding natives packages, they are OK here).

To run tests on memory usage, you need the python3 package psutil (native package 5.4.2 is OK).

Commandline options (adapt the paths):

```
-DPLUGIN_STANDARD_QPOISSON_RECON:BOOL="1" -DPLUGIN_IO_QPHOTOSCAN:BOOL="1" -DPLUGIN_IO_QE57:BOOL="1" -DPLUGIN_STANDARD_QCOMPASS:BOOL="1" -DBUILD_TESTING:BOOL="1" -DPYTHONAPI_TEST_DIRECTORY:STRING="/home/paul/projets/CloudCompare/data" -DPLUGIN_GL_QEDL:BOOL="1" -DCMAKE_BUILD_TYPE:STRING="RelWithDebInfo" -DCMAKE_INSTALL_PREFIX:PATH="/home/paul/projets/CloudCompare/CloudComPy/installRelease" -DCCCORELIB_USE_CGAL:BOOL="1" -DOPTION_USE_GDAL:BOOL="1" -DPLUGIN_STANDARD_QBROOM:BOOL="1" -DPLUGIN_EXAMPLE_GL:BOOL="1" -DOPTION_USE_SHAPE_LIB:BOOL="1" -DPLUGIN_EXAMPLE_IO:BOOL="1" -DCMAKE_INSTALL_OLDINCLUDEDIR:PATH="" -DPLUGIN_IO_QCSV_MATRIX:BOOL="1" -DPYTHONAPI_TRACES:BOOL="1" -DPLUGIN_EXAMPLE_STANDARD:BOOL="1" -DPLUGIN_STANDARD_QPCL:BOOL="1" -DPLUGIN_STANDARD_QRANSAC_SD:BOOL="1" -DPLUGIN_IO_QADDITIONAL:BOOL="1" -DPLUGIN_GL_QSSAO:BOOL="1" -DPLUGIN_STANDARD_QM3C2:BOOL="1" 
```

Options to set with cmake-gui or ccmake (adapt the paths):

```
BUILD_TESTING:BOOL=1
CCCORELIB_USE_CGAL:BOOL=1
CMAKE_BUILD_TYPE:STRING=RelWithDebInfo
CMAKE_INSTALL_OLDINCLUDEDIR:PATH=
CMAKE_INSTALL_PREFIX:PATH=/home/paul/projets/CloudCompare/CloudComPy/installRelease
OPTION_USE_GDAL:BOOL=1
OPTION_USE_SHAPE_LIB:BOOL=1
PLUGIN_EXAMPLE_GL:BOOL=1
PLUGIN_EXAMPLE_IO:BOOL=1
PLUGIN_EXAMPLE_STANDARD:BOOL=1
PLUGIN_GL_QEDL:BOOL=1
PLUGIN_GL_QSSAO:BOOL=1
PLUGIN_IO_QADDITIONAL:BOOL=1
PLUGIN_IO_QCSV_MATRIX:BOOL=1
PLUGIN_IO_QE57:BOOL=1
PLUGIN_IO_QPHOTOSCAN:BOOL=1
PLUGIN_STANDARD_QBROOM:BOOL=1
PLUGIN_STANDARD_QCOMPASS:BOOL=1
PLUGIN_STANDARD_QM3C2:BOOL=1
PLUGIN_STANDARD_QPCL:BOOL=1
PLUGIN_STANDARD_QPOISSON_RECON:BOOL=1
PLUGIN_STANDARD_QRANSAC_SD:BOOL=1
PYTHONAPI_TEST_DIRECTORY:STRING=/home/paul/projets/CloudCompare/data
PYTHONAPI_TRACES:BOOL=1
```
After the CMake configuration and generation, run make (adapt the parallel option -j to your processor):

```
make -j12 && make test && make install
```

`make test` creates Point Cloud datasets and executes Python tests scripts using the cloudCompare module.
The tests are installed in `<install-dir>/doc/PythonAPI_test`, with shell scripts to set the `PYTHONPATH` and launch one test.

### Windows 10




