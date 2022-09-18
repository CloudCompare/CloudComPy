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

#include <RegistrationTools.h>

#include "PyScalarType.h"
#include "pyccTrace.h"


void export_registrationTools(py::module &m0)
{

    py::class_<CCCoreLib::RegistrationTools>(m0, "RegistrationTools")
        ;

    py::enum_<CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS>(m0, "TRANSFORMATION_FILTERS")
        .value("SKIP_NONE", CCCoreLib::RegistrationTools::SKIP_NONE)
        .value("SKIP_RXY", CCCoreLib::RegistrationTools::SKIP_RXY)
        .value("SKIP_RYZ", CCCoreLib::RegistrationTools::SKIP_RYZ)
        .value("SKIP_RXZ", CCCoreLib::RegistrationTools::SKIP_RXZ)
        .value("SKIP_ROTATION", CCCoreLib::RegistrationTools::SKIP_ROTATION)
        .value("SKIP_TX", CCCoreLib::RegistrationTools::SKIP_TX)
        .value("SKIP_TY", CCCoreLib::RegistrationTools::SKIP_TY)
        .value("SKIP_TZ", CCCoreLib::RegistrationTools::SKIP_TZ)
        .value("SKIP_TRANSLATION", CCCoreLib::RegistrationTools::SKIP_TRANSLATION)
        .export_values();

    py::class_<CCCoreLib::ICPRegistrationTools, CCCoreLib::RegistrationTools>(m0, "ICPRegistrationTools")
        ;

    py::enum_<CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE>(m0, "CONVERGENCE_TYPE")
        .value("MAX_ERROR_CONVERGENCE", CCCoreLib::ICPRegistrationTools::MAX_ERROR_CONVERGENCE)
        .value("MAX_ITER_CONVERGENCE", CCCoreLib::ICPRegistrationTools::MAX_ITER_CONVERGENCE)
        .export_values();

}
