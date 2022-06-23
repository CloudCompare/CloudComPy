
==========================================================
Clouds, meshes, scalar fields: introspection, manipulation
==========================================================

cloud introspection
-------------------

3D Clouds contains a set of indexed points and have several optional associated features:
normals, several scalarFields, color, an octree structure...
::

    cloud = cc.loadPointCloud("CloudComPy/Data/dataSample_5.0.xyz")

    name = cloud.getName()        # 'dataSample_5 - Cloud'
    cloud.setName("sampleCloud")

    nbPoints = cloud.size()       # 1000000
    pt = cloud.getPoint(12345)    # (-4.880000114440918, -1.5499999523162842, 0.08818499743938446)

    bb = cloud.getOwnBB()         # bounding box
    minCorner = bb.minCorner()    # (-5.0, -5.0, -1.0861680507659912)
    maxCorner = bb.maxCorner()    # (4.989999771118164, 4.989999771118164, 5.0)

    g = cloud.computeGravityCenter()

    # --- octree is used in a lot of methods, and it is often computed automatically

    octree = cloud.getOctree()
    if octree is None:
        octree = cloud.computeOctree()

    # --- access to scalarFields

    nbSF = cloud.getNumberOfScalarFields()
    if cloud.hasScalarFields()
        dic = cloud.getScalarFieldDic()

    cloud.exportCoordToSF(True, True, True)
    dic = cloud.getScalarFieldDic()  # {'Coord. X': 0, 'Coord. Y': 1, 'Coord. Z': 2}

    SFX = cloud.getScalarField(dic['Coord. X'])
    sfmin = SFX.getMin()             # -5.0
    sfmax = SFX.getMax()             # 4.989999771118164
    mean, variance = SFX.computeMeanAndVariance()
    SFX.getValue(12345)              # -4.880000114440918

    # --- colors

    if not cloud.hasColors():
        cloud.setCurrentScalarField(2)
        cloud.convertCurrentScalarFieldToColors()

    # --- normals

    if not cloud.hasNormals():
        cc.computeNormals([cloud])


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