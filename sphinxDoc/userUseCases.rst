
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
They are not associated to the cloud, this will be done when the distances are computed.

.. include:: ../tests/test009.py
   :start-after: #---C2C01-begin
   :end-before:  #---C2C01-end
   :literal:
   :code: python

The cloud contains four new scalar fields:

 - ``C2C absolute distances``
 - ``C2C absolute distances (X)``
 - ``C2C absolute distances (Y)``
 - ``C2C absolute distances (Z)``
 
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

The method :py:meth:`~.cloudComPy.ccPlane.Fit` allows to adjust a plane primitive on a cloud.
The method :py:meth:`~.cloudComPy.ccPlane.getEquation` gives the 4 coefficients of the plane ([a, b, c, d] as ax+by+cz=d):

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

Interpolate scalar fields from one cloud to another
---------------------------------------------------

The concepts are introduced in the CloudCompare wiki 
`interpolate scalar fields <https://www.cloudcompare.org/doc/wiki/index.php/Scalar_fields%5CInterpolate_from_another_entity>`_

In the following example, 3 scalar fields from a cloud are selected for interpolation on another cloud.
the :py:class:`~.cloudComPy.interpolatorParameters` class is used to define the method, algorithm and values required for the interpolation.
The :py:func:`cloudComPy.interpolateScalarFieldsFrom` function computes the interpolation.
 
Here, the distance between the clouds is locally greater than the radius, so the interpolated scalar fields are filed with NaN in some places.

.. include:: ../tests/test044.py
   :start-after: #---interpolSF_01-begin
   :end-before:  #---interpolSF_01-end
   :literal:
   :code: python

The above code snippet is from :download:`test044.py <../tests/test044.py>`.

Finding an optimal bounding box
-------------------------------

:py:mod:`~.cloudComPy.minimalBoundingBox` is a pure Python plugin built with CloudComPy.

This tool is provided as an example of CloudComPy pure Python extension.

The :py:func:`~.cloudComPy.minimalBoundingBox.findRotation` function finds a minimal bounding box for a cloud, 
not oriented along the main axes, and the corresponding rotation.
It is not proven that the solution meets to the minimum, and the performance and robustness
can certainly be improved.

We have to import minimalBoundingBox

.. include:: ../tests/test027.py
   :start-after: #---optimalBB00-begin
   :end-before:  #---optimalBB00-end
   :literal:
   :code: python


We create an ellipsoid, and apply a rotation on it, so that its bounding box is far from optimal.

.. include:: ../tests/test027.py
   :start-after: #---optimalBB01-begin
   :end-before:  #---optimalBB01-end
   :literal:
   :code: python

The :py:func:`~.cloudComPy.minimalBoundingBox.findRotation` function returns several objects.

 - the optimized bounding box
 - the rotation (double precision) to go the object on axes to the original object
 - the polyline representing the optimal bounding box, aligned with the original object
 - the cloud associated to this polyline

.. include:: ../tests/test027.py
   :start-after: #---optimalBB02-begin
   :end-before:  #---optimalBB02-end
   :literal:
   :code: python

To check the results, we apply the rotation (inverse) to a copy of the cloud and polyline.

.. include:: ../tests/test027.py
   :start-after: #---optimalBB03-begin
   :end-before:  #---optimalBB03-end
   :literal:
   :code: python

The above code snippets are from :download:`test027.py <../tests/test027.py>`.

.. _cloud_comparison_M3C2:

Cloud comparison with plugin M3C2
---------------------------------

The :py:mod:`~.cloudComPy.M3C2` plugin is introduced 
in the `CloudCompare Plugins wiki - M3C2 <http://www.cloudcompare.org/doc/wiki/index.php/M3C2_(plugin)>`_.
The plugin computes signed distances between two clouds.

The :py:func:`~.cloudComPy.M3C2.computeM3C2` function relies on a large number of parameters grouped in a file.
The CloudCompare GUI offers a 'guess parameters' option and a way to save the parameters to a file.
The :py:func:`~.cloudComPy.M3C2.M3C2guessParamsToFile` function do the same job.

