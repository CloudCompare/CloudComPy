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

#include <CCGeom.h>
#include <ccObject.h>
#include <ccGLMatrixTpl.h>
#include <ccGLMatrix.h>
#include <ccMesh.h>
#include <ccGenericPrimitive.h>
#include <ccBox.h>
#include <ccCone.h>
#include <ccCylinder.h>
#include <ccPlane.h>
#include <ccQuadric.h>
#include <ccSphere.h>
#include <ccTorus.h>
#include <ccDish.h>
#include "ccPrimitivesPy_DocStrings.hpp"
#include "pyccTrace.h"

#include <QString>
#include <exception>


// templates for function pointer requires C++14
template<typename T> void (ccGLMatrixTpl<T>::*initFromParameters1)(T, const Vector3Tpl<T>&, const Vector3Tpl<T>&) = &ccGLMatrixTpl<T>::initFromParameters;
template<typename T> void (ccGLMatrixTpl<T>::*initFromParameters2)(T, T, T, const Vector3Tpl<T>&) = &ccGLMatrixTpl<T>::initFromParameters;
//template<typename T> QString (ccGLMatrixTpl<T>::*toString12Space)(int, char) = &ccGLMatrixTpl<T>::toString;

template<typename T> struct ccGLMatrixParams1
{
    T alpha_rad;
    Vector3Tpl<T> axis3D;
    Vector3Tpl<T> t3D;
};

template<typename T> struct ccGLMatrixParams2
{
    T phi_rad;
    T theta_rad;
    T psi_rad;
    Vector3Tpl<T> t3D;
};

template<typename T> ccGLMatrixParams1<T> getParameters1_py(ccGLMatrixTpl<T>& self)
{
    T alpha_rad;
    Vector3Tpl<T> axis3D;
    Vector3Tpl<T> t3D;
    self.getParameters(alpha_rad, axis3D, t3D);
    ccGLMatrixParams1<T> params;
    params.alpha_rad = alpha_rad;
    params.axis3D = axis3D;
    params.t3D = t3D;
    return params;
}

template<typename T> ccGLMatrixParams2<T>  getParameters2_py(ccGLMatrixTpl<T>& self)
{
    T phi_rad;
    T theta_rad;
    T psi_rad;
    Vector3Tpl<T> t3D;
    self.getParameters(phi_rad, theta_rad, psi_rad, t3D);
    ccGLMatrixParams2<T> params;
    params.phi_rad = phi_rad;
    params.theta_rad = theta_rad;
    params.psi_rad = psi_rad;
    params.t3D = t3D;
    return params;
}

ccGLMatrix fromDouble_py(ccGLMatrixTpl<double>& dblmat)
{
    return ccGLMatrix(dblmat.data());
}

ccGLMatrix FromToRotation_float(const Vector3Tpl<float>& from, const Vector3Tpl<float>& to)
{
    return ccGLMatrix(ccGLMatrixTpl<float>::FromToRotation(from, to));
}

ccGLMatrixd FromToRotation_double(const Vector3Tpl<double>& from, const Vector3Tpl<double>& to)
{
    return ccGLMatrixd(ccGLMatrixTpl<double>::FromToRotation(from, to));
}

ccGLMatrix Interpolate_float(float coef, const ccGLMatrixTpl<float>& glMat1, const ccGLMatrixTpl<float>& glMat2)
{
    return ccGLMatrix(ccGLMatrixTpl<float>::Interpolate(coef, glMat1, glMat2));
}

ccGLMatrixd Interpolate_double(double coef, const ccGLMatrixTpl<double>& glMat1, const ccGLMatrixTpl<double>& glMat2)
{
    return ccGLMatrixd(ccGLMatrixTpl<double>::Interpolate(coef, glMat1, glMat2));
}

ccGLMatrix FromViewDirAndUpDir_float(const Vector3Tpl<float>& forward, const Vector3Tpl<float>& up)
{
    return ccGLMatrix(ccGLMatrixTpl<float>::FromViewDirAndUpDir(forward, up));
}

ccGLMatrixd FromViewDirAndUpDir_double(const Vector3Tpl<double>& forward, const Vector3Tpl<double>& up)
{
    return ccGLMatrixd(ccGLMatrixTpl<double>::FromViewDirAndUpDir(forward, up));
}

