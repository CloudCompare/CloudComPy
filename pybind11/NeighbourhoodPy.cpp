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

#include <Neighbourhood.h>
#include "NeighbourhoodPy_DocStrings.hpp"

void export_Neighbourhood(py::module &m0)
{
	py::enum_<CCCoreLib::Neighbourhood::GeomFeature>(m0, "GeomFeature")
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
        .export_values();
     	;
}
