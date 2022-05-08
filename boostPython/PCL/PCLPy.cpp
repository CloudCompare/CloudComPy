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


#include "pyccTrace.h"
#include "PCL_DocStrings.hpp"

#include <FastGlobalRegistrationFilter.h>
#include <ccPointCloud.h>

void initTrace_PCL()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FastGlobalRegistrationFilter_setParameters_overloads, FastGlobalRegistrationFilter::setParameters, 2, 3)



BOOST_PYTHON_MODULE(_PCL)
{
    using namespace boost::python;

    scope().attr("__doc__") = PCL_doc;

    class_<FastGlobalRegistrationFilter, boost::noncopyable>("FastGlobalRegistrationFilter", PCL_FastGlobalRegistrationFilter_doc)
        .def("compute", &FastGlobalRegistrationFilter::compute, PCL_compute_doc)
        .def("getAutoRadius", &FastGlobalRegistrationFilter::getAutoRadius, PCL_getAutoRadius_doc)
        .def("setParameters", &FastGlobalRegistrationFilter::setParameters,
             FastGlobalRegistrationFilter_setParameters_overloads(
             (arg("refCloud"), arg("alignClouds"), arg("radius")=0.),
             PCL_setParameters_doc))
        ;
    def("initTrace_PCL", initTrace_PCL, PCL_initTrace_PCL_doc);

}
