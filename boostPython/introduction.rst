
#############################
Introduction
#############################

This project is a draft of what could be a Python module to interface to CloudCompare,
of equivalent level to the command mode of CloudCompare.

There are still few features available in this prototype,
the idea is to collect feedback from interested users to guide future developments.

Here is an example of a Python script:
::

    import cloudComPy as cc                                                # import the CloudComPy module
    cc.initCC()                                                            # to do once before dealing with plugins

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

The list of available functions should quickly grow.

From the Python interpreter, Docstrings provide some documentation on the available methods, the arguments.
