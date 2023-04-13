
## Build on Windows 10, with Anaconda3 or miniconda3

There are several methods to install the prerequisites on Windows 10. 
I chose to install miniconda, which is a very complete and large Python-based tools environment. 
There is a package system under miniconda, to select the products you need. It has all our prerequisites.

From miniconda prompt:
```
conda update -y -n base -c defaults conda
conda activate
conda create -y --name CloudComPy310 python=3.10
   # --- erase previous env with the same name if existing
conda activate CloudComPy310
conda config --add channels conda-forge
conda config --set channel_priority strict
conda install -y "boost=1.74" "cgal=5.4" cmake draco ffmpeg "gdal=3.5" jupyterlab laszip "matplotlib=3.5" "mysql=8.0" "numpy=1.22" "opencv=4.5" "openmp=8.0" "pcl=1.12" "pdal=2.4" "psutil=5.9" pybind11 "qhull=2020.2" "qt=5.15.4" "scipy=1.8" sphinx_rtd_theme spyder tbb tbb-devel "xerces-c=3.2"
```
For information, the list of packages actually installed for building and testing can be found in `building/conda-list`.

CMake from miniconda is used to get ctest at install, not for build.

To use FBX format plugin, install the FBX SDK, not provided by an miniconda package.

It is necessary to configure Visual Studio 2019 with CMake.

I don't master well the configuration and use of Visual Studio, so I tested two ways to use the Visual Studio GUI, 
without knowing if there is a better way to take into account the prerequisites in the Visual Studio environment.

- 1: Launch the Visual Studio GUI as is, without any additions, and give all the necessary paths for the prerequisites.

- 2: Launch the Visual Studio GUI from the `miniconda Prompt (miniconda3)` console with the command: 
`"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\devenv.exe"`. 

With the miniconda environment, many paths and variables are found automatically. 
In both cases, it is necessary to provide some configuration variables to CMake. 
With Visual Studio, these variables can be filled in a json file, which is read during the configuration step. 
I suppose it is also possible to install cmake-gui to do the same.

The following plugins require to install specific libraries, not packaged. You can deactivate these plugins in the json file if you don't want to rebuild
the libraries...

    PLUGIN_STANDARD_QCORK see Cork for boolean operation
    PLUGIN_STANDARD_QMESH_BOOLEAN 
    PLUGIN_IO_QFBX for Autodesk format, see file I/O and above
    PLUGIN_IO_QSTEP for step format (OpenCascade libraries)

Here are my json file, for the first method, with the plugins availables with miniconda packages (you will find CMakeSettings.json in the sources):

