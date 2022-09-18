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

#include <ccMesh.h>

#include <qMeshBoolean.h>

#include "pyccTrace.h"
#include "MeshBoolean_DocStrings.hpp"

void initTrace_MeshBoolean()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}


PYBIND11_MODULE(_MeshBoolean, m4)
{
    m4.doc() = MeshBoolean_doc;

    py::enum_<CSG_OPERATION>(m4, "CSG_OPERATION")
        .value("UNION", CSG_OPERATION::UNION)
        .value("INTERSECT", CSG_OPERATION::INTERSECT)
        .value("DIFF", CSG_OPERATION::DIFF)
        .value("SYM_DIFF", CSG_OPERATION::SYM_DIFF)
        .export_values();
        ;

        m4.def("computeMeshBoolean", &computeMeshBoolean,
        py::return_value_policy::reference, MeshBoolean_computeMeshBoolean_doc);
        m4.def("initTrace_MeshBoolean", initTrace_MeshBoolean, MeshBoolean_initTrace_MeshBoolean_doc);
}
