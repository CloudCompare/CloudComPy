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

void export_geometricalAnalysisTools(py::module &m0)
{
	py::enum_<CCCoreLib::GeometricalAnalysisTools::Density>(m0, "Density")
		.value("DENSITY_KNN", CCCoreLib::GeometricalAnalysisTools::DENSITY_KNN)
		.value("DENSITY_2D", CCCoreLib::GeometricalAnalysisTools::DENSITY_2D)
		.value("DENSITY_3D", CCCoreLib::GeometricalAnalysisTools::DENSITY_3D)
        .export_values();
     	;

	py::enum_<CCCoreLib::GeometricalAnalysisTools::ErrorCode>(m0, "ErrorCode")
        .value("NoError", CCCoreLib::GeometricalAnalysisTools::NoError)
        .value("InvalidInput", CCCoreLib::GeometricalAnalysisTools::InvalidInput)
        .value("NotEnoughPoints", CCCoreLib::GeometricalAnalysisTools::NotEnoughPoints)
        .value("OctreeComputationFailed", CCCoreLib::GeometricalAnalysisTools::OctreeComputationFailed)
        .value("ProcessFailed", CCCoreLib::GeometricalAnalysisTools::ProcessFailed)
        .value("UnhandledCharacteristic", CCCoreLib::GeometricalAnalysisTools::UnhandledCharacteristic)
        .value("NotEnoughMemory", CCCoreLib::GeometricalAnalysisTools::NotEnoughMemory)
        .value("ProcessCancelledByUser", CCCoreLib::GeometricalAnalysisTools::ProcessCancelledByUser)
        .export_values();
        ;

    py::class_<CCCoreLib::GeometricalAnalysisTools>(m0, "GeometricalAnalysisTools",
                                                    geometricalAnalysisToolsPy_GeometricalAnalysisTools_doc)
        .def_static("FlagDuplicatePoints",
             &CCCoreLib::GeometricalAnalysisTools::FlagDuplicatePoints,
             py::arg("theCloud"), py::arg("minDistanceBetweenPoints")=std::numeric_limits<double>::epsilon(),
             py::arg("progressCb")=nullptr,
             py::arg("inputOctree")=nullptr,
             geometricalAnalysisToolsPy_FlagDuplicatePoints_doc)
        ;

}