```
{
  "configurations": [
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "RelWithDebInfo",
      "inheritEnvironments": [ "msvc_x64_x64" ],
      "home": "${env.USERPROFILE}",
      "buildRoot": "${home}/CloudComPy/build2019/${name}",
      "installRoot": "${home}/CloudComPy/install/CloudComPy310",
      "condaBase": "${home}/miniconda3",
      "condaRoot": "${condaBase}/envs/CloudComPy310",
      "cmakeExecutable": "${condaRoot}/Library/bin/cmake.exe",
      "cmakeCommandArgs": "",
      "buildCommandArgs": "-v",
      "ctestCommandArgs": "",
      "variables": [
        {
          "name": "CMAKE_SHARED_LINKER_FLAGS",
          "value": "/machine:x64 /FORCE:MULTIPLE",
          "type": "STRING"
        },
        {
          "name": "PYTHON_PREFERED_VERSION:STRING",
          "value": "3.10",
          "type": "STRING"
        },
        {
          "name": "PYTHONAPI_TEST_DIRECTORY",
          "value": "CloudComPy/data",
          "type": "STRING"
        },
        {
          "name": "PYTHONAPI_EXTDATA_DIRECTORY",
          "value": "CloudComPy/ExternalData",
          "type": "STRING"
        },
        {
          "name": "PYTHONAPI_TRACES:BOOL",
          "value": "ON",
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
          "name": "BUILD_REFERENCE_DOC:BOOL",
          "value": "ON",
          "type": "STRING"
        },
        {
          "name": "USE_CONDA_PACKAGES",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "CONDA_BASE_DIRECTORY",
          "value": "${condaBase}",
          "type": "STRING"
        },
        {
          "name": "CONDA_ROOT_DIRECTORY",
          "value": "${condaRoot}",
          "type": "STRING"
        },
        {
          "name": "INSTALL_PREREQUISITE_LIBRARIES",
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
          "name": "CCCORELIB_USE_TBB",
          "value": "False",
          "type": "BOOL"
        },
        {
          "name": "Boost_DEBUG:BOOL",
          "value": "OFF",
          "type": "STRING"
        },
        {
          "name": "OPTION_SCALAR_DOUBLE:BOOL",
          "value": "OFF",
          "type": "STRING"
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
          "name": "PLUGIN_IO_QDRACO",
          "value": "False",
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
          "value": "false",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QLAS",
          "value": "true",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QPDAL",
          "value": "false",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QPHOTOSCAN",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QRDB",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QRDB_FETCH_DEPENDENCY",
          "value": "True",
          "type": "BOOL"
        },
        {
          "name": "PLUGIN_IO_QSTEP",
          "value": "True",
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
          "name": "PLUGIN_STANDARD_QCORK",
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
          "name": "PLUGIN_STANDARD_QMESH_BOOLEAN",
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
          "name": "Qt5_DIR",
          "value": "${condaRoot}/Library/lib/cmake/Qt5",
          "type": "STRING"
        },
        {
          "name": "Qt5LinguistTools_DIR",
          "value": "${condaRoot}/Library/lib/cmake/Qt5LinguistTools",
          "type": "STRING"
        },
        {
          "name": "Qt5Test_DIR",
          "value": "${condaRoot}/Library/lib/cmake/Qt5Test",
          "type": "STRING"
        },
        {
          "name": "BOOST_INCLUDEDIR",
          "value": "${condaRoot}/Library/include",
          "type": "STRING"
        },
        {
          "name": "BOOST_LIBRARYDIR",
          "value": "${condaRoot}/Library/lib",
          "type": "STRING"
        },
        {
          "name": "Python3_ROOT_DIR",
          "value": "${condaRoot}",
          "type": "STRING"
        },
        {
          "name": "NUMPY_INCLUDE_DIR",
          "value": "${condaRoot}/Lib/site-packages/numpy/core/include",
          "type": "STRING"
        },
        {
          "name": "CGAL_DIR",
          "value": "${condaRoot}/Library/lib/cmake/CGAL",
          "type": "PATH"
        },
        {
          "name": "CORK_INCLUDE_DIR",
          "value": "${home}/CloudComPy/cork/src",
          "type": "PATH"
        },
        {
          "name": "CORK_RELEASE_LIBRARY_FILE",
          "value": "${home}/CloudComPy/cork/win/VS2013/x64/Release/wincork2013.lib",
          "type": "FILEPATH"
        },
        {
          "name": "Draco_DIR",
          "value": "${home}/CloudComPy/draco/install/share/cmake",
          "type": "PATH"
        },
        {
          "name": "EIGEN_ROOT_DIR",
          "value": "${condaRoot}/Library/include/eigen3",
          "type": "PATH"
        },
        {
          "name": "LIBIGL_INCLUDE_DIR",
          "value": "${home}/CloudComPy/libigl/libigl/include",
          "type": "PATH"
        },
        {
          "name": "LIBIGL_RELEASE_LIBRARY_FILE",
          "value": "${home}/CloudComPy/libigl/libigl/out/install/x64-Release/lib/igl.lib",
          "type": "FILEPATH"
        },
        {
          "name": "OPENCV_DIR",
          "value": "${condaRoot}/Library/cmake",
          "type": "PATH"
        },
        {
          "name": "PCL_DIR",
          "value": "${condaRoot}/Library/cmake",
          "type": "PATH"
        },
        {
          "name": "PDAL_DIR",
          "value": "${condaRoot}/Library/lib/cmake/PDAL",
          "type": "PATH"
        },
        {
          "name": "FLANN_ROOT",
          "value": "${condaRoot}/Library",
          "type": "PATH"
        },
        {
          "name": "PC_FLANN_INCLUDE_DIRS",
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "PC_FLANN_LIBRARY_DIRS",
          "value": "${condaRoot}/Library/lib",
          "type": "PATH"
        },
        {
          "name": "FFMPEG_INCLUDE_DIR",
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "FFMPEG_LIBRARY_DIR",
          "value": "${condaRoot}/Library/lib",
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
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "GDAL_LIBRARY",
          "value": "${condaRoot}/Library/lib/gdal_i.lib",
          "type": "FILEPATH"
        },
        {
          "name": "GMP_INCLUDE_DIR",
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "GMP_LIBRARIES",
          "value": "${condaRoot}/Library/lib/mpir.lib",
          "type": "FILEPATH"
        },
        {
          "name": "MPIR_INCLUDE_DIR",
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "MPIR_RELEASE_LIBRARY_FILE",
          "value": "${condaRoot}/Library/lib/mpir.lib",
          "type": "FILEPATH"
        },
        {
          "name": "MPFR_INCLUDE_DIR",
          "value": "${condaRoot}/Library",
          "type": "PATH"
        },
        {
          "name": "MPFR_LIBRARIES",
          "value": "${condaRoot}/Library/lib/mpfr.lib",
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
          "name": "OPENCASCADE_TBB_DLL_DIR",
          "value": "${condaRoot}/Library/lib/tbb.lib",
          "type": "PATH"
        },
        {
          "name": "QHULL_DIR",
          "value": "${condaRoot}/Library/lib/cmake/Qhull",
          "type": "PATH"
        },
        {
          "name": "TBB_DIR",
          "value": "${condaRoot}/Library/lib/cmake/TBB",
          "type": "PATH"
        },
        {
          "name": "XercesC_INCLUDE_DIR",
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "XercesC_LIBRARY",
          "value": "${condaRoot}/Library/lib/xerces-c_3.lib",
          "type": "FILEPATH"
        },
        {
          "name": "ZLIB_INCLUDE_DIR",
          "value": "${condaRoot}/Library/include",
          "type": "PATH"
        },
        {
          "name": "ZLIB_LIBRARY",
          "value": "${condaRoot}/Library/lib/zlib.lib",
          "type": "FILEPATH"
        }
      ],
      "intelliSenseMode": "windows-msvc-x64"
    }
  ]
}
```

After the installation step, it is in any case necessary to load the miniconda environment (miniconda Prompt console) 
for Python and Numpy to be correctly configured.

The tests are installed in `<install-dir>/doc/PythonAPI_test`, with shell scripts to set the `PYTHONPATH` and launch one test.
When in `<install-dir>/doc/PythonAPI_test`, `ctest` launches all the tests. 

The CloudCompare GUI is installed in  `<install-dir>/bin/CloudCompare`, and works as usual. 