ccGLMatrix xRotation_float(ccGLMatrixTpl<float>& self)
{
    return ccGLMatrix(self.xRotation());
}

ccGLMatrixd xRotation_double(ccGLMatrixTpl<double>& self)
{
    return ccGLMatrixd(self.xRotation());
}

ccGLMatrix yRotation_float(ccGLMatrixTpl<float>& self)
{
    return ccGLMatrix(self.yRotation());
}

ccGLMatrixd yRotation_double(ccGLMatrixTpl<double>& self)
{
    return ccGLMatrixd(self.yRotation());
}

ccGLMatrix zRotation_float(ccGLMatrixTpl<float>& self)
{
    return ccGLMatrix(self.zRotation());
}

ccGLMatrixd zRotation_double(ccGLMatrixTpl<double>& self)
{
    return ccGLMatrixd(self.zRotation());
}

ccGLMatrix transposed_float(ccGLMatrixTpl<float>& self)
{
    return ccGLMatrix(self.transposed());
}

ccGLMatrixd transposed_double(ccGLMatrixTpl<double>& self)
{
    return ccGLMatrixd(self.transposed());
}

ccGLMatrix inverse_float(ccGLMatrixTpl<float>& self)
{
    return ccGLMatrix(self.inverse());
}

ccGLMatrixd inverse_double(ccGLMatrixTpl<double>& self)
{
    return ccGLMatrixd(self.inverse());
}

template<typename T> QString toString_def_py(ccGLMatrixTpl<T>&self)
{
    return self.toString();
}

template<typename T> ccGLMatrixTpl<T> fromString_def_py(const QString &matText)
{
    bool success = false;
    ccGLMatrixTpl<T> mat = ccGLMatrixTpl<T>::FromString(matText, success);
    if (!success) CCTRACE("ccGLMatrix from string does not succeed! Returning identity.");
    return mat;
}

template<typename T> std::vector<double> getColumn_py(ccGLMatrixTpl<T>&self, unsigned index)
{
	T* res = self.getColumn(index);
	std::vector<double> vec;
	if (index < 4)
		vec = {(double)res[0], (double)res[1], (double)res[2], (double)res[3]};
	return vec;
}

template<typename T> std::vector<double> getData_py(ccGLMatrixTpl<T>&self)
{
	const T* res = self.data();
	std::vector<double> vec;
	vec.resize(OPENGL_MATRIX_SIZE);
	for (int i=0; i<OPENGL_MATRIX_SIZE; i++)
		vec[i] = (double)res[i];
	return vec;
}

ccPlane* plane_Fit_py(CCCoreLib::GenericIndexedCloudPersist * cloud)
{
    return ccPlane::Fit(cloud, nullptr);
}

std::vector<double> plane_getEquation_py(ccPlane& self)
{
    const PointCoordinateType* coefs = self.getEquation();
    std::vector<double> vec = {coefs[0], coefs[1], coefs[2], coefs[3]};
    return vec;
}

class PyccGenericPrimitive : public ccGenericPrimitive {
public:
    /* Inherit the constructors */
    using ccGenericPrimitive::ccGenericPrimitive;

    /* Trampoline (need one for each virtual function) */
    QString getTypeName() const override {
        PYBIND11_OVERRIDE_PURE(
            QString,                 /* Return type */
            ccGenericPrimitive,      /* Parent class */
            getTypeName              /* Name of function in C++ (must match Python name) */
                                     /* Argument(s) */
        );
    }

    ccGenericPrimitive* clone() const override {
        PYBIND11_OVERRIDE_PURE(
            ccGenericPrimitive*,      /* Return type */
            ccGenericPrimitive,      /* Parent class */
            clone                    /* Name of function in C++ (must match Python name) */
                                     /* Argument(s) */
        );
    }

    bool buildUp() override {
        PYBIND11_OVERRIDE_PURE(
            bool,                    /* Return type */
            ccGenericPrimitive,      /* Parent class */
            buildUp                  /* Name of function in C++ (must match Python name) */
                                     /* Argument(s) */
        );
    }
};

