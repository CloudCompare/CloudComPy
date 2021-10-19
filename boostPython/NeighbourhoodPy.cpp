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

#include "NeighbourhoodPy.hpp"

#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

#include <Neighbourhood.h>
#include "NeighbourhoodPy_DocStrings.hpp"

namespace bp = boost::python;
namespace bnp = boost::python::numpy;

using namespace boost::python;

void export_Neighbourhood()
{
	enum_<CCCoreLib::Neighbourhood::GeomFeature>("GeomFeature")
		.value("EigenValuesSum", CCCoreLib::Neighbourhood::EigenValuesSum)
		.value("Omnivariance", CCCoreLib::Neighbourhood::Omnivariance)
		.value("EigenEntropy", CCCoreLib::Neighbourhood::EigenEntropy)
		.value("Anisotropy", CCCoreLib::Neighbourhood::Anisotropy)
		.value("Planarity", CCCoreLib::Neighbourhood::Planarity)
		.value("Linearity", CCCoreLib::Neighbourhood::Linearity)
		.value("PCA1", CCCoreLib::Neighbourhood::PCA1)
		.value("PCA2", CCCoreLib::Neighbourhood::PCA2)
		.value("SurfaceVariation", CCCoreLib::Neighbourhood::SurfaceVariation)
		.value("Sphericity", CCCoreLib::Neighbourhood::Sphericity)
		.value("Verticality", CCCoreLib::Neighbourhood::Verticality)
		.value("EigenValue1", CCCoreLib::Neighbourhood::EigenValue1)
		.value("EigenValue2", CCCoreLib::Neighbourhood::EigenValue2)
		.value("EigenValue3", CCCoreLib::Neighbourhood::EigenValue3)
     	;

}
