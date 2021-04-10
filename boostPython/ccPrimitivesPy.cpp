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

#include "ccPrimitivesPy.hpp"

//#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

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

#include <QString>
#include <exception>

using namespace boost::python;

// templates for function pointer requires C++14
template<typename T> void (ccGLMatrixTpl<T>::*initFromParameters1)(T, const Vector3Tpl<T>&, const Vector3Tpl<T>&) = &ccGLMatrixTpl<T>::initFromParameters;
template<typename T> void (ccGLMatrixTpl<T>::*initFromParameters2)(T, T, T, const Vector3Tpl<T>&) = &ccGLMatrixTpl<T>::initFromParameters;

struct ccGenericPrimitiveWrap : ccGenericPrimitive, wrapper<ccGenericPrimitive>
{
    virtual QString getTypeName()
    {
        return this->get_override("getTypeName")();
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

    class_<ccGLMatrixTpl<float> >("ccGLMatrixTpl_float")
        .def("initFromParameters", initFromParameters1<float>)
        .def("initFromParameters", initFromParameters2<float>)
        ;

    class_<ccGLMatrixTpl<double> >("ccGLMatrixTpl_double")
        .def("initFromParameters", initFromParameters1<double>)
        .def("initFromParameters", initFromParameters2<double>)
        ;

    class_<ccGLMatrix, bases<ccGLMatrixTpl<float> > >("ccGLMatrix")
        ;

    class_<ccGLMatrixd, bases<ccGLMatrixTpl<double> > >("ccGLMatrixd")
        ;

    class_<ccGenericPrimitiveWrap, bases<ccMesh>, boost::noncopyable>("ccGenericPrimitive", no_init)
        .def("getTypeName", pure_virtual(&ccGenericPrimitive::getTypeName))
        ;

    class_<ccBox, bases<ccGenericPrimitive> >("ccBox", init<QString>())
        .def(init<const CCVector3&, optional<const ccGLMatrix*, QString> >())
        ;

    class_<ccCone, bases<ccGenericPrimitive> >("ccCone", init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType, PointCoordinateType,
             optional<PointCoordinateType, PointCoordinateType,const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

    class_<ccCylinder, bases<ccGenericPrimitive> >("ccCylinder", init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

    class_<ccPlane, bases<ccGenericPrimitive> >("ccPlane", init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<const ccGLMatrix*, QString> >())
        ;

    class_<ccQuadric, boost::shared_ptr<ccQuadric>, bases<ccGenericPrimitive> >("ccQuadric",  no_init)
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper1 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper2 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper3 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper4 ))
        .def("__init__", make_constructor(&ccQuadricWrap::initWrapper5 ))
        ;

    class_<ccSphere, bases<ccGenericPrimitive> >("ccSphere", init<QString>())
        .def(init<PointCoordinateType,
             optional<const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

    class_<ccTorus, bases<ccGenericPrimitive> >("ccTorus", init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<double, bool, PointCoordinateType, const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

}
