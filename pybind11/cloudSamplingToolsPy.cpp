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

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccPointCloud.h>
#include <GenericIndexedCloudPersist.h>
#include <PointCloud.h>
#include <DgmOctree.h>
#include <ccScalarField.h>
#include <CloudSamplingTools.h>
#include <GenericProgressCallback.h>
#include "cloudSamplingToolsPy_DocStrings.hpp"

#include "PyScalarType.h"
#include "pyccTrace.h"

ccPointCloud* resampleCloudWithOctree_py(ccPointCloud* cloud,
                                         int newNumberOfPoints,
                                         CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD resamplingMethod,
                                         CCCoreLib::GenericProgressCallback* progressCb = nullptr,
                                         CCCoreLib::DgmOctree* inputOctree = nullptr)
{
    CCCoreLib::GenericIndexedCloud* gic = CCCoreLib::CloudSamplingTools::resampleCloudWithOctree(cloud, newNumberOfPoints, resamplingMethod, progressCb, inputOctree);
    ccPointCloud* result = ccPointCloud::From(gic, dynamic_cast<ccGenericPointCloud*>(cloud));
    return result;
}

ccPointCloud* resampleCloudWithOctreeAtLevel_py( CCCoreLib::GenericIndexedCloudPersist* cloud,
                                                 unsigned char octreeLevel,
                                                 CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD resamplingMethod,
                                                 CCCoreLib::GenericProgressCallback* progressCb = nullptr,
                                                 CCCoreLib::DgmOctree* inputOctree = nullptr)
{
    CCCoreLib::PointCloud* ptc = CCCoreLib::CloudSamplingTools::resampleCloudWithOctreeAtLevel(cloud, octreeLevel, resamplingMethod, progressCb, inputOctree);
    ccPointCloud* result = ccPointCloud::From(ptc, dynamic_cast<ccGenericPointCloud*>(cloud));
    return result;
}

void export_cloudSamplingTools(py::module &m0)
{

    py::enum_<CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD>(m0, "RESAMPLING_CELL_METHOD")
        .value("CELL_CENTER", CCCoreLib::CloudSamplingTools::CELL_CENTER)
        .value("CELL_GRAVITY_CENTER", CCCoreLib::CloudSamplingTools::CELL_GRAVITY_CENTER)
        .export_values();
        ;

    py::enum_<CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD>(m0, "SUBSAMPLING_CELL_METHOD")
        .value("RANDOM_POINT", CCCoreLib::CloudSamplingTools::RANDOM_POINT)
        .value("NEAREST_POINT_TO_CELL_CENTER", CCCoreLib::CloudSamplingTools::NEAREST_POINT_TO_CELL_CENTER)
        .export_values();
        ;

    py::class_<CCCoreLib::CloudSamplingTools::SFModulationParams>(m0, "SFModulationParams", CloudSamplingToolsPy_SFModulationParams_doc)
        .def(py::init<>())
        .def_readwrite("enabled", &CCCoreLib::CloudSamplingTools::SFModulationParams::enabled,
                       CloudSamplingToolsPy_enabled_doc)
        .def_readwrite("a", &CCCoreLib::CloudSamplingTools::SFModulationParams::a,
                       CloudSamplingToolsPy_a_doc)
        .def_readwrite("b", &CCCoreLib::CloudSamplingTools::SFModulationParams::b,
                       CloudSamplingToolsPy_b_doc)
        ;

     py::class_<CCCoreLib::CloudSamplingTools>(m0, "CloudSamplingTools",
                                               CloudSamplingToolsPy_CloudSamplingTools_doc)
        .def_static("resampleCloudWithOctreeAtLevel",
             &resampleCloudWithOctreeAtLevel_py,
             py::arg("cloud"), py::arg("octreeLevel"), py::arg("resamplingMethod"),
             py::arg("progressCb")=nullptr,
             py::arg("inputOctree")=nullptr,
             CloudSamplingToolsPy_resampleCloudWithOctreeAtLevel_doc, py::return_value_policy::reference)

        .def_static("resampleCloudWithOctree",
             &resampleCloudWithOctree_py,
             py::arg("cloud"), py::arg("newNumberOfPoints"), py::arg("resamplingMethod"),
             py::arg("progressCb")=nullptr,
             py::arg("inputOctree")=nullptr,
             CloudSamplingToolsPy_resampleCloudWithOctree_doc, py::return_value_policy::reference)

        .def_static("subsampleCloudWithOctreeAtLevel",
             &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel,
             py::arg("cloud"), py::arg("octreeLevel"), py::arg("subsamplingMethod"),
             py::arg("progressCb")=nullptr,
             py::arg("inputOctree")=nullptr,
            CloudSamplingToolsPy_subsampleCloudWithOctreeAtLevel_doc, py::return_value_policy::reference)

        .def_static("subsampleCloudWithOctree",
             &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctree,
             py::arg("cloud"), py::arg("newNumberOfPoints"), py::arg("subsamplingMethod"),
             py::arg("progressCb")=nullptr,
             py::arg("inputOctree")=nullptr,
             CloudSamplingToolsPy_subsampleCloudWithOctree_doc, py::return_value_policy::reference)

        .def_static("subsampleCloudRandomly",
             &CCCoreLib::CloudSamplingTools::subsampleCloudRandomly,
             py::arg("cloud"), py::arg("newNumberOfPoints"), py::arg("progressCb")=nullptr,
             CloudSamplingToolsPy_subsampleCloudRandomly_doc, py::return_value_policy::reference)

        .def_static("resampleCloudSpatially",
             &CCCoreLib::CloudSamplingTools::resampleCloudSpatially,
             py::arg("cloud"), py::arg("minDistance"),
			 py::arg("modParams")=CCCoreLib::CloudSamplingTools::SFModulationParams(false),
             py::arg("octree")=nullptr,
             py::arg("progressCb")=nullptr,
             CloudSamplingToolsPy_resampleCloudSpatially_doc, py::return_value_policy::reference)

        .def_static("sorFilter",
             &CCCoreLib::CloudSamplingTools::sorFilter,
             py::arg("cloud"), py::arg("knn")=6, py::arg("nSigma")=1.0,
             py::arg("octree")=nullptr,
             py::arg("progressCb")=nullptr,
             CloudSamplingToolsPy_sorFilter_doc, py::return_value_policy::reference)

        .def_static("noiseFilter",
             &CCCoreLib::CloudSamplingTools::noiseFilter,
             py::arg("cloud"), py::arg("kernelRadius"), py::arg("nSigma"),
             py::arg("removeIsolatedPoints")=false, py::arg("useKnn")=false,
             py::arg("knn")=6, py::arg("useAbsoluteError")=true, py::arg("absoluteError")=0,
             py::arg("octree")=nullptr,
             py::arg("progressCb")=nullptr,
             CloudSamplingToolsPy_noiseFilter_doc, py::return_value_policy::reference)
        ;
}
