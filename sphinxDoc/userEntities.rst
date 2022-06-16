
==========================================================
Clouds, meshes, scalar fields: introspection, manipulation
==========================================================

cloud introspection
-------------------

name, size, scalarfields, gravity center, bounding box, mean variance, min max
radius, hasNormals getOctree, getScalarFieldDic

cloud transformations
---------------------

scale translate ccGLMatrix, applyRigidTransformation
transformations operations (test026)

cloud normals
-------------
convertNormalToDipDirSFs orientNormalsWithFM orientNormalsWithMST
invertNormals

cloud colors
------------

color manipulation test029

scalar fields
-------------
name, min, max, mean, variance

meshes introspection and manipulation
-------------------------------------

cloneMesh, subdivide, laplacianSmooth
access to triangle nodes
computePerVertexNormals
samplePoints

primitives
----------
ccBox ccCone ccCylinder ccPlane ccSphere ccTorus ccQuadric ccDish

polylines
---------
isClosed setClosed segmentCount size is2DMode set2DMode smoothChaikin

facets
------

octrees
-------