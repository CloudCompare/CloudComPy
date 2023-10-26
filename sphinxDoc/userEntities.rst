
==========================================================
Clouds, meshes, scalar fields: introspection, manipulation
==========================================================

Clouds and scalar fields
------------------------

cloud introspection
~~~~~~~~~~~~~~~~~~~

3D Clouds contains a set of indexed points and have several optional associated features:
normals, several scalarFields, color, an octree structure, sensors...
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

Normals can be computed with :py:func:`~.cloudComPy.computeNormals`:
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
~~~~~~~~~~~~~~~~~~~~~

Basic transformations :py:meth:`~.cloudComPy.ccPointCloud.translate` and :py:meth:`~.cloudComPy.ccPointCloud.scale`
allow to translate a cloud or rescale it, with separate factors along the 3 directions and an optional center (see 
:download:`test001.py <../tests/test001.py>`).

.. include:: ../tests/test001.py
   :start-after: #---scale-translate-begin
   :end-before:  #---scale-translate-end
   :literal:
   :code: python

More complex transformations, based on rotation, require a :py:class:`cloudComPy.ccGLMatrix` object to define the transformation,
which is applied to the cloud with the method :py:meth:`~.cloudComPy.ccPointCloud.applyRigidTransformation`.

The method :py:meth:`~.cloudComPy.ccGLMatrix.initFromParameters` allows to define the transformation with rotations 
(see `Euler angles <https://en.wikipedia.org/wiki/Euler_angles>`_).
The following code is extract from :download:`test026.py <../tests/test026.py>`.

Define a transformation from a rotation angle and axis, plus a translation,
with :py:meth:`~.cloudComPy.ccGLMatrix.initFromParameters`.

.. include:: ../tests/test026.py
   :start-after: #---transformations01-begin
   :end-before:  #---transformations01-end
   :literal:
   :code: python

Sometimes, you need to store or retreive the transformation matrix data (4x4 matrix stored in major column order),
use :py:meth:`~.cloudComPy.ccGLMatrix.toString`, :py:meth:`~.cloudComPy.ccGLMatrix.fromString` 
and :py:meth:`~.cloudComPy.ccGLMatrix.data`.

.. include:: ../tests/test026.py
   :start-after: #---transformations02-begin
   :end-before:  #---transformations02-end
   :literal:
   :code: python

If you need to keep precision while iterating on transformations, for instance, it is better to work
with double precision transformations :py:class:`cloudComPy.ccGLMatrixd`

.. include:: ../tests/test026.py
   :start-after: #---transformations03-begin
   :end-before:  #---transformations03-end
   :literal:
   :code: python

For these transformation examples, we create an ellipsoid cloud:

.. include:: ../tests/test026.py
   :start-after: #---transformations04-begin
   :end-before:  #---transformations04-end
   :literal:
   :code: python

And we rotate it, with a transformation and the method :py:meth:`~.cloudComPy.ccPointCloud.applyRigidTransformation`

.. include:: ../tests/test026.py
   :start-after: #---transformations05-begin
   :end-before:  #---transformations05-end
   :literal:
   :code: python

You can get the rotation parameters of a transformation in the form of a rotation angle and a vector
with :py:meth:`~.cloudComPy.ccGLMatrix.getParameters1`

.. include:: ../tests/test026.py
   :start-after: #---transformations06-begin
   :end-before:  #---transformations06-end
   :literal:
   :code: python

You can also get the rotation parameters of a transformation in the form of the 3 angles phi, psi, theta
with :py:meth:`~.cloudComPy.ccGLMatrix.getParameters2`

.. include:: ../tests/test026.py
   :start-after: #---transformations07-begin
   :end-before:  #---transformations07-end
   :literal:
   :code: python

The product of two transformations gives a transformation

.. include:: ../tests/test026.py
   :start-after: #---transformations08-begin
   :end-before:  #---transformations08-end
   :literal:
   :code: python

To get the inverse of a transformation, use :py:meth:`~.cloudComPy.ccGLMatrix.inverse`

.. include:: ../tests/test026.py
   :start-after: #---transformations09-begin
   :end-before:  #---transformations09-end
   :literal:
   :code: python

Let's play with the 3 rotations phi, psi, theta:

.. include:: ../tests/test026.py
   :start-after: #---transformations10-begin
   :end-before:  #---transformations10-end
   :literal:
   :code: python

The transposed version of a transformation gives an inverse rotation:

.. include:: ../tests/test026.py
   :start-after: #---transformations11-begin
   :end-before:  #---transformations11-end
   :literal:
   :code: python

