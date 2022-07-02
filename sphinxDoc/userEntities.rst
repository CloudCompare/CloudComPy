
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

Normals can be computed with :py:meth:`~.cloudComPy.ccPointCloud.computeNormals`:
::

    if not cloud.hasNormals():
        cc.computeNormals([cloud])

If you need to save clouds for reopening with CloudCompare GUI, with a predefined state of what is shown
(colors, normals, scalar fields), use the ``.bin`` format and define the state with the following functions:

 - :py:meth:`~.cloudComPy.ccPointCloud.showColors`
 - :py:meth:`~.cloudComPy.ccPointCloud.showNormals`
 - :py:meth:`~.cloudComPy.ccPointCloud.showSF`
 - :py:meth:`~.cloudComPy.ccPointCloud.colorsShown`
 - :py:meth:`~.cloudComPy.ccPointCloud.normalsShown`
 - :py:meth:`~.cloudComPy.ccPointCloud.sfShown`
 - :py:meth:`~.cloudComPy.ccPointCloud.setCurrentDisplayedScalarField`

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

cloud copy, destruction
-----------------------

TODO: clone, partialClone (test019) deleteEntity


.. _Cloud_Normals:

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

The above code snippets are from `test014.py <../PythonAPI_test/test014.py>`_.

.. _Cloud_Colors:

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
with :py:meth:`~.cloudComPy.ccPointCloud.unallocateColors`
(**WARNING** be sure to have no more Python objects referencing the deleted object):
 
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

The above code snippets are from `test029.py <../PythonAPI_test/test029.py>`_.

scalar fields
-------------

when a scalar field is modified with Numpy (see :ref:`ScalarField_Numpy`),
you must reinitialise the min and max value of the scalar field with :py:meth:`~.cloudComPy.ScalarField.computeMinAndMax`.

.. include:: ../tests/test002.py
   :start-after: #---sfNumpy01-begin
   :end-before:  #---sfNumpy01-end
   :literal:
   :code: python

General information and statistics are available with:

 - :py:meth:`~.cloudComPy.ScalarField.getName`
 - :py:meth:`~.cloudComPy.ScalarField.getMin`
 - :py:meth:`~.cloudComPy.ScalarField.getMax`
 - :py:meth:`~.cloudComPy.ScalarField.computeMeanAndVariance`
 - :py:meth:`~.cloudComPy.ScalarField.getValue`

.. include:: ../tests/test002.py
   :start-after: #---sfNumpy02-begin
   :end-before:  #---sfNumpy02-end
   :literal:
   :code: python
 
To change the scalar field name, set a value on a point, fill the scalar field with a uniform value, use:

 - :py:meth:`~.cloudComPy.ScalarField.setName`
 - :py:meth:`~.cloudComPy.ScalarField.setValue`
 - :py:meth:`~.cloudComPy.ScalarField.fill`

.. include:: ../tests/test002.py
   :start-after: #---sfNumpy03-begin
   :end-before:  #---sfNumpy03-end
   :literal:
   :code: python

The above code snippets are from `test002.py <../PythonAPI_test/test002.py>`_.

Scalar fields can be built from normals or colors, and can be used to define colors:
see :ref:`Cloud_Normals` and :ref:`Cloud_Colors`.

To get a scalar field gradient, use :py:meth:`cloudComPy.ccPointCloud.computeScalarFieldGradient`
with an appropriate radius:

.. include:: ../tests/test003.py
   :start-after: #---cloudsf01-begin
   :end-before:  #---cloudsf01-end
   :literal:
   :code: python

The previous code is extract from `test003.py <../PythonAPI_test/test003.py>`_.

Some methods of :py:class:`cloudComPy.ccPointCloud`, directly wrapped from CloudCompare GUI, 
are used to select a current scalar field.
This is required by some other methods, working with the current selected scalar field:

 - :py:meth:`~.cloudComPy.ccPointCloud.setCurrentDisplayedScalarField`
 - :py:meth:`~.cloudComPy.ccPointCloud.setCurrentInScalarField`
 - :py:meth:`~.cloudComPy.ccPointCloud.setCurrentOutScalarField`
 - :py:meth:`~.cloudComPy.ccPointCloud.setCurrentScalarField`
 - :py:meth:`~.cloudComPy.ccPointCloud.getCurrentDisplayedScalarField`
 - :py:meth:`~.cloudComPy.ccPointCloud.getCurrentDisplayedScalarFieldIndex`
 - :py:meth:`~.cloudComPy.ccPointCloud.getCurrentInScalarField`
 - :py:meth:`~.cloudComPy.ccPointCloud.getCurrentOutScalarField`

