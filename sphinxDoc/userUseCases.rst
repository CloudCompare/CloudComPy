
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

The above code snippets are from :download:`test010.py <../tests/test010.py>`.

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

The above code snippets are from :download:`test038.py <../tests/test038.py>`.

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

The above code snippets are from :download:`test011.py <../tests/test011.py>`.

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


Cloud sampling, noise filter, filter by scalar field
----------------------------------------------------

The class :py:class:`cloudComPy.CloudSamplingTools` offers several cloud sampling filters.

Noise filter
~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.noiseFilter` method 
is based on the distance to the approximate local surface.
This filter removes points based on their distance relatively to the best fit plane computed on their neighbors.

The mandatory parameters are the cloud, the neighbourhood radius and the number of sigmas:

The result is a selection (:py:class:`~.cloudComPy.ReferenceCloud`)
that must be transformed in :py:class:`~.cloudComPy.ccPointCloud` with :py:meth:`~.cloudComPy.ccPointCloud.partialClone`

.. include:: ../tests/test019.py
   :start-after: #---partialClone01-begin
   :end-before:  #---partialClone01-end
   :literal:
   :code: python

Resample cloud spatially
~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.resampleCloudSpatially` method 
resamples a point cloud (process based on inter point distance)
The cloud is resampled so that there is no point nearer than a given distance to other points.
It works by picking a reference point, removing all points which are to close to this point, 
and repeating these two steps until the result is reached.

The mandatory parameters are the cloud and the distance.

The result is a selection (:py:class:`~.cloudComPy.ReferenceCloud`)
that must be transformed in :py:class:`~.cloudComPy.ccPointCloud` with :py:meth:`~.cloudComPy.ccPointCloud.partialClone`

.. include:: ../tests/test019.py
   :start-after: #---resampleCloudSpatially01-begin
   :end-before:  #---resampleCloudSpatially01-end
   :literal:
   :code: python

Subsample cloud randomly
~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.subsampleCloudRandomly` method 
subsamples a point cloud (process based on random selections).
This is a very simple subsampling algorithm that simply consists in selecting “n” different points, in a random way.

The mandatory parameters are the cloud and the total number of points to keep.

The result is a selection (:py:class:`~.cloudComPy.ReferenceCloud`)
that must be transformed in :py:class:`~.cloudComPy.ccPointCloud` with :py:meth:`~.cloudComPy.ccPointCloud.partialClone`

.. include:: ../tests/test019.py
   :start-after: #---SubsampleRandom01-begin
   :end-before:  #---SubsampleRandom01-end
   :literal:
   :code: python

resample cloud with octree at level
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.resampleCloudWithOctreeAtLevel` method 
is a resampling algorithm is applied inside each cell of the octree. 
The different resampling methods are represented as an enumerator (see :py:class:`~.cloudComPy.RESAMPLING_CELL_METHOD`)
and consist in simple processes such as replacing all the points lying in a cell by the cell center 
or by the points gravity center.

The mandatory parameters are the cloud, the octree level and the resampling method.

.. include:: ../tests/test019.py
   :start-after: #---resampleOctreeLevel01-begin
   :end-before:  #---resampleOctreeLevel01-end
   :literal:
   :code: python

resample cloud with octree
~~~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.resampleCloudWithOctree` method 
is the same as :py:meth:`~.cloudComPy.CloudSamplingTools.resampleCloudWithOctreeAtLevel` method, 
apart the fact that instead of giving a specific octree subdivision level as input parameter, 
one can specify an approximative number of points for the resulting cloud 
(the algorithm will automatically determine the corresponding octree level).

The mandatory parameters are the cloud, the approximate number of points and the resampling method.

.. include:: ../tests/test019.py
   :start-after: #---resampleOctree01-begin
   :end-before:  #---resampleOctree01-end
   :literal:
   :code: python

Statistical Outliers Removal (SOR) filter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.sorFilter` method 
removes points based on their mean distance to their distance 
(by comparing it to the average distance of all points to their neighbors). 
It is equivalent to `PCL Library <https://pointclouds.org/>`_ 
`StatisticalOutlierRemoval <https://pointclouds.org/documentation/classpcl_1_1_statistical_outlier_removal.html>`_ filter 
(see `Removing outliers using a StatisticalOutlierRemoval filter <https://pcl.readthedocs.io/projects/tutorials/en/master/statistical_outlier.html?highlight=outlier>`_)

The only mandatory parameter is the cloud.

The result is a selection (:py:class:`~.cloudComPy.ReferenceCloud`)
that must be transformed in :py:class:`~.cloudComPy.ccPointCloud` with :py:meth:`~.cloudComPy.ccPointCloud.partialClone`

.. include:: ../tests/test019.py
   :start-after: #---sorFilter01-begin
   :end-before:  #---sorFilter01-end
   :literal:
   :code: python

