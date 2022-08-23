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

#ifndef CONVERSIONS_HPP_
#define CONVERSIONS_HPP_

#include <QString>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <CCTypes.h>
#include <CCGeom.h>
#include "pyccTrace.h"

namespace pybind11
{
namespace detail
{

// from https://gist.github.com/imikejackson
/* Create a TypeCaster for auto python string <--> QString conversion */
template<> struct type_caster<QString>
{
public:
    /**
     * This macro establishes the name 'QString' in
     * function signatures and declares a local variable
     * 'value' of type QString
     */
PYBIND11_TYPE_CASTER(QString, _("QString"));

    /**
     *  @brief Conversion part 1 (Python->C++): convert a PyObject into a QString
     * instance or return false upon failure. The second argument
     * indicates whether implicit conversions should be applied.
     * @param src
     * @return boolean
     */
    bool load(handle src, bool)
    {
        if(!src)
        {
            return false;
        }
        object temp;
        handle load_src = src;
        if(PyUnicode_Check(load_src.ptr()))
        {
            temp = reinterpret_steal<object>(PyUnicode_AsUTF8String(load_src.ptr()));
            if(!temp) /* A UnicodeEncodeError occured */
            {
                PyErr_Clear();
                return false;
            }
            load_src = temp;
        }
        char* buffer = nullptr;
        ssize_t length = 0;
        int err = PYBIND11_BYTES_AS_STRING_AND_SIZE(load_src.ptr(), &buffer, &length);
        if(err == -1) /* A TypeError occured */
        {
            PyErr_Clear();
            return false;
        }
        value = QString::fromUtf8(buffer, static_cast<int>(length));
        return true;
    }

    /**
     * @brief Conversion part 2 (C++ -> Python): convert an QString instance into
     * a Python object. The second and third arguments are used to
     * indicate the return value policy and parent object (for
     * ``return_value_policy::reference_internal``) and are generally
     * ignored by implicit casters.
     *
     * @param src
     * @return
     */
    static handle cast(const QString& src, return_value_policy /* policy */, handle /* parent */)
    {
#if PY_VERSION_HEX >= 0x03030000 // Python 3.3
        assert(sizeof(QChar) == 2);
        return PyUnicode_FromKindAndData(PyUnicode_2BYTE_KIND, src.constData(), src.length());
#else
        QByteArray a = src.toUtf8();
        return PyUnicode_FromStringAndSize(a.data(), (ssize_t)a.length());
#endif
    }
};

template<typename T> struct type_caster<Tuple3Tpl<T> >
{
public:
PYBIND11_TYPE_CASTER(Tuple3Tpl<T>, _("Tuple3Tpl<T>"));

    bool load(handle src, bool)
    {
        CCTRACE("Tuple3Tpl<T> load");
        if(!src)
        {
            return false;
        }
        object temp;
        handle load_src = src;

        if (PyTuple_Check(load_src.ptr()))
        {
            if (PyTuple_GET_SIZE(load_src.ptr()) != 3)
                return false;
            for (int i=0; i<3; i++)
            {
                PyObject* iptr = PyTuple_GET_ITEM(load_src.ptr(), i);
                if (PyFloat_Check(iptr))
                    value.u[i] = PyFloat_AS_DOUBLE(iptr);
                else if (PyLong_Check(iptr))
                    value.u[i] = PyLong_AsDouble(iptr);
                else
                    return false;
            }
            return true;
        }
        else if (PyList_Check(load_src.ptr()))
        {
            if (PyList_GET_SIZE(load_src.ptr()) != 3)
                return false;
            for (int i=0; i<3; i++)
            {
                PyObject* iptr = PyList_GET_ITEM(load_src.ptr(), i);
                if (PyFloat_Check(iptr))
                    value.u[i] = PyFloat_AS_DOUBLE(iptr);
                else if (PyLong_Check(iptr))
                    value.u[i] = PyLong_AsDouble(iptr);
                else
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    static handle cast(const Tuple3Tpl<T>& src, return_value_policy /* policy */, handle /* parent */)
    {
        CCTRACE("Tuple3Tpl<T> cast");
        tuple tup = make_tuple(src.x, src.y, src.z);
        return tup.inc_ref();
    }
};

template<typename T> struct type_caster<Vector3Tpl<T> >
{
public:
PYBIND11_TYPE_CASTER(Vector3Tpl<T>, _("Vector3Tpl<T>"));

    bool load(handle src, bool)
    {
        CCTRACE("Vector3Tpl<T> load");
        if(!src)
        {
            return false;
        }
        object temp;
        handle load_src = src;

        if (PyTuple_Check(load_src.ptr()))
        {
            if (PyTuple_GET_SIZE(load_src.ptr()) != 3)
                return false;
            for (int i=0; i<3; i++)
            {
                PyObject* iptr = PyTuple_GET_ITEM(load_src.ptr(), i);
                if (PyFloat_Check(iptr))
                    value.u[i] = PyFloat_AS_DOUBLE(iptr);
                else if (PyLong_Check(iptr))
                    value.u[i] = PyLong_AsDouble(iptr);
                else
                    return false;
            }
            return true;
        }
        else if (PyList_Check(load_src.ptr()))
        {
            if (PyList_GET_SIZE(load_src.ptr()) != 3)
                return false;
            for (int i=0; i<3; i++)
            {
                PyObject* iptr = PyList_GET_ITEM(load_src.ptr(), i);
                if (PyFloat_Check(iptr))
                    value.u[i] = PyFloat_AS_DOUBLE(iptr);
                else if (PyLong_Check(iptr))
                    value.u[i] = PyLong_AsDouble(iptr);
                else
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    static handle cast(const Vector3Tpl<T>& src, return_value_policy /* policy */, handle /* parent */)
    {
        CCTRACE("Vector3Tpl<T> cast");
        tuple tup = make_tuple(src.x, src.y, src.z);
        return tup.inc_ref();
    }
};

template<typename T> struct type_caster<Vector2Tpl<T> >
{
public:
PYBIND11_TYPE_CASTER(Vector2Tpl<T>, _("Vector2Tpl<T>"));

    bool load(handle src, bool)
    {
        CCTRACE("Vector2Tpl<T> load");
        if(!src)
        {
            return false;
        }
        object temp;
        handle load_src = src;

        if (PyTuple_Check(load_src.ptr()))
        {
            if (PyTuple_GET_SIZE(load_src.ptr()) != 2)
                return false;
            for (int i=0; i<2; i++)
            {
                PyObject* iptr = PyTuple_GET_ITEM(load_src.ptr(), i);
                if (PyFloat_Check(iptr))
                    value.u[i] = PyFloat_AS_DOUBLE(iptr);
                else if (PyLong_Check(iptr))
                    value.u[i] = PyLong_AsDouble(iptr);
                else
                    return false;
            }
            return true;
        }
        else if (PyList_Check(load_src.ptr()))
        {
            if (PyList_GET_SIZE(load_src.ptr()) != 2)
                return false;
            for (int i=0; i<2; i++)
            {
                PyObject* iptr = PyList_GET_ITEM(load_src.ptr(), i);
                if (PyFloat_Check(iptr))
                    value.u[i] = PyFloat_AS_DOUBLE(iptr);
                else if (PyLong_Check(iptr))
                    value.u[i] = PyLong_AsDouble(iptr);
                else
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    static handle cast(const Vector2Tpl<T>& src, return_value_policy /* policy */, handle /* parent */)
    {
        CCTRACE("Vector2Tpl<T> cast");
        tuple tup = make_tuple(src.x, src.y);
        return tup.inc_ref();
    }
};

//template<> struct type_caster<CCVector3 >
//{
//public:
//PYBIND11_TYPE_CASTER(CCVector3, _("CCVector3"));
//
//    bool load(handle src, bool)
//    {
//        CCTRACE("CCVector3 load");
//        if(!src)
//        {
//            return false;
//        }
//        object temp;
//        handle load_src = src;
//
//        if (!PyTuple_Check(load_src.ptr()))
//            return false;
//        if (PyTuple_GET_SIZE(load_src.ptr()) != 3)
//            return false;
//        for (int i=0; i<3; i++)
//        {
//            PyObject* iptr = PyTuple_GET_ITEM(load_src.ptr(), i);
//            if (PyFloat_Check(iptr))
//                value.u[i] = PyFloat_AS_DOUBLE(iptr);
//            else if (PyLong_Check(iptr))
//                value.u[i] = PyLong_AsDouble(iptr);
//            else
//                return false;
//        }
//        return true;
//    }
//
//    static handle cast(const CCVector3& src, return_value_policy /* policy */, handle /* parent */)
//    {
//        CCTRACE("CCVector3 cast");
//        tuple tup = make_tuple(src.x, src.y, src.z);
//        return tup.inc_ref();
//    }
//};
//

//template<> struct type_caster<CCVector2 >
//{
//public:
//PYBIND11_TYPE_CASTER(CCVector2, _("CCVector2"));
//
//    bool load(handle src, bool)
//    {
//        CCTRACE("CCVector2 load");
//        if(!src)
//        {
//            return false;
//        }
//        object temp;
//        handle load_src = src;
//
//        if (!PyTuple_Check(load_src.ptr()))
//            return false;
//        if (PyTuple_GET_SIZE(load_src.ptr()) != 2)
//            return false;
//        for (int i=0; i<2; i++)
//        {
//            PyObject* iptr = PyTuple_GET_ITEM(load_src.ptr(), i);
//            if (PyFloat_Check(iptr))
//                value.u[i] = PyFloat_AS_DOUBLE(iptr);
//            else if (PyLong_Check(iptr))
//                value.u[i] = PyLong_AsDouble(iptr);
//            else
//                return false;
//        }
//        return true;
//    }
//
//    static handle cast(const CCVector2& src, return_value_policy /* policy */, handle /* parent */)
//    {
//        CCTRACE("CCVector2 cast");
//        tuple tup = make_tuple(src.x, src.y);
//        return tup.inc_ref();
//    }
//};

}
}

#endif
