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


namespace bp = boost::python;
namespace bnp = boost::python::numpy;

BOOST_PYTHON_MODULE(_MeshBoolean)
{
    using namespace boost::python;

    scope().attr("__doc__") = MeshBoolean_doc;

    enum_<CSG_OPERATION>("CSG_OPERATION")
        .value("UNION", CSG_OPERATION::UNION)
        .value("INTERSECT", CSG_OPERATION::INTERSECT)
        .value("DIFF", CSG_OPERATION::DIFF)
        .value("SYM_DIFF", CSG_OPERATION::SYM_DIFF)
        ;

    def("computeMeshBoolean", &computeMeshBoolean,
        return_value_policy<reference_existing_object>(), MeshBoolean_computeMeshBoolean_doc);

}
