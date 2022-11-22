# Release notes

These release notes start with the October 18, 2021 CloudComPy release.

## November 22, 2022 CloudComPy release:

 - update to CloudCompare master > 2.12.4 (November 20, 2022). The new LAS plugin is not yet taken into account in this release.
 - (issue #74) document RANSAC_SD plugin: extract plane equation.
 - (issue #76) fix the tracing system.
 - Various small fixes.
 
 ** NOTE**: experiments in progress on issue #49...
 
## October 07, 2022 CloudComPy release:

 - (issue #60) implement precision maps for M3C2 plugin
 - (issue #71) implement scalar field interpolation from another entity.
 - (issue #72) define a better message on Windows when setting the environment for CloudComPy
 - Various small fixes.

** NOTE**: still working on issues #49 and #50...

## September 18, 2022 CloudComPy release:

 - CloudComPy is now built with pybind11 instead of Boost.Python. For the CloudComPy user, the scripts are unchanged, 
   but there is always the possibility of small changes in the behaviour of the code or bugs not covered by the tests.
   (if so, please post issues).
 - CloudComPy is built with Python 3.10 instead of Python 3.9, you have to use a new Conda environment
   (see the README for instructions).
 - update to CloudCompare master > 2.12.4 (september 15, 2022)
 - (issue #53) update pdal version to 2.4.2: needs conda Python 3.10. Works on Linux and Windows
 - (issue #54) fix the bug on save scalar fields with laz 1.4 -test added on extra scalar field saved an correctly loaded, see test020.py
 - (issue #64) a use case with invalid points (E57 file) and scalar fields to resize, see test041.py
 - (issue #65) add possibility of automatic downloading of sample files for ctest, update README for ctest, see test041.py
 - (issue #66) get access to global shift: see ccShiftedObject::getGlobalShift, see test006.py
 - Various small fixes.

** NOTE**: issues #49 and #50 are still under analysis, they will require further development.
   The changes done in the conda packaging and the use of pybind11 are a necessary step towards solving these issues.

** WARNING ** On Windows, with the binary based on the conda packages, Python 3.10, only the English version of CloudCompare is available,
   the translations are not available: it seems that this is due to a bug in the Qt 5.15.4 version of the conda package.
   We are waiting for the correction of this bug in the packaging.

## July 19, 2022 CloudComPy release:

 - update to CloudCompare master > 2.12.4 (july 19, 2022)
 - add a user documentation with some use cases.
 - improve C2C split distances (issue #56), see test009.py
 - get access to sensor TSL info (sensors position, transformation...) (issue #57), see test041.py
 - get access to E57 structure (issue #58), see test041.py
 - compute scattering angles (issue #59), see test041.py
 - implement GuessBestParams for M2C2 plugin (issue #60), see test030.py
 - compute CSF filter on a cloud (issue #61), see test043.py
 - add a use case for memory leaks in iterative processes (issue #62), see test042.py
 - Various small fixes.

** NOTE**: issues #49, #50, #53, #54 are still under analysis, they will require further development.

## June 22, 2022 CloudComPy release:

 - update to CloudCompare 2.12.3 (june 14, 2022)
 - get the transformation matrix computed by the fast global registration function in PCL Plugin (issue #35), see test038.py
 - compute a scalarField gradient (issue #46), see test003.py
 - add an extra parameter to LoadParameters, to give a regex filter to ply reader, extra scalar fields selection (issue #51), see test040.py
 - fix ccGLMatrix constructor, provide another alternate constructor and method fromString (issue #52), see test026.py
 - build the doc on Windows
 - Various small fixes.
 
## May 13, 2022 CloudComPy release:

 - Update to CloudCompare 2.12.1 (may 7, 2022)
 - Fix the doc on RANSAC_SD, primitives types.
 - Add option for las/laz version (1.4) (issue #32), see test020.py.
 - Plugin PCL (issue #35) see test038.py.
 - Mesh triangles iterator functions (issue #36), see test011.py.
 - Cut a mesh with a polyline (Crop2D) (issue #38) see test039.py.
 - Add functions to handle display state of colors and scalar fields (for save in .bin and opening in GUI) (issue #43).
   See methods in base classes for point clouds: showColors, showSF, showNormals...
 - Various small fixes.

## April 09, 2022 CloudComPy release:

 - Update to CloudCompare 2.12.0 (april 7 2022)
 - Add access to plugin RANSAC_SD on Windows and Linux (issue #27). See test035.py.
 - Cross-section methods (issue #27). See test036.py
 - Extract Connected Components (issue #27). See test037.py
 - fix tests, activate trace on all tests to improve debug
 - add methods on primitives
 - fix a bug in conversion from C++ to Python, where objects name are missing.
 - fix keyword arguments problems when default value is nullptr
 - update README files, and add them in binary.
 - fix several mistakes in doc. 

## March 20, 2022 CloudComPy release:

 - Add access to plugin PCV (ShadeVis) on Windows and Linux (issue #24). See test032.py.
 - Add access to plugin HPR on Windows and Linux (issue#25). See test033.py.
 - Add access to plugin MeshBoolean on Windows and Linux. See test034.py.
 - The wrapping of the C2M and C2C algorithms has been rewritten.
   In some situations, the results were wrong (issue #26).
   Methods have been added or rewritten to facilitate parameterization 
   that gives correct performance: ComputeApproxDistance methods with statistics, 
   method for estimating the correct octree level.
   Automatic tests (test009.py, test010.py, test022.py) have been adapted.
   The performance tests (on Linux) shows no degradation between the GUI and CloudComPy.
 - Various improvements and bug fixes.

## February 17, 2022 CloudComPy release:

 - Allow keyword arguments on methods with optional arguments, to avoid to fill a long list of default values (issue #20).
   See for instance test025.py in doc/PythonAPI/test025.py, where the methods have about 20 arguments with only two mandatory.
   The keyword arguments are not yet implemented on methods without optional arguments.
 - Debug traces from C++ are extended to messages from CloudCompare (the same we get on the CloudCompare GUI console).
   Need the environment variable _CCTRACE_=ON
 - Fix several problems related to las driver (issue #20):
   - On Linux, set the locale to C (same as default US locale) to avoid conversion problems from string to float.
   - The latest version of PDAL (2.3.0) is more restrictive on dimension names than 2.1.0.
 - Python package restructuration to handle properly the standard plugins: to get access to the wrapped methods of a plugin,
   this plugin must be explicitely imported: see for instance doc/PythonAPI/test030.py (plugin M3C2)
   the plugin M3C2 is the only standard plugin wrapped for the moment (issue #16).
 - Add some features to Rasterization (issue #21)
 - Various improvements and bug fixes.

## January 30, 2022 CloudComPy release:

 - Build CloudCompare with all Plugins (a lot are not yet available in CloudComPy)
 - More methods related to normals (see test014)
 - On Linux only, access to M3C2 plugin in CloudComPy (see test030)
 - Draco driver available for I/O in CloudComPy (see test020)
 - Various improvements and bug fixes.

## January 15, 2022 CloudComPy release:

 - Docker build for Linux, update doc on Docker usage for build, test and use.
 - Add methods for color on ccPointCloud, access and modify color from Numpy (see test029).
 - Add QColor interface for color conversions and transformations (see test028).
 - Various improvements and bug fixes.

## December 5, 2021 CloudComPy release:

 - Windows build: restructuration of CMakeSettings.json and build with **Python 3.9**
 - complements on transformations methods (constructors, converters, data access, rotation operators...)
 - conversion of a tranformation from double precision to simple precision
 - delete entity method (**warning**: use with caution, no protection against a C++ object destruction while there are still Python objects handling the deleted object!)
 - complements on Polyline methods
 - addChild method for entities
 - a minimal bounding box function (find a rotation that minimise the bounding box Ox,Oy,Oz) written with CloudComPy (see test026.py and tes027.py) 
 - better scripts to set the environment when running CloudComPy (enCloudCompPy.bat on Windows, condaCloud.sh on Linux)
 - activate or deactivate c++ debug trace with environment variable _CCTRACE_=ON
 - add IDE **spyder** and **jupyter** on conda environments (exemple of jupyter notebook in doc/samples)
 - README restructuration