You have to import the The :py:mod:`~.cloudComPy.M3C2` plugin.
The :py:func:`~.cloudComPy.M3C2.M3C2guessParamsToFile` function requires the two clouds, the params file,
and a boolean to get a fast guess or not.
The :py:func:`~.cloudComPy.M3C2.computeM3C2` function requires only the two clouds and the params file,
and produces a cloud with new scalar field 'M3C2 distance':

.. include:: ../tests/test030.py
   :start-after: #---computeM3C2_01-begin
   :end-before:  #---computeM3C2_01-end
   :literal:
   :code: python

The params file has a simple syntax and can be created from scratch or edited.
The M3C2 params file generated below corresponds to the 'guess params' option of the GUI for the clouds of this example:

.. include:: ../tests/test030.py
   :start-after: #---M3C2params01-begin
   :end-before:  #---M3C2params01-end
   :literal:
   :code: python

To use the precision maps, you have to provide the six corresponding scalar fields in list
(3 components for the first cloud and 3 components for the second cloud),
and also two scales in a list. When these lists are provided, the computation uses the precision maps, 
and supersedes the corresponding option in the params file.

.. include:: ../tests/test030.py
   :start-after: #---computeM3C2_02-begin
   :end-before:  #---computeM3C2_02-end
   :literal:
   :code: python

The above code snippets are from :download:`test030.py <../tests/test030.py>`.

ShadeVIS (ambiant occlusion) with Plugin PCV
--------------------------------------------

The PCV plugin is documented in the
`CloudCompare Plugins wiki - PCV <http://www.cloudcompare.org/doc/wiki/index.php/ShadeVis_(plugin)>`_.
It calculates the illumination of a point cloud (or vertices of a mesh) 
as if the light was coming from a theoretical hemisphere or sphere around the object 
(the user can also provide its own set of light directions - see below). 

You have to import the The :py:mod:`~.cloudComPy.PCV` plugin.

.. include:: ../tests/test032.py
   :start-after: #---PCV01-begin
   :end-before:  #---PCV01-end
   :literal:
   :code: python

We first try to illuminate the cloud with a dish as light source, oriented to create unrealistic sharp shadows.
The cloud used as light source must have normals.

.. include:: ../tests/test032.py
   :start-after: #---PCV02-begin
   :end-before:  #---PCV02-end
   :literal:
   :code: python

The :py:func:`~.cloudComPy.PCV.computeShadeVIS` function takes a list of objects (clouds or meshes)
as first parameter, and will add a scalar field `illuminance (PCV)` to these clouds or meshes.
The second parameter is the optional light source. There are several other optional parameters.

.. include:: ../tests/test032.py
   :start-after: #---PCV03-begin
   :end-before:  #---PCV03-end
   :literal:
   :code: python

The Scalar field is renamed to keep it and compute the illuminance with the default hemispheric light source.

.. include:: ../tests/test032.py
   :start-after: #---PCV04-begin
   :end-before:  #---PCV04-end
   :literal:
   :code: python

The above code snippets are from :download:`test032.py <../tests/test032.py>`.

Compute Hidden Point Removal with plugin HPR
--------------------------------------------

The HPR plugin is described in the
`CloudCompare Plugins wiki - HPR <http://www.cloudcompare.org/doc/wiki/index.php/Hidden_Point_Removal_(plugin)>`_.

The algorithm filters out the points of a cloud that wouldn't be seen 
(by the current 3D camera) if the cloud was a closed surface. 
Therefore it tries to remove the points that should be hidden in the current viewport configuration.

Without GUI, in CloudComPy, we have to explicitely provide the coordinates of the viewpoint
in :py:func:`~.cloudComPy.HPR.computeHPR`.

.. include:: ../tests/test033.py
   :start-after: #---HPR01-begin
   :end-before:  #---HPR01-end
   :literal:
   :code: python

The above code snippet is from :download:`test033.py <../tests/test033.py>`.

Boolean operations on meshes with plugin MeshBoolean
----------------------------------------------------

The MeshBoolean plugin is described in the
`CloudCompare Plugins wiki - Mesh_Bolean <http://www.cloudcompare.org/doc/wiki/index.php/Mesh_Boolean_(plugin)>`_.

This plugin can be used to perform Boolean operations on meshes (also called CSG = Constructive Solid Geometry). 

