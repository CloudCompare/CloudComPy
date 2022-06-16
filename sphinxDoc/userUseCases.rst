
==================================
Some use cases
==================================

Create a point cloud with numpy
-------------------------------
(test017 , 25, 26, 31)

Cut a cloud or a mesh with a polyline
-------------------------------------
crop2D (cloud)
crop2D mesh (test039)

Compute distances between clouds and meshes
-------------------------------------------
distances computeApproxCloud2MeshDistance computeCloud2MeshDistances...

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
