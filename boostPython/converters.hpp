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
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

// original code from:
// https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/

#ifndef CONVERTERS_HPP_
#define CONVERTERS_HPP_

#include <boost/python.hpp>
#include <Python.h>
#include <QString>
#include <vector>
#include <CCGeom.h>
#include <ccHObject.h>

#include "pyccTrace.h"

namespace bp = boost::python;

namespace
{

struct QString_to_python_str
{
    static PyObject* convert(const QString& s)
    {
        return bp::incref(bp::object(s.toUtf8().constData()).ptr());
    }
};

struct QString_from_python_str
{
    QString_from_python_str()
    {
        CCTRACE("register QString_from_python_str");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<QString>());
    }

    // Determine if obj_ptr can be converted in a QString
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to QString?");
        if (!PyUnicode_Check(obj_ptr))
            return 0;
        return obj_ptr;
    }

    // Convert obj_ptr into a QString
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the character data from the python string
        const char* value = PyUnicode_AsUTF8(obj_ptr);

        // Verify that obj_ptr is a string (should be ensured by convertible())
        if (value == 0) bp::throw_error_already_set();

        // Grab pointer to memory into which to construct the new QString
        void* storage = ((bp::converter::rvalue_from_python_storage<QString>*) data)->storage.bytes;

        // in-place construct the new QString using the character data
        // extracted from the Python object
        new (storage) QString(value);

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

template<typename T> struct Vector3Tpl_to_python_tuple
{
    static PyObject* convert(Vector3Tpl<T> v)
    {
        return bp::incref(bp::make_tuple(v[0], v[1], v[2]).ptr());
    }
};

template<typename T> struct Vector3Tpl_from_python_tuple // T double or float
{
    Vector3Tpl_from_python_tuple()
    {
        CCTRACE("register Vector3Tpl_from_python_tuple");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<Vector3Tpl<T> >());
    }

    // Determine if obj_ptr can be converted in a Vector3Tpl<T>
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to Vector3Tpl<T>?");
        if (!PyTuple_Check(obj_ptr))
            return 0;
        if (PyTuple_GET_SIZE(obj_ptr) != 3)
            return 0;
        for (int i=0; i<3; i++)
        {
            PyObject* iptr = PyTuple_GET_ITEM(obj_ptr, i);
            if (!PyFloat_Check(iptr))
                return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a Vector3Tpl<T>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the 3 components (check already done by convertible)
        T val[3];
        for (int i=0; i<3; i++)
        {
            PyObject* iptr = PyTuple_GetItem(obj_ptr, i);
            if (!PyFloat_Check(iptr))
                bp::throw_error_already_set();
            val[i] = PyFloat_AS_DOUBLE(iptr);
        }

        // Grab pointer to memory into which to construct the new Vector3Tpl<T>
        void* storage = ((bp::converter::rvalue_from_python_storage<T[3]>*) data)->storage.bytes;

        // in-place construct the new Vector3Tpl<T> using the character data
        // extracted from the Python object
        T* res = new (storage) T[3];
        for (int i=0; i<3; i++)
            res[i] = val[i];

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

struct ccHObjectVector_from_python_list
{
    ccHObjectVector_from_python_list()
    {
        CCTRACE("register ccHObjectVector_from_python_list");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::vector<ccHObject*> >());
    }

    // Determine if obj_ptr can be converted in a ccHObject*
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to std::vector<ccHObject*>?");
        if (!PyList_Check(obj_ptr))
            return 0;
        CCTRACE("list ok");
        Py_ssize_t nbobj = PyList_GET_SIZE(obj_ptr);
        if (nbobj == 0)
            return 0;
        CCTRACE("nbobj "<< nbobj);
        for (Py_ssize_t i = 0; i<nbobj; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
            bp::extract<ccHObject*> x(iptr);
            if (!x.check())
                return 0;
            CCTRACE("  OK " << i);
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a std::vector<ccHObject*>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        Py_ssize_t nbobj = PyList_GET_SIZE(obj_ptr);

        // Grab pointer to memory into which to construct the new list<ccHObject*>
        void* storage = ((bp::converter::rvalue_from_python_storage<std::vector<ccHObject*> >*) data)->storage.bytes;

        // in-place construct the new list<ccHObject*> using the character data
        // extracted from the Python object
        std::vector<ccHObject*>* res = new (storage) std::vector<ccHObject*>;
        res->resize(nbobj);
        for (Py_ssize_t i = 0; i<nbobj; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
            ccHObject* obj = bp::extract<ccHObject*>(iptr);
            (*res)[i] = obj;
        }

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

void initializeConverters()
{
    using namespace boost::python;

    // register the to-python converter
    CCTRACE("initializeConverters");
    to_python_converter<QString, QString_to_python_str, false>(); //"false" because QString_to_python_str has no member get_pytype
    to_python_converter<Vector3Tpl<float>, Vector3Tpl_to_python_tuple<float>, false>();
    to_python_converter<Vector3Tpl<double>, Vector3Tpl_to_python_tuple<double>, false>();

    // register the from-python converter
    QString_from_python_str();
    Vector3Tpl_from_python_tuple<float>();
    Vector3Tpl_from_python_tuple<double>();
    ccHObjectVector_from_python_list();
}

} //namespace anonymous

#endif /* CONVERTERS_HPP_ */