Another way to check:

.. include:: ../tests/test026.py
   :start-after: #---transformations12-begin
   :end-before:  #---transformations12-end
   :literal:
   :code: python

An interpolation between transformations can be computed with :py:meth:`~.cloudComPy.ccGLMatrix.Interpolate`:

.. include:: ../tests/test026.py
   :start-after: #---transformations13-begin
   :end-before:  #---transformations13-end
   :literal:
   :code: python

There is also the method :py:meth:`~.cloudComPy.ccGLMatrix.FromToRotation`
to get a transformation matrix that rotates a vector to another:

.. include:: ../tests/test026.py
   :start-after: #---transformations14-begin
   :end-before:  #---transformations14-end
   :literal:
   :code: python

You can generate a ‘viewing’ matrix from a looking vector and a ‘up’ direction
with :py:meth:`~.cloudComPy.ccGLMatrix.FromViewDirAndUpDir`

.. include:: ../tests/test026.py
   :start-after: #---transformations15-begin
   :end-before:  #---transformations15-end
   :literal:
   :code: python

cloud copy, destruction
~~~~~~~~~~~~~~~~~~~~~~~

A cloud can be cloned with all its features except from the octree with :py:meth:`~.cloudComPy.ccPointCloud.cloneThis`:

.. include:: ../tests/test026.py
   :start-after: #---cloneThis01-begin
   :end-before:  #---cloneThis01-end
   :literal:
   :code: python

To free memory, a cloud can be deleted with :py:meth:`cloudComPy.deleteEntity` 
(**WARNING** be sure to have no more Python objects referencing the deleted object):

.. include:: ../tests/test026.py
   :start-after: #---deleteEntity01-begin
   :end-before:  #---deleteEntity01-end
   :literal:
   :code: python

The above code snippets are from :download:`test026.py <../tests/test026.py>`.

Some methods give a selection from a cloud as a :py:class:`cloudComPy.ReferenceCloud` which is a light structure 
referencing the selected nodes in the original cloud. To convert this selection in a new cloud, use the method
:py:meth:`~.cloudComPy.ccPointCloud.partialClone`:

.. include:: ../tests/test019.py
   :start-after: #---partialClone01-begin
   :end-before:  #---partialClone01-end
   :literal:
   :code: python

The above code snippet is from :download:`test019.py <../tests/test019.py>`.

.. _Cloud_Normals:

cloud normals
~~~~~~~~~~~~~

Once computed (see :py:func:`~.cloudComPy.computeNormals`),
normals can be exported to scalar fields with :py:meth:`~.cloudComPy.ccPointCloud.exportNormalToSF`:

.. include:: ../tests/test014.py
   :start-after: #---normals01-begin
   :end-before:  #---normals01-end
   :literal:
   :code: python

Normals can also be converted to color with :py:meth:`~.cloudComPy.ccPointCloud.convertNormalToRGB`:

The colors obtained are an HSV color field, with H = dip direction, S = dip and V = 1.

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

The above code snippets are from :download:`test014.py <../tests/test014.py>`.

.. _Cloud_Colors:

cloud colors
~~~~~~~~~~~~

The :py:class:`cloudComPy.QColor` class wraps the `Qt5 QColor <https://doc.qt.io/qt-5/qcolor.html>`_ class 
and provides provides colors based on RGB, HSV or CMYK values. 
See :download:`test028.py <../tests/test028.py>` for an example of use of :py:class:`cloudComPy.QColor` methods.

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

The above code snippets are from :download:`test029.py <../tests/test029.py>`.

scalar fields
~~~~~~~~~~~~~

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

The above code snippets are from :download:`test002.py <../tests/test002.py>`.

Some scalar fields may be shifted to prevent a loss of accuracy.
This is the case, for instance, for GPS time in some lidar files in las format.
 
To get the global shift, use:

 - :py:meth:`~.cloudComPy.ccScalarField.getGlobalShift`

.. include:: ../tests/test020.py
   :start-after: #---LASTimeShift002-begin
   :end-before:  #---LASTimeShift002-end
   :literal:
   :code: python

The above code snippets is from :download:`test020.py <../tests/test020.py>`.

To change the scalar field name, set a value on a point, fill the scalar field with a uniform value, use:

 - :py:meth:`~.cloudComPy.ScalarField.setName`
 - :py:meth:`~.cloudComPy.ScalarField.setValue`
 - :py:meth:`~.cloudComPy.ScalarField.fill`

