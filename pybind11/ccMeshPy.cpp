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
#include <ccGenericPointCloud.h>
#include <ccPointCloud.h>
#include <ccPolyline.h>
#include <GenericProgressCallback.h>

#include "ccMeshPy_DocStrings.hpp"

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

void export_ccMesh(py::module &m0)
{
    py::enum_<CCCoreLib::TRIANGULATION_TYPES>(m0, "TRIANGULATION_TYPES", ccMeshPy_TRIANGULATION_TYPES_doc)
        .value("DELAUNAY_2D_AXIS_ALIGNED", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_AXIS_ALIGNED)
        .value("DELAUNAY_2D_BEST_LS_PLANE", CCCoreLib::TRIANGULATION_TYPES::DELAUNAY_2D_BEST_LS_PLANE)
        .export_values();
        ;

    py::class_<CCCoreLib::GenericIndexedMesh>(m0, "GenericIndexedMesh") //, no_init, boost::noncopyable
        ;

    py::class_<ccGenericMesh, CCCoreLib::GenericIndexedMesh, ccShiftedObject>(m0, "ccGenericMesh") //, no_init, boost::noncopyable
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
        .def("crop2D", &ccMesh::crop2D, py::return_value_policy::reference, ccMeshPy_crop2D_doc)
        .def("size", &ccMesh::size, ccMeshPy_size_doc)
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
        ;
}
