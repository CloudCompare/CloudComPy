//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
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

#include <boost/python.hpp>

#include "initCC.h"
#include "pyCC.h"
#include "PyScalarType.h"

char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(cloudComPy)
{
    using namespace boost::python;
    def("greet", greet);

    enum_<CC_SHIFT_MODE>("CC_SHIFT_MODE")
        .value("AUTO", AUTO)
        .value("XYZ", XYZ)
        ;

    class_<ccPointCloud>("ccPointCloud")
        .def("hasScalarFields", &ccPointCloud::hasScalarFields);

    //ccPointCloud* (*l0)(const char*) = &loadPointCloud;
    def("loadPointCloud", loadPointCloud,
        return_value_policy<reference_existing_object>());


}
