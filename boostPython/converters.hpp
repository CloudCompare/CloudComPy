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
#include <ccPointCloud.h>
#include <ccBox.h>

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

template<typename T> struct Vector2Tpl_to_python_tuple
{
    static PyObject* convert(Vector2Tpl<T> v)
    {
        return bp::incref(bp::make_tuple(v[0], v[1]).ptr());
    }
};

template<typename T> struct Vector3Tpl_to_python_tuple
{
    static PyObject* convert(Vector3Tpl<T> v)
    {
        return bp::incref(bp::make_tuple(v[0], v[1], v[2]).ptr());
    }
};

template<typename T> struct Tuple3Tpl_to_python_tuple
{
    static PyObject* convert(Tuple3Tpl<T> v)
    {
        return bp::incref(bp::make_tuple(v.x, v.y, v.z).ptr());
    }
};

template<typename T> struct Vector2Tpl_from_python_tuple // T double or float
{
    Vector2Tpl_from_python_tuple()
    {
        CCTRACE("register Vector2Tpl_from_python_tuple");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<Vector2Tpl<T> >());
    }

    // Determine if obj_ptr can be converted in a Vector2Tpl<T>
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to Vector2Tpl<T>?");
        if (!PyTuple_Check(obj_ptr))
            return 0;
        if (PyTuple_GET_SIZE(obj_ptr) != 2)
            return 0;
        for (int i=0; i<2; i++)
        {
            PyObject* iptr = PyTuple_GET_ITEM(obj_ptr, i);
            if (!PyFloat_Check(iptr) && !PyLong_Check(iptr))
                return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a Vector2Tpl<T>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the 2 components (check already done by convertible)
        T val[2];
        for (int i=0; i<2; i++)
        {
            PyObject* iptr = PyTuple_GetItem(obj_ptr, i);
            if (PyFloat_Check(iptr))
                val[i] = PyFloat_AS_DOUBLE(iptr);
            else if (PyLong_Check(iptr))
                val[i] = PyLong_AsDouble(iptr);
            else
                bp::throw_error_already_set();
        }

        // Grab pointer to memory into which to construct the new Vector2Tpl<T>
        void* storage = ((bp::converter::rvalue_from_python_storage<Vector2Tpl<T> >*) data)->storage.bytes;

        // in-place construct the new Vector2Tpl<T> using the character data
        // extracted from the Python object
        new (storage) Vector2Tpl<T>(val[0], val[1]);

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
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
            if (!PyFloat_Check(iptr) && !PyLong_Check(iptr))
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
            if (PyFloat_Check(iptr))
                val[i] = PyFloat_AS_DOUBLE(iptr);
            else if (PyLong_Check(iptr))
                val[i] = PyLong_AsDouble(iptr);
            else
                bp::throw_error_already_set();
        }

        // Grab pointer to memory into which to construct the new Vector3Tpl<T>
        void* storage = ((bp::converter::rvalue_from_python_storage<Vector3Tpl<T> >*) data)->storage.bytes;

        // in-place construct the new Vector3Tpl<T> using the character data
        // extracted from the Python object
        new (storage) Vector3Tpl<T>(val);

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

