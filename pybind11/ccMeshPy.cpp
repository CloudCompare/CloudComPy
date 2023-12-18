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

#include <CCGeom.h>
#include <ccMesh.h>
#include <MeshSamplingTools.h>
#include <ccGenericPointCloud.h>
#include <ccPointCloud.h>
#include <ccPolyline.h>
#include <GenericProgressCallback.h>
#include <ccCommon.h>
#include <ccHObjectCaster.h>
#include <ccColorScalesManager.h>
#include <ccScalarField.h>

#include "ccMeshPy_DocStrings.hpp"

ccMesh* cloneMesh_py(ccMesh &self)
{
    return self.cloneMesh(nullptr, nullptr, nullptr, nullptr);
}

bool laplacianSmooth_py(ccMesh &self, unsigned nbIteration = 20, double factor=0.2)
{
    return self.laplacianSmooth(nbIteration, (float)factor);
}

ccPointCloud* mesh_samplePoints_py( ccGenericMesh& self,
                                    bool densityBased,
                                    double samplingParameter,
                                    bool withNormals = true,
                                    bool withRGB = true,
                                    bool withTexture = true,
                                    CCCoreLib::GenericProgressCallback* pDlg = nullptr)
{
    return self.samplePoints(densityBased, samplingParameter, withNormals, withRGB, withTexture, pDlg);
}

Tuple3Tpl<unsigned int> getTriangleVertIndexes_py(ccMesh &self, unsigned triangleIndex)
{
    CCCoreLib::VerticesIndexes* indexes = self.getTriangleVertIndexes(triangleIndex);
    Tuple3Tpl<unsigned int> ret = {indexes->i1, indexes->i2, indexes->i3};
    return ret;
};

py::array IndexesToNpArray_copy(ccMesh &self)
{
    CCTRACE("IndexesToNpArray with copy, ownership transfered to Python");
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    unsigned *s = (unsigned*)self.getTriangleVertIndexes(0);
    size_t ndim = 2;
    std::vector<size_t> shape =
    { nRows, 3 };
    std::vector<size_t> strides =
    { 3 * sizeof(unsigned), sizeof(unsigned) };
    return py::array(py::buffer_info(s,                                         // data as contiguous array
                                     sizeof(unsigned),                          // size of one scalar
                                     py::format_descriptor<unsigned>::format(), // data type
                                     ndim,                                      // number of dimensions
                                     shape,                                     // shape of the matrix
                                     strides                                    // strides for each axis
                                     ));
}

py::array IndexesToNpArray_py(ccMesh &self)
{
    CCTRACE("IndexesToNpArray without copy, ownership stays in C++");
    size_t nRows = self.size();
    CCTRACE("nrows: " << nRows);
    unsigned *s = (unsigned*)self.getTriangleVertIndexes(0);
    size_t ndim = 2;
    std::vector<size_t> shape =
    { nRows, 3 };
    std::vector<size_t> strides =
    { 3 * sizeof(unsigned), sizeof(unsigned) };
    auto capsule = py::capsule(s, [](void *v) { CCTRACE("C++ indexes not deleted"); });
    return py::array(py::buffer_info(s,                                         // data as contiguous array
                                     sizeof(unsigned),                          // size of one scalar
                                     py::format_descriptor<unsigned>::format(), // data type
                                     ndim,                                      // number of dimensions
                                     shape,                                     // shape of the matrix
                                     strides                                    // strides for each axis
                                     ),
                     capsule);
}

double computeMeshArea_py(ccGenericMesh* self)
{
    return CCCoreLib::MeshSamplingTools::computeMeshArea(self);
}

py::tuple computeMeshVolume_py(ccMesh* self)
{
    double V = CCCoreLib::MeshSamplingTools::computeMeshVolume(self);
    //check that the mesh is closed
    CCCoreLib::MeshSamplingTools::EdgeConnectivityStats stats;
    if (CCCoreLib::MeshSamplingTools::computeMeshEdgesConnectivity(self, stats))
    {
        bool isWarning = stats.edgesNotShared || stats.edgesSharedByMore;
        py::tuple res = py::make_tuple(V, isWarning, stats);
        return res;
    }
    else
    {
        bool isWarning = true;
        py::tuple res = py::make_tuple(V, isWarning, stats); // unitialised stats (0) means memory allocation problem...
        return res;
    }
}

bool flagVerticesByType_py(ccGenericMesh* mesh)
{
    ccPointCloud* vertices = ccHObjectCaster::ToPointCloud(mesh ? mesh->getAssociatedCloud() : nullptr);
    if (mesh && vertices)
    {
        //prepare a new scalar field
        int sfIdx = vertices->getScalarFieldIndexByName(CC_DEFAULT_MESH_VERT_FLAGS_SF_NAME);
        if (sfIdx < 0)
        {
            sfIdx = vertices->addScalarField(CC_DEFAULT_MESH_VERT_FLAGS_SF_NAME);
            if (sfIdx < 0)
            {
                CCTRACE("Not enough memory to flag the vertices of mesh " << mesh->getName().toStdString());
                return false;
            }
        }
        CCCoreLib::ScalarField* flags = vertices->getScalarField(sfIdx);

        CCCoreLib::MeshSamplingTools::EdgeConnectivityStats stats;
        if (CCCoreLib::MeshSamplingTools::flagMeshVerticesByType(mesh,flags,&stats))
        {
            vertices->setCurrentDisplayedScalarField(sfIdx);
            ccScalarField* sf = vertices->getCurrentDisplayedScalarField();
            if (sf)
            {
                sf->setColorScale(ccColorScalesManager::GetDefaultScale(ccColorScalesManager::VERTEX_QUALITY));
            }
            return true;
        }
        else
        {
            vertices->deleteScalarField(sfIdx);
            sfIdx = -1;
            CCTRACE("Not enough memory to flag the vertices of mesh " << mesh->getName().toStdString());
            return false;
        }
    }
    else
    {
        CCTRACE("internal problem");
        return false;
    }

}

