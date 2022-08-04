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



}
}

#endif
