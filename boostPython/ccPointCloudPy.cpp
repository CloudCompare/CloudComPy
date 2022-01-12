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

#include "ccPointCloudPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/exception_translator.hpp>
#include <exception>
#include <Python.h>

#include <ccPointCloud.h>
#include <ccPolyline.h>
#include <ccScalarField.h>
#include <GenericProgressCallback.h>
#include "ccColorScale.h"
#include "ccColorScalesManager.h"

#include "PyScalarType.h"
#include "pyccTrace.h"
#include "ccPointCloudPy_DocStrings.hpp"

#include <map>
#include <QColor>

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;


struct color_exception : std::exception
{
  const char* what() const noexcept { return "this point cloud has no color table!"; }
};

struct colorSize_exception : std::exception
{
  const char* what() const noexcept { return "the color array has not the same size as this cloud"; }
};

void translate(color_exception const& e)
{
    // Use the Python 'C' API to set up an exception object
    PyErr_SetString(PyExc_RuntimeError, e.what());
}

bool exportCoordToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportCoordToSF(b);
}

bool exportNormalToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportNormalToSF(b);
}

void coordsFromNPArray_copy(ccPointCloud &self, bnp::ndarray const & array)
{
    if (array.get_dtype() != bnp::dtype::get_builtin<PointCoordinateType>())
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array data type");
        bp::throw_error_already_set();
    }
    if (array.get_nd() != 2)
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array dimension");
        bp::throw_error_already_set();
    }
    if (array.shape(1) != 3)
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array, 3 coordinates required");
        bp::throw_error_already_set();
    }
    size_t nRows = array.shape(0);
    self.reserve(nRows);
    self.resize(nRows);
    PointCoordinateType *s = reinterpret_cast<PointCoordinateType*>(array.get_data());
    PointCoordinateType *d = (PointCoordinateType*)self.getPoint(0);
    memcpy(d, s, 3*nRows*sizeof(PointCoordinateType));
    CCTRACE("copied " << 3*nRows*sizeof(PointCoordinateType));
}

void colorsFromNPArray_copy(ccPointCloud &self, bnp::ndarray const & array)
{
    if (array.get_dtype() != bnp::dtype::get_builtin<ColorCompType>())
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array data type");
        bp::throw_error_already_set();
    }
    if (array.get_nd() != 2)
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array dimension");
        bp::throw_error_already_set();
    }
    if (array.shape(1) != 4)
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array, 4 components required");
        bp::throw_error_already_set();
    }
    size_t nRows = array.shape(0);
    if (nRows != self.size())
    {
    	CCTRACE("the color array has not the same size as this cloud!")
		throw colorSize_exception();
    }
    self.resizeTheRGBTable(false);
    if (self.rgbaColors() == nullptr)
    {
    	CCTRACE("no color table in this point cloud!")
		throw color_exception();
    }
    ColorCompType* s = reinterpret_cast<ColorCompType*>(array.get_data());
    ColorCompType* d = (ColorCompType*)(self.rgbaColors()->data());
//	unsigned count = self.size();
//	for (unsigned i = 0; i < count; ++i)
//    {
//		self.rgbaColors()->setValue(i, ccColor::Rgba(s[4*i], s[4*i+1], s[4*i+2], s[4*i+3]));
//    }
    memcpy(d, s, 4*nRows*sizeof(ColorCompType));
    CCTRACE("copied " << 4*nRows*sizeof(ColorCompType));
    self.colorsHaveChanged();
}

std::map<QString, int> getScalarFieldDic_py(ccPointCloud &self)
{
    std::map<QString, int> mapSF;
    int nbSF = self.getNumberOfScalarFields();
    for (int i=0; i < nbSF; i++)
    {
        mapSF[self.getScalarFieldName(i)] = i;
    }
    return mapSF;
}

CCCoreLib::ScalarField* getScalarFieldByName_py(ccPointCloud &self, const QString& name)
{
    int nbSF = self.getNumberOfScalarFields();
    for (int i=0; i < nbSF; i++)
    {
        if (self.getScalarFieldName(i) == name)
        {
            return self.getScalarField(i);
        }
    }
    return nullptr;
}

