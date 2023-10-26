Cloud Compare Python API Choices of technology, TODO list & questions
======================================================================

Choice of a wrapping tool
-------------------------

Several tools are available. I successively tested PyQt/sip, then Pyside2/Shiboken2, Boost.Python and PyBind11. 
PyBind11 is the one I'm currently using.

First try: PyQt SIP
-------------------

CloudCompare relies on Qt: PyQt - SIP is more suitable than SWIG, but a bit laborious...
A first demonstration with PyQt/sip was done in 2020. I had some experience with PyQt in other projects. 
The product runs on Linux and Windows.
This solution has several drawbacks: wrapping is laborious to write, the maintenance of the tool is fragile in the long term, 
there is a competing tool supported by Qt (Pyside2/Shiboken2 or Qt for Python) which has reached the right level of maturity, 
and has the advantage of an LGPL license while PyQt/Sip is under GPL.

Second try: Pyside2 / Shiboken2
-------------------------------
With recent versions of Qt (5.12 -), Qt offers Pyside2 and Shiboken2.
Why Pyside2 / Shiboken2?
See https://machinekoder.com/pyqt-vs-qt-for-python-pyside2-pyside/
On the Python side, great compatibility of scripts between Pyside2 and PyQt ==> no significant impact for future users.
Compared to Sip; the way to define the interface with Shiboken2 is completely different: 
instead of explicitly describing all classes, methods and functions to be wrapped, we provide the C++ include files, 
and a set of instructions to hide or modify some parts, in an xml file. This is not well documented, 
but there is the example of Qt itself.
After several unsuccessful attempts on CloudCompare, I finally understood that there were limitations in Shiboken2, 
not allowing the wrapping of some complex class inheritance: In cloudCompare, there are classes derived from template classes. 
Shiboken2 does not handle this situation correctly, the limitation is known and will probably not be fixed in the short term
(TODO: find the link on the bug reports).  

Selected tool: Boost.Python, then pybind11
------------------------------------------
Boost.Python is a mature and stable project. It is entirely based on C++, we write C++ to describe the interface, explicitly, 
for all the classes, methods, functions that we want to show.
The documentation is succinct and austere (small tutorial and reference documentation). 
Learning is laborious, but the tool is very powerful and introducing new classes and methods becomes easier, 
when most of the type converters have been defined.

I have now replaced Boost.Python by pybind11: pybind11 is rewritten on the basis of Boost.Python, offers more possibilities, and continues to evolve.

Windows 10 generation and testing
---------------------------------
See the README on the current construction and testing method.

Wrapping extension
------------------

The interface is still incomplete, but allows to do a lot of tests.

cloudComPy module
- load and save entities (point clouds, meshes, primitives)
- create primitives (box, cone, cylinder, plane, sphere, torus, quadric, dish)
- curvature calculation
- scalar field filtering
- ...

pointcloud, mesh, primitives, polyline, ScalarField, distanceComputationtools:
See the available methodes in the Python test scripts

Duplicate qCC code elements
---------------------------
The Cloud Compare Python API code is gathered in a PYCC directory (Cloud Compare Python API) and also in the pybind11 directory.
It is based on potentially all the CloudCompare libraries, except the application itself (qCC).
It does not require any modification of the called libraries.
By cons, need to duplicate code from qCC, without GUI graphics calls.
For example, for the calculation of curvature, retrieved from `ccLibAlgorithms::ComputeGeomCharacteristic`, 
`ccLibAlgorithms::GetDensitySFName`.
Do we need an intermediate layer of callable processing from the GUI and the Python interface?

Numpy, ownership of ScalarFields
--------------------------------
It is possible to transform the cloud coordinates or a ScalarField into Numpy Array with or without copying data.

When data is borrowed from C++ (not copied), ownership remains on the C++ side: 
the destruction of data can only be done via a CloudCompare method (destruction of the C++ cloudCompare object owning the data). 
Nothing exists yet in the Python interface for CloudCompare objects destruction.

When data is copied, ownership of the copied data is transfered to Python. 
The PyArray object owns the copied data, the Python garbage collector is in charge of the PyArray and its data. 
In the other direction, we can overwrite the data of an existing ScalarField with that of a Numpy Array of the same type, 
same dimensions and size (vector of the same number of elements).
- There are build tests to ensure that premature destruction or memory leaks are avoided.

Python references counter, Ownership
------------------------------------
Python's garbage collector relies on a references counter on which we can act in the C ++ interface (Py_INCREF, PY_DECREF).
The Wrapper makes it possible to say explicitly who has the ownership of the objects created at the interface.
Establish rules and tests to be sure of writing reliable code (Numpy Array <--> Coordinates or ScalarField is an important particular case).
Currently the interface does not prohibit potentially destructive actions.
NB: no problem detected to date on the interface, with simple tests ...

Portable Python tests
---------------------
The test scripts are dependent on local paths, and on a local dataset.
- the dataset is generated by scripts.
- the test scripts are configured to run at build time and install time
- ctest is used to run the tests at build step (make test, works only on Linux), and install step (ctest)

Windows Packaging
-----------------
Windows version is built with Anaconda3 packages. Anaconda provides almost all the libraries and environment required.
Only a few plugins require libraries that are not packaged.

TODO List
---------

