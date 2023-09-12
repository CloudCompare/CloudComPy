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

#include <QString>
#include <vector>

#include <ccPointCloud.h>
#include <ccMesh.h>
#include <ccHObject.h>
#include <ccHObjectCaster.h>

#include <qCSF.h>

#include "pyccTrace.h"
#include "CSF_DocStrings.hpp"

void initTrace_CSF()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

PYBIND11_MODULE(_CSF, m7)
{
    m7.doc() = CSF_doc;

    m7.def("computeCSF", qCSF::computeCSF,
            py::arg("pc"), py::arg("csfRigidness")=2, py::arg("maxIteration")=500, py::arg("clothResolution")=2.0,
            py::arg("classThreshold")=0.5, py::arg("csfPostprocessing")=false, py::arg("computeMesh")=false,
            CSF_computeCSF_doc);
    m7.def("initTrace_CSF", initTrace_CSF, CSF_initTrace_CSF_doc);
}

