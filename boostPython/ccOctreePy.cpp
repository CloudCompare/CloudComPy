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

#include "ccOctreePy.hpp"

//#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <DgmOctree.h>
#include <ccOctree.h>
#include "ccOctreePy_DocStrings.hpp"

#include "PyScalarType.h"
#include "pyccTrace.h"
#include <QObject>

namespace bp = boost::python;
//namespace bnp = boost::python::numpy;

using namespace boost::python;

template <typename T>
T* get_pointer(QSharedPointer<T> const& p)
{
    return p.get();
}

namespace boost
{
    namespace python
    {
        template <typename T> struct pointee<QSharedPointer<T> >
        {
            typedef T type;
        };
    }
}

std::vector<Vector3Tpl<float> > DgmOctree_getBoundingBox_py(CCCoreLib::DgmOctree& self)
{
    std::vector<Vector3Tpl<float> > bb;
    Vector3Tpl<float> a, b;
    self.getBoundingBox(a, b);
    bb.push_back(a);
    bb.push_back(b);
    return bb;
}

void export_ccOctree()
{
    class_<CCCoreLib::DgmOctree>("DgmOctree", no_init)
        .def("getNumberOfProjectedPoints", &CCCoreLib::DgmOctree::getNumberOfProjectedPoints, DgmOctree_getNumberOfProjectedPoints_doc)
        .def("getBoundingBox", &DgmOctree_getBoundingBox_py, DgmOctree_getBoundingBox_doc)
        ;

    class_<ccOctree, QSharedPointer<ccOctree>, bases<CCCoreLib::DgmOctree>, boost::noncopyable>("ccOctree", ccOctree_ccOctree_doc, no_init)
        ;
}
