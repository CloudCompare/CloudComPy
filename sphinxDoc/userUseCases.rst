
==================================
Some use cases
==================================

Create a point cloud with numpy
-------------------------------

The way to set coordinates of a cloud from a Numpy array have been described in :ref:`coordinates_Numpy`
and the method to create scalar fields from Numpy arrays have been exposed in :ref:`ScalarField_Numpy`.

A first example generates a kind of spherical cloud with sinusoidal altitude fluctuations. One scalar field is defined 
with the local altitude. Nodes are randomly generated.

.. include:: ../tests/test017.py
   :start-after: #---createCloudNumpy01-begin
   :end-before:  #---createCloudNumpy01-end
   :literal:
   :code: python

The above code snippet is from :download:`test017.py <../tests/test017.py>`.

Another similar example in which we create a kind of 2.5D wavelet and add the altitude scalar field
after the cloud creation with :py:meth:`~.cloudComPy.ccPointCloud.exportCoordToSF`.
Nodes are randomly generated.

.. include:: ../tests/test025.py
   :start-after: #---createCloudNumpy02-begin
   :end-before:  #---createCloudNumpy02-end
   :literal:
   :code: python

The above code snippet is from :download:`test025.py <../tests/test025.py>`.

A third example draw a polyline from a bounding box obtained, for instance, 
with :py:meth:`~.cloudComPy.ccPointCloud.getOwnBB`.

Firstly, we create a Numpy array from the corner coordinates of the bounding box.
The nodes are ordered to build a polyline:

.. include:: ../tests/test026.py
   :start-after: #---coordsBoundinGBox-begin
   :end-before:  #---coordsBoundinGBox-end
   :literal:
   :code: python

Then, we create the cloud from the Numpy array, apply a transformation, and build the polyline.

.. include:: ../tests/test026.py
   :start-after: #---polyFromCloud01-begin
   :end-before:  #---polyFromCloud01-end
   :literal:
   :code: python

The above code snippets are from :download:`test026.py <../tests/test026.py>`.

.. _crop_cloud:

Cut a cloud or a mesh with a polyline
-------------------------------------
crop2D (cloud)
crop2D mesh (test039)

Compute distances between clouds and meshes
-------------------------------------------
distances computeApproxCloud2MeshDistance computeCloud2MeshDistances...
split distances.

Cloud registration
------------------
registration ICP (test010)
The following code is extract from `test010.py <..//PythonAPI_test/test010.py>`_.

.. include:: ../tests/test010.py
   :start-after: #---ICP-fragment-begin
   :end-before:  #---ICP-fragment-end
   :literal:
   :code: python

Registration with plugin PCL (test038)

Cloud triangulation: create a mesh
----------------------------------
create mesh triangulate

Fitting ccPlane
---------------
ccPlane.Fit

Searching points in neighbourhood
---------------------------------
(test016)

Cloud sampling, noise filter
----------------------------
sample cloud, noise filter... (test019)
filterBySFValue

Generate histograms
-------------------
(test022)

Compute a 2.5D volume
---------------------
ComputeVolume25D (test023)

Cloud rasterisation
-------------------
Rasterization (test025)

Finding an optimal bounding box
-------------------------------
(test027)

Cloud comparison with plugin M3C2
---------------------------------
(test030)

ShadeVIS (ambiant occlusion) with Plugin PCV
--------------------------------------------
(test032)

Compute Hidden Point Removal with plugin HPR
--------------------------------------------
(test033)

Boolean operations on meshes with plugin MeshBoolean
----------------------------------------------------
(test034)

Finding primitives on a cloud with plugin RANSAC-SD
---------------------------------------------------
(test035)

Sclices and contours
--------------------
(test036)

ExtractConnectedComponents
--------------------------
(test037)
