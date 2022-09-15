//##########################################################################
//#                                                                        #
//#                              CloudComPy                                #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; either version 3 of the License, or     #
//#  any later version.                                                    #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#  You should have received a copy of the GNU General Public License     #
//#  along with this program. If not, see <https://www.gnu.org/licenses/>. #
//#                                                                        #
//#          Copyright 2020-2021 Paul RASCLE www.openfields.fr             #
//#                                                                        #
//##########################################################################

#ifndef CONVERTERS_HPP_
#define CONVERTERS_HPP_

#include "cloudComPy.hpp"

#include <QString>
#include <vector>
#include <map>

#include <CCGeom.h>
#include <ccHObject.h>
#include <ccOctree.h>
#include <DgmOctree.h>
#include <ccPointCloud.h>
#include <ReferenceCloud.h>
#include <ccMesh.h>
#include <ccPolyline.h>
#include <ccBox.h>
#include <ccPlane.h>
#include <ScalarField.h>
#include <ccGLMatrix.h>
#include <ccBBox.h>
#include <ccSensor.h>
#include <ccGBLSensor.h>

#include "ccOctreePy.hpp"
#include "pyccTrace.h"

namespace
{

template<typename K, typename V> struct map_to_python_dict
{
    static PyObject* convert(const std::map<K, V>& map)
    {
        bp::dict dictionary;
        for (auto iter : map)
        {
            dictionary[iter.first] = iter.second;
        }
        return bp::incref(dictionary.ptr());
    }
};

struct ccOctree_to_python
{
    static PyObject* convert(ccOctree* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ccHObject_to_python
{
    static PyObject* convert(ccHObject* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ccPointCloud_to_python
{
    static PyObject* convert(ccPointCloud* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ccSensor_to_python
{
    static PyObject* convert(ccSensor* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ccGBLSensor_to_python
{
    static PyObject* convert(ccGBLSensor* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ScalarField_to_python
{
    static PyObject* convert(CCCoreLib::ScalarField* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ReferenceCloud_to_python
{
    static PyObject* convert(CCCoreLib::ReferenceCloud* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ccMesh_to_python
{
    static PyObject* convert(ccMesh* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

struct ccPlane_to_python
{
    static PyObject* convert(ccPlane* c)
    {
        return bp::incref(bp::object(c).ptr());
    }
};

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

template<typename T> struct vector_to_python_list
{
    static PyObject* convert(std::vector<T> v)
    {
        bp::list vec;
        for (int i=0; i<v.size(); i++)
            vec.append(v[i]);
        return bp::incref(vec.ptr());
    }
};

template<typename T> struct vector_to_python_listref
{
    static PyObject* convert(std::vector<T> v)
    {
        bp::list vec;
        for (int i=0; i<v.size(); i++)
            vec.append(boost::ref(v[i]));
        return bp::incref(vec.ptr());
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

template<typename T> struct Vector2Tpl_from_python_list // T double or float
{
    Vector2Tpl_from_python_list()
    {
        CCTRACE("register Vector2Tpl_from_python_list");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<Vector2Tpl<T> >());
    }

    // Determine if obj_ptr can be converted in a Vector2Tpl<T>
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to Vector2Tpl<T>?");
        if (!PyList_Check(obj_ptr))
            return 0;
        if (PyList_GET_SIZE(obj_ptr) != 2)
            return 0;
        for (int i=0; i<2; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
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
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
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

template<typename T> struct Vector3Tpl_from_python_list // T double or float
{
    Vector3Tpl_from_python_list()
    {
        CCTRACE("register Vector3Tpl_from_python_list");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<Vector3Tpl<T> >());
    }

    // Determine if obj_ptr can be converted in a Vector3Tpl<T>
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to Vector3Tpl<T>?");
        if (!PyList_Check(obj_ptr))
            return 0;
        if (PyList_GET_SIZE(obj_ptr) != 3)
            return 0;
        for (int i=0; i<3; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
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
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
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

template<typename T> struct Vector_from_python_list // T double or float
{
    Vector_from_python_list()
    {
        CCTRACE("register std::vector_from_python_list");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::vector<T> >());
    }

    // Determine if obj_ptr can be converted in a Vector3Tpl<T>
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to std::vector<T>?");
        if (!PyList_Check(obj_ptr))
            return 0;
        int nbElem = PyList_GET_SIZE(obj_ptr);
        for (int i=0; i<nbElem; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
            if (!PyFloat_Check(iptr) && !PyLong_Check(iptr))
                return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a std::vector<T>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        // Extract the components (check already done by convertible)
        int nbElem = PyList_GET_SIZE(obj_ptr);
        CCTRACE("construct vector size " << nbElem);

        // Grab pointer to memory into which to construct the new Vector3Tpl<T>
        void* storage = ((bp::converter::rvalue_from_python_storage<std::vector<T> >*) data)->storage.bytes;
        std::vector<T>* val = new (storage) std::vector<T>;
        val->resize(nbElem);
        for (int i=0; i<nbElem; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
            if (PyFloat_Check(iptr))
                (*val)[i] = PyFloat_AS_DOUBLE(iptr);
            else if (PyLong_Check(iptr))
                (*val)[i] = PyLong_AsDouble(iptr);
            else
                bp::throw_error_already_set();
        }
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

template<typename T> struct Vector_from_python_tuple // T double or float
{
    Vector_from_python_tuple()
    {
        CCTRACE("register Vector_from_python_tuple");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::vector<T> >());
    }

    // Determine if obj_ptr can be converted in a std::vector<T> size 6
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to std::vector<T>?");
        if (!PyTuple_Check(obj_ptr))
            return 0;
        Py_ssize_t nbElems = PyTuple_GET_SIZE(obj_ptr);
        for (Py_ssize_t i=0; i<nbElems; i++)
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
        // Extract the components (check already done by convertible)
        Py_ssize_t nbElems = PyTuple_GET_SIZE(obj_ptr);
        std::vector<T> val;
        val.resize(nbElems);
        for (Py_ssize_t i=0; i<nbElems; i++)
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
        res->resize(nbElems);
        for (Py_ssize_t i=0; i<nbElems; i++)
            (*res)[i] = val[i];

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

template<typename T> struct Vector_from_python_tuple_tuple // tuple(Vector3Tpl<T>), T in double, float, long
{
    Vector_from_python_tuple_tuple()
    {
        CCTRACE("register Vector_from_python_tuple_tuple");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::vector<Vector3Tpl<T> > >());
    }

    // Determine if obj_ptr can be converted in a std::vector<T> size 6
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to std::vector<Vector3Tpl<T> >?");
        if (!PyTuple_Check(obj_ptr))
            return 0;
        Py_ssize_t nbElems = PyTuple_GET_SIZE(obj_ptr);
        CCTRACE("nbElems:" << nbElems);
        for (Py_ssize_t i=0; i<nbElems; i++)
        {
            PyObject* iptr = PyTuple_GET_ITEM(obj_ptr, i);
            if (!PyTuple_Check(iptr))
                return 0;
            Py_ssize_t nbElems2 = PyTuple_GET_SIZE(iptr);
            CCTRACE("nbElems2:" << nbElems2);
            if (nbElems2 != 3)
                return 0;
            for (Py_ssize_t j=0; j<nbElems2; j++)
            {
                PyObject* jptr = PyTuple_GET_ITEM(iptr, j);
                if (!PyFloat_Check(jptr) && !PyLong_Check(jptr))
                    return 0;
            }
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a std::vector<Vector3Tpl<T> >
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the components (check already done by convertible)
        Py_ssize_t nbElems = PyTuple_GET_SIZE(obj_ptr);
        std::vector<Vector3Tpl<T> > val;
        val.resize(nbElems);
        for (Py_ssize_t i=0; i<nbElems; i++)
        {
            PyObject* iptr = PyTuple_GetItem(obj_ptr, i);
            Py_ssize_t nbElems2 = PyTuple_GET_SIZE(iptr);
            for (Py_ssize_t j=0; j<nbElems2; j++)
            {
                PyObject* jptr = PyTuple_GET_ITEM(iptr, j);
                if (PyFloat_Check(jptr))
                    val[i][j] = PyFloat_AS_DOUBLE(jptr);
                else if (PyLong_Check(jptr))
                    val[i][j] = PyLong_AsDouble(jptr);
                else
                    bp::throw_error_already_set();
            }
        }

        // Grab pointer to memory into which to construct the new T*
        void* storage = ((bp::converter::rvalue_from_python_storage<std::vector<Vector3Tpl<T> > >*) data)->storage.bytes;

        // in-place construct the new Vector3Tpl<T> using the character data
        // extracted from the Python object
        std::vector<Vector3Tpl<T> > *res = new (storage) std::vector<Vector3Tpl<T> >;
        res->resize(nbElems);
        for (Py_ssize_t i=0; i<nbElems; i++)
            (*res)[i] = val[i];

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

struct ccPointCloudVector_from_python_list
{
    ccPointCloudVector_from_python_list()
    {
        CCTRACE("register ccPointCloudVector_from_python_list");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<std::vector<ccPointCloud*> >());
    }

    // Determine if obj_ptr can be converted in a cvPointCloud*
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to std::vector<ccPointCloud*>?");
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
            CCTRACE("  NOK " << i);
            return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a std::vector<ccPointCloud*>
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        Py_ssize_t nbobj = PyList_GET_SIZE(obj_ptr);

        // Grab pointer to memory into which to construct the new list<ccHObject*>
        void* storage = ((bp::converter::rvalue_from_python_storage<std::vector<ccPointCloud*> >*) data)->storage.bytes;

        // in-place construct the new list<ccPointCloud*> using the character data
        // extracted from the Python object
        std::vector<ccPointCloud*>* res = new (storage) std::vector<ccPointCloud*>;
        res->resize(nbobj);
        for (Py_ssize_t i = 0; i<nbobj; i++)
        {
            PyObject* iptr = PyList_GetItem(obj_ptr, i);
            bp::extract<ccPointCloud*> cl(iptr);
            if (cl.check())
            {
                CCTRACE("  Extract OK ccPointCloud" << i);
                (*res)[i] = cl();
            }
        }

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
            bp::extract<ccPolyline*> po(iptr);
            if (po.check())
            {
                CCTRACE("  OK ccPolyline" << i);
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
            bp::extract<ccPolyline*> po(iptr);
            if (po.check())
            {
                CCTRACE("  OK ccPolyline" << i);
                (*res)[i] = po();
                continue;
            }
        }

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

struct ccGLMatrix_from_python_ccGLMatrixTpl
{
    ccGLMatrix_from_python_ccGLMatrixTpl()
    {
        CCTRACE("register ccGLMatrix_from_python_ccGLMatrixTpl");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<ccGLMatrix>());
    }

    // Determine if obj_ptr can be converted in a ccGLMatrix
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to ccGLMatrix?");
        bp::extract<ccGLMatrixTpl<float>> glm(obj_ptr);
        if (glm.check())
        {
            CCTRACE("OK ccGLMatrixTpl<float>");
        }
        else
        {
            CCTRACE("NOK");
            return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a ccGLMatrix
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct ccGLMatrix");

        // Grab pointer to memory into which to construct the new ccGLMatrix
        void* storage = ((bp::converter::rvalue_from_python_storage<ccGLMatrix>*) data)->storage.bytes;

        // in-place construct the new ccGLMatrix using the character data
        // extracted from the Python object
        ccGLMatrix* res = new (storage) ccGLMatrix(bp::extract<ccGLMatrixTpl<float>>(obj_ptr));

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

struct ccGLMatrixd_from_python_ccGLMatrixTpl
{
    ccGLMatrixd_from_python_ccGLMatrixTpl()
    {
        CCTRACE("register ccGLMatrixd_from_python_ccGLMatrixTpl");
        bp::converter::registry::push_back(&convertible, &construct, bp::type_id<ccGLMatrixd>());
    }

    // Determine if obj_ptr can be converted in a ccGLMatrixd
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to ccGLMatrixd?");
        bp::extract<ccGLMatrixTpl<double>> glm(obj_ptr);
        if (glm.check())
        {
            CCTRACE("OK ccGLMatrixd");
        }
        else
        {
            CCTRACE("NOK");
            return 0;
        }
        return obj_ptr;
    }

    // Convert obj_ptr into a ccGLMatrixd
    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct ccGLMatrixd");

        // Grab pointer to memory into which to construct the new ccGLMatrixd
        void* storage = ((bp::converter::rvalue_from_python_storage<ccGLMatrixd>*) data)->storage.bytes;

        // in-place construct the new ccGLMatrixd using the character data
        // extracted from the Python object
        ccGLMatrixd* res = new (storage) ccGLMatrixd(bp::extract<ccGLMatrixTpl<double>>(obj_ptr));

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

struct ccBBox_from_python_ccBBox
{
    ccBBox_from_python_ccBBox()
    {
        CCTRACE("register ccBBox_from_python_ccBBox");
    }

    // Determine if obj_ptr can be converted in a ccBBox
    static void* convertible(PyObject* obj_ptr)
    {
        CCTRACE("convertible to ccBBox?");
        bp::extract<ccBBox> bpo(obj_ptr);
        if (bpo.check())
        {
            CCTRACE("OK ccBBox");
        }
        else
        {
            CCTRACE("NOK");
            return 0;
        }
        return obj_ptr;
    }

    static void construct(PyObject* obj_ptr, bp::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct ccBBox");

        // Grab pointer to memory into which to construct the new ccBBox
        void* storage = ((bp::converter::rvalue_from_python_storage<ccBBox>*) data)->storage.bytes;

        // in-place construct the new ccBBox using the character data
        // extracted from the Python object
        ccBBox* res = new (storage) ccBBox(bp::extract<ccBBox>(obj_ptr));

        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};

void initializeConverters(py::module &m0)
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
    // register the to-python converter
    CCTRACE("initializeConverters");
//    to_python_converter<ccOctree*, ccOctree_to_python, false>();
//    to_python_converter<ccHObject*, ccHObject_to_python, false>();
//    to_python_converter<ccPointCloud*, ccPointCloud_to_python, false>();
//    to_python_converter<ccSensor*, ccSensor_to_python, false>();
//    to_python_converter<ccGBLSensor*, ccGBLSensor_to_python, false>();
//    to_python_converter<ccMesh*, ccMesh_to_python, false>();
//    to_python_converter<ccPlane*, ccPlane_to_python, false>();
//    to_python_converter<CCCoreLib::ScalarField*, ScalarField_to_python, false>();
//    to_python_converter<QString, QString_to_python_str, false>(); //"false" because QString_to_python_str has no member get_pytype
//    to_python_converter<Vector2Tpl<float>, Vector2Tpl_to_python_tuple<float>, false>();
//    to_python_converter<Vector2Tpl<double>, Vector2Tpl_to_python_tuple<double>, false>();
//    to_python_converter<Vector3Tpl<float>, Vector3Tpl_to_python_tuple<float>, false>();
//    to_python_converter<Vector3Tpl<double>, Vector3Tpl_to_python_tuple<double>, false>();
//    to_python_converter<Tuple3Tpl<unsigned int>, Tuple3Tpl_to_python_tuple<unsigned int>, false>();
//    to_python_converter<Tuple3Tpl<unsigned char>, Tuple3Tpl_to_python_tuple<unsigned char>, false>();
//    to_python_converter<Tuple3Tpl<short>, Tuple3Tpl_to_python_tuple<short>, false>();
//    to_python_converter<Tuple3Tpl<int>, Tuple3Tpl_to_python_tuple<int>, false>();
//    to_python_converter<std::vector<int>, vector_to_python_list<int>, false>();
//    to_python_converter<std::vector<unsigned int>, vector_to_python_list<unsigned int>, false>();
//    to_python_converter<std::vector<float>, vector_to_python_list<float>, false>();
//    to_python_converter<std::vector<double>, vector_to_python_list<double>, false>();
//    to_python_converter<std::vector<unsigned long long>, vector_to_python_list<unsigned long long>, false>();
//    to_python_converter<std::vector<Vector3Tpl<float> >, vector_to_python_list<Vector3Tpl<float> >, false>();
//    to_python_converter<std::vector<Vector3Tpl<double> >, vector_to_python_list<Vector3Tpl<double> >, false>();
//    to_python_converter<std::vector<PointDescriptor_persistent_py>, vector_to_python_list<PointDescriptor_persistent_py>, false>();
//    to_python_converter<std::vector<CCCoreLib::DgmOctree::CellDescriptor>, vector_to_python_list<CCCoreLib::DgmOctree::CellDescriptor>, false>();
//    to_python_converter<std::vector<CCCoreLib::DgmOctree::IndexAndCode>, vector_to_python_list<CCCoreLib::DgmOctree::IndexAndCode>, false>();
//    to_python_converter<std::vector<CCCoreLib::DgmOctree::PointDescriptor>, vector_to_python_list<CCCoreLib::DgmOctree::PointDescriptor>, false>();
//    to_python_converter<std::vector<ccHObject*>, vector_to_python_listref<ccHObject*>, false>();
//    to_python_converter<std::vector<ccMesh*>, vector_to_python_listref<ccMesh*>, false>();
//    to_python_converter<std::vector<ccPointCloud*>, vector_to_python_listref<ccPointCloud*>, false>();
//    to_python_converter<std::vector<ccPolyline*>, vector_to_python_listref<ccPolyline*>, false>();
//    to_python_converter<std::vector<ccSensor*>, vector_to_python_listref<ccSensor*>, false>();
//    to_python_converter<std::vector<QString>, vector_to_python_list<QString>, false>();
//    to_python_converter<std::map<QString, int>, map_to_python_dict<QString, int>, false>();
//    // register the from-python converter
//    QString_from_python_str();
//    Vector2Tpl_from_python_tuple<float>();
//    Vector2Tpl_from_python_tuple<double>();
//    Vector2Tpl_from_python_list<float>();
//    Vector2Tpl_from_python_list<double>();
//    Vector3Tpl_from_python_tuple<float>();
//    Vector3Tpl_from_python_tuple<double>();
//    Vector3Tpl_from_python_list<float>();
//    Vector3Tpl_from_python_list<double>();
//    Vector_from_python_list<float>();
//    Vector_from_python_list<double>();
//    Tuple3Tpl_from_python_tuple<int>();
//    Tuple3Tpl_from_python_tuple<unsigned int>();
//    Tuple3Tpl_from_python_tuple<unsigned char>();
//    Tuple3Tpl_from_python_tuple<short>();
//    Vector_from_python_tuple<unsigned int>();
//    Vector_from_python_tuple<float>();
//    Vector_from_python_tuple<double>();
//    Vector_from_python_tuple<unsigned long long>();
//    Vector_from_python_tuple_tuple<float>();
//    Vector_from_python_tuple_tuple<double>();
//    ccHObjectVector_from_python_list();
//    ccPointCloudVector_from_python_list();
//    ccGLMatrix_from_python_ccGLMatrixTpl();
//    ccGLMatrixd_from_python_ccGLMatrixTpl();
//    ccBBox_from_python_ccBBox();
}

} //namespace anonymous

#endif /* CONVERTERS_HPP_ */