* DONE: build the reference documentation on Windows
* DONE: A tutorial, to find some examples for the principal use cases : replaced by a user documentation with use cases
* DONE: (issue #35) Plugin PCL
* DONE: (issue #36) Mesh triangles iterator functions
* DONE: (issue #32) add option for las/laz version (1.3, 1.4)
* DONE: Fix the doc on RANSAC_SD, primitives types
* DONE: (issue #38) Cut a mesh with a polyline (Crop2D)
* DONE: (issue #43) add functions to handle display state of colors and scalar fields (for save in .bin and opening in GUI)
* DONE: (issue #46) compute gradients for scalar fields
* DONE: (issue #35) get the transformation matrix computed by the fast global registration function in PCL Plugin
* DONE: (issue #51) try reading .ply files with extra columns interpreted as scalarFields, regardless of their names
* DONE: (issue #52) fix the bug in ccGLMatrix constructor
* DONE: (issue #56) improve C2C split distances
* DONE: (issue #57) get access to sensor TSL info (sensors position, transformation...)
* DONE: (issue #58) get access to E57 structure, check version of libE57
* DONE: (issue #59) implement sensors and scattering angles calculation
* DONE: (issue #60) implement GuessBestParams for M2C2 plugin
* DONE: (issue #40) build examples of loop with objects created and destroyed for a stable memory usage
* DONE: (issue #49) evaluate the feasibility of a virtual 3D view snapshot
* DONE: (issue #62) fix or document memory leak problem
* DONE: (issue #61) add CSF filter
* DONE: Try PyBind11 as a replacement for boost.python : potentially faster and lighter.
* DONE: (issue #53) update pdal version to 2.4.2: needs conda Python 3.10. Works on Linux and Windows
* DONE: (issue #54) fix the bug on save scalar fields with laz 1.4 -test added on extra scalar field saved an correctly loaded
* DONE: (issue #66) get access to global shift: see ccShiftedObject::getGlobalShift
* DONE: (issue #65) check possibility of automatic downloading of sample files for ctest, update README for ctest
* DONE: rename Boost.Python directory into pybind11 and adapt scripts.
* DONE: (issue #64) a use case with invalid points (E57 file) and scalar fields to resize
* DONE: (issue #60) implement precision maps for M3C2 plugin
* DONE: (issue #71) implement scalar field interpolation from another entity.
* DONE: (issue #72) define a better message on Windows when setting the environment for CloudComPy
* DONE: (issue #74) document RANSAC_SD plugin: extract plane equation.
* DONE: (issue #76) fix the tracing system.
* DONE: (issue #77) Canupo plugin: at least the same as in command mode
* DONE: Take into account the new las io plugin of CloudCompare, both in CloudCompare and CloudComPy
* DONE: Wrap the Surface Revolution Analysis plugin of CloudCompare in CloudComPy
* DONE: (issue #82) remove old wrap of M3C2 PLUGIN (WRAP_PLUGIN_M3C2)
* DONE: (issue #83) Fix bug in RANSAC SD python plugin: meshes not detected when sensors in dataset
* DONE: (issue #85) fix install procedure, Qt 5.15.6 is not compatible with Qt 5.15.4
* DONE: reactivate the Draco IO Plugin on Windows (link problem)
* DONE: (issue #89) update the docker scripts
* DONE: (issue #90) abort on deleteInstance on a cloud created by pybind11 constructor
* DONE: (issue #91) add 'up direction' in computeRoughness
* DONE: fix CloudComPy behavior with CloudCompare commit "Smart BIN file version (#1744)"
* DONE: (issue #94) retrieve time shift applied automatically on scalar field 
* DONE: (issue #95) merge components (from ExtractConnectedComponents), retrieve small components not listed
* DONE: merge label components development
* DONE: (issue #100) build with a fix on pcl library to export large clouds, pcd format. Add an optional test
* DONE: (issue #103) extract section feature
* DONE: (issue #109) add missing features in Rasterize function (SF cell statistics, percentile)
* DONE: (issue #111) resampleCloudSpatially modParams parameter should be optional
* DONE: (issue #114) importFile method: extend to all ccHObject types known to CloudComPy
* DONE: (issue #116) in ExtractConnectedComponents, when the actual number of components is larger than the maxNumberComponents parameter, return the truncated result.
* DONE: (issue #104) CloudComPy for Mac
* DONE: (issue #120) Interactive display pop up using cloudComPy.render()
* DONE: add the CSF mesh to the output (follow issue #61)
* DONE: (issue #125) ComputeVolume25D: parameters (Empty Cell, Max Edge Length, Grid Cell Height)
* DONE: (issue #127) Set background color when rendering images
* DONE: (issue #128) implement PoissonReconstruction
* DONE: (issue #130) Specify which e57 scans to import from a multi-scan e57 file
* DONE: (issue #130) bug on the selection of scans to import in a multi-scan e57 file (should use QRegExp::exactMatch)
* DONE: (issue #134) access to normals as a Numpy array
* DONE: fix path related to CloudComPy web site changes, in documentation and tests
- TODO: try to have ctest working at build step on Windows and Linux (incomplete)
- TODO: A kind of automatic test coverage, to find examples for a particular function.
- TODO: Fix the Link problem in Windows to avoid the option /force:multiple at link step
- TODO: add functions to get an inventory of all C++ entities, for cleaning memory purpose...
- TODO: (issue #50) evaluate the feasibility of pip or conda packaging of CloudComPy
- TODO: check Qt 5.15.6 for the translation bug observed with Qt 5.15.4
- TODO: (issue #105) propose a solution for CloudComPy on a cluster (without display)
- TODO: (issue #118) Build a version with double precision scalar field.
- TODO: (issue #129) Check the behavior of macOS version of CloudCompare/CloudComPy on CloudCompare bug #1646 (poisson)
- TODO: allow installation path with space (like "c:\program files\xxx") (follow issue #72)
