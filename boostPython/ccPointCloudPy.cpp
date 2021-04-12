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

#include "ccPointCloudPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccPointCloud.h>
#include <ccPolyline.h>

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

bool exportCoordToSF_py(ccPointCloud &self, bool x, bool y, bool z)
{
    bool b[3];
    b[0] =x; b[1] = y; b[2] = z;
    return self.exportCoordToSF(b);
}

bnp::ndarray CoordsToNpArray_copy(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray with copy, ownership transfered to Python");
    bnp::dtype dt = bnp::dtype::get_builtin<float>(); // coordinates always in simple precision
    size_t nRows = self.size();
    bp::tuple shape = bp::make_tuple(nRows, 3);
    bp::tuple stride = bp::make_tuple(3*sizeof(float), sizeof(float));
    float *s = (float*)self.getPoint(0);
    CCTRACE("--- copy " << 3*nRows*sizeof(float));
    float *d = new float[3*nRows];
    memcpy(d, s, 3*nRows*sizeof(float));
    CCTRACE("--- copied");
    bnp::ndarray result = bnp::from_data(d, dt, shape, stride, bp::object());
    return result;
}

bnp::ndarray CoordsToNpArray_py(ccPointCloud &self)
{
    CCTRACE("CoordsToNpArray without copy, ownership stays in C++");
    bnp::dtype dt = bnp::dtype::get_builtin<float>(); // coordinates always in simple precision
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    bp::tuple shape = bp::make_tuple(nRows, 3);
    bp::tuple stride = bp::make_tuple(3*sizeof(float), sizeof(float));
    float *s = (float*)self.getPoint(0);
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

int (ccPointCloud::*addScalarFieldt)(const char*) = &ccPointCloud::addScalarField;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ccPointCloud_overloads, scale, 3, 4)

void export_ccPointCloud()
{
    class_<CCCoreLib::GenericIndexedCloudPersist, boost::noncopyable>("GenericIndexedCloudPersist", no_init)
        ;

    class_<ccGenericPointCloud, bases<CCCoreLib::GenericIndexedCloudPersist>, boost::noncopyable>("ccGenericPointCloud", no_init)
        ;

    class_<CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString>, bases<ccGenericPointCloud>, boost::noncopyable>("PointCloudTpl_ccGenericPointCloud_QString", no_init)
        ;

    class_<ccPointCloud, bases<CCCoreLib::PointCloudTpl<ccGenericPointCloud, QString> > >("ccPointCloud", no_init)
        .def("addScalarField", addScalarFieldt)
        .def("computeGravityCenter", &ccPointCloud::computeGravityCenter)
        .def("crop2D", &crop2D_py, return_value_policy<reference_existing_object>())
        .def("deleteAllScalarFields", &ccPointCloud::deleteAllScalarFields)
        .def("deleteScalarField", &ccPointCloud::deleteScalarField)
        .def("exportCoordToSF", &exportCoordToSF_py)
        .def("getCurrentInScalarField", &ccPointCloud::getCurrentInScalarField, return_value_policy<reference_existing_object>())
        .def("getCurrentOutScalarField", &ccPointCloud::getCurrentOutScalarField, return_value_policy<reference_existing_object>())
        .def("getName", &ccPointCloud::getName)
        .def("getNumberOfScalarFields", &ccPointCloud::getNumberOfScalarFields)
        .def("getScalarField", &ccPointCloud::getScalarField, return_value_policy<reference_existing_object>())
        .def("getScalarFieldName", &ccPointCloud::getScalarFieldName)
        .def("hasScalarFields", &ccPointCloud::hasScalarFields)
        .def("renameScalarField", &ccPointCloud::renameScalarField)
        .def("reserve", &ccPointCloud::reserve)
        .def("scale", &ccPointCloud::scale, ccPointCloud_overloads())
        .def("setCurrentInScalarField", &ccPointCloud::setCurrentInScalarField)
        .def("setCurrentOutScalarField", &ccPointCloud::setCurrentOutScalarField)
        .def("size", &ccPointCloud::size)
        .def("toNpArray", &CoordsToNpArray_py)
        .def("toNpArrayCopy", &CoordsToNpArray_copy)
        .def("translate", &ccPointCloud::translate)
       ;
}
