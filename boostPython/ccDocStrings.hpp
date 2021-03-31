//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
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

#ifndef BOOSTPYTHON_CCDOCSTRINGS_HPP_
#define BOOSTPYTHON_CCDOCSTRINGS_HPP_

const char* loadPointCloud_doc= R"(
Load a 3D cloud from a file
parameters:
- filename
- shift mode from (CC_SHIFT_MODE.AUTO, CC_SHIFT_MODE.XYZ),  optional, default AUTO
    CC_SHIFT_MODE.AUTO: automatic shift of coordinates
    CC_SHIFT_MODE.XYZ:  coordinates shift given by x, y, z parameters
- skip optional parameter not used yet! default 0
- x, y, z:: optional shift values for coordinates (mode XYZ),  default 0
return a ccPointCloud object.
Usage: see ccPointCloud doc.)";





#endif /* BOOSTPYTHON_CCDOCSTRINGS_HPP_ */
