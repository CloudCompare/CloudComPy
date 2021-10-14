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

#include "geometricalAnalysisToolsPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <ccPointCloud.h>
#include <GenericIndexedCloudPersist.h>
#include <PointCloud.h>
#include <DgmOctree.h>
#include <ccScalarField.h>
#include <CCToolbox.h>
#include <GeometricalAnalysisTools.h>
#include <GenericProgressCallback.h>
#include <ccMesh.h>
#include "geometricalAnalysisToolsPy_DocStrings.hpp"

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

BOOST_PYTHON_FUNCTION_OVERLOADS(FlagDuplicatePoints_overloads, CCCoreLib::GeometricalAnalysisTools::FlagDuplicatePoints, 1, 4)

void export_geometricalAnalysisTools()
{
	enum_<CCCoreLib::GeometricalAnalysisTools::Density>("Density")
		.value("DENSITY_KNN", CCCoreLib::GeometricalAnalysisTools::DENSITY_KNN)
		.value("DENSITY_2D", CCCoreLib::GeometricalAnalysisTools::DENSITY_2D)
		.value("DENSITY_3D", CCCoreLib::GeometricalAnalysisTools::DENSITY_3D)
     	;

    enum_<CCCoreLib::GeometricalAnalysisTools::ErrorCode>("ErrorCode")
        .value("NoError", CCCoreLib::GeometricalAnalysisTools::NoError)
        .value("InvalidInput", CCCoreLib::GeometricalAnalysisTools::InvalidInput)
        .value("NotEnoughPoints", CCCoreLib::GeometricalAnalysisTools::NotEnoughPoints)
        .value("OctreeComputationFailed", CCCoreLib::GeometricalAnalysisTools::OctreeComputationFailed)
        .value("ProcessFailed", CCCoreLib::GeometricalAnalysisTools::ProcessFailed)
        .value("UnhandledCharacteristic", CCCoreLib::GeometricalAnalysisTools::UnhandledCharacteristic)
        .value("NotEnoughMemory", CCCoreLib::GeometricalAnalysisTools::NotEnoughMemory)
        .value("ProcessCancelledByUser", CCCoreLib::GeometricalAnalysisTools::ProcessCancelledByUser)
        ;

    class_<CCCoreLib::GeometricalAnalysisTools, boost::noncopyable>("GeometricalAnalysisTools",
                                                                    geometricalAnalysisToolsPy_GeometricalAnalysisTools_doc, no_init)
        .def("FlagDuplicatePoints",
             &CCCoreLib::GeometricalAnalysisTools::FlagDuplicatePoints,
             FlagDuplicatePoints_overloads(geometricalAnalysisToolsPy_FlagDuplicatePoints_doc))
            .staticmethod("FlagDuplicatePoints")

        ;


}
