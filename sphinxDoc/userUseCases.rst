
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

Cut a cloud with a polyline
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The method :py:meth:`~.cloudComPy.ccPointCloud.crop2D` is used to cut a cloud by a 2D polyline.
The normal to the plan of the polyline can only be one of the cardinal axes oX, oY or oZ.
The original cloud is not modified, a new cloud is created with either the nodes inside or the nodes outside.

First step, get a cloud and the polyline tool:

.. include:: ../tests/test007.py
   :start-after: #---cloudCrop2D01-begin
   :end-before:  #---cloudCrop2D01-end
   :literal:
   :code: python

Second step, close the polyline:

.. include:: ../tests/test007.py
   :start-after: #---cloudCrop2D02-begin
   :end-before:  #---cloudCrop2D02-end
   :literal:
   :code: python

Third step, cut:

.. include:: ../tests/test007.py
   :start-after: #---cloudCrop2D03-begin
   :end-before:  #---cloudCrop2D03-end
   :literal:
   :code: python

The above code snippets are from :download:`test007.py <../tests/test007.py>`.

Cut a mesh with a polyline
~~~~~~~~~~~~~~~~~~~~~~~~~~

As with the cloud, the method :py:meth:`~.cloudComPy.ccMesh.crop2D` is used to cut a mesh by a 2D polyline.
The normal to the plan of the polyline can only be one of the cardinal axes oX, oY or oZ.
The original mesh is not modified, a new mesh is created with either the triangles inside or the triangles outside.

In this example, scalar fields and normals are defined, to check they are kept in the result.

.. include:: ../tests/test039.py
   :start-after: #---crop2Dmesh01-begin
   :end-before:  #---crop2Dmesh01-end
   :literal:
   :code: python

The above code snippet is from :download:`test039.py <../tests/test039.py>`.

Compute distances between clouds and meshes
-------------------------------------------

The idea is to build a scalarField giving, for each node in a cloud, 
the distance between this node and another cloud or mesh.
The process can be long, several tools help to find an optimal set of parameters.

Compute distances between a cloud and a mesh (C2M)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The method :py:meth:`cloudComPy.DistanceComputationTools.computeApproxCloud2MeshDistance` 
gives an approximate solution, without too much cpu time. You get an approximate distance scalar field,
and statistics (min, max, mean, variance, error max).

.. include:: ../tests/test009.py
   :start-after: #---C2M01-begin
   :end-before:  #---C2M01-end
   :literal:
   :code: python

The structure :py:meth:`cloudComPy.Cloud2MeshDistancesComputationParams` 
is used to define the optimal parameters for accurate distance calculation.
For instance, if the cloud and mesh are close, filling the parameter ``maxSearchDist`` can greatly speed up the process, 
but setting it to a non-zero value force a single thread processing.
The result is a new scalar field in the cloud, with accurate distances to the mesh.

.. include:: ../tests/test009.py
   :start-after: #---C2M02-begin
   :end-before:  #---C2M02-end
   :literal:
   :code: python

The above code snippet is from :download:`test009.py <../tests/test009.py>`.

.. _compute_C2C:

Compute distances between two clouds (C2C)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The method :py:meth:`cloudComPy.DistanceComputationTools.computeApproxCloud2CloudDistance` 
gives an approximate solution, without too much cpu time. You get an approximate distance scalar field,
and statistics (min, max, mean, variance, error max).

.. include:: ../tests/test010.py
   :start-after: #---C2C01-begin
   :end-before:  #---C2C01-end
   :literal:
   :code: python

The structure :py:meth:`cloudComPy.Cloud2CloudDistancesComputationParams` 
is used to define the optimal parameters for accurate distance calculation.
For instance, if the two clouds are close, filling the parameter ``maxSearchDist`` can greatly speed up the process, 
but setting it to a non-zero value force a single thread processing.
The result is a new scalar field in the cloud, with accurate distances to the cloud.

.. include:: ../tests/test010.py
   :start-after: #---C2C02-begin
   :end-before:  #---C2C02-end
   :literal:
   :code: python

The above code snippet is from :download:`test010.py <../tests/test010.py>`.

Compute split distances between two clouds (C2C)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The use case is similar to :ref:`compute_C2C`
but we need to split the distance components according to the cardinal directions.
Here, we set the parameter ``setSplitDistances`` to `cloud.size()` to create 3 scalar fields of the cloud size.
They are not associated to the cloud, and we have to do that explicitely if we need it.

.. include:: ../tests/test009.py
   :start-after: #---C2C01-begin
   :end-before:  #---C2C01-end
   :literal:
   :code: python

The above code snippet is from :download:`test009.py <../tests/test009.py>`.

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
