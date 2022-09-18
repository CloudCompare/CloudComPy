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

#include "cloudComPy.hpp"

#include <DgmOctree.h>

#include <ReferenceCloud.h>
#include <ccOctree.h>
#include <CCGeom.h>
#include <pybind11/detail/type_caster_base.h>

#include "ccOctreePy_DocStrings.hpp"

#include "PyScalarType.h"
#include "pyccTrace.h"
#include <QObject>
#include <QSharedPointer>

struct PointDescriptor_persistent_py
{
    const CCVector3 point;
    unsigned pointIndex;
    double squareDistd;
    PointDescriptor_persistent_py();
    PointDescriptor_persistent_py(const CCCoreLib::DgmOctree::PointDescriptor& pt);
};

//template <typename T>
//T* get_pointer(QSharedPointer<T> const& p)
//{
//    return p.data();
//}
//
//namespace boost
//{
//    namespace python
//    {
//        template <typename T> struct pointee<QSharedPointer<T> >
//        {
//            typedef T type;
//        };
//    }
//}

PointDescriptor_persistent_py::PointDescriptor_persistent_py() :
        point(0, 0, 0), pointIndex(0), squareDistd(-1.0)
{};

PointDescriptor_persistent_py::PointDescriptor_persistent_py(const CCCoreLib::DgmOctree::PointDescriptor& pt) :
        point(pt.point->x, pt.point->y, pt.point->z), pointIndex(pt.pointIndex), squareDistd(pt.squareDistd)
{};

py::tuple DgmOctree_BoxNeighbourhood_getAxes_py(CCCoreLib::DgmOctree::BoxNeighbourhood& self)
{
    py::tuple res;
    if (self.axes == nullptr)
        res = py::make_tuple(CCVector3(1.,0.,0.), CCVector3(0.,1.,0.), CCVector3(0.,0.,1.));
    else
        res = py::make_tuple(self.axes[0], self.axes[1], self.axes[2]);
    return res;
}

void DgmOctree_BoxNeighbourhood_setAxes_py(CCCoreLib::DgmOctree::BoxNeighbourhood& self, std::vector<CCVector3> theAxes)
{
    if (theAxes.size() != 3)
        throw std::invalid_argument("3 axes required");
    self.axes = new CCVector3[3]; // TODO: memory leak!
    self.axes[0] = theAxes[0];
    self.axes[1] = theAxes[1];
    self.axes[2] = theAxes[2];
}

CCVector3 DgmOctree_computeCellCenter_py(CCCoreLib::DgmOctree& self, CCCoreLib::DgmOctree::CellCode code,
                                         unsigned char level, bool isCodeTruncated = false)
{
    CCVector3 center;
    self.computeCellCenter(code, level, center, isCodeTruncated);
    return center;
}

CCVector3 DgmOctree_computeCellCenter2_py(CCCoreLib::DgmOctree& self, Tuple3i cellPos, unsigned char level)
{
    CCVector3 center;
    self.computeCellCenter(cellPos, level, center);
    return center;
}

py::tuple DgmOctree_findPointNeighbourhood_py(CCCoreLib::DgmOctree& self,
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
    py::tuple res = py::make_tuple(nbFound, finalNeighbourhoodSize, maxSquareDist);
    return res;
}

CCCoreLib::DgmOctree::CellCode DgmOctree_GenerateTruncatedCellCode_py(CCCoreLib::DgmOctree& self,
                                                                      Tuple3i cellPos, unsigned char level)
{
    return self.GenerateTruncatedCellCode(cellPos, level);
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
                                                 CCCoreLib::DgmOctree::CylindricalNeighbourhood& params)
{
    self.getPointsInCylindricalNeighbourhood(params);
    std::vector<PointDescriptor_persistent_py> pn;
    for (const auto v : params.neighbours)
    {
        pn.push_back(PointDescriptor_persistent_py(v));
    }
    return pn;
}

