
=========================================
Saving Clouds and other entities to files
=========================================

.. _save_formats:

Known formats
-------------

CloudComPy can write a lot of formats of point clouds and meshes.
The ``.bin`` binary format is the native format of CloudCompare and can store several entities of all types.
Take a look at the details of the formats supported by CloudCompare on `wiki <https://www.cloudcompare.org/doc/wiki/index.php/FILE_I/O>`_.

For clouds, the extensions known by CloudComPy are:
``.asc`` ``.bin`` ``.drc`` ``.dxf`` ``.E57`` ``.las`` ``.laz`` ``.pcd`` ``.ply`` ``.pv`` ``.sbf`` ``.shp`` ``.vtk`` ``.xyz``

For meshes:
``.dxf`` ``.bin`` ``.fbx``  ``.obj`` ``.off`` ``.ply`` ``.stl`` ``.vtk``

For polylines:
``.dxf`` ``.poly`` ``.sx`` ``.shp``

See :download:`test020.py <../tests/test020.py>` for an example.

Saving a Point Cloud
--------------------

For most formats, the only parameters of :py:func:`cloudComPy.SavePointCloud` are the cloud and the filename
(CloudComPy does not offer yet all the flexibility of CloudCompare).
The format is defined by the filename extension.
Return code is 0 in case of success:
::

    ret = cc.SavePointCloud(cloud, "CloudComPy/Data/dataSample.bin")


The formats ``.las`` and ``.laz`` (las compressed) offers an optional parameter: `version="1.4"`
to change from the default version.
::

    ret = cc.SavePointCloud(cloud, "CloudComPy/Data/dataSample.laz")
    ret = cc.SavePointCloud(cloud, "CloudComPy/Data/dataSample_v14.laz", version="1.4")

Saving a mesh
-------------

For all formats, the only parameters of :py:func:`cloudComPy.SaveMesh` are the mesh and the filename.
(CloudComPy does not offer yet all the flexibility of CloudCompare).
The format is defined by the filename extension.
Return code is 0 in case of success.
::

    ret = cc.SaveMesh(mesh, "CloudComPy/Data/mesh.stl")


Saving several entities in one file
-----------------------------------

All the entities to save (clouds, meshes, polylines...) must be put in a list.
the :py:func:`cloudComPy.SaveEntities` function return code is 0 in case of success.
::

    ret = cc.SaveEntities([cloud, cloud1, cloud2, mesh, poly], "CloudComPy/Data/mixedEntities.bin")
