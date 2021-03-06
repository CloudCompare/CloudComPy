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

#include "cloudSamplingToolsPy.hpp"

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

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

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


BOOST_PYTHON_FUNCTION_OVERLOADS(resampleCloudWithOctreeAtLevel_py_overloads, resampleCloudWithOctreeAtLevel_py, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(resampleCloudWithOctree_py_overloads, resampleCloudWithOctree_py, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(subsampleCloudWithOctreeAtLevel_overloads, CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(subsampleCloudWithOctree_overloads, CCCoreLib::CloudSamplingTools::subsampleCloudWithOctree, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(subsampleCloudRandomly_overloads, CCCoreLib::CloudSamplingTools::subsampleCloudRandomly, 2, 3)
BOOST_PYTHON_FUNCTION_OVERLOADS(resampleCloudSpatially_overloads, CCCoreLib::CloudSamplingTools::resampleCloudSpatially, 3, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(sorFilter_overloads, CCCoreLib::CloudSamplingTools::sorFilter, 1, 5)
BOOST_PYTHON_FUNCTION_OVERLOADS(noiseFilter_overloads, CCCoreLib::CloudSamplingTools::noiseFilter, 3,10)

void export_cloudSamplingTools()
{

    enum_<CCCoreLib::CloudSamplingTools::RESAMPLING_CELL_METHOD>("RESAMPLING_CELL_METHOD")
        .value("CELL_CENTER", CCCoreLib::CloudSamplingTools::CELL_CENTER)
        .value("CELL_GRAVITY_CENTER", CCCoreLib::CloudSamplingTools::CELL_GRAVITY_CENTER)
        ;

    enum_<CCCoreLib::CloudSamplingTools::SUBSAMPLING_CELL_METHOD>("SUBSAMPLING_CELL_METHOD")
        .value("RANDOM_POINT", CCCoreLib::CloudSamplingTools::RANDOM_POINT)
        .value("NEAREST_POINT_TO_CELL_CENTER", CCCoreLib::CloudSamplingTools::NEAREST_POINT_TO_CELL_CENTER)
        ;

    class_<CCCoreLib::CloudSamplingTools::SFModulationParams>("SFModulationParams", CloudSamplingToolsPy_SFModulationParams_doc)
        .def_readwrite("enabled", &CCCoreLib::CloudSamplingTools::SFModulationParams::enabled,
                       CloudSamplingToolsPy_enabled_doc)
        .def_readwrite("a", &CCCoreLib::CloudSamplingTools::SFModulationParams::a,
                       CloudSamplingToolsPy_a_doc)
        .def_readwrite("b", &CCCoreLib::CloudSamplingTools::SFModulationParams::b,
                       CloudSamplingToolsPy_b_doc)
        ;

         class_<CCCoreLib::CloudSamplingTools, boost::noncopyable>("CloudSamplingTools",
                                                              CloudSamplingToolsPy_CloudSamplingTools_doc, no_init)
        .def("resampleCloudWithOctreeAtLevel",
             &resampleCloudWithOctreeAtLevel_py,
             resampleCloudWithOctreeAtLevel_py_overloads(CloudSamplingToolsPy_resampleCloudWithOctreeAtLevel_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("resampleCloudWithOctreeAtLevel")

        .def("resampleCloudWithOctree",
             &resampleCloudWithOctree_py,
             resampleCloudWithOctree_py_overloads(CloudSamplingToolsPy_resampleCloudWithOctree_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("resampleCloudWithOctree")

        .def("subsampleCloudWithOctreeAtLevel",
             &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel,
             subsampleCloudWithOctreeAtLevel_overloads(CloudSamplingToolsPy_subsampleCloudWithOctreeAtLevel_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("subsampleCloudWithOctreeAtLevel")

        .def("subsampleCloudWithOctree",
             &CCCoreLib::CloudSamplingTools::subsampleCloudWithOctree,
             subsampleCloudWithOctree_overloads(CloudSamplingToolsPy_subsampleCloudWithOctree_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("subsampleCloudWithOctree")

        .def("subsampleCloudRandomly",
             &CCCoreLib::CloudSamplingTools::subsampleCloudRandomly,
             subsampleCloudRandomly_overloads(CloudSamplingToolsPy_subsampleCloudRandomly_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("subsampleCloudRandomly")

        .def("resampleCloudSpatially",
             &CCCoreLib::CloudSamplingTools::resampleCloudSpatially,
             resampleCloudSpatially_overloads(CloudSamplingToolsPy_resampleCloudSpatially_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("resampleCloudSpatially")

        .def("sorFilter",
             &CCCoreLib::CloudSamplingTools::sorFilter,
             sorFilter_overloads(CloudSamplingToolsPy_sorFilter_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("sorFilter")

        .def("noiseFilter",
             &CCCoreLib::CloudSamplingTools::noiseFilter,
             noiseFilter_overloads(CloudSamplingToolsPy_noiseFilter_doc)[return_value_policy<reference_existing_object>()])
            .staticmethod("noiseFilter")
        ;
}
