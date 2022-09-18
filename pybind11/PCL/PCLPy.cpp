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

PYBIND11_MODULE(_PCL, m6)
{
    m6.doc() = PCL_doc;

    py::class_<FastGlobalRegistrationFilter>(m6, "FastGlobalRegistrationFilter", PCL_FastGlobalRegistrationFilter_doc)
        .def(py::init<>())
        .def("compute", &FastGlobalRegistrationFilter::compute, PCL_FGRFcompute_doc)
        .def("getTransformation", &FastGlobalRegistrationFilter::getTransformation, PCL_FGRFgetTransformation_doc)
        .def("getAutoRadius", &FastGlobalRegistrationFilter::getAutoRadius, PCL_FGRFgetAutoRadius_doc)
        .def("setParameters", &FastGlobalRegistrationFilter::setParameters,
             py::arg("refCloud"), py::arg("alignClouds"), py::arg("radius")=0.,
             PCL_FGRFsetParameters_doc)
        ;

    py::class_<NormalEstimation>(m6, "NormalEstimation", PCL_NormalEstimation_doc)
        .def(py::init<>())
        .def("compute", &NormalEstimation::compute, PCL_NEcompute_doc)
        .def("setParameters", &NormalEstimation::setParameters,
             py::arg("cloud"), py::arg("useKnn")=false, py::arg("ovewriteCurvature")=true, py::arg("knn")=10, py::arg("radius")=0.,
             PCL_NLsetParameters_doc)
        ;

    py::class_<MLSSmoothingUpsampling>(m6, "MLSSmoothingUpsampling", PCL_MLSSmoothingUpsampling_doc)
        .def(py::init<>())
        .def("compute", &MLSSmoothingUpsampling::compute, PCL_MLScompute_doc)
        .def("getOutputCloud", &MLSSmoothingUpsampling::getOutputCloud, py::return_value_policy::reference, PCL_MLSgetOutputCloud_doc)
        .def("setParameters", &MLSSmoothingUpsampling::setParameters,
             py::arg("cloud"), py::arg("computeNormals")=true, py::arg("searchRadius")=0.01,
             py::arg("polyfit")=true, py::arg("order")=2, py::arg("sqrGauss")=0.0001,
             py::arg("upsampleMethod")=0, py::arg("upsampingRadius")=0.01, py::arg("upsamplingStep")=0.01,
             py::arg("stepPointDensity")=4, py::arg("dilationVoxelSize")=0.01, py::arg("dilationIterations")=0,
             PCL_MLSsetParameters_doc)
        ;

    m6.def("initTrace_PCL", initTrace_PCL, PCL_initTrace_PCL_doc);
}