.. include:: ../tests/test002.py
   :start-after: #---sfNumpy03-begin
   :end-before:  #---sfNumpy03-end
   :literal:
   :code: python

The above code snippet is from :download:`test002.py <../tests/test002.py>`.

Scalar fields can be built from normals or colors, and can be used to define colors:
see :ref:`Cloud_Normals` and :ref:`Cloud_Colors`.

To get a scalar field gradient, use :py:meth:`cloudComPy.ccPointCloud.computeScalarFieldGradient`
with an appropriate radius:

.. include:: ../tests/test003.py
   :start-after: #---cloudsf01-begin
   :end-before:  #---cloudsf01-end
   :literal:
   :code: python

The previous code is extract from :download:`test003.py <../tests/test003.py>`.

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

The previous code is extract from :download:`test019.py <../tests/test019.py>`.

If you need to free some memory and do not need any more some scalar fields,
you can use the following methods of :py:class:`cloudComPy.ccPointCloud`
(**WARNING** be sure to have no more Python objects referencing the deleted object):

 - :py:meth:`~.cloudComPy.ccPointCloud.deleteAllScalarFields`
 - :py:meth:`~.cloudComPy.ccPointCloud.deleteScalarField`

sensors
~~~~~~~
Acquisition data files sometimes contain information on the sensors used to capture data.

A short introduction to sensors in CloudCompare is given in
`CloudCompare wiki - Sensors <https://www.cloudcompare.org/doc/wiki/index.php/Entities#Sensors>`_.

CloudComPy gives access to basic information on sensors: 

- type of sensor,
- position and orientation,
- graphic scale,
- uncertainty.

For the test, we need a data file to download 
`here <http://sourceforge.net/projects/e57-3d-imgfmt/files/E57Example-data/manitouNoInvalidPoints.e57/download>`_.

.. include:: ../tests/test041.py
   :start-after: #---sensor001-begin
   :end-before:  #---sensor001-end
   :literal:
   :code: python

There are several point clouds in the example, with one sensor per cloud.
The :py:meth:`~.cloudComPy.ccPointCloud.getSensors` method gives a list of sensors associated to the cloud.
Generic sensor :py:class:`cloudComPy.ccSensor` provides the following methods:

- :py:meth:`~.cloudComPy.ccSensor.getType`
- :py:meth:`~.cloudComPy.ccHObject.getClassID`
- :py:meth:`~.cloudComPy.ccSensor.getGraphicScale`
- :py:meth:`~.cloudComPy.ccSensor.getRigidTransformation`

The Ground Based Lidar (GBL) sensor :py:class:`cloudComPy.ccGBLSensor` provides also:

- :py:meth:`~.cloudComPy.ccGBLSensor.getUncertainty`

.. include:: ../tests/test041.py
   :start-after: #---sensor002-begin
   :end-before:  #---sensor002-end
   :literal:
   :code: python

Sensors are also used to compute scattering angles with :py:meth:`~.cloudComPy.ccSensor.ComputeScatteringAngles`.

.. include:: ../tests/test041.py
   :start-after: #---sensor004-begin
   :end-before:  #---sensor004-end
   :literal:
   :code: python


The above code snippets are from :download:`test041.py <../tests/test041.py>`.

meshes
------

create a mesh from a cloud
~~~~~~~~~~~~~~~~~~~~~~~~~~

When a cloud represents a kind of 2.5D elevated surface, a mesh can be built using the nodes of a the cloud
with :py:meth:`cloudComPy.ccMesh.triangulate`.

.. include:: ../tests/test011.py
   :start-after: #---triangulate01-begin
   :end-before:  #---triangulate01-end
   :literal:
   :code: python

The above code snippet is from :download:`test011.py <../tests/test011.py>`.

meshes introspection
~~~~~~~~~~~~~~~~~~~~

In CloudCompare and CloudComPy 3D meshes are triangular meshes (:py:class:`~.cloudComPy.ccMesh`), 
built on a set of vertices, which constitute an associated cloud.
To get this cloud, use :py:meth:`~.cloudComPy.ccMesh.getAssociatedCloud` which gives a :py:class:`~.cloudComPy.ccPointCloud`.

.. include:: ../tests/test020.py
   :start-after: #---meshcloud01-begin
   :end-before:  #---meshcloud01-end
   :literal:
   :code: python

The above code snippet is from :download:`test020.py <../tests/test020.py>`.

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

The above code snippet is from :download:`test014.py <../tests/test014.py>`.