void export_ccMesh(py::module &m0)
{
    py::class_<CCCoreLib::MeshSamplingTools::EdgeConnectivityStats>(m0, "EdgeConnectivityStats", ccMeshPy_EdgeConnectivityStats_doc)
        .def_readonly("edgesCount", &CCCoreLib::MeshSamplingTools::EdgeConnectivityStats::edgesCount)
        .def_readonly("edgesNotShared", &CCCoreLib::MeshSamplingTools::EdgeConnectivityStats::edgesNotShared)
        .def_readonly("edgesSharedByTwo", &CCCoreLib::MeshSamplingTools::EdgeConnectivityStats::edgesSharedByTwo)
        .def_readonly("edgesSharedByMore", &CCCoreLib::MeshSamplingTools::EdgeConnectivityStats::edgesSharedByMore)
        ;

    py::enum_<CCCoreLib::TRIANGULATION_TYPES>(m0, "TRIANGULATION_TYPES", ccMeshPy_TRIANGULATION_TYPES_doc)
        .value("DELAUNAY_2D_AXIS_ALIGNED", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_AXIS_ALIGNED)
        .value("DELAUNAY_2D_BEST_LS_PLANE", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_BEST_LS_PLANE)
        .export_values();
        ;

    py::class_<CCCoreLib::GenericIndexedMesh>(m0, "GenericIndexedMesh") //, no_init, boost::noncopyable
        ;

    py::class_<ccGenericMesh, CCCoreLib::GenericIndexedMesh, ccShiftedObject>(m0, "ccGenericMesh") //, no_init, boost::noncopyable
        .def("computeMeshArea", &computeMeshArea_py , ccGenericMeshPy_computeMeshArea_doc)
        .def("flagVerticesByType", &flagVerticesByType_py, ccGenericMeshPy_flagVerticesByType_doc)
        .def("samplePoints",
             &mesh_samplePoints_py,
              py::arg("densityBased"), py::arg("samplingParameter"),
              py::arg("withNormals")=true, py::arg("withRGB")=true, py::arg("withTexture")=true,
              py::arg("pDLg")=nullptr,
             ccGenericMeshPy_samplePoints_doc, py::return_value_policy::reference)
        ;

    py::class_<ccMesh, ccGenericMesh>(m0, "ccMesh", ccMeshPy_ccMesh_doc)
        .def("clearTriNormals", &ccMesh::clearTriNormals, ccMeshPy_clearTriNormals_doc)
        .def("cloneMesh", &cloneMesh_py, py::return_value_policy::reference, ccMeshPy_cloneMesh_doc)
        .def("computeMeshVolume", &computeMeshVolume_py , ccMeshPy_computeMeshVolume_doc)
        .def("crop2D", &ccMesh::crop2D, py::return_value_policy::reference, ccMeshPy_crop2D_doc)
        .def("size", &ccMesh::size, ccMeshPy_size_doc)
        .def("flipTriangles", &ccMesh::flipTriangles, ccMeshPy_flipTriangles_doc)
        .def("getAssociatedCloud", &ccMesh::getAssociatedCloud,
             py::return_value_policy::reference, ccMeshPy_getAssociatedCloud_doc)
        .def("getTriangleVertIndexes", &getTriangleVertIndexes_py, ccMeshPy_getTriangleVertIndexes_doc)
        .def("IndexesToNpArray", &IndexesToNpArray_py, ccMeshPy_IndexesToNpArray_doc)
        .def("IndexesToNpArray_copy", &IndexesToNpArray_copy, ccMeshPy_IndexesToNpArray_copy_doc)
        .def("laplacianSmooth", &laplacianSmooth_py,
             py::arg("nbIteration")=20, py::arg("factor")=0.2,
              ccMeshPy_laplacianSmooth_doc)
        .def("subdivide", &ccMesh::subdivide, py::return_value_policy::reference, ccMeshPy_subdivide_doc)
        .def_static("triangulate",
             &ccMesh::Triangulate,
             py::arg("cloud"), py::arg("type"), py::arg("updateNormals")=false, py::arg("maxEdgeLength")=0, py::arg("dim")=2,
                     ccMeshPy_triangulate_doc, py::return_value_policy::reference)
        .def_static("triangulateTwoPolylines",
             &ccMesh::TriangulateTwoPolylines,
             py::arg("p1"), py::arg("p2"), py::arg("projectionDir")=nullptr,
                     ccMeshPy_triangulateTwoPolylines_doc, py::return_value_policy::reference)
        ;
}