In the example, we try an intersection of two primitives, a sphere and a cylinder,
 with :py:func:`~.cloudComPy.MeshBoolean.computeMeshBoolean`.

.. include:: ../tests/test034.py
   :start-after: #---MeshBoolean01-begin
   :end-before:  #---MeshBoolean01-end
   :literal:
   :code: python

The above code snippet is from :download:`test034.py <../tests/test034.py>`.

Finding primitives on a cloud with plugin RANSAC-SD
---------------------------------------------------

The RANSAC-SD plugin is documented in the
`CloudCompare Plugins wiki - RANSAC Shape Detection <https://www.cloudcompare.org/doc/wiki/index.php/RANSAC_Shape_Detection_(plugin)>`_.

The RANSAC-SD plugin (RANdom SAmple Consensus) is used to detect simple shapes
(planes, spheres, cylinders, cones, torus) in a cloud.

You have to import the The :py:mod:`~.cloudComPy.RANSAC_SD` plugin.

The :py:func:`~.cloudComPy.RANSAC_SD.computeRANSAC_SD` function 
requires a set of parameters: :py:class:`~.cloudComPy.RANSAC_SD.RansacParams`.

The :py:meth:`~.cloudComPy.RANSAC_SD.RansacParams.optimizeForCloud` method helps to get a correct set of parameters.
By default, the algorithm searchs only planes, spheres and cylinders, 
you can enable or disable a primitive type with  ``setPrimEnabled``.

For the example we regroup 4 clouds (samples from 3 spheres and 1 cylinder) in a single cloud.

.. include:: ../tests/test035.py
   :start-after: #---RANSACSD01-begin
   :end-before:  #---RANSACSD01-end
   :literal:
   :code: python

The :py:func:`~.cloudComPy.RANSAC_SD.computeRANSAC_SD` function returns a list of meshes and clouds for the shapes found.

.. include:: ../tests/test035.py
   :start-after: #---RANSACSD02-begin
   :end-before:  #---RANSACSD02-end
   :literal:
   :code: python

For each type of primitive (plane, sphere, cylinder,..) it is possible to get the parameters of the equation.
For instance, a plane is identified with