Normals on triangles can be deleted with :py:meth:`~.cloudComPy.ccMesh.clearTriNormals`.

If you need to iterate through the triangles and their vertices, use :py:meth:`~.cloudComPy.ccMesh.getTriangleVertIndexes`:

.. include:: ../tests/test011.py
   :start-after: #---triangleVertices01-begin
   :end-before:  #---triangleVertices01-end
   :literal:
   :code: python

The above code snippet is from :download:`test011.py <../tests/test011.py>`.

If you want to save meshes to reopen them with the CloudCompare GUI, with a predefined state of what is displayed
(colors, normals, scalar fields), use the ``.bin`` format and define the state with the following functions:

 - :py:meth:`~.cloudComPy.ccMesh.showColors`
 - :py:meth:`~.cloudComPy.ccMesh.showNormals`
 - :py:meth:`~.cloudComPy.ccMesh.showSF`
 - :py:meth:`~.cloudComPy.ccMesh.colorsShown`
 - :py:meth:`~.cloudComPy.ccMesh.normalsShown`
 - :py:meth:`~.cloudComPy.ccMesh.sfShown`

meshes modifications
~~~~~~~~~~~~~~~~~~~~

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

The above code snippets are from :download:`test011.py <../tests/test011.py>`.

Generate a cloud from a mesh
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A mesh can be used to create a cloud with :py:meth:`~.cloudComPy.ccMesh.samplePoints`,
with a target either of number of points or of density of points:

.. include:: ../tests/test015.py
   :start-after: #---samplePoints01-begin
   :end-before:  #---samplePoints01-end
   :literal:
   :code: python

The above code snippet is from :download:`test015.py <../tests/test015.py>`.

mesh copy, destruction
~~~~~~~~~~~~~~~~~~~~~~

A mesh can be cloned with all its features, except from the octree, with :py:meth`~.cloudComPy.ccMesh.cloneMesh`:

.. include:: ../tests/test011.py
   :start-after: #---cloneMesh01-begin
   :end-before:  #---cloneMesh01-end
   :literal:
   :code: python

To free memory, a mesh can be deleted with :py:meth:`cloudComPy.deleteEntity` 
(**WARNING** be sure to have no more Python objects referencing the deleted object):

.. include:: ../tests/test011.py
   :start-after: #---deleteEntity02-begin
   :end-before:  #---deleteEntity02-end
   :literal:
   :code: python

The above code snippets are from :download:`test011.py <../tests/test011.py>`.

primitives
----------

Primitives regroups all the geometric generators provided by CloudCompare:

 - :py:class:`~.cloudComPy.ccBox`
 - :py:class:`~.cloudComPy.ccCone`
 - :py:class:`~.cloudComPy.ccCylinder`
 - :py:class:`~.cloudComPy.ccPlane`
 - :py:class:`~.cloudComPy.ccQuadric`
 - :py:class:`~.cloudComPy.ccSphere`
 - :py:class:`~.cloudComPy.ccTorus`
 - :py:class:`~.cloudComPy.ccDish`

All the primitives derives from :py:class:`~.cloudComPy.ccGenericPrimitive`
which is derived from :py:class:`~.cloudComPy.ccMesh`. Thus, all the mesh methods apply here.

All primitive constructors use as argument: geometrical features, an optional drawing precision,
an optional transformation (always possible afterwards).

The primitives have some more methods than the meshes:

 - :py:meth:`~.cloudComPy.ccGenericPrimitive.getTransformation`
 - :py:meth:`~.cloudComPy.ccGenericPrimitive.getTypeName`

.. include:: ../tests/test008.py
   :start-after: #---primitiveTypeName01-begin
   :end-before:  #---primitiveTypeName01-end
   :literal:
   :code: python

The above code snippet is from :download:`test011.py <../tests/test011.py>`.

Box
~~~

.. include:: ../tests/test009.py
   :start-after: #---ccBox01-begin
   :end-before:  #---ccBox01-end
   :literal:
   :code: python

Cone
~~~~

Cone axis corresponds to the ‘Z’ dimension by default.

.. include:: ../tests/test009.py
   :start-after: #---ccCone01-begin
   :end-before:  #---ccCone01-end
   :literal:
   :code: python

