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
| Qt       | 5.9.5                     | 5.9.7                         | 5.9 ?   |
| Python   | 3.6.7                     | 3.7                           | 3.6     |
| PyQt5    | 5.10.1                    | 5.9.2                         | 5.9 ?   |
| sip      | 4.19.7                    | 4.19.8                        | 4.19 ?  |
| Numpy    | 1.13.3                    | 1.18.1                        | 1.13    |



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

The CloudCompare GUI is installed in the same place, and works as usual.	

### Windows 10

There are several methods to install the prerequisites on Windows 10. I chose to install Anaconda, which is a very complete and large Python-based tools environment.
There is a package system under Anaconda, to select the products you need. It has all our prerequisites.

It is necessary to configure Visual Studio with CMake.

I don't master well the configuration and use of Visual Studio, so I tested two ways to use the Visual Studio GUI, without knowing if there is a better way to take into account the prerequisites in the Visual Studio environment.

- 1: Launch the Visual Studio GUI as is, without any additions, and give all the necessary paths for the prerequisites.

- 2: Launch the Visual Studio GUI from the `Anaconda Prompt (Anaconda3)` console with the command: `"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"`. 

With the Anaconda environment, many paths and variables are found automatically.
In both cases, it is necessary to provide some configuration variables to CMake. With Visual Studio, these variables can be filled in a json file, which is read during the configuration step. I suppose it is also possible to install cmake-gui to do the same.

Here are my json file, for the first method (a minimal configuration of CloudCompare, without optional plugins):

```
{
  "configurations": [
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "RelWithDebInfo",
      "inheritEnvironments": [
        "msvc_x64_x64"
      ],
      "buildRoot": "${env.USERPROFILE}\\CMakeBuilds\\${workspaceHash}\\build\\${name}",
      "installRoot": "${env.USERPROFILE}\\CMakeBuilds\\${workspaceHash}\\install\\${name}",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": "",
      "variables": [
        {
          "name": "Qt5_DIR",
          "value": "${env.USERPROFILE}\\anaconda3\\Library\\lib\\cmake\\Qt5"
        },
        {
          "name": "Qt5LinguistTools_DIR",
          "value": "${env.USERPROFILE}\\anaconda3\\Library\\lib\\cmake\\Qt5LinguistTools"
        },
        {
          "name": "QT5_ROOT_PATH",
          "value": "${env.USERPROFILE}\\anaconda3\\Library"
        },
        {
          "name": "PYTHON_PREFERED_VERSION:STRING",
          "value": "3.7"
        },
        {
          "name": "PYQT_PYTHONPATH",
          "value": "${env.USERPROFILE}\\anaconda3\\Lib\\site-packages"
        },
        {
          "name": "PYQT_PYUIC_EXECUTABLE",
          "value": "${env.USERPROFILE}\\anaconda3\\Library\\bin\\pyuic5.bat"
        },
        {
          "name": "SIP_INCLUDE_DIR",
          "value": "${env.USERPROFILE}\\anaconda3\\include"
        },
        {
          "name": "SIP_EXECUTABLE",
          "value": "${env.USERPROFILE}\\anaconda3\\Library\\bin\\sip.exe"
        },
        {
          "name": "PYQT_SIPS",
          "value": "${env.USERPROFILE}\\anaconda3\\sip\\PyQt5"
        },
        {
          "name": "NUMPY_INCLUDE_DIR",
          "value": "${env.USERPROFILE}\\anaconda3\\Lib\\site-packages\\numpy\\core\\include"
        },
        {
          "name": "PYTHONAPI_TEST_DIRECTORY",
          "value": "${env.USERPROFILE}\\CloudCompare\\data"
        },
        {
          "name": "PYTHONAPI_TRACES:BOOL",
          "value": "ON"
        },
        {
          "name": "BUILD_PY_TESTING:BOOL",
          "value": "ON"
        },
        {
          "name": "BUILD_TESTING:BOOL",
          "value": "ON"
        }
      ]
    }
  ]
}
```

With the first method, Anaconda environment is not set, and make test does not work.

After the installation step, it is in any case necessary to load the Anaconda environment (Anaconda Prompt console) for Python and Numpy to be correctly configured.

The tests are installed in `<install-dir>/doc/PythonAPI_test`, with shell scripts to set the `PYTHONPATH` and launch one test.

The CloudCompare GUI is installed in the same place, and works as usual.

There is still a lot of work to do to make a correct packaging, but it is already possible to test the Python interface.

In addition to feedback on the interface itself and the extensions to be made, I take advice on best practices for configuration and use of Visual Studio tools :-)