ccGLMatrix getTransformation_py(ccGenericPrimitive& self)
{
    ccGLMatrix m = self.getTransformation();
    return m;
}

class ccGLMatrixWrap
{
public:
    static ccGLMatrix* initWrapper1(std::vector<double> vec)
    {
        CCTRACE("vector size: " << vec.size());
        if (vec.size() != 16)
             throw std::range_error("ccGLMatrix constructor takes an array with 16 elements (column major order)");
        return new ccGLMatrix(vec.data());
    }
};

class ccGLMatrixdWrap
{
public:
    static ccGLMatrixd* initWrapper1(std::vector<double> vec)
    {
        CCTRACE("vector size: " << vec.size());
        if (vec.size() != 16)
             throw std::range_error("ccGLMatrixd constructor takes an array with 16 elements (column major order)");
        return new ccGLMatrixd(vec.data());
    }
};

class ccQuadricWrap
{
public:
//    static ccQuadric* initWrapper1( CCVector2 minCorner,
//                                    CCVector2 maxCorner,
//                                    std::vector<PointCoordinateType> eqv)
//    {
//        if (eqv.size() != 6)
//            throw std::range_error("equation parameters: vector of 6 float/double required");
//        PointCoordinateType* eq = eqv.data();
//        return new ccQuadric(minCorner, maxCorner, eq, 0, 0, "Quadric", ccQuadric::DEFAULT_DRAWING_PRECISION);
//    }
//
//    static ccQuadric* initWrapper2( CCVector2 minCorner,
//                                    CCVector2 maxCorner,
//                                    std::vector<PointCoordinateType> eqv,
//                                    const Tuple3ub* dims = nullptr)
//    {
//        if (eqv.size() != 6)
//            throw std::range_error("equation parameters: vector of 6 float/double required");
//        PointCoordinateType* eq = eqv.data();
//        return new ccQuadric(minCorner, maxCorner, eq, dims, 0, "Quadric",  ccQuadric::DEFAULT_DRAWING_PRECISION);
//    }
//
//    static ccQuadric* initWrapper3( CCVector2 minCorner,
//                                    CCVector2 maxCorner,
//                                    std::vector<PointCoordinateType> eqv,
//                                    const Tuple3ub* dims = nullptr,
//                                    const ccGLMatrix* transMat = nullptr)
//    {
//        if (eqv.size() != 6)
//            throw std::range_error("equation parameters: vector of 6 float/double required");
//        PointCoordinateType* eq = eqv.data();
//        return new ccQuadric(minCorner, maxCorner, eq, dims, transMat, "Quadric",  ccQuadric::DEFAULT_DRAWING_PRECISION);
//    }
//
//    static ccQuadric* initWrapper4( CCVector2 minCorner,
//                                    CCVector2 maxCorner,
//                                    std::vector<PointCoordinateType> eqv,
//                                    const Tuple3ub* dims = nullptr,
//                                    const ccGLMatrix* transMat = nullptr,
//                                    QString name = QString("Quadric"))
//    {
//        if (eqv.size() != 6)
//            throw std::range_error("equation parameters: vector of 6 float/double required");
//        PointCoordinateType* eq = eqv.data();
//        return new ccQuadric(minCorner, maxCorner, eq, dims, transMat, name,  ccQuadric::DEFAULT_DRAWING_PRECISION);
//    }

    static ccQuadric* initWrapper5( CCVector2 minCorner,
                                    CCVector2 maxCorner,
                                    std::vector<PointCoordinateType> eqv,
                                    Tuple3ub dims = Tuple3ub(0,1,2),
                                    const ccGLMatrix* transMat = nullptr,
                                    QString name = QString("Quadric"),
                                    unsigned precision = ccQuadric::DEFAULT_DRAWING_PRECISION)
    {
        if (eqv.size() != 6)
            throw std::range_error("equation parameters: vector of 6 float/double required");
        PointCoordinateType* eq = eqv.data();
        return new ccQuadric(minCorner, maxCorner, eq, &dims, transMat, name, precision);
    }
};