bnp::ndarray CoordsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray with copy, ownership transfered to Python");
    bnp::dtype dt = bnp::dtype::get_builtin<PointCoordinateType>(); // coordinates always in simple precision
    size_t nRows = self.size();
    bp::tuple shape = bp::make_tuple(nRows, 3);
    bp::tuple stride = bp::make_tuple(3*sizeof(PointCoordinateType), sizeof(PointCoordinateType));
    PointCoordinateType *s = (PointCoordinateType*)self.getPoint(0);
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result.copy();
}

bnp::ndarray CoordsToNpArray_py(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray without copy, ownership stays in C++");
    bnp::dtype dt = bnp::dtype::get_builtin<PointCoordinateType>(); // coordinates always in simple precision
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    bp::tuple shape = bp::make_tuple(nRows, 3);
    bp::tuple stride = bp::make_tuple(3*sizeof(PointCoordinateType), sizeof(PointCoordinateType));
    PointCoordinateType *s = (PointCoordinateType*)self.getPoint(0);
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result;
}

bnp::ndarray ColorsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("ColorsToNpArray with copy, ownership transfered to Python");
    if (self.rgbaColors() == nullptr)
    {
    	CCTRACE("no color in this point cloud!")
		throw color_exception();
    }
    bnp::dtype dt = bnp::dtype::get_builtin<ColorCompType>(); // colors components (unsigned char)
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    bp::tuple shape = bp::make_tuple(nRows, 4); // r, g, b a
    bp::tuple stride = bp::make_tuple(4*sizeof(ColorCompType), sizeof(ColorCompType));
    ColorCompType *s = (ColorCompType*)(self.rgbaColors()->data());
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result.copy();
}

bnp::ndarray ColorsToNpArray_py(ccPointCloud &self)
{
    CCTRACE("ColorsToNpArray without copy, ownership stays in C++");
    if (self.rgbaColors() == nullptr)
    {
    	CCTRACE("no color in this point cloud!")
		throw color_exception();
    }
    bnp::dtype dt = bnp::dtype::get_builtin<ColorCompType>(); // colors components (unsigned char)
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    bp::tuple shape = bp::make_tuple(nRows, 4); // r, g, b a
    bp::tuple stride = bp::make_tuple(4*sizeof(ColorCompType), sizeof(ColorCompType));
    ColorCompType *s = (ColorCompType*)(self.rgbaColors()->data());
    bnp::ndarray result = bnp::from_data(s, dt, shape, stride, bp::object());
    return result;
}

ccPointCloud* crop2D_py(ccPointCloud &self, const ccPolyline* poly, unsigned char orthoDim, bool inside = true)
{
    ccPointCloud* croppedCloud = nullptr;
    CCTRACE("ortho dim " <<  orthoDim);
    CCCoreLib::ReferenceCloud* ref = self.crop2D(poly, orthoDim, inside);
    if (ref && (ref->size() != 0))
    {
        croppedCloud = self.partialClone(ref);
        delete ref;
        ref = nullptr;
    }
    return croppedCloud;
}

void fuse_py(ccPointCloud &self, ccPointCloud* other)
{
    self += other;
}

bp::tuple partialClone_py(ccPointCloud &self,
                          const CCCoreLib::ReferenceCloud* selection)
{
    int warnings;
    ccPointCloud* cloud = self.partialClone(selection, &warnings);
    bp::tuple res = bp::make_tuple(cloud, warnings);
    return res;
}

bool setColorGradientDefault_py(ccPointCloud &self, unsigned char heightDim)
{
    ccColorScale::Shared colorScale(nullptr);
    colorScale = ccColorScalesManager::GetDefaultScale();
    bool success = self.setRGBColorByHeight(heightDim, colorScale);
    return success;
}

