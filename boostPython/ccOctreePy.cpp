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

#include <boost/python.hpp>

#include <DgmOctree.h>
#include <ReferenceCloud.h>
#include <ccOctree.h>
#include <CCGeom.h>

#include "ccOctreePy_DocStrings.hpp"

#include "PyScalarType.h"
#include "pyccTrace.h"
#include <QObject>

namespace bp = boost::python;

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

PointDescriptor_persistent_py::PointDescriptor_persistent_py() :
        point(0, 0, 0), pointIndex(0), squareDistd(-1.0)
{};

PointDescriptor_persistent_py::PointDescriptor_persistent_py(const CCCoreLib::DgmOctree::PointDescriptor& pt) :
        point(pt.point->x, pt.point->y, pt.point->z), pointIndex(pt.pointIndex), squareDistd(pt.squareDistd)
{};

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

bp::tuple DgmOctree_findPointNeighbourhood_py(CCCoreLib::DgmOctree& self,
                                              CCVector3 _queryPoint,
                                              CCCoreLib::ReferenceCloud* Yk,
                                              unsigned maxNumberOfNeighbors,
                                              unsigned char level,
                                              double maxSearchDist = 0)
{
    unsigned nbFound;
    double maxSquareDist;
    int finalNeighbourhoodSize;
    nbFound = self.findPointNeighbourhood(&_queryPoint, Yk, maxNumberOfNeighbors, level, maxSquareDist,
                                          maxSearchDist, &finalNeighbourhoodSize);
    bp::tuple res = bp::make_tuple(Yk, nbFound, finalNeighbourhoodSize, maxSquareDist);
    return res;
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

CCCoreLib::DgmOctree::CellCode DgmOctree_getCellCode_py(CCCoreLib::DgmOctree& self,unsigned index)
{
    CCCoreLib::DgmOctree::CellCode code = self.getCellCode(index);
    return code;
}

std::vector<CCCoreLib::DgmOctree::CellCode> DgmOctree_getCellCodes_py(CCCoreLib::DgmOctree& self,
                                                                      unsigned char level,
                                                                      bool truncatedCodes = false)
{
    std::vector<CCCoreLib::DgmOctree::CellCode> vec;
    self.getCellCodes(level, vec, truncatedCodes);
    return vec;
}

std::vector<CCCoreLib::DgmOctree::IndexAndCode> DgmOctree_getCellCodesAndIndexes_py(CCCoreLib::DgmOctree& self,
    unsigned char level,
    bool truncatedCodes = false)
{
    std::vector<CCCoreLib::DgmOctree::IndexAndCode> vec;
    self.getCellCodesAndIndexes(level, vec, truncatedCodes);
    return vec;
}

std::vector<int> DgmOctree_getCellDistanceFromBorders_py(CCCoreLib::DgmOctree& self,
                                                         const Tuple3i& cellPos,
                                                         unsigned char level)
{
    int* cellDists;
    self.getCellDistanceFromBorders(cellPos, level, cellDists);
    std::vector<int> res;
    res.push_back(cellDists[0]); res.push_back(cellDists[1]); res.push_back(cellDists[2]);
    return res;
}

std::vector<int> DgmOctree_getCellDistanceFromBordersN_py(CCCoreLib::DgmOctree& self,
                                                          const Tuple3i& cellPos,
                                                          unsigned char level,
                                                          int neighbourhoodLength)
{
    int* cellDists =0;
    self.getCellDistanceFromBorders(cellPos, level, neighbourhoodLength, cellDists);
    std::vector<int> res;
    if (cellDists)
    {
        res.push_back(cellDists[0]);
        res.push_back(cellDists[1]);
        res.push_back(cellDists[2]);
    }
    return res;
}

std::vector<unsigned int> DgmOctree_getCellIndexes_py(CCCoreLib::DgmOctree& self,
                                                      unsigned char level)
{
    std::vector<unsigned int> vec;
    self.getCellIndexes(level, vec);
    return vec;
}

unsigned DgmOctree_getCellNumber_py(CCCoreLib::DgmOctree& self, unsigned char level)
{
    unsigned num = self.getCellNumber(level);
    return num;
}

double DgmOctree_getCellSize_py(CCCoreLib::DgmOctree& self, unsigned char level)
{
    double res = self.getCellSize(level);
    return res;
}

std::vector<int> DgmOctree_getMaxFillIndexes_py(CCCoreLib::DgmOctree& self, unsigned char level)
{
    std::vector<int> res;
    const int* a = self.getMaxFillIndexes(level);
    res.push_back(a[0]); res.push_back(a[1]); res.push_back(a[2]);
    return res;
}

std::vector<int> DgmOctree_getMinFillIndexes_py(CCCoreLib::DgmOctree& self, unsigned char level)
{
    std::vector<int> res;
    const int* a = self.getMinFillIndexes(level);
    res.push_back(a[0]); res.push_back(a[1]); res.push_back(a[2]);
    return res;
}

CCCoreLib::ReferenceCloud* DgmOctree_getPointsInCell_py(CCCoreLib::DgmOctree& self,
                                                        CCCoreLib::DgmOctree::CellCode cellCode,
                                                        unsigned char level,
                                                        CCCoreLib::ReferenceCloud* subset,
                                                        bool isCodeTruncated = false,
                                                        bool clearOutputCloud = true)
{
    self.getPointsInCell(cellCode, level, subset, isCodeTruncated, clearOutputCloud);
    return subset;
}

CCCoreLib::ReferenceCloud* DgmOctree_getPointsInCellByCellIndex_py(CCCoreLib::DgmOctree& self,
                                                                   CCCoreLib::ReferenceCloud* cloud,
                                                                   unsigned cellIndex,
                                                                   unsigned char level,
                                                                   bool clearOutputCloud = true)
{
    self.getPointsInCellByCellIndex(cloud, cellIndex, level, clearOutputCloud);
    return cloud;
}

BOOST_PYTHON_FUNCTION_OVERLOADS(DgmOctree_findPointNeighbourhood_py_overloads, DgmOctree_findPointNeighbourhood_py, 5, 6)
BOOST_PYTHON_FUNCTION_OVERLOADS(DgmOctree_getCellCodes_py_overloads, DgmOctree_getCellCodes_py, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(DgmOctree_getCellCodesAndIndexes_py_overloads, DgmOctree_getCellCodesAndIndexes_py, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(DgmOctree_getPointsInCellByCellIndex_py_overloads, DgmOctree_getPointsInCellByCellIndex_py, 4, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(DgmOctree_getPointsInCell_py_overloads, DgmOctree_getPointsInCell_py, 4, 6)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getPointsInCellsWithSortedCellCodes_overloads,
                                       CCCoreLib::DgmOctree::getPointsInCellsWithSortedCellCodes, 3, 4)

void export_ccOctree()
{
    class_<PointDescriptor_persistent_py>("PointDescriptor", DgmOctree_PointDescriptor_doc)
        .add_property("point", make_getter(&PointDescriptor_persistent_py::point,
                      return_value_policy<return_by_value>()))
        .def_readonly("pointIndex", &PointDescriptor_persistent_py::pointIndex)
        .def_readonly("squareDistd", &PointDescriptor_persistent_py::squareDistd)
        ;

    class_<CCCoreLib::DgmOctree::CellDescriptor>("CellDescriptor", DgmOctree_CellDescriptor_doc)
        .def_readonly("center", &CCCoreLib::DgmOctree::CellDescriptor::center)
        .def_readonly("index", &CCCoreLib::DgmOctree::CellDescriptor::index)
        ;

    class_<CCCoreLib::DgmOctree::IndexAndCode>("IndexAndCode", DgmOctree_IndexAndCode_doc)
                .def_readonly("theIndex", &CCCoreLib::DgmOctree::IndexAndCode::theIndex)
                .def_readonly("theCode", &CCCoreLib::DgmOctree::IndexAndCode::theCode)
                .def("codeComp", &CCCoreLib::DgmOctree::IndexAndCode::codeComp, DgmOctree_codeComp_doc)
                    .staticmethod("codeComp")
                .def("indexComp", &CCCoreLib::DgmOctree::IndexAndCode::indexComp, DgmOctree_indexComp_doc)
                    .staticmethod("indexComp")
                .def(self < self)
                .def(self > self)
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
        .def("findBestLevelForAGivenCellNumber",
             &CCCoreLib::DgmOctree::findBestLevelForAGivenCellNumber,
             DgmOctree_findBestLevelForAGivenCellNumber_doc)
        .def("findBestLevelForAGivenNeighbourhoodSizeExtraction",
             &CCCoreLib::DgmOctree::findBestLevelForAGivenNeighbourhoodSizeExtraction,
             DgmOctree_findBestLevelForAGivenNeighbourhoodSizeExtraction_doc)
        .def("findBestLevelForAGivenPopulationPerCell",
             &CCCoreLib::DgmOctree::findBestLevelForAGivenPopulationPerCell,
             DgmOctree_findBestLevelForAGivenPopulationPerCell_doc)
        .def("findPointNeighbourhood", &DgmOctree_findPointNeighbourhood_py,
             DgmOctree_findPointNeighbourhood_py_overloads(DgmOctree_findPointNeighbourhood_doc))
        .def("getCellCode", &DgmOctree_getCellCode_py, DgmOctree_getCellCode_doc)
        .def("getCellDistanceFromBorders", &DgmOctree_getCellDistanceFromBorders_py, DgmOctree_getCellDistanceFromBorders_doc)
        .def("getCellDistanceFromBorders", &DgmOctree_getCellDistanceFromBordersN_py, DgmOctree_getCellDistanceFromBordersN_doc)
        .def("getCellNumber", &DgmOctree_getCellNumber_py, DgmOctree_getCellNumber_doc)
        .def("getBoundingBox", &DgmOctree_getBoundingBox_py, DgmOctree_getBoundingBox_doc)
        .def("getCellCodes", &DgmOctree_getCellCodes_py, DgmOctree_getCellCodes_py_overloads(DgmOctree_getCellCodes_doc))
        .def("getCellCodesAndIndexes", &DgmOctree_getCellCodesAndIndexes_py,
             DgmOctree_getCellCodesAndIndexes_py_overloads(DgmOctree_getCellCodesAndIndexes_doc))
        .def("getCellindexes", &DgmOctree_getCellIndexes_py, DgmOctree_getCellIndexes_doc)
        .def("getCellSize", &DgmOctree_getCellSize_py, DgmOctree_getCellSize_doc)
        .def("getMaxFillIndexes", &DgmOctree_getMaxFillIndexes_py, DgmOctree_getMaxFillIndexes_doc)
        .def("getMinFillIndexes", &DgmOctree_getMinFillIndexes_py, DgmOctree_getMinFillIndexes_doc)
        .def("getNumberOfProjectedPoints",
             &CCCoreLib::DgmOctree::getNumberOfProjectedPoints,
             DgmOctree_getNumberOfProjectedPoints_doc)
        .def("getOctreeMins", &CCCoreLib::DgmOctree::getOctreeMins,
             DgmOctree_getOctreeMins_doc, return_value_policy<reference_existing_object>())
        .def("getOctreeMaxs", &CCCoreLib::DgmOctree::getOctreeMaxs,
             DgmOctree_getOctreeMaxs_doc, return_value_policy<reference_existing_object>())
        .def("getPointsInBoxNeighbourhood",
             &DgmOctree_getPointsInBoxNeighbourhood_py,
             DgmOctree_getPointsInBoxNeighbourhood_doc)
        .def("getPointsInCell", &DgmOctree_getPointsInCell_py,
             return_value_policy<reference_existing_object>(),
             DgmOctree_getPointsInCell_py_overloads(DgmOctree_getPointsInCell_doc))
        .def("getPointsInCellByCellIndex", &DgmOctree_getPointsInCellByCellIndex_py,
             return_value_policy<reference_existing_object>(),
             DgmOctree_getPointsInCellByCellIndex_py_overloads(DgmOctree_getPointsInCellByCellIndex_doc))
        .def("getPointsInCellsWithSortedCellCodes", &CCCoreLib::DgmOctree::getPointsInCellsWithSortedCellCodes,
             getPointsInCellsWithSortedCellCodes_overloads(DgmOctree_getPointsInCellsWithSortedCellCodes_doc)
             [return_value_policy<reference_existing_object>()])
        .def("getPointsInCylindricalNeighbourhood",
             &DgmOctree_getPointsInCylindricalNeighbourhood_py,
             DgmOctree_getPointsInCylindricalNeighbourhood_doc)
        .def("getPointsInSphericalNeighbourhood",
             &DgmOctree_getPointsInSphericalNeighbourhood_py,
             DgmOctree_getPointsInSphericalNeighbourhood_doc)
        ;

    class_<ccOctree, QSharedPointer<ccOctree>, bases<CCCoreLib::DgmOctree>, boost::noncopyable>("ccOctree", ccOctree_ccOctree_doc, no_init)
        ;
}
