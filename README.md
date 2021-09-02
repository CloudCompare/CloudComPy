# CloudComPy: Introduction
Python wrapper for CloudCompare

## Project site (under construction)
This [site](https://www.simulation.openfields.fr/index.php/projets/cloudcompy) already contains reference material,
and Windows 10 binaries for an Anaconda3 environment (see below).

## What is CloudComPy?
This project is a draft of what could be a Python module to interface to CloudCompare, 
of equivalent level to the command mode of CloudCompare.

There are still missing features in this prototype, 
the idea is to collect feedback from interested users to guide future developments.

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

The list of available functions should quickly grow.

From the Python interpreter, Docstrings provide some documentation on the available methods, the arguments.

# CloudComPy: availables binaries

## testing a CloudComPy binary on Windows 10, with Anaconda 3

The binary *CloudComPy37_-date-.7z* available in this [directory](https://www.simulation.openfields.fr/binaries/) is built in an Anaconda3 environment
(see below for the corresponding building instructions).
As CloudComPy is under development and not yet fully stabilized, these instructions and the link are subject to change from time to time...

**This binary works only on Windows 10, and with Anaconda3 as described below, not anywhere else!**

You need a recent installation of Anaconda3.

You need to create an environment for CloudComPy in Anaconda3, with the Anaconda3 prompt console:

```
conda activate
conda create --name CloudComPy37 python=3.7
   # --- erase previous env if existing
conda activate CloudComPy37
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install numpy psutil "boost<1.70" xerces-c pcl gdal cgal cmake "pdal<2.3.0" opencv ffmpeg mysql m2-perl-net-ssleay matplotlib
```

Install the binary in the directory of your choice.
From the Anaconda3 prompt, you can test :

### Testing CloudCompare:

```
conda activate CloudComPy37
cd <path install>\CloudComPy37\CloudCompare
CloudCompare.exe
```

### Python tests:

```
conda activate CloudComPy37
cd  <path install>\CloudComPy37\doc\PythonAPI_test
```

To execute all the tests (about one minute)

```
ctest
```

To complete the pythonpath and run a script

```
envPyCC.bat
python test001.py
```

The files created with the tests are in your user space: %USERPROFILE%\CloudComPy\dat

## testing a CloudComPy binary on Linux, with Anaconda 3

The binary *CloudComPy_Conda39_Linux64_-date-.tgz* available in this [directory](https://www.simulation.openfields.fr/binaries/) is built in an Anaconda3 environment
(see below for the corresponding building instructions).
As CloudComPy is under development and not yet fully stabilized, these instructions and the link are subject to change from time to time...

**This binary works only on Linux 64, and with Anaconda3 as described below, not anywhere else!**

**Only tested un Ubuntu 20.04, please report any problems on other distributions.**

You need a recent installation of Anaconda3.

You need to create an environment for CloudComPy in Anaconda3, from the terminal
(here, I chose to activate Anaconda environment on demand: please adapt the instructions to your installation):

```
. ~/anaconda3/etc/profile.d/conda.sh
conda activate
conda create --name CloudComPy39 python=3.9
# --- erase previous env if existing
conda activate CloudComPy39
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install qt numpy psutil boost xerces-c pcl gdal cgal cmake pdal opencv ffmpeg mysql "qhull=2019.1" matplotlib "eigen=3.3.9"
```

Install the binary in the directory of your choice.
Before using CloudCompare or CloudComPy, you need to load the environment. 
From a new prompt (replace <path install> by its value): 

```
. ~/anaconda3/etc/profile.d/conda.sh
conda activate CloudComPy39
export LD_LIBRARY_PATH=~/anaconda3/envs/CloudComPy39/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=<path install>/lib/cloudcompare:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=<path install>/lib/cloudcompare/plugins:${LD_LIBRARY_PATH}
```
/
From the prompt, you can test :

### Testing CloudCompare:

```
<path install>/bin/CloudCompare
```

### Python tests:

```
cd  <path install>/doc/PythonAPI_test
```

To execute all the tests (about one minute)

```
ctest
```

To complete the pythonpath and run a script

```
. envPyCC
python test001.py
```

The files created with the tests are in your user space: %USERPROFILE%\CloudComPy\data


#CloudComPy: building

Prerequisites for CloudComPy are Python3, BoostPython and Numpy plus, of course, everything needed to build CloudCompare.

With CloudComPy you build CloudCompare and the associated Python module.

Compilation is done with CMake, minimum version 3.10, recommended version 3.13 or newer (a lot of false warnings with 3.10).

## prerequisites versions
The minimum required version of each prerequisite is not always precisely identified. Examples of constructions that work are given here.

First example: Linux, Ubuntu 20.04, all native packages.

Second example: Windows 10, Visual Studio 2019, Anaconda3 to get all the prerequisites plus a lot more...


| Platform | Linux Ubuntu 20.04 (clang 10) | Windows 10 Visual Studio 2019 | minimum |
| -------- | ----------------------------- | ------------------------------| ------- |
| Qt       | 5.12.8                        | 5.9.7                         | 5.9 ?   |
| Python   | 3.8.5                         | 3.7.10                        | 3.6     |
| Boost    | 1.71                          | 1.68                          | 1.68 ?  |
| Numpy    | 1.17.4                        | 1.20.2                        | 1.13 ?  |



## Ubuntu 20.04, native packages

On Ubuntu 20.04, you can install the development versions of the prerequisites with:
**TODO: complete the list, many packages are required...**

```
sudo apt-get intall qtbase5-dev python3 libpython3-dev python3-numpy cmake
```
To run tests on memory usage, you need the python3 package python3-psutil.

Commandline options (adapt the paths):

```
-DPYTHON_PREFERED_VERSION:STRING="3.8" -DPLUGIN_STANDARD_QM3C2:BOOL="1" -DBUILD_PY_TESTING:BOOL="1" -DPYTHONAPI_TEST_DIRECTORY:STRING="projets/CloudComPy/Data" -DCMAKE_BUILD_TYPE:STRING="RelWithDebInfo" -DCCCORELIB_USE_CGAL:BOOL="1" -DPLUGIN_STANDARD_QRANSAC_SD:BOOL="1" -DPLUGIN_EXAMPLE_IO:BOOL="1" -DCMAKE_INSTALL_PREFIX:PATH="/home/paul/projets/CloudComPy/installRelease" -DBUILD_TESTING:BOOL="1" -DOPTION_USE_GDAL:BOOL="1" -DPLUGIN_GL_QEDL:BOOL="1" -DPLUGIN_IO_QCSV_MATRIX:BOOL="1" -DPLUGIN_GL_QSSAO:BOOL="1" -DPLUGIN_STANDARD_QPCL:BOOL="0" -DPLUGIN_IO_QADDITIONAL:BOOL="1" -DPLUGIN_STANDARD_QCOMPASS:BOOL="1" -DPLUGIN_STANDARD_QMPLANE:BOOL="1" -DPLUGIN_IO_QPHOTOSCAN:BOOL="1" -DPYTHONAPI_TRACES:BOOL="1" -DPLUGIN_IO_QE57:BOOL="1" -DPLUGIN_STANDARD_QBROOM:BOOL="1" -DPLUGIN_EXAMPLE_STANDARD:BOOL="1" -DPLUGIN_STANDARD_QPOISSON_RECON:BOOL="1" -DPLUGIN_EXAMPLE_GL:BOOL="1"```
```

I use clang series compilers, but it should work with g++,gcc as well. Options to set with cmake-gui or ccmake (adapt the paths):

```
BUILD_PY_TESTING:BOOL=1
BUILD_TESTING:BOOL=1
CCCORELIB_USE_CGAL:BOOL=1
CMAKE_BUILD_TYPE:STRING=RelWithDebInfo
CMAKE_INSTALL_PREFIX:PATH=/home/paul/projets/CloudComPy/installRelease
OPTION_USE_GDAL:BOOL=1
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
PLUGIN_STANDARD_QMPLANE:BOOL=1
PLUGIN_STANDARD_QPCL:BOOL=0
PLUGIN_STANDARD_QPOISSON_RECON:BOOL=1
PLUGIN_STANDARD_QRANSAC_SD:BOOL=1
PYTHONAPI_TEST_DIRECTORY:STRING=projets/CloudComPy/Data
PYTHONAPI_TRACES:BOOL=1
PYTHON_PREFERED_VERSION:STRING=3.8
```

After the CMake configuration and generation, run make (adapt the parallel option -j to your processor):

```
make -j12 && make test && make install
```

`make test` creates Point Cloud datasets and executes Python tests scripts using the cloudCompare module.
The tests are installed in `<install-dir>/doc/PythonAPI_test`, with shell scripts to set the `PYTHONPATH` and launch one test.
When in `<install-dir>/doc/PythonAPI_test`, `ctest` launches all the tests. 

The CloudCompare GUI is installed in  `<install-dir>/bin/CloudCompare`, and works as usual.	

## Windows 10, with Anaconda3

There are several methods to install the prerequisites on Windows 10. 
I chose to install Anaconda, which is a very complete and large Python-based tools environment. 
There is a package system under Anaconda, to select the products you need. It has all our prerequisites.

From Anaconda prompt:
```
conda activate
conda create --name CloudComPy37 python=3.7
# --- erase previous env if existing
conda activate CloudComPy37
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install numpy psutil "boost<1.70" xerces-c pcl gdal cgal cmake "pdal<2.3.0" opencv ffmpeg mysql m2-perl-net-ssleay matplotlib
```
For information, the list of packages actually installed for building and testing can be found in `building/conda-list`.

CMake from Anaconda is used to get ctest at install, not for build.

I do not use Qt from conda packages, I still have a problem at runtime when reading xyz files with the HEAD of CloudCompare
(after commit "New features (#1420)" from 2021-03-07).
With a separate install of Qt 5.15.2 binaries, it works fine...

To use FBX format plugin, install the FBX SDK, not provided by an Anaconda package.

It is necessary to configure Visual Studio 2019 with CMake.

I don't master well the configuration and use of Visual Studio, so I tested two ways to use the Visual Studio GUI, 
without knowing if there is a better way to take into account the prerequisites in the Visual Studio environment.

- 1: Launch the Visual Studio GUI as is, without any additions, and give all the necessary paths for the prerequisites.

- 2: Launch the Visual Studio GUI from the `Anaconda Prompt (Anaconda3)` console with the command: 
`"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe"`. 

With the Anaconda environment, many paths and variables are found automatically. 
In both cases, it is necessary to provide some configuration variables to CMake. 
With Visual Studio, these variables can be filled in a json file, which is read during the configuration step. 
I suppose it is also possible to install cmake-gui to do the same.

Here are my json file, for the first method, with the plugins availables with Anaconda packages:

```
{
  "configurations": [
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "RelWithDebInfo",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "buildRoot": "C:/Users/paulr/CloudComPy/build/${name}",
      "installRoot": "C:/Users/paulr/CloudComPy/install/CloudComPy37",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": "",
      "variables": [
        {
          "name": "Qt5_DIR",
          "value": "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5",
          "type": "STRING"
        },
        {
          "name": "Qt5LinguistTools_DIR",
          "value": "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5LinguistTools",
          "type": "STRING"
        },
        {
          "name": "Qt5Test_DIR",
          "value": "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5Test",
          "type": "STRING"
        },
        {
          "name": "BOOST_INCLUDEDIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "STRING"
        },
        {
          "name": "BOOST_LIBRARYDIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib",
          "type": "STRING"
        },
        {
          "name": "Boost_DEBUG:BOOL",
          "value": "OFF",
          "type": "STRING"
        },
        {
          "name": "Python3_ROOT_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37",
          "type": "STRING"
        },
        {
          "name": "PYTHON_PREFERED_VERSION:STRING",
          "value": "3.7",
          "type": "STRING"
        },
        {
          "name": "NUMPY_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Lib/site-packages/numpy/core/include",
          "type": "STRING"
        },
        {
          "name": "PYTHONAPI_TEST_DIRECTORY",
          "value": "CloudComPy/data",
          "type": "STRING"
        },
        {
          "name": "PYTHONAPI_TRACES:BOOL",
          "value": "ON",
          "type": "STRING"
        },
        {
          "name": "INSTALL_PREREQUISITE_LIBRARIES",
          "value": "OFF",
          "type": "STRING"
        },
        {
          "name": "BUILD_PY_TESTING:BOOL",
          "value": "ON",
          "type": "STRING"
        },
        {
          "name": "BUILD_TESTING:BOOL",
          "value": "ON",
          "type": "STRING"
        },
        {
          "name": "OPTION_SCALAR_DOUBLE:BOOL",
          "value": "OFF",
          "type": "STRING"
        },
        {
          "name": "CCCORELIB_USE_CGAL",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "CCCORELIB_USE_QT_CONCURRENT",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "OPTION_USE_GDAL",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "OPTION_USE_SHAPE_LIB",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_EXAMPLE_GL",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_EXAMPLE_IO",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_EXAMPLE_STANDARD",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_GL_QEDL",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_GL_QSSAO",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QADDITIONAL",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QCORE",
          "value": "true",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QCSV_MATRIX",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QE57",
          "value": "true",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QFBX",
          "value": "true",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QLAS_FWF",
          "value": "False",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QPDAL",
          "value": "true",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QPHOTOSCAN",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QSTEP",
          "value": "False",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_MASONRY_QAUTO_SEG",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_MASONRY_QMANUAL_SEG",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QANIMATION",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QBROOM",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QCANUPO",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QCOLORIMETRIC_SEGMENTER",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QCOMPASS",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QCSF",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QFACETS",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QHOUGH_NORMALS",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QHPR",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QJSONRPC",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QM3C2",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QMPLANE",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QPCL",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QPCV",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QPOISSON_RECON",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QRANSAC_SD",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_STANDARD_QSRA",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "POISSON_RECON_WITH_OPEN_MP",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "QANIMATION_WITH_FFMPEG_SUPPORT",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "CGAL_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/cmake/CGAL",
          "type": "PATH"
        },
        {
          "name": "EIGEN_ROOT_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include/eigen3",
          "type": "PATH"
        },
        {
          "name": "OPENCV_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/cmake",
          "type": "PATH"
        },
        {
          "name": "PCL_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/cmake",
          "type": "PATH"
        },
        {
          "name": "PDAL_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/cmake/PDAL",
          "type": "PATH"
        },
        {
          "name": "FFMPEG_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "PATH"
        },
        {
          "name": "FFMPEG_LIBRARY_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib",
          "type": "PATH"
        },
        {
          "name": "FBX_SDK_INCLUDE_DIR",
          "value": "C:/Program Files/Autodesk/FBX/FBX SDK/2020.2.1/include",
          "type": "PATH"
        },
        {
          "name": "FBX_SDK_LIBRARY_FILE",
          "value": "C:/Program Files/Autodesk/FBX/FBX SDK/2020.2.1/lib/vs2019/x64/release/libfbxsdk-md.lib",
          "type": "FILEPATH"
        },
        {
          "name": "FBX_XML2_LIBRARY_FILE",
          "value": "C:/Program Files/Autodesk/FBX/FBX SDK/2020.2.1/lib/vs2019/x64/release/libxml2-md.lib",
          "type": "FILEPATH"
        },
        {
          "name": "FBX_ZLIB_LIBRARY_FILE",
          "value": "C:/Program Files/Autodesk/FBX/FBX SDK/2020.2.1/lib/vs2019/x64/release/zlib-md.lib",
          "type": "FILEPATH"
        },
        {
          "name": "GDAL_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "PATH"
        },
        {
          "name": "GDAL_LIBRARY",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/gdal_i.lib",
          "type": "FILEPATH"
        },
        {
          "name": "GMP_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "PATH"
        },
        {
          "name": "GMP_LIBRARIES",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/mpir.lib",
          "type": "FILEPATH"
        },
        {
          "name": "LASLIB_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include/laszip",
          "type": "PATH"
        },
        {
          "name": "LASLIB_RELEASE_LIBRARY",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/laszip3.lib",
          "type": "FILEPATH"
        },
        {
          "name": "LASZIP_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include/laszip",
          "type": "PATH"
        },
        {
          "name": "MPFR_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library",
          "type": "PATH"
        },
        {
          "name": "MPFR_LIBRARIES",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/mpfr.lib",
          "type": "FILEPATH"
        },
        {
          "name": "OPENCASCADE_INC_DIR",
          "value": "C:/OpenCASCADE-7.5.0-vc14-64/opencascade-7.5.0/inc",
          "type": "PATH"
        },
        {
          "name": "OPENCASCADE_LIB_DIR",
          "value": "C:/OpenCASCADE-7.5.0-vc14-64/opencascade-7.5.0/win64/vc14/lib",
          "type": "PATH"
        },
        {
          "name": "OPENCASCADE_DLL_DIR",
          "value": "C:/OpenCASCADE-7.5.0-vc14-64/opencascade-7.5.0/win64/vc14/bin",
          "type": "PATH"
        },
        {
          "name": "TBB_INCLUDE_DIRS",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "PATH"
        },
        {
          "name": "XercesC_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "PATH"
        },
        {
          "name": "XercesC_LIBRARY",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/lib/xerces-c_3.lib",
          "type": "FILEPATH"
        },
        {
          "name": "ZLIB_INCLUDE_DIR",
          "value": "C:/Users/paulr/anaconda3/envs/CloudComPy37/Library/include",
          "type": "PATH"
        },
        {
          "name": "ZLIB_LIBRARY",
          "value": "C:/Users/paulr/anaconda3/Library/lib/zlib.lib",
          "type": "FILEPATH"
        }
      ],
      "intelliSenseMode": "windows-clang-x64"
    }
  ]
}
```

After the installation step, it is in any case necessary to load the Anaconda environment (Anaconda Prompt console) 
for Python and Numpy to be correctly configured.

The tests are installed in `<install-dir>/doc/PythonAPI_test`, with shell scripts to set the `PYTHONPATH` and launch one test.
When in `<install-dir>/doc/PythonAPI_test`, `ctest` launches all the tests. 

The CloudCompare GUI is installed in  `<install-dir>/bin/CloudCompare`, and works as usual. 

## Ubuntu 20.04, with Anaconda3

You need a recent installation of Anaconda3.

You need to create an environment for CloudComPy in Anaconda3, from the terminal
(here, I chose to activate Anaconda environment on demand: please adapt the instructions to your installation):

```
. ~/anaconda3/etc/profile.d/conda.sh
conda activate
conda create --name CloudComPy39 python=3.9
# --- erase previous env if existing
conda activate CloudComPy39
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install qt numpy psutil boost xerces-c pcl gdal cgal cmake pdal opencv ffmpeg mysql "qhull=2019.1" matplotlib "eigen=3.3.9"
```

Before building CloudComPy and CloudCompare or using them, you need to load the environment. 
From a new prompt: 

```
. ~/anaconda3/etc/profile.d/conda.sh
conda activate CloudComPy39
export LD_LIBRARY_PATH=~/anaconda3/envs/CloudComPy39/lib:${LD_LIBRARY_PATH}
```

I use clang series compilers, but it should work with g++,gcc as well. Options to set with cmake-gui or ccmake (adapt the paths):

```
BUILD_PY_TESTING:BOOL=1
BUILD_TESTING:BOOL=1
CCCORELIB_USE_CGAL:BOOL=1
CCCORELIB_USE_TBB:BOOL=1
CMAKE_BUILD_TYPE:STRING=RelWithDebInfo
CMAKE_INSTALL_PREFIX:PATH=/home/paul/projets/CloudComPy/installConda39
FBX_SDK_INCLUDE_DIR:PATH=/home/paul/projets/CloudComPy/fbxSdk/include
FBX_SDK_LIBRARY_FILE:FILEPATH=/home/paul/projets/CloudComPy/fbxSdk/lib/gcc/x64/release/libfbxsdk.a
GDAL_CONFIG:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/bin/gdal-config
GDAL_INCLUDE_DIR:PATH=/home/paul/anaconda3/envs/CloudComPy39/include
GDAL_LIBRARY:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/lib/libgdal.so
GMP_INCLUDE_DIR:PATH=/home/paul/anaconda3/envs/CloudComPy39/include
GMP_LIBRARIES:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/lib/libgmp.so
GMP_LIBRARIES_DIR:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/
MPFR_INCLUDE_DIR:PATH=/home/paul/anaconda3/envs/CloudComPy39/include
MPFR_LIBRARIES:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/lib/libmpfr.so
MPFR_LIBRARIES_DIR:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/
OPTION_USE_GDAL:BOOL=1
PLUGIN_EXAMPLE_GL:BOOL=1
PLUGIN_EXAMPLE_IO:BOOL=1
PLUGIN_EXAMPLE_STANDARD:BOOL=1
PLUGIN_GL_QEDL:BOOL=1
PLUGIN_GL_QSSAO:BOOL=1
PLUGIN_IO_QADDITIONAL:BOOL=1
PLUGIN_IO_QCSV_MATRIX:BOOL=1
PLUGIN_IO_QE57:BOOL=1
PLUGIN_IO_QFBX:BOOL=1
PLUGIN_IO_QPDAL:BOOL=1
PLUGIN_IO_QPHOTOSCAN:BOOL=1
PLUGIN_STANDARD_QBROOM:BOOL=1
PLUGIN_STANDARD_QCOMPASS:BOOL=1
PLUGIN_STANDARD_QHPR:BOOL=1
PLUGIN_STANDARD_QM3C2:BOOL=1
PLUGIN_STANDARD_QPCL:BOOL=1
PLUGIN_STANDARD_QPOISSON_RECON:BOOL=1
PLUGIN_STANDARD_QRANSAC_SD:BOOL=1
PYTHONAPI_TEST_DIRECTORY:STRING=CloudComPy/data
PYTHONAPI_TRACES:BOOL=1
PYTHON_PREFERED_VERSION:STRING=3.9
XercesC_INCLUDE_DIR:PATH=/home/paul/anaconda3/envs/CloudComPy39/include
XercesC_LIBRARY_RELEASE:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/lib/libxerces-c.so
ZLIB_INCLUDE_DIR:PATH=/home/paul/anaconda3/envs/CloudComPy39/include
ZLIB_LIBRARY_RELEASE:FILEPATH=/home/paul/anaconda3/envs/CloudComPy39/lib/libz.so
```

