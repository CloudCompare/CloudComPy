# Release notes

These release notes start with the October 18, 2021 CloudComPy release.

## January 15, 2022 CloudComPy release:

 - Docker build for Linux, update doc on Docker usage for build, test and use.
 - Add methods for color on ccPointCloud, access and modify color from Numpy.
 - Add QColor interface for color conversions and transformations.
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