
=============================
cloudComPy: a first example
=============================

This project proposes a Python module to interface to `CloudCompare <https://cloudcompare.org>`_,
of equivalent level to the command mode of CloudCompare.

`CloudComPy <https://github.com/CloudCompare/CloudComPy>`_ does not yet cover all the features of CloudCompare. 
Features are added according to the needs reported by users, 
feel free to post issues in `GitHub <https://github.com/CloudCompare/CloudComPy/issues>`_.

Here is an example of a Python script:
::

    os.environ["_CCTRACE_"]="ON"                                           # only if you want debug traces from C++

    import cloudComPy as cc                                                # import the CloudComPy module

    cloud = cc.loadPointCloud("myCloud.xyz")                               # read a point cloud from a file
    print("cloud name: %s"%cloud.getName())

    res=cc.computeCurvature(cc.CurvatureType.GAUSSIAN_CURV, 0.05, [cloud]) # compute curvature as a scalar field
    nsf = cloud.getNumberOfScalarFields()
    sfCurv=cloud.getScalarField(nsf-1)
    cloud.setCurrentOutScalarField(nsf-1)
    filteredCloud=cc.filterBySFValue(0.01, sfCurv.getMax(), cloud)         # keep only the points above a given curvature

    ok = filteredCloud.exportCoordToSF(False, False, True)                 # Z coordinate as a scalar Field
    nsf = cloud.getNumberOfScalarFields()
    sf1=filteredCloud.getScalarField(nsf-1)
    mean, var = sf1.computeMeanAndVariance()

    # using Numpy...

    coordinates = filteredCloud.toNpArrayCopy()                            # coordinates as a numpy array
    x=coordinates[:,0]                                                     # x column
    y=coordinates[:,1]
    z=coordinates[:,2]

    f=(2*x-y)*(x+3*y)                                                      # elementwise operation on arrays

    asf1=sf1.toNpArray()                                                   # scalar field as a numpy array
    sf1.fromNpArrayCopy(f)                                                 # replace scalar field values by a numpy array

    res=cc.SavePointCloud(filteredCloud,"myModifiedCloud.bin")             #save the point cloud to a file


As you can see, it is possible to read and write point clouds,
access scalar fields from Numpy (with or without copy of data), call some CloudCompare functions to transform point clouds.

From the Python interpreter, Docstrings provide some documentation on the available methods, the arguments.
