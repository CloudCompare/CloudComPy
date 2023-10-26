
============================================================================
operations on coordinates, scalar fields, colors, normals, meshes with Numpy
============================================================================
.. _coordinates_Numpy:

Read, modify or create cloud coordinates with Numpy
---------------------------------------------------

There are two ways to access the cloud coordinates from Numpy.

 - If you want to modify the data in place, without copy, use :py:meth:`~.cloudComPy.ccPointCloud.toNpArray`

 - if you prefer to copy the scalar field data, use :py:meth:`~.cloudComPy.ccPointCloud.toNpArrayCopy`

  .. include:: ../tests/test004.py
     :start-after: #---coordsToNumpy01-begin
     :end-before:  #---coordsToNumpy01-end
     :literal:
     :code: python

  The above code snippet is from :download:`test004.py <../tests/test004.py>`.

You can fill the cloud coordinates with :py:meth:`~.cloudComPy.ccPointCloud.coordsFromNPArray_copy`.

 - The array should have a shape (numberOfPoints,3).

 - **WARNING** Be sure to have an array data in C-style contiguous order.
   For instance, a transpose operation do not reorder the data in memory.
   Check the boolean array.flags ``array.flags['C_CONTIGUOUS']`` and, if ``False``,
   reorder with ``array.copy(order='C')``.

 - The cloud memory is reserved /resized automatically.
 
See below :ref:`CloudCreateFromNumpy`.

.. _ScalarField_Numpy:

Read, modify or create a scalar field with Numpy
------------------------------------------------

There are two ways to access to the scalar field data from Numpy.

 - If you want to modify the data in place, without copy, use :py:meth:`~.cloudComPy.ScalarField.toNpArray`

  .. include:: ../tests/test002.py
     :start-after: #---sfNumpy01-begin
     :end-before:  #---sfNumpy01-end
     :literal:
     :code: python

  The above code snippet is from :download:`test002.py <../tests/test002.py>`.

 - if you prefer to copy the scalar field data, use :py:meth:`~.cloudComPy.ScalarField.toNpArrayCopy`

You can fill a cloud scalar field with :py:meth:`~.cloudComPy.ScalarField.fromNpArrayCopy`.
The method checks if the scalar field and the array have the same size, before copy.
 
See below :ref:`CloudCreateFromNumpy`.

.. _CloudCreateFromNumpy:

An example of point cloud with scalar field created from Numpy arrays
---------------------------------------------------------------------

.. include:: ../tests/test017.py
   :start-after: #---createCloudNumpy01-begin
   :end-before:  #---createCloudNumpy01-end
   :literal:
   :code: python

The above code snippet is from :download:`test017.py <../tests/test017.py>`.

Access to the indexes of triangles nodes in a mesh
--------------------------------------------------

In a mesh, the array of triangles indexes has a shape(number of triangles, 3),
with indexes corresponding to the indexes of nodes in the associated cloud.

The method :py:meth:`~.cloudComPy.ccMesh.IndexesToNpArray` gives access to the array without copy.
the corresponding nodes coordinates are given by the method :py:meth:`~.cloudComPy.ccPointCloud.toNpArray`
from the associated cloud (:py:meth:`~.cloudComPy.ccMesh.getAssociatedCloud`.
The method :py:meth:`~.cloudComPy.ccMesh.IndexesToNpArray_copy` creates a copy of the array of indexes.

.. include:: ../tests/test011.py
   :start-after: #---triangleIndexes01-begin
   :end-before:  #---triangleIndexes01-end
   :literal:
   :code: python

The above code snippet is from :download:`test011.py <../tests/test011.py>`.

Access to the array of colors in cloud
--------------------------------------

The colors are stored in an array of shape(number of nodes, 4), the four components are R, G, B and alpha,
each stored in a 8bits integer.

The method :py:meth:`~.cloudComPy.ccPointCloud.colorsToNpArray` gives access to the array of colors without copy, 
the method :py:meth:`~.cloudComPy.ccPointCloud.colorsToNpArrayCopy` creates a copy of the array.

.. include:: ../tests/test029.py
   :start-after: #---colorsArray01-begin
   :end-before:  #---colorsArray01-end
   :literal:
   :code: python

The above code snippet is from :download:`test029.py <../tests/test029.py>`.

Read, modify or create normals with Numpy
-----------------------------------------

Normals can  be exported as Numpy arrays with :py:meth:`~.cloudComPy.ccPointCloud.normalsToNpArrayCopy`.
As normals are stored compressed in the CloudCompare cloud,
the method decompress the values in a new Numpy array (owned by Python).
In the example below, we check that this is the same as converting normals into scalar fields
and exporting these scalar fields to Numpy arrays.

.. include:: ../tests/test054.py
   :start-after: #---normals01-begin
   :end-before:  #---normals01-end
   :literal:
   :code: python

It is also possible to import Numpy arrays as normals  with :py:meth:`~.cloudComPy.ccPointCloud.normalsFromNpArrayCopy`.
The numpy array must have the right type, shape and size.
During import, the normals are normalized and compressed.
In the following example, whe invert and denormalize the numpy array of normals before import/

.. include:: ../tests/test054.py
   :start-after: #---normals02-begin
   :end-before:  #---normals02-end
   :literal:
   :code: python

The above code snippets are from :download:`test054.py <../tests/test054.py>`.
