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
#include <CCGeom.h>

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
    return p.data();
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

PointDescriptor_persistent_py::PointDescriptor_persistent_py() :
        point(0, 0, 0), pointIndex(0), squareDistd(-1.0)
{};


PointDescriptor_persistent_py::PointDescriptor_persistent_py(const CCCoreLib::DgmOctree::PointDescriptor& pt) :
        point(pt.point->x, pt.point->y, pt.point->z), pointIndex(pt.pointIndex), squareDistd(pt.squareDistd)
{};

struct CylindricalNeighbourhood_py
{
    CCVector3 center;
    CCVector3 dir;
    PointCoordinateType radius;
    PointCoordinateType maxHalfLength;
    unsigned char level;
    bool onlyPositiveDir;
    CylindricalNeighbourhood_py()
        : center(0,0,0)
        , dir(0,0,1)
        , radius(0)
        , maxHalfLength(0)
        , level(0)
        , onlyPositiveDir(false)
    {CCTRACE("CylindricalNeighbourhood_py center " << center.x << " " << center.y << " " << center.z);}
};

struct BoxNeighbourhood_py
{
    CCVector3 center;
    CCVector3* axes;
    CCVector3 dimensions;
    unsigned char level;
    BoxNeighbourhood_py() :
            center(0, 0, 0), axes(nullptr), dimensions(0, 0, 0), level(0)
    {CCTRACE("BoxNeighbourhood_py center " << center.x << " " << center.y << " " << center.z);}
};

std::vector<PointDescriptor_persistent_py>
DgmOctree_getPointsInSphericalNeighbourhood_py(CCCoreLib::DgmOctree& self,
                                               const CCVector3& sphereCenter,
                                               PointCoordinateType radius,
                                               unsigned char level)
{
    std::vector<CCCoreLib::DgmOctree::PointDescriptor> neighbours;
    self.getPointsInSphericalNeighbourhood(sphereCenter, radius, neighbours, level);
    std::vector<PointDescriptor_persistent_py> pn;
    for (const auto v : neighbours)
    {
        pn.push_back(PointDescriptor_persistent_py(v));
    }
    return pn;
}

std::vector<PointDescriptor_persistent_py>
DgmOctree_getPointsInCylindricalNeighbourhood_py(CCCoreLib::DgmOctree& self,
                                                 const CylindricalNeighbourhood_py& params)
{
    CCCoreLib::DgmOctree::CylindricalNeighbourhood cpar;
    cpar.center = params.center;
    cpar.dir = params.dir;
    cpar.radius = params.radius;
    cpar.maxHalfLength = params.maxHalfLength;
    cpar.level = params.level;
    cpar.onlyPositiveDir = params.onlyPositiveDir;
    self.getPointsInCylindricalNeighbourhood(cpar);
    std::vector<PointDescriptor_persistent_py> pn;
    for (const auto v : cpar.neighbours)
    {
        pn.push_back(PointDescriptor_persistent_py(v));
    }
    return pn;
}

std::vector<PointDescriptor_persistent_py>
DgmOctree_getPointsInBoxNeighbourhood_py(CCCoreLib::DgmOctree& self,
                                         const BoxNeighbourhood_py& params)
{
    CCCoreLib::DgmOctree::BoxNeighbourhood bpar;
    bpar.center = params.center;
    bpar.axes = params.axes; // TODO: check and copy
    bpar.dimensions = params.dimensions;
    bpar.level = params.level;
    self.getPointsInBoxNeighbourhood(bpar);
    std::vector<PointDescriptor_persistent_py> pn;
    for (const auto v : bpar.neighbours)
    {
        pn.push_back(PointDescriptor_persistent_py(v));
    }
    return pn;
}

void export_ccOctree()
{
    class_<PointDescriptor_persistent_py>("PointDescriptor", DgmOctree_PointDescriptor_doc)
        .add_property("point", make_getter(&PointDescriptor_persistent_py::point,
                      return_value_policy<return_by_value>()))
        .def_readonly("pointIndex", &PointDescriptor_persistent_py::pointIndex)
        .def_readonly("squareDistd", &PointDescriptor_persistent_py::squareDistd)
        ;

    class_<CylindricalNeighbourhood_py>("CylindricalNeighbourhood_py", DgmOctree_CylindricalNeighbourhood_doc)
        .add_property("center",
                      make_getter(&CylindricalNeighbourhood_py::center,
                      return_value_policy<return_by_value>()),
                      make_setter(&CylindricalNeighbourhood_py::center))
        .add_property("dir",
                      make_getter(&CylindricalNeighbourhood_py::dir,
                      return_value_policy<return_by_value>()),
                      make_setter(&CylindricalNeighbourhood_py::dir))
        .def_readwrite("radius", &CylindricalNeighbourhood_py::radius)
        .def_readwrite("maxHalfLength", &CylindricalNeighbourhood_py::maxHalfLength)
        .def_readwrite("level", &CylindricalNeighbourhood_py::level)
        .def_readwrite("onlyPositiveDir", &CylindricalNeighbourhood_py::onlyPositiveDir)
        ;

    class_<BoxNeighbourhood_py>("BoxNeighbourhood_py", DgmOctree_BoxNeighbourhood_doc)
        .add_property("center",
                      make_getter(&BoxNeighbourhood_py::center,
                      return_value_policy<return_by_value>()),
                      make_setter(&BoxNeighbourhood_py::center))
        // TODO: manage axes
        .add_property("dimensions",
                      make_getter(&BoxNeighbourhood_py::dimensions,
                      return_value_policy<return_by_value>()),
                      make_setter(&BoxNeighbourhood_py::dimensions))
        .def_readwrite("level", &BoxNeighbourhood_py::level)
        ;

    class_<CCCoreLib::DgmOctree>("DgmOctree", no_init)
        .def("findBestLevelForAGivenNeighbourhoodSizeExtraction",
              &CCCoreLib::DgmOctree::findBestLevelForAGivenNeighbourhoodSizeExtraction,
              DgmOctree_findBestLevelForAGivenNeighbourhoodSizeExtraction_doc)
        .def("getNumberOfProjectedPoints",
             &CCCoreLib::DgmOctree::getNumberOfProjectedPoints,
             DgmOctree_getNumberOfProjectedPoints_doc)
        .def("getBoundingBox", &DgmOctree_getBoundingBox_py, DgmOctree_getBoundingBox_doc)
        .def("getPointsInBoxNeighbourhood",
             &DgmOctree_getPointsInBoxNeighbourhood_py,
             DgmOctree_getPointsInBoxNeighbourhood_doc)
        .def("getPointsInCylindricalNeighbourhood",
             &DgmOctree_getPointsInCylindricalNeighbourhood_py,
             DgmOctree_getPointsInCylindricalNeighbourhood_doc)
        .def("getPointsInSphericalNeighbourhood",
             &DgmOctree_getPointsInSphericalNeighbourhood_py,
             DgmOctree_getPointsInSphericalNeighbourhood_doc)
        ;


//    findPointNeighbourhood
//    getPointsInCylindricalNeighbourhood
//    getPointsInCylindricalNeighbourhoodProgressive
//    getPointsInBoxNeighbourhood

    class_<ccOctree, QSharedPointer<ccOctree>, bases<CCCoreLib::DgmOctree>, boost::noncopyable>("ccOctree", ccOctree_ccOctree_doc, no_init)
        ;
}
