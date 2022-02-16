# Release notes

These release notes start with the October 18, 2021 CloudComPy release.

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