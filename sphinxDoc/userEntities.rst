
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

Several methods based on neighbourood computations require a radius parameter,
often estimated with an average neighbour points.
::

    radius = cc.GetPointCloudRadius(clouds=[cloud], nodes=10)

The octree is used in a lot of methods, to speed up access to the neighbourhood of the points, 
and it is often computed automatically.
::

    octree = cloud.getOctree()
    if octree is None:
        octree = cloud.computeOctree()

Access to scalarFields: ScalarFields are accessible by the :py:meth:`~.cloudComPy.ccPointCloud.getScalarField`
method from their index (integer).
It is sometimes more convenient to find them by their name from the scalarFields dictionary,
which gives the index from the name.
::

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

The cloud can have a color field that can be seen as a special type of scalarField.
There are methods to convert scalarField to colors and vice-versa.
::

    if not cloud.hasColors():
        cloud.setCurrentScalarField(2)
        cloud.convertCurrentScalarFieldToColors()

Normals
::

    if not cloud.hasNormals():
        cc.computeNormals([cloud])

cloud transformations
---------------------

Basic transformations :py:meth:`~.cloudComPy.ccPointCloud.translate` and :py:meth:`~.cloudComPy.ccPointCloud.scale`
allow to translate a cloud or rescale it, with separate factors along the 3 directions and an optional center (see 
`test001.py <../PythonAPI_test/test001.py>`_).

.. include:: ../tests/test001.py
   :start-after: #---scale-translate-begin
   :end-before:  #---scale-translate-end
   :literal:
   :code: python

More complex transformations, based on rotation, require a :py:class:`cloudComPy.ccGLMatrix` object to define the transformation,
which is applied to the cloud with the method :py:meth:`~.cloudComPy.ccPointCloud.applyRigidTransformation`.

The method :py:meth:`~.cloudComPy.ccGLMatrix.initFromParameters` allows to define the transformation with rotations 
(see `Euler angles <https://en.wikipedia.org/wiki/Euler_angles>`_).
The following code is extract from `test026.py <../PythonAPI_test/test026.py>`_.

.. include:: ../tests/test026.py
   :start-after: #---transformations-begin
   :end-before:  #---transformations-end
   :literal:
   :code: python

cloud normals
-------------

Once computed, normals can be exported to scalar fields with :py:meth:`~.cloudComPy.ccPointCloud.exportNormalToSF`:

.. include:: ../tests/test014.py
   :start-after: #---normals01-begin
   :end-before:  #---normals01-end
   :literal:
   :code: python

Normals can also be converted to color with :py:meth:`~.cloudComPy.ccPointCloud.convertNormalToRGB`:

.. include:: ../tests/test014.py
   :start-after: #---normals02-begin
   :end-before:  #---normals02-end
   :literal:
   :code: python

To compute strike and dip fields from normals, use :py:meth:`~.cloudComPy.ccPointCloud.convertNormalToDipDirSFs` 
(see definition for `Strike and dip <https://en.wikipedia.org/wiki/Strike_and_dip>`_):

.. include:: ../tests/test014.py
   :start-after: #---normals03-begin
   :end-before:  #---normals03-end
   :literal:
   :code: python

Normals can be inverted with :py:meth:`cloudComPy.invertNormals`,
reoriented with a `Fast Marching method <https://en.wikipedia.org/wiki/Fast_marching_method>`_ 
or a `Minimum Spanning Tree method <https://en.wikipedia.org/wiki/Fast_marching_method>`_, 
with :py:meth:`~.cloudComPy.ccPointCloud.orientNormalsWithFM`
and :py:meth:`~.cloudComPy.ccPointCloud.orientNormalsWithMST`.

.. include:: ../tests/test014.py
   :start-after: #---normals04-begin
   :end-before:  #---normals04-end
   :literal:
   :code: python

The previous code is extract from `test014.py <../PythonAPI_test/test014.py>`_.

cloud colors
------------

The :py:class:`cloudComPy.QColor` class wraps the `Qt5 QColor <https://doc.qt.io/qt-5/qcolor.html>`_ class 
and provides provides colors based on RGB, HSV or CMYK values. 
See `test028.py <../PythonAPI_test/test028.py>`_ for an example of use of :py:class:`cloudComPy.QColor` methods.

Several methods of :py:class:`cloudComPy.ccPointCloud` allow to colorize the cloud:

 - :py:meth:`~.cloudComPy.ccPointCloud.colorize`
 - :py:meth:`~.cloudComPy.ccPointCloud.fromRgb`
 - :py:meth:`~.cloudComPy.ccPointCloud.fromRgbF`
 - :py:meth:`~.cloudComPy.ccPointCloud.setColorGradient`
 - :py:meth:`~.cloudComPy.ccPointCloud.setColorGradientDefault`
 - :py:meth:`~.cloudComPy.ccPointCloud.setColorGradientBanded`
 - :py:meth:`~.cloudComPy.ccPointCloud.changeColorLevels`
 - :py:meth:`~.cloudComPy.ccPointCloud.convertRGBToGreyScale`

.. include:: ../tests/test029.py
   :start-after: #---colors01-begin
   :end-before:  #---colors01-end
   :literal:
   :code: python

ScalarFields can be used to define or modify colors, colors can be used to define scalarFields:

 - :py:meth:`~.cloudComPy.ccPointCloud.convertCurrentScalarFieldToColors`
 - :py:meth:`~.cloudComPy.ccPointCloud.enhanceRGBWithIntensitySF`
 - :py:meth:`~.cloudComPy.ccPointCloud.sfFromColor`

.. include:: ../tests/test029.py
   :start-after: #---colors02-begin
   :end-before:  #---colors02-end
   :literal:
   :code: python
 
If you don't need any more colors, it is possible to free some memory 
with :py:meth:`~.cloudComPy.ccPointCloud.unallocateColors`:
 
.. include:: ../tests/test029.py
   :start-after: #---colors03-begin
   :end-before:  #---colors03-end
   :literal:
   :code: python

With two clouds "sharing a same region", it is possible to define the color of one cloud
by interpolation from the other cloud, with :py:meth:`~.cloudComPy.ccPointCloud.interpolateColorsFrom`

.. include:: ../tests/test029.py
   :start-after: #---colors04-begin
   :end-before:  #---colors04-end
   :literal:
   :code: python

The previous code is extract from `test029.py <../PythonAPI_test/test029.py>`_.

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