void export_ccPrimitives(py::module &m0)
{
    // TODO: expose more construtors

    py::class_<ccGLMatrixParams1<float> >(m0, "ccGLMatrixParams1_float", ccPrimitivesPy_ccGLMatrixParams1_doc)
        .def_readwrite("alpha_rad", &ccGLMatrixParams1<float>::alpha_rad)
        .def_readwrite("axis3D",&ccGLMatrixParams1<float>::axis3D)
        .def_readwrite("t3D", &ccGLMatrixParams1<float>::t3D)
        ;

    py::class_<ccGLMatrixParams1<double> >(m0, "ccGLMatrixParams1_double", ccPrimitivesPy_ccGLMatrixParams1_doc)
        .def_readwrite("alpha_rad", &ccGLMatrixParams1<double>::alpha_rad)
        .def_readwrite("axis3D", &ccGLMatrixParams1<double>::axis3D)
        .def_readwrite("t3D", &ccGLMatrixParams1<double>::t3D)
        ;

    py::class_<ccGLMatrixParams2<float> >(m0, "ccGLMatrixParams2_float", ccPrimitivesPy_ccGLMatrixParams2_doc)
        .def_readwrite("phi_rad", &ccGLMatrixParams2<float>::phi_rad)
        .def_readwrite("theta_rad", &ccGLMatrixParams2<float>::theta_rad)
        .def_readwrite("psi_rad", &ccGLMatrixParams2<float>::psi_rad)
        .def_readwrite("t3D", &ccGLMatrixParams2<float>::t3D)
        ;

    py::class_<ccGLMatrixParams2<double> >(m0, "ccGLMatrixParams2_double", ccPrimitivesPy_ccGLMatrixParams2_doc)
        .def_readwrite("phi_rad", &ccGLMatrixParams2<double>::phi_rad)
        .def_readwrite("theta_rad", &ccGLMatrixParams2<double>::theta_rad)
        .def_readwrite("psi_rad", &ccGLMatrixParams2<double>::psi_rad)
        .def_readwrite("t3D", &ccGLMatrixParams2<double>::t3D)
        ;

    py::class_<ccGLMatrixTpl<float> >(m0, "ccGLMatrixTpl_float")
    	.def(py::init<const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&>())
        .def_static("fromDouble", &fromDouble_py, ccPrimitivesPy_fromDouble_doc)
        .def("initFromParameters", initFromParameters1<float>, ccPrimitivesPy_initFromParameters1_doc)
        .def("initFromParameters", initFromParameters2<float>, ccPrimitivesPy_initFromParameters2_doc)
        .def("toString", &toString_def_py<float>, ccPrimitivesPy_toString_doc)
        .def_static("fromString", &fromString_def_py<float>, ccPrimitivesPy_fromString_doc)
		.def("getColumn", &getColumn_py<float>, ccPrimitivesPy_getColumn_doc)
        .def("getParameters1", &getParameters1_py<float>, ccPrimitivesPy_getParameters1_py_doc)
        .def("getParameters2", &getParameters2_py<float>, ccPrimitivesPy_getParameters2_py_doc)
		.def("data", &getData_py<float>, ccPrimitivesPy_data_doc)
		.def("Interpolate", &ccGLMatrixTpl<float>::Interpolate, ccPrimitivesPy_Interpolate_doc)
		.def("FromToRotation", &ccGLMatrixTpl<float>::FromToRotation, ccPrimitivesPy_FromToRotation_doc)
		.def("FromViewDirAndUpDir", &ccGLMatrixTpl<float>::FromViewDirAndUpDir, ccPrimitivesPy_FromViewDirAndUpDir_doc)
		.def("xRotation", &ccGLMatrixTpl<float>::xRotation, ccPrimitivesPy_xRotation_doc)
		.def("yRotation", &ccGLMatrixTpl<float>::yRotation, ccPrimitivesPy_yRotation_doc)
		.def("zRotation", &ccGLMatrixTpl<float>::zRotation, ccPrimitivesPy_zRotation_doc)
		.def("clearTranslation", &ccGLMatrixTpl<float>::clearTranslation, ccPrimitivesPy_clearTranslation_doc)
		.def("invert", &ccGLMatrixTpl<float>::invert, ccPrimitivesPy_invert_doc)
		.def("inverse", &ccGLMatrixTpl<float>::inverse, ccPrimitivesPy_inverse_doc)
		.def("transpose", &ccGLMatrixTpl<float>::transpose, ccPrimitivesPy_transpose_doc)
		.def("transposed", &ccGLMatrixTpl<float>::transposed, ccPrimitivesPy_transposed_doc)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self * py::self)
      ;

    py::class_<ccGLMatrixTpl<double> >(m0, "ccGLMatrixTpl_double")
    	.def(py::init<const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&>())
        .def("initFromParameters", initFromParameters1<double>, ccPrimitivesPy_initFromParameters1_doc)
        .def("initFromParameters", initFromParameters2<double>, ccPrimitivesPy_initFromParameters2_doc)
        .def("toString", &toString_def_py<double>, ccPrimitivesPy_toString_doc)
        .def_static("fromString", &fromString_def_py<double>, ccPrimitivesPy_fromString_doc)
		.def("getColumn", &getColumn_py<double>, ccPrimitivesPy_getColumn_doc)
        .def("getParameters1", &getParameters1_py<double>, ccPrimitivesPy_getParameters1_py_doc)
        .def("getParameters2", &getParameters2_py<double>, ccPrimitivesPy_getParameters2_py_doc)
		.def("data", &getData_py<double>, ccPrimitivesPy_data_doc)
		.def("Interpolate", &ccGLMatrixTpl<double>::Interpolate, ccPrimitivesPy_Interpolate_doc)
		.def("FromToRotation", &ccGLMatrixTpl<double>::FromToRotation, ccPrimitivesPy_FromToRotation_doc)
		.def("FromViewDirAndUpDir", &ccGLMatrixTpl<double>::FromViewDirAndUpDir, ccPrimitivesPy_FromViewDirAndUpDir_doc)
		.def("xRotation", &ccGLMatrixTpl<double>::xRotation, ccPrimitivesPy_xRotation_doc)
		.def("yRotation", &ccGLMatrixTpl<double>::yRotation, ccPrimitivesPy_yRotation_doc)
		.def("zRotation", &ccGLMatrixTpl<double>::zRotation, ccPrimitivesPy_zRotation_doc)
		.def("clearTranslation", &ccGLMatrixTpl<double>::clearTranslation, ccPrimitivesPy_clearTranslation_doc)
		.def("invert", &ccGLMatrixTpl<double>::invert, ccPrimitivesPy_invert_doc)
		.def("inverse", &ccGLMatrixTpl<double>::inverse, ccPrimitivesPy_inverse_doc)
		.def("transpose", &ccGLMatrixTpl<double>::transpose, ccPrimitivesPy_transpose_doc)
		.def("transposed", &ccGLMatrixTpl<double>::transposed, ccPrimitivesPy_transposed_doc)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= py::self)
        .def(py::self * py::self)
        ;

    py::class_<ccGLMatrix, ccGLMatrixTpl<float> >(m0, "ccGLMatrix", ccPrimitivesPy_ccGLMatrix_doc)
        .def(py::init<>())
        .def(py::init<const ccGLMatrixTpl<float>&>())
        .def(py::init<const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&>())
        .def(py::init( &ccGLMatrixWrap::initWrapper1 ))
        .def("FromToRotation", &FromToRotation_float, ccPrimitivesPy_FromToRotation_doc)
        .def("Interpolate", &Interpolate_float, ccPrimitivesPy_Interpolate_doc)
        .def("FromViewDirAndUpDir", &FromViewDirAndUpDir_float, ccPrimitivesPy_FromViewDirAndUpDir_doc)
        .def("xRotation", &xRotation_float, ccPrimitivesPy_xRotation_doc)
        .def("yRotation", &yRotation_float, ccPrimitivesPy_yRotation_doc)
        .def("zRotation", &zRotation_float, ccPrimitivesPy_zRotation_doc)
        .def("inverse", &inverse_float, ccPrimitivesPy_inverse_doc)
        .def("transposed", &transposed_float, ccPrimitivesPy_transposed_doc)
        ;

    py::class_<ccGLMatrixd, ccGLMatrixTpl<double> >(m0, "ccGLMatrixd", ccPrimitivesPy_ccGLMatrixd_doc)
        .def(py::init<>())
        .def(py::init<const ccGLMatrixTpl<double>&>())
        .def(py::init<const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&>())
        .def(py::init(&ccGLMatrixdWrap::initWrapper1 ))
        .def("FromToRotation", &FromToRotation_double, ccPrimitivesPy_FromToRotation_doc)
        .def("Interpolate", &Interpolate_double, ccPrimitivesPy_Interpolate_doc)
        .def("FromViewDirAndUpDir", &FromViewDirAndUpDir_double, ccPrimitivesPy_FromViewDirAndUpDir_doc)
        .def("xRotation", &xRotation_double, ccPrimitivesPy_xRotation_doc)
        .def("yRotation", &yRotation_double, ccPrimitivesPy_yRotation_doc)
        .def("zRotation", &zRotation_double, ccPrimitivesPy_zRotation_doc)
        .def("inverse", &inverse_double, ccPrimitivesPy_inverse_doc)
        .def("transposed", &transposed_double, ccPrimitivesPy_transposed_doc)
        ;

    py::class_<ccGenericPrimitive, PyccGenericPrimitive>(m0, "ccGenericPrimitive") //boost::noncopyable, no_init
        .def(py::init<QString, const ccGLMatrix*, unsigned>(),
             py::arg("name")=QString(), py::arg("transMat")=nullptr, py::arg("uniqueID")= 0xFFFFFFFF)
        .def("getTypeName", (&ccGenericPrimitive::getTypeName), ccPrimitivesPy_getTypeName_doc)
        .def("getTransformation", &getTransformation_py, ccPrimitivesPy_getTransformation_doc)
        ;

    py::class_<ccBox, ccGenericPrimitive, ccMesh,
               std::unique_ptr<ccBox, py::nodelete>>(m0, "ccBox", ccPrimitivesPy_ccBox_doc)
        .def(py::init<QString>())
        .def(py::init<const CCVector3&, const ccGLMatrix*, QString >(),
             py::arg("dims"), py::arg("transMat")=nullptr, py::arg("name")=QString("Box"))
        .def("getDimensions", &ccBox::getDimensions, ccPrimitivesPy_ccBox_getDimensions_doc)
        ;

    py::class_<ccCone, ccGenericPrimitive, ccMesh,
               std::unique_ptr<ccCone, py::nodelete>>(m0, "ccCone", ccPrimitivesPy_ccCone_doc)
        .def(py::init<QString>())
        .def(py::init<PointCoordinateType, PointCoordinateType, PointCoordinateType,
                      PointCoordinateType, PointCoordinateType, const ccGLMatrix*, QString, unsigned, unsigned>(),
                      py::arg("bottomRadius"), py::arg("topRadius"), py::arg("height"),
                      py::arg("xOff")=0, py::arg("xOff")=0,
                      py::arg("transMat")=nullptr, py::arg("name")=QString("Cone"),
                      py::arg("precision")= 24, py::arg("uniqueID")= 0xFFFFFFFF)
        .def("getHeight", &ccCone::getHeight, ccPrimitivesPy_getHeight_doc)
        .def("getBottomRadius", &ccCone::getBottomRadius, ccPrimitivesPy_getBottomRadius_doc)
        .def("getTopRadius", &ccCone::getTopRadius, ccPrimitivesPy_getTopRadius_doc)
        .def("getBottomCenter", &ccCone::getBottomCenter, ccPrimitivesPy_getBottomCenter_doc)
        .def("getTopCenter", &ccCone::getTopCenter, ccPrimitivesPy_getTopCenter_doc)
        .def("getSmallCenter", &ccCone::getSmallCenter, ccPrimitivesPy_getSmallCenter_doc)
        .def("getLargeCenter", &ccCone::getLargeCenter, ccPrimitivesPy_getLargeCenter_doc)
        .def("getSmallRadius", &ccCone::getSmallRadius, ccPrimitivesPy_getSmallRadius_doc)
        .def("getLargeRadius", &ccCone::getLargeRadius, ccPrimitivesPy_getLargeRadius_doc)
        ;

    py::class_<ccCylinder, ccCone,
               std::unique_ptr<ccCylinder, py::nodelete>>(m0,"ccCylinder", ccPrimitivesPy_ccCylinder_doc)
        .def(py::init<QString>())
        .def(py::init<PointCoordinateType, PointCoordinateType,
                      const ccGLMatrix*, QString, unsigned, unsigned>(),
                      py::arg("radius"), py::arg("height"),
                      py::arg("transMat")=nullptr, py::arg("name")=QString("Cylinder"),
                      py::arg("precision")=24, py::arg("uniqueID")=0xFFFFFFFF)
        ;

    py::class_<ccPlane, ccGenericPrimitive, ccMesh,
               std::unique_ptr<ccPlane, py::nodelete>>(m0, "ccPlane", ccPrimitivesPy_ccPlane_doc)
        .def(py::init<QString>())
        .def(py::init<PointCoordinateType, PointCoordinateType,
             const ccGLMatrix*, QString>(),
             py::arg("xWidth"), py::arg("yWidth"),
             py::arg("transMat")=nullptr, py::arg("name")=QString("Plane"))
        .def_static("Fit", &plane_Fit_py, ccPrimitivesPy_ccPlane_Fit_doc, py::return_value_policy::reference)
        .def("getEquation", &plane_getEquation_py, ccPrimitivesPy_ccPlane_getEquation_doc)
        .def("getCenter", &ccPlane::getCenter, ccPrimitivesPy_ccPlane_getCenter_doc)
        .def("getNormal", &ccPlane::getNormal, ccPrimitivesPy_ccPlane_getNormal_doc)
        ;

    py::class_<ccQuadric, ccGenericPrimitive, ccMesh,
              std::unique_ptr<ccQuadric, py::nodelete>>(m0, "ccQuadric", ccPrimitivesPy_ccQuadric_doc)
        .def(py::init(&ccQuadricWrap::initWrapper5),
            py::arg("minCorner"), py::arg("maxCorner"), py::arg("eqv"),
            py::arg("dims")=Tuple3ub(0,1,2), py::arg("transMat")=nullptr,
            py::arg("name")=QString("Quadric"), py::arg("precision")= ccQuadric::DEFAULT_DRAWING_PRECISION )
        ;

    py::class_<ccSphere, ccGenericPrimitive, ccMesh,
               std::unique_ptr<ccSphere, py::nodelete>>(m0, "ccSphere", ccPrimitivesPy_ccSphere_doc)
        .def(py::init<QString>())
        .def(py::init<PointCoordinateType,
                      const ccGLMatrix*, QString, unsigned, unsigned>(),
                      py::arg("radius"),
                      py::arg("transMat")=nullptr, py::arg("name")=QString("Sphere"),
                      py::arg("precision")=24, py::arg("uniqueID")=0xFFFFFFFF)
        .def("getRadius", &ccSphere::getRadius, ccPrimitivesPy_ccSphere_getRadius_doc)
        ;

    py::class_<ccTorus, ccGenericPrimitive, ccMesh,
               std::unique_ptr<ccTorus, py::nodelete>>(m0, "ccTorus", ccPrimitivesPy_ccTorus_doc)
        .def(py::init<QString>())
        .def(py::init<PointCoordinateType, PointCoordinateType,
             double, bool, PointCoordinateType, const ccGLMatrix*, QString, unsigned, unsigned>(),
             py::arg("insideRadius"), py::arg("outsideRadius"),
             py::arg("angle_rad")=2.0*M_PI, py::arg("rectangularSection")=false, py::arg("rectSectionHeight")=0,
             py::arg("transMat")=nullptr, py::arg("name")=QString("Torus"),
             py::arg("precision")=24, py::arg("uniqueID")=0xFFFFFFFF)
        ;

    py::class_<ccDish, ccGenericPrimitive, ccMesh,
               std::unique_ptr<ccDish, py::nodelete>>(m0, "ccDish", ccPrimitivesPy_ccDish_doc)
        .def(py::init<QString>())
        .def(py::init<PointCoordinateType, PointCoordinateType,
             PointCoordinateType, const ccGLMatrix*, QString, unsigned>(),
             py::arg("radius"), py::arg("height"),
             py::arg("radius2")=0,
             py::arg("transMat")=nullptr, py::arg("name")=QString("Dish"),
             py::arg("precision")=24)
        ;

}
