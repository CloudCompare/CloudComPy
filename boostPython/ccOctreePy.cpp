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

bp::tuple DgmOctree_BoxNeighbourhood_getAxes_py(CCCoreLib::DgmOctree::BoxNeighbourhood& self)
{
    if (self.axes == nullptr)
        return bp::make_tuple(CCVector3(1.,0.,0.), CCVector3(0.,1.,0.), CCVector3(0.,0.,1.));
    bp::tuple res = bp::make_tuple(self.axes[0], self.axes[1], self.axes[2]);
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
    int cellDists[3];
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

bp::tuple DgmOctree_getTheCellPosWhichIncludesThePointLI_py(CCCoreLib::DgmOctree& self, CCVector3 thePoint, unsigned char level)
{
    Tuple3i cellPos;
    bool inBounds;
    self.getTheCellPosWhichIncludesThePoint(&thePoint, cellPos, level, inBounds);
    return bp::make_tuple(cellPos, inBounds);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(DgmOctree_computeCellCenter_py_overloads, DgmOctree_computeCellCenter_py, 3, 4)
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

    class_<CCCoreLib::DgmOctree::CylindricalNeighbourhood>("CylindricalNeighbourhood", DgmOctree_CylindricalNeighbourhood_doc)
        .add_property("center",
                      make_getter(&CCCoreLib::DgmOctree::CylindricalNeighbourhood::center,
                      return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::CylindricalNeighbourhood::center))
        .add_property("dir",
                      make_getter(&CCCoreLib::DgmOctree::CylindricalNeighbourhood::dir,
                      return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::CylindricalNeighbourhood::dir))
        .def_readwrite("radius", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::radius)
        .def_readwrite("maxHalfLength", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::maxHalfLength)
        .def_readwrite("level", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::level)
        .def_readwrite("onlyPositiveDir", &CCCoreLib::DgmOctree::CylindricalNeighbourhood::onlyPositiveDir)
        ;

    class_<CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood,
           bases<CCCoreLib::DgmOctree::CylindricalNeighbourhood> >("ProgressiveCylindricalNeighbourhood",
                                                                   DgmOctree_ProgressiveCylindricalNeighbourhood_doc)
        .def_readwrite("currentHalfLength", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::currentHalfLength)
        .def_readwrite("potentialCandidates", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::potentialCandidates)
        .def_readwrite("prevMinCornerPos", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::prevMinCornerPos)
        .def_readwrite("prevMaxCornerPos", &CCCoreLib::DgmOctree::ProgressiveCylindricalNeighbourhood::prevMaxCornerPos)
        ;

    class_<CCCoreLib::DgmOctree::BoxNeighbourhood>("BoxNeighbourhood", DgmOctree_BoxNeighbourhood_doc)
        .add_property("center",
                      make_getter(&CCCoreLib::DgmOctree::BoxNeighbourhood::center,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::BoxNeighbourhood::center))
        .add_property("axes", &DgmOctree_BoxNeighbourhood_getAxes_py, &DgmOctree_BoxNeighbourhood_setAxes_py)
        .add_property("dimensions",
                      make_getter(&CCCoreLib::DgmOctree::BoxNeighbourhood::dimensions,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::BoxNeighbourhood::dimensions))
        .def_readwrite("level", &CCCoreLib::DgmOctree::BoxNeighbourhood::level)
        ;

    class_<CCCoreLib::DgmOctree::NearestNeighboursSearchStruct>("NearestNeighboursSearchStruct",
                                                                DgmOctree_NearestNeighboursSearchStruct_doc)
        .add_property("queryPoint",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::queryPoint,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::queryPoint))
        .def_readwrite("level", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::level)
        .def_readwrite("minNumberOfNeighbors", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minNumberOfNeighbors)
        .add_property("cellPos",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellPos,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellPos))
        .add_property("cellCenter",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellCenter,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::cellCenter))
        .def_readwrite("maxSearchSquareDistd", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::maxSearchSquareDistd)
        .add_property("minimalCellsSetToVisit",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minimalCellsSetToVisit,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::minimalCellsSetToVisit))
        .add_property("pointsInNeighbourhood",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::pointsInNeighbourhood,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::pointsInNeighbourhood))
        .def_readwrite("alreadyVisitedNeighbourhoodSize",
                       &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::alreadyVisitedNeighbourhoodSize)
        .def_readwrite("theNearestPointIndex", &CCCoreLib::DgmOctree::NearestNeighboursSearchStruct::theNearestPointIndex)
        ;

    class_< CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct,
            bases<CCCoreLib::DgmOctree::NearestNeighboursSearchStruct > >("NearestNeighboursSphericalSearchStruct",
                                                                          DgmOctree_NearestNeighboursSphericalSearchStruct_doc)
