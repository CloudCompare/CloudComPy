## Build on Ubuntu 20.04 with native packages

On Ubuntu 20.04, you can install the development versions of the prerequisites with:

**TODO: complete the list, many packages are required...**

```
sudo apt-get install qtbase5-dev python3 libpython3-dev python3-numpy cmake
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
FBX_SDK_INCLUDE_DIR:PATH=/home/paul/projets/CloudComPy/fbxSdk/include
FBX_SDK_LIBRARY_FILE:FILEPATH=/home/paul/projets/CloudComPy/fbxSdk/lib/gcc/x64/release/libfbxsdk.a
FBX_XML2_LIBRARY_FILE:FILEPATH=
FBX_ZLIB_LIBRARY_FILE:FILEPATH=
OPTION_USE_GDAL:BOOL=1
PLUGIN_EXAMPLE_GL:BOOL=1
PLUGIN_EXAMPLE_IO:BOOL=1
PLUGIN_EXAMPLE_STANDARD:BOOL=1
PLUGIN_GL_QEDL:BOOL=1
PLUGIN_GL_QSSAO:BOOL=1
PLUGIN_IO_QADDITIONAL:BOOL=1
PLUGIN_IO_QCORE:BOOL=1
PLUGIN_IO_QCSV_MATRIX:BOOL=1
PLUGIN_IO_QE57:BOOL=1
PLUGIN_IO_QFBX:BOOL=1
PLUGIN_IO_QPHOTOSCAN:BOOL=1
PLUGIN_STANDARD_QBROOM:BOOL=1
PLUGIN_STANDARD_QCOMPASS:BOOL=1
PLUGIN_STANDARD_QM3C2:BOOL=1
PLUGIN_STANDARD_QPCL:BOOL=1
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

