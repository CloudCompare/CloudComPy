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
#include "ccPrimitivesPy.hpp"

#include <CCGeom.h>
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

using namespace boost::python;

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

struct ccGenericPrimitiveWrap : ccGenericPrimitive, wrapper<ccGenericPrimitive>
{
    virtual QString getTypeName()
    {
        return this->get_override("getTypeName")();
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
    static boost::shared_ptr<ccGLMatrix> initWrapper1(std::vector<double> vec)
    {
        CCTRACE("vector size: " << vec.size());
        if (vec.size() != 16)
             throw std::range_error("ccGLMatrix constructor takes an array with 16 elements (column major order)");
        return boost::shared_ptr<ccGLMatrix>(new ccGLMatrix(vec.data()));
    }
};

class ccGLMatrixdWrap
{
public:
    static boost::shared_ptr<ccGLMatrixd> initWrapper1(std::vector<double> vec)
    {
        CCTRACE("vector size: " << vec.size());
        if (vec.size() != 16)
             throw std::range_error("ccGLMatrixd constructor takes an array with 16 elements (column major order)");
        return boost::shared_ptr<ccGLMatrixd>(new ccGLMatrixd(vec.data()));
    }
};

class ccQuadricWrap
{
public:
    static boost::shared_ptr<ccQuadric> initWrapper1(CCVector2 minCorner,
                                                    CCVector2 maxCorner,
                                                    std::vector<PointCoordinateType> eqv)
    {
        if (eqv.size() != 6)
            throw std::range_error("equation parameters: vector of 6 float/double required");
        PointCoordinateType* eq = eqv.data();
        return boost::shared_ptr<ccQuadric>( new ccQuadric(minCorner, maxCorner, eq, 0, 0, "Quadric", ccQuadric::DEFAULT_DRAWING_PRECISION));
    }

    static boost::shared_ptr<ccQuadric> initWrapper2(CCVector2 minCorner,
                                                    CCVector2 maxCorner,
                                                    std::vector<PointCoordinateType> eqv,
                                                    const Tuple3ub* dims = 0)
    {
        if (eqv.size() != 6)
            throw std::range_error("equation parameters: vector of 6 float/double required");
        PointCoordinateType* eq = eqv.data();
        return boost::shared_ptr<ccQuadric>( new ccQuadric(minCorner, maxCorner, eq, dims, 0, "Quadric",  ccQuadric::DEFAULT_DRAWING_PRECISION));
    }

    static boost::shared_ptr<ccQuadric> initWrapper3(CCVector2 minCorner,
                                                    CCVector2 maxCorner,
                                                    std::vector<PointCoordinateType> eqv,
                                                    const Tuple3ub* dims = 0,
                                                    const ccGLMatrix* transMat = 0)
    {
        if (eqv.size() != 6)
            throw std::range_error("equation parameters: vector of 6 float/double required");
        PointCoordinateType* eq = eqv.data();
        return boost::shared_ptr<ccQuadric>( new ccQuadric(minCorner, maxCorner, eq, dims, transMat, "Quadric",  ccQuadric::DEFAULT_DRAWING_PRECISION));
    }

    static boost::shared_ptr<ccQuadric> initWrapper4(CCVector2 minCorner,
                                                    CCVector2 maxCorner,
                                                    std::vector<PointCoordinateType> eqv,
                                                    const Tuple3ub* dims = 0,
                                                    const ccGLMatrix* transMat = 0,
                                                    QString name = QString("Quadric"))
    {
        if (eqv.size() != 6)
            throw std::range_error("equation parameters: vector of 6 float/double required");
        PointCoordinateType* eq = eqv.data();
        return boost::shared_ptr<ccQuadric>( new ccQuadric(minCorner, maxCorner, eq, dims, transMat, name,  ccQuadric::DEFAULT_DRAWING_PRECISION));
    }