#ifdef TEST_CELLS_FOR_SPHERICAL_NN
        .add_property("pointsInSphericalNeighbourhood",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::pointsInSphericalNeighbourhood,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::pointsInSphericalNeighbourhood))
        .add_property("cellsInNeighbourhood",
                      make_getter(&CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::cellsInNeighbourhood,
                                  return_value_policy<return_by_value>()),
                      make_setter(&CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::cellsInNeighbourhood))
        .def_readwrite("maxInD2", &CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::maxInD2)
        .def_readwrite("minOutD2", &CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::minOutD2)
#endif
        .def_readwrite("ready", &CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::ready)
        .def("prepare", &CCCoreLib::DgmOctree::NearestNeighboursSphericalSearchStruct::prepare,
             NearestNeighboursSphericalSearchStruct_prepare_doc)
        ;

    class_<CCCoreLib::DgmOctree>("DgmOctree", no_init)
        .def("computeCellCenter", &DgmOctree_computeCellCenter_py,
             DgmOctree_computeCellCenter_py_overloads(DgmOctree_computeCellCenter_py_doc))
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
             DgmOctree_findPointNeighbourhood_py_overloads(DgmOctree_findPointNeighbourhood_doc))
        .def("findTheNearestNeighborStartingFromCell",
             &CCCoreLib::DgmOctree::findTheNearestNeighborStartingFromCell, DgmOctree_findTheNearestNeighborStartingFromCell_doc)
        .def("GenerateTruncatedCellCode", &DgmOctree_GenerateTruncatedCellCode_py, DgmOctree_GenerateTruncatedCellCode_doc)
        .def("getBoundingBox", &DgmOctree_getBoundingBox_py, DgmOctree_getBoundingBox_doc)
        .def("getCellCode", &DgmOctree_getCellCode_py, DgmOctree_getCellCode_doc)
        .def("getCellCodes", &DgmOctree_getCellCodes_py, DgmOctree_getCellCodes_py_overloads(DgmOctree_getCellCodes_doc))
        .def("getCellCodesAndIndexes", &DgmOctree_getCellCodesAndIndexes_py,
             DgmOctree_getCellCodesAndIndexes_py_overloads(DgmOctree_getCellCodesAndIndexes_doc))
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
        .def("getPointsInCell", &DgmOctree_getPointsInCell_py,
             DgmOctree_getPointsInCell_py_overloads(DgmOctree_getPointsInCell_doc)
             [return_value_policy<reference_existing_object>()])
        .def("getPointsInCellByCellIndex", &DgmOctree_getPointsInCellByCellIndex_py,
             DgmOctree_getPointsInCellByCellIndex_py_overloads(DgmOctree_getPointsInCellByCellIndex_doc)
             [return_value_policy<reference_existing_object>()])
        .def("getPointsInCellsWithSortedCellCodes", &CCCoreLib::DgmOctree::getPointsInCellsWithSortedCellCodes,
             getPointsInCellsWithSortedCellCodes_overloads(DgmOctree_getPointsInCellsWithSortedCellCodes_doc)
             [return_value_policy<reference_existing_object>()])
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

    class_<ccOctree, QSharedPointer<ccOctree>, bases<CCCoreLib::DgmOctree>, boost::noncopyable>("ccOctree", ccOctree_ccOctree_doc, no_init)
        ;
}
