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
#include <NormalEstimation.h>
#include <MLSSmoothingUpsampling.h>
#include <ccPointCloud.h>

void initTrace_PCL()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(FastGlobalRegistrationFilter_setParameters_overloads, FastGlobalRegistrationFilter::setParameters, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(NormalEstimation_setParameters_overloads, NormalEstimation::setParameters, 1, 5)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(MLSSmoothingUpsampling_setParameters_overloads, MLSSmoothingUpsampling::setParameters, 1, 12)



BOOST_PYTHON_MODULE(_PCL)
{
    using namespace boost::python;

    scope().attr("__doc__") = PCL_doc;

    class_<FastGlobalRegistrationFilter, boost::noncopyable>("FastGlobalRegistrationFilter", PCL_FastGlobalRegistrationFilter_doc)
        .def("compute", &FastGlobalRegistrationFilter::compute, PCL_FGRFcompute_doc)
        .def("getTransformation", &FastGlobalRegistrationFilter::getTransformation, PCL_FGRFgetTransformation_doc)
        .def("getAutoRadius", &FastGlobalRegistrationFilter::getAutoRadius, PCL_FGRFgetAutoRadius_doc)
        .def("setParameters", &FastGlobalRegistrationFilter::setParameters,
             FastGlobalRegistrationFilter_setParameters_overloads(
             (arg("refCloud"), arg("alignClouds"), arg("radius")=0.),
             PCL_FGRFsetParameters_doc))
        ;

    class_<NormalEstimation, boost::noncopyable>("NormalEstimation", PCL_NormalEstimation_doc)
        .def("compute", &NormalEstimation::compute, PCL_NEcompute_doc)
        .def("setParameters", &NormalEstimation::setParameters,
             NormalEstimation_setParameters_overloads(
             (arg("cloud"), arg("useKnn")=false, arg("ovewriteCurvature")=true, arg("knn")=10, arg("radius")=0.),
             PCL_NLsetParameters_doc))
        ;

    class_<MLSSmoothingUpsampling, boost::noncopyable>("MLSSmoothingUpsampling", PCL_MLSSmoothingUpsampling_doc)
        .def("compute", &MLSSmoothingUpsampling::compute, PCL_MLScompute_doc)
        .def("getOutputCloud", &MLSSmoothingUpsampling::getOutputCloud, return_value_policy<reference_existing_object>(), PCL_MLSgetOutputCloud_doc)
        .def("setParameters", &MLSSmoothingUpsampling::setParameters,
             MLSSmoothingUpsampling_setParameters_overloads(
             (arg("cloud"), arg("computeNormals")=true, arg("searchRadius")=0.01,
             arg("polyfit")=true, arg("order")=2, arg("sqrGauss")=0.0001,
             arg("upsampleMethod")=0, arg("upsampingRadius")=0.01, arg("upsamplingStep")=0.01,
             arg("stepPointDensity")=4, arg("dilationVoxelSize")=0.01, arg("dilationIterations")=0),
             PCL_MLSsetParameters_doc))
        ;

    def("initTrace_PCL", initTrace_PCL, PCL_initTrace_PCL_doc);

}