    static boost::shared_ptr<ccQuadric> initWrapper5(CCVector2 minCorner,
                                                    CCVector2 maxCorner,
                                                    std::vector<PointCoordinateType> eqv,
                                                    const Tuple3ub* dims = 0,
                                                    const ccGLMatrix* transMat = 0,
                                                    QString name = QString("Quadric"),
                                                    unsigned precision = ccQuadric::DEFAULT_DRAWING_PRECISION)
    {
        if (eqv.size() != 6)
            throw std::range_error("equation parameters: vector of 6 float/double required");
        PointCoordinateType* eq = eqv.data();
        return boost::shared_ptr<ccQuadric>( new ccQuadric(minCorner, maxCorner, eq, dims, transMat, name, precision));
    }
};

void export_ccPrimitives()
{
    // TODO: expose more construtors

    class_<ccGLMatrixParams1<float> >("ccGLMatrixParams1_float", ccPrimitivesPy_ccGLMatrixParams1_doc)
        .def_readwrite("alpha_rad", &ccGLMatrixParams1<float>::alpha_rad)
        .add_property("axis3D",
                      make_getter(&ccGLMatrixParams1<float>::axis3D,
                      return_value_policy<return_by_value>()),
                      make_setter(&ccGLMatrixParams1<float>::axis3D))
        .add_property("t3D",
                      make_getter(&ccGLMatrixParams1<float>::t3D,
                      return_value_policy<return_by_value>()),
                      make_setter(&ccGLMatrixParams1<float>::t3D))
        ;

    class_<ccGLMatrixParams1<double> >("ccGLMatrixParams1_double", ccPrimitivesPy_ccGLMatrixParams1_doc)
        .def_readwrite("alpha_rad", &ccGLMatrixParams1<double>::alpha_rad)
        .add_property("axis3D",
                      make_getter(&ccGLMatrixParams1<double>::axis3D,
                      return_value_policy<return_by_value>()),
                      make_setter(&ccGLMatrixParams1<double>::axis3D))
        .add_property("t3D",
                      make_getter(&ccGLMatrixParams1<double>::t3D,
                      return_value_policy<return_by_value>()),
                      make_setter(&ccGLMatrixParams1<double>::t3D))
        ;

    class_<ccGLMatrixParams2<float> >("ccGLMatrixParams2_float", ccPrimitivesPy_ccGLMatrixParams2_doc)
        .def_readwrite("phi_rad", &ccGLMatrixParams2<float>::phi_rad)
        .def_readwrite("theta_rad", &ccGLMatrixParams2<float>::theta_rad)
        .def_readwrite("psi_rad", &ccGLMatrixParams2<float>::psi_rad)
        .add_property("t3D",
                      make_getter(&ccGLMatrixParams2<float>::t3D,
                      return_value_policy<return_by_value>()),
                      make_setter(&ccGLMatrixParams2<float>::t3D))
        ;

    class_<ccGLMatrixParams2<double> >("ccGLMatrixParams2_double", ccPrimitivesPy_ccGLMatrixParams2_doc)
        .def_readwrite("phi_rad", &ccGLMatrixParams2<double>::phi_rad)
        .def_readwrite("theta_rad", &ccGLMatrixParams2<double>::theta_rad)
        .def_readwrite("psi_rad", &ccGLMatrixParams2<double>::psi_rad)
        .add_property("t3D",
                      make_getter(&ccGLMatrixParams2<double>::t3D,
                      return_value_policy<return_by_value>()),
                      make_setter(&ccGLMatrixParams2<double>::t3D))
        ;

    class_<ccGLMatrixTpl<float> >("ccGLMatrixTpl_float")
    	.def(init<const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&>())
        .def("fromDouble", &fromDouble_py, ccPrimitivesPy_fromDouble_doc)
            .staticmethod("fromDouble")
        .def("initFromParameters", initFromParameters1<float>, ccPrimitivesPy_initFromParameters1_doc)
        .def("initFromParameters", initFromParameters2<float>, ccPrimitivesPy_initFromParameters2_doc)
        .def("toString", &toString_def_py<float>, ccPrimitivesPy_toString_doc)
        .def("fromString", &fromString_def_py<float>, ccPrimitivesPy_fromString_doc)
        .staticmethod("fromString")
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
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self * self)
      ;

