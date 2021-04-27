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

#include "registrationToolsPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <RegistrationTools.h>

#include "PyScalarType.h"
#include "pyccTrace.h"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

void export_registrationTools()
{

    class_<CCCoreLib::RegistrationTools, boost::noncopyable>("RegistrationTools", no_init)
        ;

    enum_<CCCoreLib::RegistrationTools::TRANSFORMATION_FILTERS>("TRANSFORMATION_FILTERS")
        .value("SKIP_NONE", CCCoreLib::RegistrationTools::SKIP_NONE)
        .value("SKIP_RXY", CCCoreLib::RegistrationTools::SKIP_RXY)
        .value("SKIP_RYZ", CCCoreLib::RegistrationTools::SKIP_RYZ)
        .value("SKIP_RXZ", CCCoreLib::RegistrationTools::SKIP_RXZ)
        .value("SKIP_ROTATION", CCCoreLib::RegistrationTools::SKIP_ROTATION)
        .value("SKIP_TX", CCCoreLib::RegistrationTools::SKIP_TX)
        .value("SKIP_TY", CCCoreLib::RegistrationTools::SKIP_TY)
        .value("SKIP_TZ", CCCoreLib::RegistrationTools::SKIP_TZ)
        .value("SKIP_TRANSLATION", CCCoreLib::RegistrationTools::SKIP_TRANSLATION)
        ;

    class_<CCCoreLib::ICPRegistrationTools, bases<CCCoreLib::RegistrationTools>, boost::noncopyable>("ICPRegistrationTools", no_init)
        ;

    enum_<CCCoreLib::ICPRegistrationTools::CONVERGENCE_TYPE>("CONVERGENCE_TYPE")
        .value("MAX_ERROR_CONVERGENCE", CCCoreLib::ICPRegistrationTools::MAX_ERROR_CONVERGENCE)
        .value("MAX_ITER_CONVERGENCE", CCCoreLib::ICPRegistrationTools::MAX_ITER_CONVERGENCE)
        ;

}
