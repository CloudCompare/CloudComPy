
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

This corresponds to the CloudCompare GUI `Interactive Segmentation Tool <https://www.cloudcompare.org/doc/wiki/index.php/Interactive_Segmentation_Tool>`_
when used with an imported polygon.
 
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


The `CloudCompare wiki <https://www.cloudcompare.org/doc/wiki/index.php/Main_Page>`_ provides a good introduction to 
the `distances computation <https://www.cloudcompare.org/doc/wiki/index.php/Distances_Computation>`_.

See also :ref: `cloud_comparison_M3C2`.

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

Cloud registration with the CloudCompare implementation of ICP algorithm
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The `CloudCompare wiki <https://www.cloudcompare.org/doc/wiki/index.php/Main_Page>`_ provides a good introduction to 
the `alignment and registration process <https://www.cloudcompare.org/doc/wiki/index.php/Alignment_and_Registration>`_.

To test the registration with ICP (iterative ClosestPoint) we get two clouds with an exact overlap of 10% and
we apply a small rotation-translation on one cloud. 

.. include:: ../tests/test010.py
   :start-after: #---ICP01-begin
   :end-before:  #---ICP01-end
   :literal:
   :code: python
   
Then we apply the ICP algorithm :py:meth:`cloudComPy.ICP` which gives a transformation to apply 
to get the precise overlap of the clouds.

.. include:: ../tests/test010.py
   :start-after: #---ICP02-begin
   :end-before:  #---ICP02-end
   :literal:
   :code: python

The above code snippets are from `test010.py <..//PythonAPI_test/test010.py>`_.

Cloud registration with the PCL plugin
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The `PCL plugin <http://www.cloudcompare.org/doc/wiki/index.php/Point_Cloud_Library_Wrapper_(plugin)>`_ 
of CloudCompare wraps several methods of the `PCL Library <https://pointclouds.org/>`_.
The `Fast Global Registration <http://www.cloudcompare.org/doc/wiki/index.php/Fast_Global_Registration>`_ 
is one of these methods.

To test this tool, we start by cloning a cloud and applying a rotation on it.
The algorithm requires to compute normals on clouds.
We keep clones of the clouds for testing purposes.

.. include:: ../tests/test038.py
   :start-after: #---PCLregistration01-begin
   :end-before:  #---PCLregistration01-end
   :literal:
   :code: python

We need to import the :py:mod:`~.cloudComPy.PCL` plugin:

.. include:: ../tests/test038.py
   :start-after: #---PCLregistration02-begin
   :end-before:  #---PCLregistration02-end
   :literal:
   :code: python

The :py:mod:`~.cloudComPy.PCL` plugin gives access to the class :py:class:`~.cloudComPy.PCL.FastGlobalRegistrationFilter`

.. include:: ../tests/test038.py
   :start-after: #---PCLregistration03-begin
   :end-before:  #---PCLregistration03-end
   :literal:
   :code: python

The above code snippets are from `test038.py <..//PythonAPI_test/test038.py>`_.

Cloud triangulation: create a mesh
----------------------------------

The :py:meth:`cloudComPy.ccMesh.triangulate` methods is adapted to create 
a Delaunay 2.5D mesh from a point cloud that represents a kind of 2.5D elevated surface.

When the best fitting plane of the cloud is axis aligned (dim=2 means Z axis):

.. include:: ../tests/test011.py
   :start-after: #---triangulate01-begin
   :end-before:  #---triangulate01-end
   :literal:
   :code: python

Otherwise we tell the algorithm to find the best fitting plane:

.. include:: ../tests/test011.py
   :start-after: #---triangulate02-begin
   :end-before:  #---triangulate02-end
   :literal:
   :code: python

The above code snippets are from `test011.py <..//PythonAPI_test/test011.py>`_.

Fitting ccPlane
---------------

The method :py:meth:`~.cloudComPy.ccPlane.Fit` allows to adjust a plane primitive on a cloud:

.. include:: ../tests/test012.py
   :start-after: #---planeFit01-begin
   :end-before:  #---planeFit01-end
   :literal:
   :code: python

The above code snippet is from :download:`test012.py <../tests/test012.py>`.

See :ref:`plane_properties` for the plane methods.

See also :ref:`2D_polygons` if you want the 2D polygon delimiting the projection of the cloud on the fitting plane.

Searching points in neighbourhood
---------------------------------

To find cloud nodes in the neighbourhood of a given point, the octree offers several methods,
to search in a sphere, a cylinder, a box. See :ref:`octrees` for examples of use.


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

.. _cloud_comparison_M3C2:

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
