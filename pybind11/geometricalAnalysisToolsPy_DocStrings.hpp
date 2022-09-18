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

#ifndef GEOMETRICALANALYSISTOOLSPY_DOCSTRINGS_HPP_
#define GEOMETRICALANALYSISTOOLSPY_DOCSTRINGS_HPP_

const char* geometricalAnalysisToolsPy_GeometricalAnalysisTools_doc= R"(
Several algorithms to compute point-clouds geometric characteristics  (curvature, density, etc.)
)";

const char* geometricalAnalysisToolsPy_FlagDuplicatePoints_doc= R"(
Flag duplicate points.


This method only requires an output scalar field. Duplicate points will be
associated to scalar value 1 (and 0 for the others).

:param GenericIndexedCloudPersist theCloud: processed cloud
:param float,optional minDistanceBetweenPoints: min distance between (output) points,
       default C++ std::numeric_limits<double>::epsilon() i.e. ~ 2.2 e-16
:param GenericProgressCallback,optional progressCb: client application can get some notification of the process progress
       through this callback mechanism (see GenericProgressCallback)
:param DgmOctree,optional inputOctree: if not set as input, octree will be automatically computed.

:return: success (0) or error code (<0)
:rtype: ErrorCode
)";


#endif /* GEOMETRICALANALYSISTOOLSPY_DOCSTRINGS_HPP_ */
