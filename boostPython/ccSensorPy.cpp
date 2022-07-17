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
#include "ccSensorPy.hpp"
#include "ccSensorPy_DocStrings.hpp"

#include <ccSensor.h>
#include <ccGBLSensor.h>
#include <ccCommon.h>
#include <ccPointCloud.h>
#include <ccHObjectCaster.h>
#include "ccGLMatrix.h"

using namespace boost::python;

ccGLMatrix getRigidTransformationPy(ccSensor& self)
{
    ccGLMatrix mat = self.getRigidTransformation();
    return mat;
}

CC_SENSOR_TYPE getGBLSensorType(ccGBLSensor& self)
{
    return CC_SENSOR_TYPE::GROUND_BASED_LIDAR;
}

void ComputeScatteringAnglesPy(ccSensor& self, ccPointCloud* cloud=nullptr, bool toDegreeFlag=true)
{
    //from mainwindow.cpp MainWindow::doActionComputeScatteringAngles
    CCTRACE("ComputeScatteringAngles");

    //sensor center
    CCVector3 sensorCenter;
    if (!self.getActiveAbsoluteCenter(sensorCenter))
    {
        CCTRACE("sensorCenter not found!");
        return;
    }

    if (!cloud)
    {
        ccHObject* defaultCloud = self.getParent();
        if (!defaultCloud)
        {
            CCTRACE("no parent");
            return;
        }
        cloud = ccHObjectCaster::ToPointCloud(defaultCloud);
        if (!cloud)
        {
            CCTRACE("parent is not a ccPointCloud!");
            return;
        }
    }
    if (!cloud->hasNormals())
    {
        CCTRACE("The cloud must have normals!");
        return;
    }
    //prepare a new scalar field
    const char* defaultScatAnglesSFname = toDegreeFlag ? CC_DEFAULT_DEG_SCATTERING_ANGLES_SF_NAME : CC_DEFAULT_RAD_SCATTERING_ANGLES_SF_NAME;
    int sfIdx = cloud->getScalarFieldIndexByName(defaultScatAnglesSFname);
    if (sfIdx < 0)
    {
        sfIdx = cloud->addScalarField(defaultScatAnglesSFname);
        if (sfIdx < 0)
        {
            CCTRACE("Not enough memory!");
            return;
        }
    }
    CCCoreLib::ScalarField* angles = cloud->getScalarField(sfIdx);

    //perform computations
    for (unsigned i = 0; i < cloud->size(); ++i)
    {
        //the point position
        const CCVector3* P = cloud->getPoint(i);

        //build the ray
        CCVector3 ray = *P - sensorCenter;
        ray.normalize();

        //get the current normal
        CCVector3 normal(cloud->getPointNormal(i));
        //normal.normalize(); //should already be the case!

        //compute the angle
        PointCoordinateType cosTheta = ray.dot(normal);
        ScalarType theta = std::acos(std::min(std::abs(cosTheta), 1.0f));

        if (toDegreeFlag)
        {
            theta = CCCoreLib::RadiansToDegrees( theta );
        }

        angles->setValue(i,theta);
    }

    angles->computeMinAndMax();
    cloud->setCurrentDisplayedScalarField(sfIdx);
    cloud->showSF(true);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(ComputeScatteringAnglesPy_overloads, ComputeScatteringAnglesPy, 1, 3)


void export_ccSensor()
{
    enum_<CC_SENSOR_TYPE>("CC_SENSOR_TYPE")
        .value("UNKNOWN_SENSOR", CC_SENSOR_TYPE::UNKNOWN_SENSOR)
        .value("GROUND_BASED_LIDAR", CC_SENSOR_TYPE::GROUND_BASED_LIDAR)
         ;

    class_<ccSensor, bases<ccHObject> >("ccSensor",
            ccSensorPy_ccSensor_doc, no_init)
         .def("getType", &ccSensor::getType, ccSensorPy_ccSensor_getType_doc)
         .def("getRigidTransformation", &getRigidTransformationPy,
              ccSensorPy_ccSensor_getRigidTransformation_doc)
         .def("getGraphicScale", &ccSensor::getGraphicScale, ccSensorPy_ccSensor_getGraphicScale_doc)
         .def("ComputeScatteringAngles", ComputeScatteringAnglesPy,
              ComputeScatteringAnglesPy_overloads(
              (arg("self"), arg("cloud")=0, arg("toDegreeFlag")=true),
              ccSensorPy_ccSensor_ComputeScatteringAnglesPy_doc))
         //.def("", &ccSensor::, ccSensorPy_ccSensor__doc)
         //.def("", &ccSensor::, ccSensorPy_ccSensor__doc)
         ;

    class_<ccGBLSensor, bases<ccSensor> >("ccGBLSensor",
            ccSensorPy_ccGBLSensor_doc, no_init)
         .def("getUncertainty", &ccGBLSensor::getUncertainty, ccSensorPy_ccGBLSensor_getUncertainty_doc)
         .def("getType", getGBLSensorType, ccSensorPy_ccGBLSensor_getType_doc)
         ;
}
