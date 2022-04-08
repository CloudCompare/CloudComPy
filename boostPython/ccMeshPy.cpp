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
#include "ccMeshPy.hpp"

#include <CCGeom.h>
#include <ccMesh.h>
#include <ccGenericPointCloud.h>
#include <ccPointCloud.h>
#include <GenericProgressCallback.h>

#include "ccMeshPy_DocStrings.hpp"

namespace bp = boost::python;
using namespace boost::python;

ccMesh* cloneMesh_py(ccMesh &self)
{
    return self.cloneMesh(nullptr, nullptr, nullptr, nullptr);
}

bool laplacianSmooth_py(ccMesh &self, unsigned nbIteration = 20, double factor=0.2)
{
    return self.laplacianSmooth(nbIteration, (float)factor);
}

ccPointCloud* mesh_samplePoints_py( ccGenericMesh&self,
                                    bool densityBased,
                                    double samplingParameter,
                                    bool withNormals = true,
                                    bool withRGB = true,
                                    bool withTexture = true,
                                    CCCoreLib::GenericProgressCallback* pDlg = nullptr)
{
    return self.samplePoints(densityBased, samplingParameter, withNormals, withRGB, withTexture, pDlg);
}

BOOST_PYTHON_FUNCTION_OVERLOADS(ccMesh_triangulate_overloads, ccMesh::Triangulate, 2, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(laplacianSmooth_py_overloads, laplacianSmooth_py, 1, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(mesh_samplePoints_py_overloads, mesh_samplePoints_py, 3, 7)

void export_ccMesh()
{
    enum_<CCCoreLib::TRIANGULATION_TYPES>("TRIANGULATION_TYPES", ccMeshPy_TRIANGULATION_TYPES_doc)
        .value("DELAUNAY_2D_AXIS_ALIGNED", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_AXIS_ALIGNED)
        .value("DELAUNAY_2D_BEST_LS_PLANE", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_BEST_LS_PLANE)
        ;

    class_<CCCoreLib::GenericIndexedMesh, boost::noncopyable>("GenericIndexedMesh", no_init)
        ;

    class_<ccGenericMesh, bases<CCCoreLib::GenericIndexedMesh, ccShiftedObject>, boost::noncopyable>("ccGenericMesh", no_init)
        .def("samplePoints",
             &mesh_samplePoints_py,
             mesh_samplePoints_py_overloads(
             (arg("self"), arg("densityBased"), arg("samplingParameter"),
              arg("withNormals")=true, arg("withRGB")=true, arg("withTexture")=true,
              arg("pDLg")=bp::ptr((CCCoreLib::GenericProgressCallback*)nullptr)),
             ccGenericMeshPy_samplePoints_doc)[return_value_policy<reference_existing_object>()])
        ;

    class_<ccMesh, bases<ccGenericMesh>, boost::noncopyable>("ccMesh", ccMeshPy_ccMesh_doc, no_init)
        .def("clearTriNormals", &ccMesh::clearTriNormals, ccMeshPy_clearTriNormals_doc)
        .def("cloneMesh", &cloneMesh_py, return_value_policy<reference_existing_object>(), ccMeshPy_cloneMesh_doc)
        .def("size", &ccMesh::size)
        .def("getAssociatedCloud", &ccMesh::getAssociatedCloud,
             return_value_policy<reference_existing_object>(), ccMeshPy_getAssociatedCloud_doc)
        .def("laplacianSmooth", &laplacianSmooth_py, laplacianSmooth_py_overloads(
             (arg("self"), arg("nbIteration")=20, arg("factor")=0.2 ),
              ccMeshPy_laplacianSmooth_doc))
        .def("subdivide", &ccMesh::subdivide, return_value_policy<reference_existing_object>(), ccMeshPy_subdivide_doc)
        .def("triangulate",
             &ccMesh::Triangulate,
             ccMesh_triangulate_overloads(
                     (arg("cloud"), arg("type"), arg("updateNormals")=false, arg("maxEdgeLength")=0, arg("dim")=2),
                     ccMeshPy_triangulate_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("triangulate")
        ;
}