subsample cloud with octree at level
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.subsampleCloudWithOctreeAtLevel` method 
applies a subsampling algorithm inside each cell of the octree. 
The different subsampling methods are represented as an enumerator 
(see :py:class:`~.cloudComPy.SUBSAMPLING_CELL_METHOD`) 
and consist in simple processes such as choosing a random point, 
or the one closest to the cell center. 

The mandatory parameters are the cloud, the octree level and the subsampling method.

The result is a selection (:py:class:`~.cloudComPy.ReferenceCloud`)
that must be transformed in :py:class:`~.cloudComPy.ccPointCloud` with :py:meth:`~.cloudComPy.ccPointCloud.partialClone`

.. include:: ../tests/test019.py
   :start-after: #---subsampleOctreeLevel01-begin
   :end-before:  #---subsampleOctreeLevel01-end
   :literal:
   :code: python

subsample cloud with octree
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :py:meth:`~.cloudComPy.CloudSamplingTools.subsampleCloudWithOctree` method 
is the same as :py:meth:`~.cloudComPy.CloudSamplingTools.subsampleCloudWithOctreeAtLevel` method, 
apart the fact that instead of giving a specific octree subdivision level as input parameter, 
one can specify an approximative number of points for the resulting cloud 
(the algorithm will automatically determine the corresponding octree level).

The mandatory parameters are the cloud, the approximate number of points and the subsampling method.

The result is a selection (:py:class:`~.cloudComPy.ReferenceCloud`)
that must be transformed in :py:class:`~.cloudComPy.ccPointCloud` with :py:meth:`~.cloudComPy.ccPointCloud.partialClone`

.. include:: ../tests/test019.py
   :start-after: #---subsampleOctree01-begin
   :end-before:  #---subsampleOctree01-end
   :literal:
   :code: python

All the above code snippets are from :download:`test019.py <../tests/test019.py>`.

filter by scalar field values
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The :py:func:`cloudComPy.filterBySFValue` method 
creates a new point cloud by filtering points using the current out ScalarField
(see :py:meth:`cloudComPy.ccPointCloud.setCurrentOutScalarField`).
It keeps the points whose ScalarField value is between the min and max parameters.

Here, we create a scalar field based on curvature:

.. include:: ../tests/test003.py
   :start-after: #---curvature01-begin
   :end-before:  #---curvature01-end
   :literal:
   :code: python

Then, we apply the filter:

.. include:: ../tests/test003.py
   :start-after: #---filterSFValue01-begin
   :end-before:  #---filterSFValue01-end
   :literal:
   :code: python

The above code snippets are from :download:`test003.py <../tests/test003.py>`.

Generate histograms
-------------------

To draw plots or to generate csv files from scalar field data, we use numpy, matplotlib and csv.

histogram as a figure
~~~~~~~~~~~~~~~~~~~~~

Here, we want to obtain a histogram of distances as a figure from a scalar field.
We get a numpy array from the scalar field.

.. include:: ../tests/test022.py
   :start-after: #---histogram01-begin
   :end-before:  #---histogram01-end
   :literal:
   :code: python

We need matplotlib and numpy.

.. include:: ../tests/test022.py
   :start-after: #---histogram02-begin
   :end-before:  #---histogram02-end
   :literal:
   :code: python

Building a png histogram from a Numpy array is a standard use of matplotlib.

.. include:: ../tests/test022.py
   :start-after: #---histogram03-begin
   :end-before:  #---histogram03-end
   :literal:
   :code: python
   
The above code snippets are from :download:`test022.py <../tests/test022.py>`.

histogram as a csv file
~~~~~~~~~~~~~~~~~~~~~~~

Here, we want to obtain a histogram of distances as a csv file from a scalar field.
We get a numpy array from the scalar field.

.. include:: ../tests/test022.py
   :start-after: #---histogram01-begin
   :end-before:  #---histogram01-end
   :literal:
   :code: python

We need matplotlib and csv.

.. include:: ../tests/test022.py
   :start-after: #---histogram04-begin
   :end-before:  #---histogram04-end
   :literal:
   :code: python

Building a csv histogram from a Numpy array is a standard use of csv.

.. include:: ../tests/test022.py
   :start-after: #---histogram05-begin
   :end-before:  #---histogram05-end
   :literal:
   :code: python
   
The above code snippets are from :download:`test022.py <../tests/test022.py>`.

Compute a 2.5D volume
---------------------

This corresponds to the CloudCompare GUI `2.5D Volume <https://www.cloudcompare.org/doc/wiki/index.php/2.5D_Volume>`_