A :py:class:`~.cloudComPy.ccCone` offers several methods to retreive its geometric features:

 - :py:meth:`~.cloudComPy.ccCone.getBottomCenter`
 - :py:meth:`~.cloudComPy.ccCone.getBottomRadius`
 - :py:meth:`~.cloudComPy.ccCone.getHeight`
 - :py:meth:`~.cloudComPy.ccCone.getLargeCenter`
 - :py:meth:`~.cloudComPy.ccCone.getLargeRadius`
 - :py:meth:`~.cloudComPy.ccCone.getSmallCenter`
 - :py:meth:`~.cloudComPy.ccCone.getSmallRadius`
 - :py:meth:`~.cloudComPy.ccCone.getTopCenter`
 - :py:meth:`~.cloudComPy.ccCone.getTopRadius`

Cylinder
~~~~~~~~

.. include:: ../tests/test009.py
   :start-after: #---ccCylinder01-begin
   :end-before:  #---ccCylinder01-end
   :literal:
   :code: python

A :py:class:`~.cloudComPy.ccCylinder` derives from a :py:class:`~.cloudComPy.ccCone`.
It is internally represented by a cone with the same top and bottom radius.

.. _plane_properties:

Plane
~~~~~

.. include:: ../tests/test009.py
   :start-after: #---ccPlane01-begin
   :end-before:  #---ccPlane01-end
   :literal:
   :code: python

By default, plane normal corresponds to 'Z' dimension.

A :py:class:`~.cloudComPy.ccPlane` offers several methods to retreive its geometric features:

 - :py:meth:`~.cloudComPy.ccPlane.getCenter`
 - :py:meth:`~.cloudComPy.ccPlane.getEquation`
 - :py:meth:`~.cloudComPy.ccPlane.getNormal`

The method :py:meth:`~.cloudComPy.ccPlane.Fit` allows to adjust a plane primitive on a cloud:

.. include:: ../tests/test012.py
   :start-after: #---planeFit01-begin
   :end-before:  #---planeFit01-end
   :literal:
   :code: python

The above code snippet is from :download:`test012.py <../tests/test012.py>`.

Quadric
~~~~~~~

.. include:: ../tests/test009.py
   :start-after: #---ccQuadric01-begin
   :end-before:  #---ccQuadric01-end
   :literal:
   :code: python

Quadric orthogonal dimension is ‘Z’ by default.

Sphere
~~~~~~

.. include:: ../tests/test009.py
   :start-after: #---ccSphere01-begin
   :end-before:  #---ccSphere01-end
   :literal:
   :code: python

A :py:class:`~.cloudComPy.ccSphere` offers the method :py:meth:`~.cloudComPy.ccSphere.getRadius`.

Torus
~~~~~

.. include:: ../tests/test009.py
   :start-after: #---ccTorus01-begin
   :end-before:  #---ccTorus01-end
   :literal:
   :code: python

Dish
~~~~

.. include:: ../tests/test009.py
   :start-after: #---ccDish01-begin
   :end-before:  #---ccDish01-end
   :literal:
   :code: python

Unless Otherwise noted, the above code snippets are from :download:`test009.py <../tests/test009.py>`.

polylines
---------

A :py:class:`~.cloudComPy.ccPolyline` is considered as a cloud of points (in a specific order)
with a open/closed state information.
Polylines are often used as tools to cut clouds (see :ref:`crop_cloud`).

A polyline can be created from a cloud (the points have to be in the good order):

.. include:: ../tests/test026.py
   :start-after: #---polyFromCloud01-begin
   :end-before:  #---polyFromCloud01-end
   :literal:
   :code: python

Another way (here, we simulate the cloud creation):

.. include:: ../tests/test026.py
   :start-after: #---polyFromCloud02-begin
   :end-before:  #---polyFromCloud02-end
   :literal:
   :code: python

The above code snippets are from :download:`test026.py <../tests/test026.py>`.

A polyline can be imported from several formats (see :ref:`load_formats`).

The ``.poly`` format is a very simple ASCII format, a line per point, x y z. for example:
::

    -4.0  4.0  0.0
     0.0  3.5  0.0
     4.0  4.0  0.0
     3.5  0.0  0.0
     4.0 -4.0  0.0
     0.0 -3.5  0.0
    -4.0 -4.0  0.0
    -3.5  0.0  0.0

The class :py:class:`~.cloudComPy.ccPolyline` offers several information methods:

 - :py:meth:`~.cloudComPy.ccPolyline.isClosed`
 - :py:meth:`~.cloudComPy.ccPolyline.is2DMode`
 - :py:meth:`~.cloudComPy.ccPolyline.segmentCount`
 - :py:meth:`~.cloudComPy.ccPolyline.size`
 - :py:meth:`~.cloudComPy.ccPolyline.getAssociatedCloud`
 - :py:meth:`~.cloudComPy.ccPolyline.getBoundingBox`