::
	mesh.isA((cc.CC_TYPES.PLANE)

For the plane primitive, the method :py:meth:`~.cloudComPy.ccPlane.getEquation` returns the 4 coefficients of the plane equation: [a, b, c, d] as ax+by+cz=d

The above code snippets are from :download:`test035.py <../tests/test035.py>`.

Compute Cloth Simulation Filter on a cloud with CSF plugin
----------------------------------------------------------

The concepts are presented in the 
`CloudCompare wiki - CSF (plugin) <https://www.cloudcompare.org/doc/wiki/index.php/CSF_(plugin)>`_.

You have to import the The :py:mod:`~.cloudComPy.CSF` plugin.

The :py:func:`~.cloudComPy.CSF.computeCSF` function takes a ccPointCloud in input and several optional parameters.
The function produce two clouds: "ground points" and "off-ground points", plus optionally, the cloth mesh.

A first example with default parameters:

.. include:: ../tests/test043.py
   :start-after: #---CSF01-begin
   :end-before:  #---CSF01-end
   :literal:
   :code: python

And a second example:

.. include:: ../tests/test043.py
   :start-after: #---CSF02-begin
   :end-before:  #---CSF02-end
   :literal:
   :code: python

The above code snippets are from :download:`test043.py <../tests/test043.py>`.

Classify a point cloud with Canupo plugin and a trained classifier
------------------------------------------------------------------

The concepts are presented in the 
`CloudCompare wiki - CANUPO (plugin) <https://www.cloudcompare.org/doc/wiki/index.php/CANUPO_(plugin)>`_.

You have to import the The :py:mod:`~.cloudComPy.Canupo` plugin.

For the test, an example of cloud and a classifier are provided:

.. include:: ../tests/test046.py
   :start-after: #---Canupo001-begin
   :end-before:  #---Canupo001-end
   :literal:
   :code: python

The :py:func:`~.cloudComPy.Canupo.Classify` function takes in input a ccPointCloud, a classifier file and several optional parameters.
The function produce two scalar fields in the cloud: 'CANUPO.class' and 'CANUPO.confidence'.

.. include:: ../tests/test046.py
   :start-after: #---Canupo002-begin
   :end-before:  #---Canupo002-end
   :literal:
   :code: python

The above code snippets are from :download:`test046.py <../tests/test046.py>`.

Compute distance between a cloud and a surface of revolution, with SRA plugin
-----------------------------------------------------------------------------

The concepts are presented in the 
`CloudCompare wiki - SRA (plugin) <https://www.cloudcompare.org/doc/wiki/index.php/Surface_of_Revolution_Analysis_(plugin)>`_.

You have to import the The :py:mod:`~.cloudComPy.SRA` plugin.

The surface of revolution is defined by a profile.
See `CloudCompare wiki - SRA (plugin) <https://www.cloudcompare.org/doc/wiki/index.php/Surface_of_Revolution_Analysis_(plugin)>`_ 
for a descritpion of the text file format for the profile.
This profile can be loaded with the :py:func:`~.cloudComPy.SRA.loadProfile` function.

The :py:meth:`~.cloudComPy.SRA.qSRA.doComputeRadialDists` method of :py:class:`~.cloudComPy.SRA.qSRA` class
computes the radial distances between the cloud and the surface of revolution
and store the result as a scalar field named 'Radial distance'.

An example:

.. include:: ../tests/test045.py
   :start-after: #---SRA01-begin
   :end-before:  #---SRA01-end
   :literal:
   :code: python

Distance map can be exported as a cloud or a mesh with the :py:func:`~.cloudComPy.SRA.exportMapAsCloud` 
and :py:func:`~.cloudComPy.SRA.exportMapAsMesh` functions:

.. include:: ../tests/test045.py
   :start-after: #---SRA02-begin
   :end-before:  #---SRA02-end
   :literal:
   :code: python

.. include:: ../tests/test045.py
   :start-after: #---SRA03-begin
   :end-before:  #---SRA03-end
   :literal:
   :code: python

The above code snippets are from :download:`test045.py <../tests/test045.py>`.

Surface Mesh reconstruction, with Poisson Reconstruction plugin
---------------------------------------------------------------

The concepts are presented in the 
`CloudCompare wiki - Poisson Surface Reconstruction (plugin) <https://www.cloudcompare.org/doc/wiki/index.php/Poisson_Surface_Reconstruction_(plugin)>`_.

You have to import the The :py:mod:`~.cloudComPy.PoissonRecon` plugin.

To use this plugin, you have to provide a cloud with normals.

The :py:meth:`~.cloudComPy.PoissonRecon.PR.PoissonReconstruction` static method of :py:class:`~.cloudComPy.PoissonRecon.PR` class
computes the triangular mesh on the given cloud.

.. include:: ../tests/test053.py
   :start-after: #---PoissonRecon02-begin
   :end-before:  #---PoissonRecon02-end
   :literal:
   :code: python

The above code snippet is from :download:`test053.py <../tests/test053.py>`.

Sclices and contours
--------------------

The concepts are presented in the 
`CloudCompare wiki - Cross Section <https://www.cloudcompare.org/doc/wiki/index.php/Cross_Section>`_.

Given a bounding box used as a cut tool, and a list of entities to sclice (clouds and meshes),
the :py:func:`~.cloudComPy.ExtractSlicesAndContours` function allows to:

 - segment the entities (slice)
 - extract the envelope of all the points inside the slice
 - extract one or several contours of the points inside each slice
 - repeat the segmentation or extraction processes above along one or several dimensions
   (to extract multiple 'slices' for instances)

For the example, we create a list of objects to slice, one cloud and two meshes.

.. include:: ../tests/test036.py
   :start-after: #---slicemat-begin
   :end-before:  #---slicemat-end
   :literal:
   :code: python

We define a bounding box as a cutting tool:

.. include:: ../tests/test036.py
   :start-after: #---slicebbox-begin
   :end-before:  #---slicebbox-end
   :literal:
   :code: python

First, generate a single slice: the function returns a list containing a single slice

.. include:: ../tests/test036.py
   :start-after: #---slice00-begin
   :end-before:  #---slice00-end
   :literal:
   :code: python

then, repeat the slices along Z with a gap: the function returns a list containing several slices

.. include:: ../tests/test036.py
   :start-after: #---slice01-begin
   :end-before:  #---slice01-end
   :literal:
   :code: python

next, the same as above, plus envelopes:
the function returns a tuple(list of slices, list of envelopes)

.. include:: ../tests/test036.py
   :start-after: #---slice02-begin
   :end-before:  #---slice02-end
   :literal:
   :code: python

after, the same as above, with a rotation of the bounding box:
the function returns a tuple(list of slices, list of envelopes)

.. include:: ../tests/test036.py
   :start-after: #---slice03-begin
   :end-before:  #---slice03-end
   :literal:
   :code: python

finally, the same as above, plus contours: 
the function returns a tuple(list of slices, list of envelopes, list of contours)

.. include:: ../tests/test036.py
   :start-after: #---slice04-begin
   :end-before:  #---slice04-end
   :literal:
   :code: python

The above code snippets are from :download:`test036.py <../tests/test036.py>`.

Extract Sections (generate cloud slices and profiles)
-----------------------------------------------------

The concepts are presented in the 
`CloudCompare wiki - Extract Sections <https://www.cloudcompare.org/doc/wiki/index.php/Extract_Sections#Generating_cloud_slices_and_profiles>`_.

This set of tools allows the user to import poylines on top of a point cloud so as to extract sections and profiles. 

For the test, we use a cloud representing the altimetry around a river, and a 2D polyline giving the river axis.
The :py:meth:`~.cloudComPy.ccPolyline.generateOrthoSections` method automatically generate orthogonal sections with a given polyline at regular intervals. 

.. include:: ../tests/test052.py
   :start-after: #---extractSections01-begin
   :end-before:  #---extractSections01-end
   :literal:
   :code: python

With clouds and polylines, the :py:func:`~.cloudComPy.extractPointsAlongSections` function allows to extract either cloud slices along the polylines,
or to define vertical profiles (polylines) built on the clouds.
The :py:func:`~.cloudComPy.unfoldPointsAlongPolylines` function unfolds a polyline and the clouds points near the polyline (with a given thickness).
The result is a new cloud.

.. include:: ../tests/test052.py
   :start-after: #---extractSections02-begin
   :end-before:  #---extractSections02-end
   :literal:
   :code: python

The above code snippets are from :download:`test052.py <../tests/test052.py>`.

Extract Connected Components
----------------------------

The tool used here is described in
`CloudCompare wiki - Label Connected Components <https://www.cloudcompare.org/doc/wiki/index.php/Label_Connected_Components>`_.

This tool segments the selected cloud(s) in smaller parts separated by a minimum distance. 
Each part is a connected component (i.e. a set of 'connected' points). 

For the test, we create a cloud with clear gaps, using a sclice operation:

.. include:: ../tests/test037.py
   :start-after: #---extractCC01-begin
   :end-before:  #---extractCC01-end
   :literal:
   :code: python

The :py:func:`~.cloudComPy.ExtractConnectedComponents` function use the octree level 
to define the size of the gap between the components. 

.. include:: ../tests/test037.py
   :start-after: #---extractCC02-begin
   :end-before:  #---extractCC02-end
   :literal:
   :code: python

The function returns a tuple (number of clouds processed, list of components, list of residual components).
The "residual component" corresponds to all the remaining nodes of an input cloud: see for instance:

.. include:: ../tests/test048.py
   :start-after: #---extractCC01-begin
   :end-before:  #---extractCC01-end
   :literal:
   :code: python

The above code snippets are from :download:`test037.py <../tests/test037.py>` and  :download:`test048.py <../tests/test048.py>`.

Merge several entities (clouds or meshes)
-----------------------------------------

The :py:func:`~.cloudComPy.MergeEntities` function allows to merge several clouds, or several meshes.

Merging a list of clouds:

.. include:: ../tests/test048.py
   :start-after: #---extractCC01-begin
   :end-before:  #---extractCC01-end
   :literal:
   :code: python

Merging a list of meshes:


.. include:: ../tests/test048.py
   :start-after: #---merge02-begin
   :end-before:  #---merge02-end
   :literal:
   :code: python


The above code snippets are from :download:`test048.py <../tests/test048.py>`.

Avoid memory leaks in an iterative process
------------------------------------------

In a pure Python script, the garbage collector takes care of the objects that are no longer referenced.
With CloudComPy, there is no shared reference count between a CloudComPy Python object and the CloudCompare C++ object wrapped.
In other words, if a Python object is deleted, the wrapped C++ object remains. 
Conversely, if a CloudCompare C++ object is deleted, 
the corresponding Python object is not automatically deleted, but is no longer usable.

To avoid memory leaks, you should delete explicitely the objects that are no longer used with 
the :py:func:`cloudComPy.deleteEntity` function.

For instance, we create some data:

.. include:: ../tests/test042.py
   :start-after: #---extractCC01-begin
   :end-before:  #---extractCC01-end
   :literal:
   :code: python

Then, we iterate on :py:func:`~.cloudComPy.ExtractConnectedComponents`

.. include:: ../tests/test042.py
   :start-after: #---extractCC02-begin
   :end-before:  #---extractCC02-end
   :literal:
   :code: python

I we do not delete explicitely the extracted components, they are kept forever in CloudCompare.
To avoid the memory leak:

.. include:: ../tests/test042.py
   :start-after: #---extractCC03-begin
   :end-before:  #---extractCC03-end
   :literal:
   :code: python

The above code snippets are from :download:`test042.py <../tests/test042.py>`.

Render a 3D scene to an image file
----------------------------------

In order to create a 3D view of one or more entities and write a 2D file image (png, jpeg,...), 
the entities can be added to the scene with the :py:func:`cloudComPy.addToRenderScene` function.

The :py:func:`cloudComPy.render` function renders and writes the image.

.. include:: ../tests/test051.py
   :start-after: #---render001-begin
   :end-before:  #---render001-end
   :literal:
   :code: python

The :py:func:`cloudComPy.removeFromRenderScene` function allows to remove an entity from the scene, but it deletes the C++ object.
Thus, the Python object becomes invalid.

.. include:: ../tests/test051.py
   :start-after: #---render002-begin
   :end-before:  #---render002-end
   :literal:
   :code: python

Several functions allow to define a standard point of view for the scene:

 - :py:func:`cloudComPy.setFrontView`
 - :py:func:`cloudComPy.setBottomView`
 - :py:func:`cloudComPy.setTopView`
 - :py:func:`cloudComPy.setBackView`
 - :py:func:`cloudComPy.setLeftView`
 - :py:func:`cloudComPy.setRightView`
 - :py:func:`cloudComPy.setIsoView1`
 - :py:func:`cloudComPy.setIsoView2`

Different modes of perspective are available:

 - :py:func:`cloudComPy.setOrthoView`
 - :py:func:`cloudComPy.setCenteredPerspectiveView`
 - :py:func:`cloudComPy.setViewerPerspectiveView`
 
 To define a zoom on the whole scene or on selected entities, use the :py:func:`cloudComPy.setGlobalZoom` and :py:func:`cloudComPy.zoomOnSelectedEntity` functions.

The :py:func:`cloudComPy.setCustomView` and py:func:`cloudComPy.setCameraPos` functions are used to define a custom point of view.

Several functions and methods can be used to set the background color, display the scalar bar...

 - :py:func:`cloudComPy.setBackgroundColor`
 - :py:func:`cloudComPy.setTextDefaultCol`
 - :py:func:`cloudComPy.setColorScaleShowHistogram`
 - :py:meth:`~.cloudComPy.ccPointCloud.showSFColorsScale`

.. include:: ../tests/test051.py
   :start-after: #---render003-begin
   :end-before:  #---render003-end
   :literal:
   :code: python

By default, the :py:func:`cloudComPy.render` function returns immediately after rendering the image.
It is possible to select an interactive mode by setting the last optional parameter (bool isInteractive) to True.
In this mode, the Python script is suspended, it is possible to adjust the 3D view.When the vue is correct,
you have to select the action "resume Python script" in the "options" menu to save the render and resume the Python script execution.

The above code snippets are from :download:`test051.py <../tests/test051.py>`.



