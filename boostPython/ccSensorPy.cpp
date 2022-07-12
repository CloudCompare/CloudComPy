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

#include <ccSensor.h>
#include <ccGBLSensor.h>
#include "ccSensorPy_DocStrings.hpp"

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
         //.def("", &ccSensor::, ccSensorPy_ccSensor__doc)
         //.def("", &ccSensor::, ccSensorPy_ccSensor__doc)
         //.def("", &ccSensor::, ccSensorPy_ccSensor__doc)
         ;

    class_<ccGBLSensor, bases<ccSensor> >("ccGBLSensor",
            ccSensorPy_ccGBLSensor_doc, no_init)
         .def("getUncertainty", &ccGBLSensor::getUncertainty, ccSensorPy_ccGBLSensor_getUncertainty_doc)
         .def("getType", getGBLSensorType, ccSensorPy_ccGBLSensor_getType_doc)
         ;
}