The :py:func:`cloudComPy.ComputeVolume25D` method 
computes a 2.5D volume between a cloud and a ground plane,or two clouds,
following a given direction (X, Y or Z).
If only one cloud is given, the direction (X, Y or Z) defines the normal to the plane used for calculation.
The calculation uses a cartesian grid, you provide the gridStep.

The :py:class:`cloudComPy.ReportInfoVol` structure is completed by the calculation and the following information:

+---------------------------------+-------------------------------------------------------------------------+
| volume:                         | the resulting volume                                                    |
+---------------------------------+-------------------------------------------------------------------------+
| addedVolume:                    | the positive part of the volume (ceil > floor)                          |
+---------------------------------+-------------------------------------------------------------------------+
| removedVolume:                  | the negative part of the volume (ceil < floor)                          |
+---------------------------------+-------------------------------------------------------------------------+
| surface:                        | the section of the point cloud along in the given direction             |
+---------------------------------+-------------------------------------------------------------------------+
| matchingPercent:                | percentage of the section matching ceil and floor                       |
+---------------------------------+-------------------------------------------------------------------------+
| ceilNonMatchingPercent:         | percentage of the ceil section non matching floor                       |
+---------------------------------+-------------------------------------------------------------------------+
| float groundNonMatchingPercent: | percentage of the floor section non matching ceil                       |
+---------------------------------+-------------------------------------------------------------------------+
| int averageNeighborsPerCell:    | average Neighbor number per cell                                        |
|                                 |                                                                         |
|                                 | (see ``gridStep`` argument of :py:func:`~.cloudComPy.ComputeVolume25D`) |
+---------------------------------+-------------------------------------------------------------------------+

A first example with a flat floor:

.. include:: ../tests/test023.py
   :start-after: #---computeVol25D01-begin
   :end-before:  #---computeVol25D01-end
   :literal:
   :code: python

check the results:

.. include:: ../tests/test023.py
   :start-after: #---computeVol25D02-begin
   :end-before:  #---computeVol25D02-end
   :literal:
   :code: python

A second example with a cloud floor, translated to obtain non matching parts:

.. include:: ../tests/test023.py
   :start-after: #---computeVol25D03-begin
   :end-before:  #---computeVol25D03-end
   :literal:
   :code: python

check the results:

.. include:: ../tests/test023.py
   :start-after: #---computeVol25D04-begin
   :end-before:  #---computeVol25D04-end
   :literal:
   :code: python

The above code snippets are from :download:`test023.py <../tests/test023.py>`.

Cloud rasterization
-------------------

The main purpose of this tool is to 'rasterize' a point cloud (i.e. convert it to a 2.5D grid) 
and then export it as a new cloud or a raster image (GeoTiff) for instance.
Concepts are introduced in the CloudCompare wiki `Rasterize <https://www.cloudcompare.org/doc/wiki/index.php/Rasterize>`_

Three functions are available for rasterization.

 - :py:func:`cloudComPy.RasterizeGeoTiffOnly`
 - :py:func:`cloudComPy.RasterizeToCloud`
 - :py:func:`cloudComPy.RasterizeToMesh`

All functions have a lot of parameters, 
to produce or not GeoTiff files, with or without scalar fields or colors,
to fill or not the empty cells, to export statistic scalar fields...

The ``gridStep`` parameter is mandatory in all rasterize methods. 

The code snippets below are from  :download:`test025.py <../tests/test025.py>`.
See the clouds and meshes generated by this test to understand the different options.

We start from a wave cloud created with a random generator,
to obtain points with random (x,y) coordinates (not on a grid).

.. include:: ../tests/test025.py
   :start-after: #---createCloudNumpy02-begin
   :end-before:  #---createCloudNumpy02-end
   :literal:
   :code: python

For a simple rasterisation to cloud:

.. include:: ../tests/test025.py
   :start-after: #---resterize01-begin
   :end-before:  #---resterize01-end
   :literal:
   :code: python

With the default options, empty cells stays empty.

Below, an example where the empty cells are filled with a forced constant height,
and with a GeoTiff file With raster altitude.

.. include:: ../tests/test025.py
   :start-after: #---resterize03-begin
   :end-before:  #---resterize03-end
   :literal:
   :code: python

Below, an example where the empty cells are filled with an iterpolated height,
and with a GeoTiff file With raster altitude and scalar fields.

.. include:: ../tests/test025.py
   :start-after: #---resterize04-begin
   :end-before:  #---resterize04-end
   :literal:
   :code: python

Next, we generate only the GeoTiff file With raster altitude and scalar fields.

.. include:: ../tests/test025.py
   :start-after: #---resterize05-begin
   :end-before:  #---resterize05-end
   :literal:
   :code: python

Finally, a simple rasterization to mesh:

.. include:: ../tests/test025.py
   :start-after: #---resterize02-begin
   :end-before:  #---resterize02-end
   :literal:
   :code: python

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
