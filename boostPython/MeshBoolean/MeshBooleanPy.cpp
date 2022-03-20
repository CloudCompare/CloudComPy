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

//enum CSG_OPERATION { UNION, INTERSECT, DIFF, SYM_DIFF };

ccMesh* computeMeshBoolean( ccMesh* meshA,
                            ccMesh* meshB,
                            CSG_OPERATION operation)
{
    //try to convert both meshes to IGLMesh structures
    IGLMesh iglMeshA;
    if (!ToIGLMesh(meshA, iglMeshA))
        return nullptr;
    IGLMesh iglMeshB;
    if (!ToIGLMesh(meshB, iglMeshB))
        return nullptr;

    qMeshBoolean::s_params.app = nullptr;
    qMeshBoolean::s_params.meshA = &iglMeshA;
    qMeshBoolean::s_params.meshB = &iglMeshB;
    qMeshBoolean::s_params.nameA = meshA->getName();
    qMeshBoolean::s_params.nameB = meshB->getName();
    qMeshBoolean::s_params.operation = operation;

    bool res = DoPerformMeshBooleanOp();

    if (!res)
    {
        CCTRACE("error in computeMeshBoolean");
        return nullptr;
    }

    //convert the updated mesh (A) to a new ccMesh structure
    ccMesh* result = FromIGLMesh(qMeshBoolean::s_params.output);

    if (result)
    {
        //set name
        QString opName;
        switch (operation)
        {
        case CSG_OPERATION::UNION:
            opName = "union";
            break;
        case CSG_OPERATION::INTERSECT:
            opName = "isect";
            break;
        case CSG_OPERATION::DIFF:
            opName = "diff";
            break;
        case CSG_OPERATION::SYM_DIFF:
            opName = "sym_diff";
            break;
        default:
            assert(false);
            break;
        }
        result->setName(QString("(%1).%2.(%3)").arg(meshA->getName()).arg(opName).arg(meshB->getName()));

        //normals
        bool hasNormals = false;
        if (meshA->hasTriNormals())
            hasNormals = result->computePerTriangleNormals();
        else if (meshA->hasNormals())
            hasNormals = result->computePerVertexNormals();
        meshA->showNormals(hasNormals && meshA->normalsShown());
    }
    return result;
}

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