std::vector<PointDescriptor_persistent_py>
DgmOctree_getPointsInCylindricalNeighbourhoodProgressive_py(CCCoreLib::DgmOctree& self,
                                                            CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood& params)
{
    self.getPointsInCylindricalNeighbourhoodProgressive(params);
    std::vector<PointDescriptor_persistent_py> pn;
    for (const auto v : params.neighbours)
    {
        pn.push_back(PointDescriptor_persistent_py(v));
    }
    return pn;
}

std::vector<PointDescriptor_persistent_py>
DgmOctree_getPointsInBoxNeighbourhood_py(CCCoreLib::DgmOctree& self,
                                         CCCoreLib::DgmOctree::BoxNeighbourhood& params)
{
    self.getPointsInBoxNeighbourhood(params);
    std::vector<PointDescriptor_persistent_py> pn;
    for (const auto v : params.neighbours)
    {
        pn.push_back(PointDescriptor_persistent_py(v));
    }
    return pn;
}

CCCoreLib::DgmOctree::CellCode DgmOctree_getCellCode_py(CCCoreLib::DgmOctree& self,unsigned index)
{
    const CCCoreLib::DgmOctree::CellCode code = self.getCellCode(index);
    CCTRACE("index: " << index << " CellCode: " << code);
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
    int cellDists[6];
    self.getCellDistanceFromBorders(cellPos, level, cellDists);
    std::vector<int> res;
    for(int i=0; i<6; i++)
        res.push_back(cellDists[i]);
    return res;
}