    class_<ccGLMatrixTpl<double> >("ccGLMatrixTpl_double")
    	.def(init<const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&>())
        .def("initFromParameters", initFromParameters1<double>, ccPrimitivesPy_initFromParameters1_doc)
        .def("initFromParameters", initFromParameters2<double>, ccPrimitivesPy_initFromParameters2_doc)
        .def("toString", &toString_def_py<double>, ccPrimitivesPy_toString_doc)
        .def("fromString", &fromString_def_py<double>, ccPrimitivesPy_fromString_doc)
        .staticmethod("fromString")
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
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self * self)
        ;

    class_<ccGLMatrix, bases<ccGLMatrixTpl<float> > >("ccGLMatrix", ccPrimitivesPy_ccGLMatrix_doc)
        .def(init<const ccGLMatrixTpl<float>&>())
        .def(init<const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&, const Vector3Tpl<float>&>())
        .def("__init__", make_constructor(&ccGLMatrixWrap::initWrapper1 ))
        .def("FromToRotation", &FromToRotation_float, ccPrimitivesPy_FromToRotation_doc)
        .def("Interpolate", &Interpolate_float, ccPrimitivesPy_Interpolate_doc)
        .def("FromViewDirAndUpDir", &FromViewDirAndUpDir_float, ccPrimitivesPy_FromViewDirAndUpDir_doc)
        .def("xRotation", &xRotation_float, ccPrimitivesPy_xRotation_doc)
        .def("yRotation", &yRotation_float, ccPrimitivesPy_yRotation_doc)
        .def("zRotation", &zRotation_float, ccPrimitivesPy_zRotation_doc)
        .def("inverse", &inverse_float, ccPrimitivesPy_inverse_doc)
        .def("transposed", &transposed_float, ccPrimitivesPy_transposed_doc)
        ;

    class_<ccGLMatrixd, bases<ccGLMatrixTpl<double> > >("ccGLMatrixd", ccPrimitivesPy_ccGLMatrixd_doc)
        .def(init<const ccGLMatrixTpl<double>&>())
        .def(init<const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&, const Vector3Tpl<double>&>())
        .def("__init__", make_constructor(&ccGLMatrixdWrap::initWrapper1 ))
        .def("FromToRotation", &FromToRotation_double, ccPrimitivesPy_FromToRotation_doc)
        .def("Interpolate", &Interpolate_double, ccPrimitivesPy_Interpolate_doc)
        .def("FromViewDirAndUpDir", &FromViewDirAndUpDir_double, ccPrimitivesPy_FromViewDirAndUpDir_doc)
        .def("xRotation", &xRotation_double, ccPrimitivesPy_xRotation_doc)
        .def("yRotation", &yRotation_double, ccPrimitivesPy_yRotation_doc)
        .def("zRotation", &zRotation_double, ccPrimitivesPy_zRotation_doc)
        .def("inverse", &inverse_double, ccPrimitivesPy_inverse_doc)
        .def("transposed", &transposed_double, ccPrimitivesPy_transposed_doc)
        ;

    class_<ccGenericPrimitiveWrap, bases<ccMesh>, boost::noncopyable>("ccGenericPrimitive", no_init)
        .def("getTypeName", pure_virtual(&ccGenericPrimitive::getTypeName), ccPrimitivesPy_getTypeName_doc)
        .def("getTransformation", &getTransformation_py, ccPrimitivesPy_getTransformation_doc)
        ;

    class_<ccBox, bases<ccGenericPrimitive>, boost::noncopyable>("ccBox", ccPrimitivesPy_ccBox_doc, init<QString>())
        .def(init<const CCVector3&, optional<const ccGLMatrix*, QString> >())
        ;

    class_<ccCone, bases<ccGenericPrimitive>, boost::noncopyable>("ccCone", ccPrimitivesPy_ccCone_doc, init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType, PointCoordinateType,
             optional<PointCoordinateType, PointCoordinateType,const ccGLMatrix*, QString, unsigned, unsigned> >())
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

    class_<ccCylinder, bases<ccCone>, boost::noncopyable>("ccCylinder", ccPrimitivesPy_ccCylinder_doc, init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

    class_<ccPlane, bases<ccGenericPrimitive>, boost::noncopyable>("ccPlane", ccPrimitivesPy_ccPlane_doc, init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<const ccGLMatrix*, QString> >())
        .def("Fit", &plane_Fit_py, ccPrimitivesPy_ccPlane_Fit_doc, return_value_policy<reference_existing_object>())
            .staticmethod("Fit")
        .def("getEquation", &plane_getEquation_py, ccPrimitivesPy_ccPlane_getEquation_doc)
        .def("getCenter", &ccPlane::getCenter, ccPrimitivesPy_ccPlane_getCenter_doc)
        .def("getNormal", &ccPlane::getNormal, ccPrimitivesPy_ccPlane_getNormal_doc)
        ;

    class_<ccQuadric, boost::shared_ptr<ccQuadric>, bases<ccGenericPrimitive>, boost::noncopyable >("ccQuadric", ccPrimitivesPy_ccQuadric_doc, no_init)
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper1 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper2 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper3 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper4 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper5 ))
        ;

    class_<ccSphere, bases<ccGenericPrimitive>, boost::noncopyable >("ccSphere", ccPrimitivesPy_ccSphere_doc, init<QString>())
        .def(init<PointCoordinateType,
             optional<const ccGLMatrix*, QString, unsigned, unsigned> >())
        .def("getRadius", &ccSphere::getRadius, ccPrimitivesPy_ccSphere_getRadius_doc)
        ;

    class_<ccTorus, bases<ccGenericPrimitive>, boost::noncopyable >("ccTorus", ccPrimitivesPy_ccTorus_doc, init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<double, bool, PointCoordinateType, const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

    class_<ccDish, bases<ccGenericPrimitive>, boost::noncopyable >("ccDish", ccPrimitivesPy_ccDish_doc, init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<PointCoordinateType, const ccGLMatrix*, QString, unsigned> >())
        ;

}
