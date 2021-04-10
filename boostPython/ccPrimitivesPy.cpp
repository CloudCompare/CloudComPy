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

//    class_<ccQuadric, bases<ccGenericPrimitive> >("ccQuadric", init<QString>())
//        .def(init<CCVector2, CCVector2, const PointCoordinateType[6],
//             optional<const Tuple3ub*, const ccGLMatrix*, QString, unsigned> >())
//        ;

    class_<ccSphere, bases<ccGenericPrimitive> >("ccSphere", init<QString>())
        .def(init<PointCoordinateType,
             optional<const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

    class_<ccTorus, bases<ccGenericPrimitive> >("ccTorus", init<QString>())
        .def(init<PointCoordinateType, PointCoordinateType,
             optional<double, bool, PointCoordinateType, const ccGLMatrix*, QString, unsigned, unsigned> >())
        ;

}
