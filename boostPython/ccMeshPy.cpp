//##########################################################################
//#                                                                        #
//#                                PYCC                                    #
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

#include "ccMeshPy.hpp"

#include <boost/python.hpp>

#include <CCGeom.h>
#include <ccMesh.h>
#include <ccGenericPointCloud.h>
#include <ccPointCloud.h>
#include <GenericProgressCallback.h>

#include "ccMeshPy_DocStrings.hpp"

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
             mesh_samplePoints_py_overloads(ccGenericMeshPy_samplePoints_doc)[return_value_policy<reference_existing_object>()])
        ;

    class_<ccMesh, bases<ccGenericMesh> >("ccMesh", ccMeshPy_ccMesh_doc, no_init)
        .def("cloneMesh", &cloneMesh_py, return_value_policy<reference_existing_object>(), ccMeshPy_cloneMesh_doc)
        .def("size", &ccMesh::size)
        .def("getAssociatedCloud", &ccMesh::getAssociatedCloud,
             return_value_policy<reference_existing_object>(), ccMeshPy_getAssociatedCloud_doc)
        .def("laplacianSmooth", &laplacianSmooth_py, laplacianSmooth_py_overloads(ccMeshPy_laplacianSmooth_doc))
        .def("subdivide", &ccMesh::subdivide, return_value_policy<reference_existing_object>(), ccMeshPy_subdivide_doc)
        .def("triangulate",
             &ccMesh::Triangulate,
             ccMesh_triangulate_overloads(ccMeshPy_triangulate_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("triangulate")
        ;
}