std::vector<int> DgmOctree_getCellDistanceFromBordersN_py(CCCoreLib::DgmOctree& self,
                                                          const Tuple3i& cellPos,
                                                          unsigned char level,
                                                          int neighbourhoodLength)
{
    int cellDists[6];
    self.getCellDistanceFromBorders(cellPos, level, neighbourhoodLength, cellDists);
    std::vector<int> res;
    for(int i=0; i<6; i++)
        res.push_back(cellDists[i]);
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

Tuple3i DgmOctree_getCellPos_py(CCCoreLib::DgmOctree& self, CCCoreLib::DgmOctree::CellCode code, unsigned char level, bool isCodeTruncated)
{
    Tuple3i cellPos;
    self.getCellPos(code, level, cellPos, isCodeTruncated);
    return cellPos;
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

CCVector3 DgmOctree_getOctreeMaxs_py(CCCoreLib::DgmOctree& self)
{
    CCVector3 res = self.getOctreeMaxs();
    return res;
}

CCVector3 DgmOctree_getOctreeMins_py(CCCoreLib::DgmOctree& self)
{
    CCVector3 res = self.getOctreeMins();
    return res;
}

Tuple3i DgmOctree_getTheCellPosWhichIncludesThePoint_py(CCCoreLib::DgmOctree& self, CCVector3 thePoint)
{
    Tuple3i cellPos;
    self.getTheCellPosWhichIncludesThePoint(&thePoint, cellPos);
    return cellPos;
}

Tuple3i DgmOctree_getTheCellPosWhichIncludesThePointL_py(CCCoreLib::DgmOctree& self, CCVector3 thePoint, unsigned char level)
{
    Tuple3i cellPos;
    self.getTheCellPosWhichIncludesThePoint(&thePoint, cellPos, level);
    return cellPos;
}

py::tuple DgmOctree_getTheCellPosWhichIncludesThePointLI_py(CCCoreLib::DgmOctree& self, CCVector3 thePoint, unsigned char level)
{
    Tuple3i cellPos;
    bool inBounds;
    self.getTheCellPosWhichIncludesThePoint(&thePoint, cellPos, level, inBounds);
    return py::make_tuple(cellPos, inBounds);
}

//namespace pybind11 { namespace detail { template <> struct move_always<ccOctree> : std::true_type {}; } }
//void try_static_assert() {
//  static_assert(
//      !py::detail::is_copy_constructible<ccOctree>::value,
//      "copy constructible is not OK");  // Compiles
//  static_assert(
//      std::is_move_constructible<ccOctree>::value,
//      "move constructible should be OK");  // Compiles
//}

void export_ccOctree(py::module &m0)
{
    py::class_<PointDescriptor_persistent_py>(m0, "PointDescriptor", DgmOctree_PointDescriptor_doc)
        .def(py::init<>())
        .def_readonly("point", &PointDescriptor_persistent_py::point)
        .def_readonly("pointIndex", &PointDescriptor_persistent_py::pointIndex)
        .def_readonly("squareDistd", &PointDescriptor_persistent_py::squareDistd)
        ;

    py::class_<CCCoreLib::DgmOctree::CellDescriptor>(m0, "CellDescriptor", DgmOctree_CellDescriptor_doc)
        .def(py::init<>())
        .def_readonly("center", &CCCoreLib::DgmOctree::CellDescriptor::center)
        .def_readonly("index", &CCCoreLib::DgmOctree::CellDescriptor::index)
        ;

    py::class_<CCCoreLib::DgmOctree::IndexAndCode>(m0, "IndexAndCode", DgmOctree_IndexAndCode_doc)
        .def(py::init<>())
        .def_readonly("theIndex", &CCCoreLib::DgmOctree::IndexAndCode::theIndex)
        .def_readonly("theCode", &CCCoreLib::DgmOctree::IndexAndCode::theCode)
        .def_static("codeComp", &CCCoreLib::DgmOctree::IndexAndCode::codeComp, DgmOctree_codeComp_doc)
        .def_static("indexComp", &CCCoreLib::DgmOctree::IndexAndCode::indexComp, DgmOctree_indexComp_doc)
        .def(py::self < py::self)
        .def(py::self > py::self)
        ;

    py::class_<CCCoreLib::DgmOctree::CylindricalNeighbourhood>(m0, "CylindricalNeighbourhood", DgmOctree_CylindricalNeighbourhood_doc)
        .def(py::init<>())
        .def_readwrite("center",&CCCoreLib::DgmOctree::CylindricalNeighbourhood::center)
        .def_readwrite("dir", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::dir)
        .def_readwrite("radius", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::radius)
        .def_readwrite("maxHalfLength", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::maxHalfLength)
        .def_readwrite("level", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::level)
        .def_readwrite("onlyPositiveDir", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::onlyPositiveDir)
        ;

    py::class_<CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood,
                CCCoreLib::DgmOctree::CylindricalNeighbourhood >(m0, "ProgressiveCylindricalNeighbourhood",
                                                                   DgmOctree_ProgressiveCylindricalNeighbourhood_doc)
        .def(py::init<>())
        .def_readwrite("currentHalfLength", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::currentHalfLength)
        .def_readwrite("potentialCandidates", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::potentialCandidates)
        .def_readwrite("prevMinCornerPos", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::prevMinCornerPos)
        .def_readwrite("prevMaxCornerPos", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::prevMaxCornerPos)
        ;

    py::class_<CCCoreLib::DgmOctree::BoxNeighbourhood>(m0, "BoxNeighbourhood", DgmOctree_BoxNeighbourhood_doc)
        .def(py::init<>())
        .def_readwrite("center", &CCCoreLib::DgmOctree::BoxNeighbourhood::center)
        .def_property("axes", &DgmOctree_BoxNeighbourhood_getAxes_py, &DgmOctree_BoxNeighbourhood_setAxes_py)
        .def_readwrite("dimensions", &CCCoreLib::DgmOctree::BoxNeighbourhood::dimensions)
        .def_readwrite("level", &CCCoreLib::DgmOctree::BoxNeighbourhood::level)
        ;

    py::class_<CCCoreLib::DgmOctree::NearestNeighboursSearchStruct>(m0, "NearestNeighboursSearchStruct",
                                                                DgmOctree_NearestNeighboursSearchStruct_doc)
        .def(py::init<>())
        .def_readwrite("queryPoint", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::queryPoint)
        .def_readwrite("level", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::level)
        .def_readwrite("minNumberOfNeighbors", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minNumberOfNeighbors)
        .def_readwrite("cellPos", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellPos)
        .def_readwrite("cellCenter", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellCenter)
        .def_readwrite("maxSearchSquareDistd", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::maxSearchSquareDistd)
        .def_readwrite("minimalCellsSetToVisit", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minimalCellsSetToVisit)
        .def_readwrite("pointsInNeighbourhood", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::pointsInNeighbourhood)
        .def_readwrite("alreadyVisitedNeighbourhoodSize",
                       &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::alreadyVisitedNeighbourhoodSize)
        .def_readwrite("theNearestPointIndex", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::theNearestPointIndex)
        ;

    py::class_<CCCoreLib::DgmOctree>(m0, "DgmOctree") //, no_init)
        .def("computeCellCenter", &DgmOctree_computeCellCenter_py,
             py::arg("code"), py::arg("level"), py::arg("isCodeTruncated")=false,
             DgmOctree_computeCellCenter_py_doc)
        .def("computeCellCenter", &DgmOctree_computeCellCenter2_py,DgmOctree_computeCellCenter2_py_doc)
        .def("findBestLevelForAGivenCellNumber",
             &CCCoreLib::DgmOctree::findBestLevelForAGivenCellNumber,
             DgmOctree_findBestLevelForAGivenCellNumber_doc)
        .def("findBestLevelForAGivenNeighbourhoodSizeExtraction",
             &CCCoreLib::DgmOctree::findBestLevelForAGivenNeighbourhoodSizeExtraction,
             DgmOctree_findBestLevelForAGivenNeighbourhoodSizeExtraction_doc)
        .def("findBestLevelForAGivenPopulationPerCell",
             &CCCoreLib::DgmOctree::findBestLevelForAGivenPopulationPerCell,
             DgmOctree_findBestLevelForAGivenPopulationPerCell_doc)
        .def("findNearestNeighborsStartingFromCell",
             &CCCoreLib::DgmOctree::findNearestNeighborsStartingFromCell, DgmOctree_findNearestNeighborsStartingFromCell_doc)
        .def("findNeighborsInASphereStartingFromCell",
             &CCCoreLib::DgmOctree::findNeighborsInASphereStartingFromCell, DgmOctree_findNeighborsInASphereStartingFromCell_doc)
        .def("findPointNeighbourhood", &DgmOctree_findPointNeighbourhood_py,
             py::arg("_queryPoint"), py::arg("Yk"), py::arg("maxNumberOfNeighbors"), py::arg("level"), py::arg("maxSearchDist")=0,
             DgmOctree_findPointNeighbourhood_doc)
        .def("findTheNearestNeighborStartingFromCell",
             &CCCoreLib::DgmOctree::findTheNearestNeighborStartingFromCell, DgmOctree_findTheNearestNeighborStartingFromCell_doc)
        .def("GenerateTruncatedCellCode", &DgmOctree_GenerateTruncatedCellCode_py, DgmOctree_GenerateTruncatedCellCode_doc)
        .def("getBoundingBox", &DgmOctree_getBoundingBox_py, DgmOctree_getBoundingBox_doc)
        .def("getCellCode", &DgmOctree_getCellCode_py, DgmOctree_getCellCode_doc)
        .def("getCellCodes", &DgmOctree_getCellCodes_py,
              py::arg("level"), py::arg("truncatedCodes")=false,
              DgmOctree_getCellCodes_doc)
        .def("getCellCodesAndIndexes", &DgmOctree_getCellCodesAndIndexes_py,
             py::arg("level"), py::arg("truncatedCodes")=false,
             DgmOctree_getCellCodesAndIndexes_doc)
        .def("getCellDistanceFromBorders", &DgmOctree_getCellDistanceFromBorders_py, DgmOctree_getCellDistanceFromBorders_doc)
        .def("getCellDistanceFromBorders", &DgmOctree_getCellDistanceFromBordersN_py, DgmOctree_getCellDistanceFromBordersN_doc)
        .def("getCellIndexes", &DgmOctree_getCellIndexes_py, DgmOctree_getCellIndexes_doc)
        .def("getCellNumber", &DgmOctree_getCellNumber_py, DgmOctree_getCellNumber_doc)
        .def("getCellPos", &DgmOctree_getCellPos_py, DgmOctree_getCellPos_doc)
        .def("getCellSize", &DgmOctree_getCellSize_py, DgmOctree_getCellSize_doc)
        .def("getMaxFillIndexes", &DgmOctree_getMaxFillIndexes_py, DgmOctree_getMaxFillIndexes_doc)
        .def("getMinFillIndexes", &DgmOctree_getMinFillIndexes_py, DgmOctree_getMinFillIndexes_doc)
        .def("getNumberOfProjectedPoints",
             &CCCoreLib::DgmOctree::getNumberOfProjectedPoints,
             DgmOctree_getNumberOfProjectedPoints_doc)
        .def("getOctreeMaxs", &DgmOctree_getOctreeMaxs_py, DgmOctree_getOctreeMaxs_doc)
        .def("getOctreeMins", &DgmOctree_getOctreeMins_py, DgmOctree_getOctreeMins_doc)
        .def("getPointsInBoxNeighbourhood",
             &DgmOctree_getPointsInBoxNeighbourhood_py,
             DgmOctree_getPointsInBoxNeighbourhood_doc)
        .def("getPointsInCell", &CCCoreLib::DgmOctree::getPointsInCell,
             py::arg("cellCode"), py::arg("level"), py::arg("subset"), py::arg("isCodeTruncated")=false, py::arg("clearOutputCloud")=true,
             DgmOctree_getPointsInCell_doc)
        .def("getPointsInCellByCellIndex", &CCCoreLib::DgmOctree::getPointsInCellByCellIndex,
             py::arg("cloud"), py::arg("cellIndex"), py::arg("level"), py::arg("clearOutputCloud")=true,
             DgmOctree_getPointsInCellByCellIndex_doc)
        .def("getPointsInCellsWithSortedCellCodes", &CCCoreLib::DgmOctree::getPointsInCellsWithSortedCellCodes,
             py::arg("cellCodes"), py::arg("level"), py::arg("subset"), py::arg("areCodesTruncated")=false,
             DgmOctree_getPointsInCellsWithSortedCellCodes_doc, py::return_value_policy::reference)
        .def("getPointsInCylindricalNeighbourhood",
             &DgmOctree_getPointsInCylindricalNeighbourhood_py,
             DgmOctree_getPointsInCylindricalNeighbourhood_doc)
        .def("getPointsInCylindricalNeighbourhoodProgressive",
             &DgmOctree_getPointsInCylindricalNeighbourhoodProgressive_py,
             DgmOctree_getPointsInCylindricalNeighbourhoodProgressive_doc)
        .def("getPointsInSphericalNeighbourhood",
             &DgmOctree_getPointsInSphericalNeighbourhood_py,
             DgmOctree_getPointsInSphericalNeighbourhood_doc)
        .def("getTheCellPosWhichIncludesThePoint", &DgmOctree_getTheCellPosWhichIncludesThePoint_py,
             DgmOctree_getTheCellPosWhichIncludesThePoint_doc)
        .def("getTheCellPosWhichIncludesThePoint", &DgmOctree_getTheCellPosWhichIncludesThePointL_py,
             DgmOctree_getTheCellPosWhichIncludesThePointL_doc)
        .def("getTheCellPosWhichIncludesThePointInbBounds", &DgmOctree_getTheCellPosWhichIncludesThePointLI_py,
             DgmOctree_getTheCellPosWhichIncludesThePointLI_doc)
        ;

    // TODO: missing methods in dgmOctree ?
    // computeCellLimits
    // computeMeanOctreeDensity
    // ComputeMinDistanceToCellBorder
    // diff
    // executeFunctionForAllCellsAtLevel
    // extractCCs executeFunctionForAllCellsStartingAtLevel
    // findBestLevelForComparisonWithOctree
    // MultiThreadSupport
    // pointsAndTheirCellCodes
    // rayCast

    //py::class_<ccOctree, CCCoreLib::DgmOctree, std::unique_ptr<ccOctree, py::nodelete>>(m0, "ccOctree", ccOctree_ccOctree_doc); //        ;, boost::noncopyable>
    //py::class_<ccOctree, CCCoreLib::DgmOctree>(m0, "ccOctree", ccOctree_ccOctree_doc); //        ;, boost::noncopyable>
}
