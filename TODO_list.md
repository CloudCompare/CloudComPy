Cloud Compare Python API Choices of technology, TODO list & questions
======================================================================

Choice of a wrapping tool
-------------------------

Several tools are available. I successively tested PyQt/sip, then Pyside2/Shiboken2, and BoostPython. 
BostPython is the one I'm currently using.

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

Selected tool: BoostPython
--------------------------
BoostPython is a mature and stable project. It is entirely based on C++, we write C++ to describe the interface, explicitly, 
for all the classes, methods, functions that we want to show.
The documentation is succinct and austere (small tutorial and reference documentation). 
Learning is laborious, but the tool is very powerful and introducing new classes and methods becomes easier, 
when most of the type converters have been defined.

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
The Cloud Compare Python API code is gathered in a PYCC directory (Cloud Compare Python API).
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
NB: no problem detected to date on the reduced interface, with simple tests ...

Portable Python tests
---------------------
The test scripts are dependent on local paths, and on a local dataset.
- the dataset is generated by scripts.
- the test scripts are configured to run at build time and install time
- ctest is used to run the tests at build step (make test, works only on Linux), and install step (ctest)

Windows Packaging
-----------------
Windows version is built with Anaconda3 packages. Anaconda provides all the libraries and environment required.

TODO List
---------

- TODO: try to have ctest working at build step on Windows and Linux (incomplete)
- TODO: Try PyBind11 as a replacement for boost.python : potentially faster and lighter.
- TODO: build the reference documentation on Windows
- TODO: A tutorial, to find some examples for the principal use cases.
- TODO: A kind of automatic test coverage, to find examples for a particular function.
- TODO: Fix the Link problem in Windows to avoid the option /force:multiple at link step
- TODO: Plugin PCL (issue#35)
- TODO: Mesh triangles iterator functions (#issue36)
- TODO: add option for las/laz version (1.3, 1.4) (#issue32)

