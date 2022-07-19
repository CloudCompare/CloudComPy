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

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

void initTrace_CSF()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

BOOST_PYTHON_FUNCTION_OVERLOADS(computeCSF_overloads, qCSF::computeCSF, 1, 6)


BOOST_PYTHON_MODULE(_CSF)
{
    using namespace boost::python;

    scope().attr("__doc__") = CSF_doc;

    def("computeCSF", qCSF::computeCSF,
            computeCSF_overloads(
        (arg("pc"), arg("csfRigidness")=2, arg("maxIteration")=500, arg("clothResolution")=2.0,
         arg("classThreshold")=0.5, arg("csfPostprocessing")=false), CSF_computeCSF_doc));
    def("initTrace_CSF", initTrace_CSF, CSF_initTrace_CSF_doc);

}

