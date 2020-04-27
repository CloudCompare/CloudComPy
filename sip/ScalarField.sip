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

// sip implementation of ScalarField

namespace CCCoreLib
{
    class ScalarField
    {
    %TypeHeaderCode
    #include <ccTrace.h>
    #include <ccNumpyIncludes.h>
    #include <ScalarField.h>
    #include <PyScalarType.h>
    %End
    
    %Docstring
    A simple scalar field (to be associated to a point cloud)
    A monodimensional array of scalar values.
    Invalid values can be represented by NAN_VALUE.
    Dealing with numpy (method toNpArray) requires to initialize first numpy interface:
    cloudCompare.CCCoreLib.ScalarField.initNumpyApi()
    %End
    
    public:
        explicit ScalarField(const char* name = nullptr);
        
        const char* getName() const;
        
        SIP_PYTUPLE computeMeanAndVariance() [ void (ScalarType&, ScalarType*) ];
        %Docstring
        Return a tuple (mean, variance)
        %End
        %MethodCode
        ScalarType mean;
        ScalarType variance;
        CCTRACE("computeMeanAndVariance");
        sipCpp->computeMeanAndVariance(mean, &variance);
        CCTRACE("mean: " << mean);
        CCTRACE("variance: " << variance);
        PyObject *t;
        t = Py_BuildValue("(dd)", mean, variance);
        sipRes = t;
        %End
        
        void computeMinAndMax();
        %Docstring
        Should be called before getMin or getMax.
        No return!
        %End
        
        // Python float is double precision
        double getMin() [ ScalarType ()];
        %Docstring
        Return minimal value of the ScalarField.
        Call computeMinAndMax before!
        %End
        
        // Python float is double precision
        double getMax() [ ScalarType ()];
        %Docstring
        Return maximal value of the ScalarField.
        Call computeMinAndMax before!
        %End
    
        PyObject* toNpArray(bool copy=true); // if copy == false, ownership C++, otherwise ownership Python
        %Docstring
        Wrap the ScalarField data into a numpy Array.
        parameters:
        copy default True.
        returns PyArray: a one dimension array of (number of Points)
        If copy is True, data is copied, the PyArray object owns its data.
          Ownership is transfered to Python:
          the PyArray object and its data will be handled by the Python Garbage Collector
        If copy is False, data is not copied, the PyArray object does not own the data.
          PyArray object ownership stays in CloudCompare C++ library : never deleted (small leak ?)
        %End
        %MethodCode
        if( !sipCpp->empty() )
        {
            int type_num = CC_NPY_FLOAT;
            CCTRACE("Type num : " << type_num);
            npy_intp nRows = sipCpp->size(); //size_t
            npy_intp dims[1] = {nRows};
            CCTRACE("nrows: " << nRows);
            PyScalarType *s=sipCpp->data();
            CCTRACE("data address: " << s);
            CCTRACE("p[0] : " << s[0]);
            CCTRACE("p[1] : " << s[1]);
            CCTRACE("p[-1] : " << s[nRows-1]);
            if (a0)
            {
                CCTRACE("--- copy " << nRows*sizeof(PyScalarType));
                PyScalarType *d = new PyScalarType[nRows];
                memcpy(d, s, nRows*sizeof(PyScalarType));
                CCTRACE("--- copied");
                sipRes = PyArray_SimpleNewFromData(1, dims, type_num, d );
                PyArray_ENABLEFLAGS((PyArrayObject*)sipRes, NPY_ARRAY_OWNDATA); // the PyArray object owns is data now
                sipTransferBack(sipRes);                                        // transfer ownership of PyArray to Python
                CCTRACE("OwnerShip of copied scalarField (PyArray object) transfered to Python")
            }
            else
            {
                sipRes = PyArray_SimpleNewFromData(1, dims, type_num, s ); // by default the PyArray created like this does not own its data
                sipTransferTo(sipRes, NULL);                               // keep ownership iof PyArray n C++
                CCTRACE("OwnerShip of scalarField (PyArray object pointing to scalarField) kept in C++"); // potential memory leak, limited 
            }
        }
        else
        {
            npy_intp dims[1] = {0};
            sipRes = PyArray_ZEROS(1, dims, CC_NPY_FLOAT, 0);
        }
        %End
        
        bool fromNpArray(SIP_PYOBJECT p);
        %Docstring
        Copy data from numpy array into an existing ScalarField.
        Check if the numpy array is of PyScalarType, one dimension, same size as the ScalarField,
        before overwriting ScalarField data.
        %End
        %MethodCode
        if (PyObject_TypeCheck(a0, &PyArray_Type))
        {
            CCTRACE("fromNpArray: ndarray");
            PyArrayObject* pya = (PyArrayObject*)a0;
            int nd = PyArray_NDIM(pya);
            CCTRACE("nDim: " << nd);
            npy_intp* dims = PyArray_DIMS(pya);
            npy_intp nRows = dims[0];
            CCTRACE("nRows: " << nRows);
            int atype = PyArray_TYPE(pya);
            CCTRACE("type: " << atype << " " << CC_NPY_FLOAT);
            if ( nd==1 &&  nRows==sipCpp->size() && atype==CC_NPY_FLOAT)
            {
                CCTRACE("OK");
                PyScalarType* d = sipCpp->data();
                CCTRACE("dest address: " << d);
                PyScalarType* s = (PyScalarType*)PyArray_DATA(pya);
                CCTRACE("orig address: " << s);
                memcpy(d, s, nRows*sizeof(PyScalarType));
                sipCpp->computeMinAndMax();
                sipRes = true;
            }
            else
            {
                CCTRACE("NOK (dim, size or type)");
                sipRes = true;
            }
        }
        else
        {
            CCTRACE("fromNpArray: not a ndarray");
            sipRes = false;
        }
        %End
        
    protected:
        ~ScalarField();
    };
    
};
