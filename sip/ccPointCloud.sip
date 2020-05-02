//##########################################################################
//#                                                                        #
//#                                PYCC                                    #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

// sip implementation of ccPointCloud


class ccPointCloud
{
%TypeHeaderCode
#include <ccTrace.h>
#include <ccNumpyIncludes.h>
#include <ccPointCloud.h>
#include <PyScalarType.h>
%End

%Docstring
A 3D cloud and its associated features ( normals, scalar fields, etc.)
A point cloud can have multiple features:
- normals (compressed) (TODO)
- scalar fields
- an octree strucutre
- other children objects (meshes, calibrated pictures, etc.) (TODO)
%End

public:
    ccPointCloud(QString name = QString(), unsigned uniqueID = ccUniqueIDGenerator::InvalidUniqueID) throw();

    QString getName() const;

    unsigned size() const;

    SIP_PYTUPLE computeGravityCenter() [CCVector3 () ];
    %Docstring
    Return a tuple of the 3 coordinates of the gravity center of the cloud
    %End
    %MethodCode
    CCVector3  g = sipCpp->computeGravityCenter();
    PyObject *t;
    t = Py_BuildValue("(ddd)", g.x, g.y, g.z);
    sipRes = t;
    %End

    bool hasScalarFields() const;
    %Docstring
    Return True or False.
    %End

    void scale(double fx, double fy, double fz, SIP_PYTUPLE center = Py_BuildValue("(ddd)", 0., 0., 0.)) [ void (PointCoordinateType, PointCoordinateType, PointCoordinateType, CCVector3) ];
    %Docstring
    Scale the cloud with separate factors along the 3 directions x,y,z and an optional center (default: (0,0,0)).
    parameters:
    - x,
    - y,
    - z,
    - center tuple: (xc, yc, zc), default (0,0,0)
    %End
    %MethodCode
    double v[3];
    if (PyArg_ParseTuple(a3, "ddd", &v[0], &v[1], &v[2]))
    {
        sipCpp->scale(a0, a1, a2, CCVector3(v[0], v[1], v[2]));
    }
    else
    {
        // PyArg_ParseTuple() will have raised the exception.
        sipIsErr = 1;
    }
    %End

    void translate(SIP_PYTUPLE t) [ void (const CCVector3&) ];
    %Docstring
    translate the cloud of (x,y,z)
    parameters:
    translation tuple: (x,y,z)
    %End
    %MethodCode
    double v[3];
    if (PyArg_ParseTuple(a0, "ddd", &v[0], &v[1], &v[2]))
    {
        sipCpp->translate(CCVector3(v[0], v[1], v[2]));
    }
    else
    {
        // PyArg_ParseTuple() will have raised the exception.
        sipIsErr = 1;
    }
    %End

    bool exportCoordToSF(SIP_PYTUPLE b) [ bool (bool*) ];
    %Docstring
    export coordinates to ScalarFields
    parameters:
    tuple of booleans: (isXexported, isYexported, isZexported)
    return status
    %End
    %MethodCode
    int v[3];
    if (PyArg_ParseTuple(a0, "ppp", &v[0], &v[1], &v[2]))
    {
        bool b[3];
        b[0] = v[0]; b[1] = v[1]; b[2] = v[2];
        sipRes = sipCpp->exportCoordToSF(b);
    }
    else
    {
        sipRes = false;
        // PyArg_ParseTuple() will have raised the exception.
        sipIsErr = 1;
    }
    %End

    unsigned getNumberOfScalarFields() const;

    CCCoreLib::ScalarField* getScalarField(int index) const;
    %Docstring
    Return a ScalarField if index is valid, otherwise None.
    %End

    const char* getScalarFieldName(int index) const;
    %Docstring
    Return the ScalarField name if index is valid, otherwise None.
    %End

    void setCurrentOutScalarField(int index);
    %Docstring
    Set the current 'out' ScalarField index. No validity check. Use -1 to set None.
    %End

    CCCoreLib::ScalarField* getCurrentOutScalarField() const;
    %Docstring
    Return the current 'out' ScalarField if there is one, otherwise None.
    %End

    void setCurrentInScalarField(int index);
    %Docstring
    Set the current 'in' ScalarField index. No validity check. Use -1 to set None.
    %End

    CCCoreLib::ScalarField* getCurrentInScalarField() const;
    %Docstring
    Return the current 'in' ScalarField if there is one, otherwise None.
    %End

    bool renameScalarField(int index, const char* newName);
    %Docstring
    Rename the ScalarField if index is valid.
    Warning: Name should not be already used by other ScalarFields of the cloud!
    TODO: check the names in use.
    parameters
    - index of the ScalarField
    - new name
    return True in case of success
    %End

    PyObject* toNpArray(bool copy=true); // if copy == false, ownership C++, otherwise ownership Python
    %Docstring
    Wrap the PointCloud coordinates into a numpy Array.
    parameters:
    copy default True.
    returns PyArray of shape (number of Points, 3)
    If copy is True, data is copied, the PyArray object owns its data.
      Ownership is transfered to Python:
      the PyArray object and its data will be handled by the Python Garbage Collector
    If copy is False, data is not copied, the PyArray object does not own the data.
      PyArray object ownership stays in CloudCompare C++ library : never deleted (small leak ?)
    %End
    %MethodCode
    if( sipCpp->size() > 0 )
    {
        int type_num = CC_NPY_FLOAT;
        CCTRACE("Type num : " << type_num);
        npy_intp nRows = sipCpp->size(); //size_t
        npy_intp dims[2] = {nRows, 3};
        CCTRACE("nrows: " << nRows);
        PyScalarType *s = (PyScalarType*)sipCpp->getPoint(0);
        CCTRACE("data address: " << s);
        CCTRACE("x[0] : " << s[0]);
        CCTRACE("y[0] : " << s[1]);
        CCTRACE("z[0] : " << s[2]);
        CCTRACE("x[-1] : " << s[3*(nRows-1)]);
        CCTRACE("y[-1] : " << s[3*(nRows-1)+1]);
        CCTRACE("z[-1] : " << s[3*(nRows-1)+2]);
        CCTRACE("isCopy " << a0);
        if (a0)
        {
            CCTRACE("--- copy " << 3*nRows*sizeof(PyScalarType));
            PyScalarType *d = new PyScalarType[3*nRows];
            memcpy(d, s, 3*nRows*sizeof(PyScalarType));
            CCTRACE("--- copied");
            sipRes = PyArray_SimpleNewFromData(2, dims, type_num, d );
            PyArray_ENABLEFLAGS((PyArrayObject*)sipRes, NPY_ARRAY_OWNDATA); // the PyArray object owns is data now
            sipTransferBack(sipRes);                                        // transfer ownership of PyArray to Python
            CCTRACE("OwnerShip of copied coordinates (PyArray object) transfered to Python")
        }
        else
        {
            sipRes = PyArray_SimpleNewFromData(2, dims, type_num, s ); // by default the PyArray created like this does not own its data
            sipTransferTo(sipRes, NULL);                               // keep ownership iof PyArray n C++
            CCTRACE("OwnerShip of coordinates (PyArray object pointing to CloudCoordinates) kept in C++"); // potential memory leak, limited
        }
    }
    else
    {
        npy_intp dims[2] = {0, 0};
        sipRes = PyArray_ZEROS(2, dims, CC_NPY_FLOAT, 0);
    }
    CCTRACE("---");
    %End
};