bool setColorGradient_py(ccPointCloud &self, unsigned char heightDim, QColor first, QColor second)
{
    ccColorScale::Shared colorScale(nullptr);
    colorScale = ccColorScale::Create("Temp scale");
    colorScale->insert(ccColorScaleElement(0.0, first), false);
    colorScale->insert(ccColorScaleElement(1.0, second), true);
    bool success = self.setRGBColorByHeight(heightDim, colorScale);
    return success;
}

bool setColorGradientBanded_py(ccPointCloud &self, unsigned char heightDim, double frequency)
{
    bool success = self.setRGBColorByBanding(heightDim, frequency);
    return success;
}

int (ccPointCloud::*addScalarFieldt)(const char*) = &ccPointCloud::addScalarField;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ccPointCloud_scale_overloads, scale, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ccPointCloud_cloneThis_overloads, cloneThis, 0, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ccPointCloud_colorize_overloads, colorize, 3, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(filterPointsByScalarValue_overloads, ccPointCloud::filterPointsByScalarValue, 2,3)

void export_ccPointCloud()
{
    enum_<ccPointCloud::CLONE_WARNINGS>("CLONE_WARNINGS")
        .value("WRN_OUT_OF_MEM_FOR_COLORS", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_COLORS)
        .value("WRN_OUT_OF_MEM_FOR_NORMALS", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_NORMALS)
        .value("WRN_OUT_OF_MEM_FOR_SFS", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_SFS)
        .value("WRN_OUT_OF_MEM_FOR_FWF", ccPointCloud::CLONE_WARNINGS::WRN_OUT_OF_MEM_FOR_FWF)
        ;

    class_<ccPointCloud, bases<CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString> > >("ccPointCloud",
                                                                                          ccPointCloudPy_ccPointCloud_doc,
                                                                                          init< optional<QString, unsigned> >())
        .def("addScalarField", addScalarFieldt, ccPointCloudPy_addScalarField_doc)
        .def("applyRigidTransformation", &ccPointCloud::applyRigidTransformation, ccPointCloudPy_applyRigidTransformation_doc)
        .def("cloneThis", &ccPointCloud::cloneThis,
             ccPointCloud_cloneThis_overloads(ccPointCloudPy_cloneThis_doc)[return_value_policy<reference_existing_object>()])
        .def("colorize", &ccPointCloud::colorize, ccPointCloud_colorize_overloads(ccPointCloudPy_colorize_doc))
        .def("computeGravityCenter", &ccPointCloud::computeGravityCenter, ccPointCloudPy_computeGravityCenter_doc)
        .def("colorsFromNPArray_copy", &colorsFromNPArray_copy, ccPointCloudPy_colorsFromNPArray_copy_doc)
        .def("coordsFromNPArray_copy", &coordsFromNPArray_copy, ccPointCloudPy_coordsFromNPArray_copy_doc)
        .def("convertRGBToGreyScale", &ccPointCloud::convertRGBToGreyScale, ccPointCloudPy_convertRGBToGreyScale_doc)
        .def("crop2D", &crop2D_py, return_value_policy<reference_existing_object>(), ccPointCloudPy_crop2D_doc)
        .def("deleteAllScalarFields", &ccPointCloud::deleteAllScalarFields, ccPointCloudPy_deleteAllScalarFields_doc)
        .def("deleteScalarField", &ccPointCloud::deleteScalarField, ccPointCloudPy_deleteScalarField_doc)
        .def("exportCoordToSF", &exportCoordToSF_py, ccPointCloudPy_exportCoordToSF_doc)
        .def("exportNormalToSF", &exportNormalToSF_py, ccPointCloudPy_exportNormalToSF_doc)
        .def("filterPointsByScalarValue", &ccPointCloud::filterPointsByScalarValue,
             filterPointsByScalarValue_overloads(ccPointCloudPy_filterPointsByScalarValue_doc)
             [return_value_policy<reference_existing_object>()])
        .def("fuse", &fuse_py, ccPointCloudPy_fuse_doc)
        .def("getCurrentDisplayedScalarField", &ccPointCloud::getCurrentDisplayedScalarField,
             return_value_policy<reference_existing_object>(), ccPointCloudPy_getCurrentDisplayedScalarField_doc)
        .def("getCurrentDisplayedScalarFieldIndex", &ccPointCloud::getCurrentDisplayedScalarFieldIndex,
             ccPointCloudPy_getCurrentDisplayedScalarFieldIndex_doc)
        .def("getCurrentInScalarField", &ccPointCloud::getCurrentInScalarField,
             return_value_policy<reference_existing_object>(), ccPointCloudPy_getCurrentInScalarField_doc)
        .def("getCurrentOutScalarField", &ccPointCloud::getCurrentOutScalarField,
             return_value_policy<reference_existing_object>(), ccPointCloudPy_getCurrentOutScalarField_doc)
        .def("getNumberOfScalarFields", &ccPointCloud::getNumberOfScalarFields, ccPointCloudPy_getNumberOfScalarFields_doc)
        .def("getScalarField", &ccPointCloud::getScalarField,
             return_value_policy<reference_existing_object>(), ccPointCloudPy_getScalarField_doc)
        .def("getScalarField", &getScalarFieldByName_py,
             return_value_policy<reference_existing_object>(), ccPointCloudPy_getScalarFieldByName_doc)
        .def("getScalarFieldDic", &getScalarFieldDic_py, ccPointCloudPy_getScalarFieldDic_doc)
        .def("getScalarFieldName", &ccPointCloud::getScalarFieldName, ccPointCloudPy_getScalarFieldName_doc)
        .def("hasColors", &ccPointCloud::hasColors, ccPointCloudPy_hasColors_doc)
        .def("hasNormals", &ccPointCloud::hasNormals, ccPointCloudPy_hasNormals_doc)
        .def("hasScalarFields", &ccPointCloud::hasScalarFields, ccPointCloudPy_hasScalarFields_doc)
        .def("partialClone", &partialClone_py, ccPointCloudPy_partialClone_doc)
        .def("renameScalarField", &ccPointCloud::renameScalarField, ccPointCloudPy_renameScalarField_doc)
        .def("reserve", &ccPointCloud::reserve, ccPointCloudPy_reserve_doc)
        .def("resize", &ccPointCloud::resize, ccPointCloudPy_resize_doc)
        .def("scale", &ccPointCloud::scale, ccPointCloud_scale_overloads(ccPointCloudPy_scale_doc))
        .def("setColorGradient", &setColorGradient_py, ccPointCloudPy_setColorGradient_doc)
        .def("setColorGradientBanded", &setColorGradientBanded_py, ccPointCloudPy_setColorGradientBanded_doc)
        .def("setColorGradientDefault", &setColorGradientDefault_py, ccPointCloudPy_setColorGradientDefault_doc)
        .def("setCurrentDisplayedScalarField", &ccPointCloud::setCurrentDisplayedScalarField,
             ccPointCloudPy_setCurrentDisplayedScalarField_doc)
        .def("setCurrentScalarField", &ccPointCloud::setCurrentScalarField, ccPointCloudPy_setCurrentScalarField_doc)
        .def("setCurrentInScalarField", &ccPointCloud::setCurrentInScalarField, ccPointCloudPy_setCurrentInScalarField_doc)
        .def("setCurrentOutScalarField", &ccPointCloud::setCurrentOutScalarField, ccPointCloudPy_setCurrentOutScalarField_doc)
        .def("size", &ccPointCloud::size, ccPointCloudPy_size_doc)
		.def("shrinkToFit", &ccPointCloud::shrinkToFit, ccPointCloudPy_shrinkToFit_doc)
        .def("toNpArray", &CoordsToNpArray_py, ccPointCloudPy_toNpArray_doc)
        .def("toNpArrayCopy", &CoordsToNpArray_copy, ccPointCloudPy_toNpArrayCopy_doc)
        .def("colorsToNpArray", &ColorsToNpArray_py, ccPointCloudPy_colorsToNpArray_doc)
        .def("colorsToNpArrayCopy", &ColorsToNpArray_copy, ccPointCloudPy_colorsToNpArrayCopy_doc)
        .def("translate", &ccPointCloud::translate, ccPointCloudPy_translate_doc)
       ;
}

