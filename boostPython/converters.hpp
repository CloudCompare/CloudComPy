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

#include "pyccTrace.h"

namespace
{

struct QString_to_python_str
{
    static PyObject* convert(const QString& s)
    {
        return boost::python::incref(boost::python::object(s.toUtf8().constData()).ptr());
    }
};

struct QString_from_python_str
{
    QString_from_python_str()
    {
        CCTRACE("register QString_from_python_str");
        boost::python::converter::registry::push_back(&convertible, &construct, boost::python::type_id<QString>());
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
    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        CCTRACE("construct");
        // Extract the character data from the python string
        const char* value = PyUnicode_AsUTF8(obj_ptr);

        // Verify that obj_ptr is a string (should be ensured by convertible())
        if (value == 0) boost::python::throw_error_already_set();

        // Grab pointer to memory into which to construct the new QString
        void* storage = ((boost::python::converter::rvalue_from_python_storage<QString>*) data)->storage.bytes;

        // in-place construct the new QString using the character data
        // extracted from the Python object
        new (storage) QString(value);

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

    // register the from-python converter
    QString_from_python_str();
}

} //namespace anonymous

#endif /* CONVERTERS_HPP_ */