The status of the polyline can be modified:

 - :py:meth:`~.cloudComPy.ccPolyline.setClosed`
 - :py:meth:`~.cloudComPy.ccPolyline.set2DMode`

When creating the polyline from a cloud (see examples above) you will need:

 - :py:meth:`~.cloudComPy.ccPolyline.addChild`
 - :py:meth:`~.cloudComPy.ccPolyline.addPointIndex`

The method :py:meth:`~.cloudComPy.ccPolyline.smoothChaikin` is used to smooth the polyline
by adding nodes over several iterations:

.. include:: ../tests/test007.py
   :start-after: #---smoothChaikin01-begin
   :end-before:  #---smoothChaikin01-end
   :literal:
   :code: python

The above code snippet is from :download:`test007.py <../tests/test007.py>`.

.. _2D_polygons:

2D polygons (facets)
--------------------

The class :py:class:`~.cloudComPy.ccFacet` is a tool used to fit  a 2D polygon to a point cloud.
It is very similar to the :py:meth:`~.cloudComPy.ccPlane.Fit` method but the extents of the fitted plane
follows the contour of the cloud (on its 2D projection on the plane).

The 2D polygon is created using the static method :py:meth:`~.cloudComPy.ccFacet.Create`.
If you want a convex contour, leave the argument ``maxEdgeLength`` to its default, 0.
A positive value will gives a concave contour respecting maxEdgeLength.

It is possible to provide a plane equation as optional argument (see :py:meth:`cloudComPy.ccPlane.getEquation`).

The class :py:class:`~.cloudComPy.ccFacet` provides several useful methods:

================================================== =========================================================
:py:meth:`~.cloudComPy.ccFacet.getCenter`          the facet plane center
:py:meth:`~.cloudComPy.ccFacet.getNormal`          the facet plane normal
:py:meth:`~.cloudComPy.ccFacet.getPlaneEquation`   the facet plane equation: [a, b, c, d] as ``ax+by+cz=d``
:py:meth:`~.cloudComPy.ccFacet.getContour`         the contour polyline
:py:meth:`~.cloudComPy.ccFacet.getContourVertices` the point cloud with the contour vertices
:py:meth:`~.cloudComPy.ccFacet.getSurface`         the area of the polygon
================================================== =========================================================

.. include:: ../tests/test021.py
   :start-after: #---facetCreate01-begin
   :end-before:  #---facetCreate01-end
   :literal:
   :code: python

With the plane equation:

.. include:: ../tests/test021.py
   :start-after: #---facetCreate02-begin
   :end-before:  #---facetCreate02-end
   :literal:
   :code: python

The above code snippets are from :download:`test021.py <../tests/test021.py>`.

.. _octrees:

octrees
-------

The Octree structure of CloudCompare is very efficient for *nearest neighbour extraction* and used throughout CloudCompare.
The `CloudCompare wiki <https://www.cloudcompare.org/doc/wiki/index.php/Main_Page>`_ provides a good introduction to 
the `CloudCompare octree <https://www.cloudcompare.org/doc/wiki/index.php/CloudCompare_octree>`_.

Most of the time, you don't need to manipulate explicitely the octree, but some methods have been wrapped in CloudComPy,
to allow for some very specific processing.

If you need to search points in particular neighbourhood, :py:class:`~.cloudComPy.ccOctree` provides useful methods
illustrated in the following example. 

Firstly, compute the octree. Define your neighbourhood either with a radius 
or the number of nodes you want in this neighbourhood.

.. include:: ../tests/test016.py
   :start-after: #---octree01-begin
   :end-before:  #---octree01-end
   :literal:
   :code: python

Search neighbours in a sphere:

.. include:: ../tests/test016.py
   :start-after: #---octree02-begin
   :end-before:  #---octree02-end
   :literal:
   :code: python

Get information on the points found:

.. include:: ../tests/test016.py
   :start-after: #---octree05-begin
   :end-before:  #---octree05-end
   :literal:
   :code: python

It is also possible to search in a cylinder:

.. include:: ../tests/test016.py
   :start-after: #---octree03-begin
   :end-before:  #---octree03-end
   :literal:
   :code: python

or in a box:

.. include:: ../tests/test016.py
   :start-after: #---octree04-begin
   :end-before:  #---octree04-end
   :literal:
   :code: python

The above code snippets are from :download:`test016.py <../tests/test016.py>` where you will find
examples of how to use the other methods in the octree.