template<typename T> struct Tuple3Tpl_from_python_tuple // T int, short, unsigned int, unsigned char
{
    Tuple3Tpl_from_python_tuple()
    {
        CCTRACE("register Tuple3Tpl_from_python_tuple");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<Tuple3Tpl<T> >());
    }

    // Determine if obj_ptr can be converted in a Tuple3Tpl<T>
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to Tuple3Tpl<T>?");
        if (!PyTuple_Check(obj_ptr))
            return 0;
        if (PyTuple_GET_SIZE(obj_ptr) != 3)
            return 0;
        for (int i=0; i<3; i++)
        {
            PyObject* iptr = PyTuple_GET_ITEM(obj_ptr, i);
            if (!PyLong_Check(iptr))
                return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a Tuple3Tpl<T>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the 3 components (check already done by convertible)
        T val[3];
        for (int i=0; i<3; i++)
        {
            PyObject* iptr = PyTuple_GetItem(obj_ptr, i);
            if (!PyLong_Check(iptr))
                bp::throw_error_already_set();
            val[i] = PyLong_AsLong(iptr);
        }

        // Grab pointer to memory into which to construct the new Tuple3Tpl<T>
        void* storage = ((bp::converter::rvalue_from_python_storage<Tuple3Tpl<T> >*) data)->storage.bytes;

        // in-place construct the new Tuple3Tpl<T> using the character data
        // extracted from the Python object
        new (storage) Tuple3Tpl<T>(val);

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

template<typename T> struct Vector6_from_python_tuple // T double or float
{
    Vector6_from_python_tuple()
    {
        CCTRACE("register Vector6_from_python_tuple");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::vector<T> >());
    }

    // Determine if obj_ptr can be converted in a std::vector<T> size 6
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to std::vector<T>?");
        if (!PyTuple_Check(obj_ptr))
            return 0;
        if (PyTuple_GET_SIZE(obj_ptr) != 6)
            return 0;
        for (int i=0; i<6; i++)
        {
            PyObject* iptr = PyTuple_GET_ITEM(obj_ptr, i);
            if (!PyFloat_Check(iptr) && !PyLong_Check(iptr))
                return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a std::vector<T>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the 6 components (check already done by convertible)
        T val[6];
        for (int i=0; i<6; i++)
        {
            PyObject* iptr = PyTuple_GetItem(obj_ptr, i);
            if (PyFloat_Check(iptr))
                val[i] = PyFloat_AS_DOUBLE(iptr);
            else if (PyLong_Check(iptr))
                val[i] = PyLong_AsDouble(iptr);
            else
                bp::throw_error_already_set();
        }

        // Grab pointer to memory into which to construct the new T*
        void* storage = ((bp::converter::rvalue_from_python_storage<std::vector<T> >*) data)->storage.bytes;

        // in-place construct the new Vector3Tpl<T> using the character data
        // extracted from the Python object
        std::vector<T> *res = new (storage) std::vector<T>;
        res->resize(6);
        for (int i=0; i<6; i++)
            (*res)[i] = val[i];

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
            bp::extract<ccPointCloud*> cl(iptr);
            if (cl.check())
            {
                CCTRACE("  OK ccPointCloud" << i);
                continue;
            }
            bp::extract<ccMesh*> mh(iptr);
            if (mh.check())
            {
                CCTRACE("  OK ccMesh" << i);
                continue;
            }
            CCTRACE("  NOK " << i);
            return 0;
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
            bp::extract<ccPointCloud*> cl(iptr);
            if (cl.check())
            {
                CCTRACE("  Extract OK ccPointCloud" << i);
                (*res)[i] = cl();
                continue;
            }
            bp::extract<ccMesh*> mh(iptr);
            if (mh.check())
            {
                CCTRACE("  OK ccMesh" << i);
                (*res)[i] = mh();
                continue;
            }
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
    to_python_converter<Vector2Tpl<float>, Vector2Tpl_to_python_tuple<float>, false>();
    to_python_converter<Vector2Tpl<double>, Vector2Tpl_to_python_tuple<double>, false>();
    to_python_converter<Vector3Tpl<float>, Vector3Tpl_to_python_tuple<float>, false>();
    to_python_converter<Vector3Tpl<double>, Vector3Tpl_to_python_tuple<double>, false>();
    to_python_converter<Tuple3Tpl<unsigned int>, Tuple3Tpl_to_python_tuple<unsigned int>, false>();
    to_python_converter<Tuple3Tpl<unsigned char>, Tuple3Tpl_to_python_tuple<unsigned char>, false>();
    to_python_converter<Tuple3Tpl<short>, Tuple3Tpl_to_python_tuple<short>, false>();
    to_python_converter<Tuple3Tpl<int>, Tuple3Tpl_to_python_tuple<int>, false>();

    // register the from-python converter
    QString_from_python_str();
    Vector2Tpl_from_python_tuple<float>();
    Vector2Tpl_from_python_tuple<double>();
    Vector3Tpl_from_python_tuple<float>();
    Vector3Tpl_from_python_tuple<double>();
    Tuple3Tpl_from_python_tuple<int>();
    Tuple3Tpl_from_python_tuple<unsigned int>();
    Tuple3Tpl_from_python_tuple<unsigned char>();
    Tuple3Tpl_from_python_tuple<short>();
    Vector6_from_python_tuple<float>();
    Vector6_from_python_tuple<double>();
    ccHObjectVector_from_python_list();
}

} //namespace anonymous

#endif /* CONVERTERS_HPP_ */