For instance, the :py:meth:`~.cloudComPy.ccPointCloud.filterPointsByScalarValue` method
relies on :py:meth:`~.cloudComPy.ccPointCloud.setCurrentDisplayedScalarField` to work.
The points of the input cloud are filtered by keeping (or excluding) the points 
for which the value of the scalar field belongs to a (min,max) interval.

.. include:: ../tests/test019.py
   :start-after: #---filterPoints01-begin
   :end-before:  #---filterPoints01-end
   :literal:
   :code: python

The previous code is extract from `test019.py <../PythonAPI_test/test019.py>`_.

If you need to free some memory and do not need any more some scalar fields,
you can use the following methods of :py:class:`cloudComPy.ccPointCloud`
(**WARNING** be sure to have no more Python objects referencing the deleted object):

 - :py:meth:`~.cloudComPy.ccPointCloud.deleteAllScalarFields`
 - :py:meth:`~.cloudComPy.ccPointCloud.deleteScalarField`

meshes introspection and manipulation
-------------------------------------

In CloudCompare and CloudComPy 3D meshes are triangular meshes (:py:class:`~.cloudComPy.ccMesh`), 
built on a set of vertices, which constitute an associated cloud.
To get this cloud, use :py:meth:`~.cloudComPy.ccMesh.getAssociatedCloud` which gives a :py:class:`~.cloudComPy.ccPointCloud`.

.. include:: ../tests/test020.py
   :start-after: #---meshcloud01-begin
   :end-before:  #---meshcloud01-end
   :literal:
   :code: python

When a cloud represents a kind of 2.5D elevated surface, a mesh can be built using the nodes of a the cloud
with :py:meth:`cloudComPy.ccMesh.triangulate`.

.. include:: ../tests/test011.py
   :start-after: #---triangulate01-begin
   :end-before:  #---triangulate01-end
   :literal:
   :code: python

A mesh have a name, distinct of the associated cloud name, a size which is the number of triangles.

 - :py:meth:`~.cloudComPy.ccMesh.getName`
 - :py:meth:`~.cloudComPy.ccMesh.setName`
 - :py:meth:`~.cloudComPy.ccMesh.size`
 
A mesh can have normals, computed either on vertices or on triangles:

.. include:: ../tests/test014.py
   :start-after: #---meshNormals01-begin
   :end-before:  #---meshNormals01-end
   :literal:
   :code: python

If you need to iterate through the triangles and their vertices, use :py:meth:`~.cloudComPy.ccMesh.getTriangleVertIndexes`:

.. include:: ../tests/test011.py
   :start-after: #---triangleVertices01-begin
   :end-before:  #---triangleVertices01-end
   :literal:
   :code: python

If you want to save meshes to reopen them with the CloudCompare GUI, with a predefined state of what is displayed
(colors, normals, scalar fields), use the ``.bin`` format and define the state with the following functions:

 - :py:meth:`~.cloudComPy.ccMesh.showColors`
 - :py:meth:`~.cloudComPy.ccMesh.showNormals`
 - :py:meth:`~.cloudComPy.ccMesh.showSF`
 - :py:meth:`~.cloudComPy.ccMesh.colorsShown`
 - :py:meth:`~.cloudComPy.ccMesh.normalsShown`
 - :py:meth:`~.cloudComPy.ccMesh.sfShown`

A mesh can be refined using :py:meth:`~.cloudComPy.ccMesh.subdivide` 
to force all triangles to have an area smaller than a given maximum. The result is a new mesh.

.. include:: ../tests/test011.py
   :start-after: #---subdivide01-begin
   :end-before:  #---subdivide01-end
   :literal:
   :code: python

The mesh can be "smoothed" with :py:meth:`~.cloudComPy.ccMesh.laplacianSmooth`
by moving the vertices slightly over several iterations:

.. include:: ../tests/test011.py
   :start-after: #---laplacianSmooth01-begin
   :end-before:  #---laplacianSmooth01-end
   :literal:
   :code: python

The above code snippets are from `test011.py <../PythonAPI_test/test011.py>`_.

A mesh can be used to create a cloud with `~.cloudComPy.ccMesh.samplePoints`,
with a target either of number of points or of density of points:

.. include:: ../tests/test015.py
   :start-after: #---samplePoints01-begin
   :end-before:  #---samplePoints01-end
   :literal:
   :code: python

The above code snippet is from `test051.py <../PythonAPI_test/test015.py>`_.

clone, deleteEntity cloneMesh, 

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