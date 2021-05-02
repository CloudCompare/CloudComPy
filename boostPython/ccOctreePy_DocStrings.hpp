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

#ifndef CCOCTREEPY_DOCSTRINGS_HPP_
#define CCOCTREEPY_DOCSTRINGS_HPP_

const char* DgmOctree_getBoundingBox_doc= R"(
Returns the octree bounding box
Method to request the octree bounding box limits
return tuple ((Xmin,Ymin,Zmin), (Xmax,Ymax,Zmax)))";

const char* DgmOctree_getNumberOfProjectedPoints_doc= R"(
Returns the number of points projected into the octree
return the number of projected points)";

const char* DgmOctree_getOctreeMins_doc= R"(
Returns the lower boundaries of the octree
return the lower coordinates along X,Y and Z)";

const char* DgmOctree_getOctreeMaxs_doc= R"(
Returns the higher boundaries of the octree
return the higher coordinates along X,Y and Z)";

const char* ccOctree_ccOctree_doc= R"(
Octree structure
Extends the CCCoreLib::DgmOctree class.
The octree structure used throughout the library
Implements the GenericOctree interface.
Corresponds to the octree structure developed during Daniel
Girardeau-Montaut's PhD (see PhD manuscript, Chapter 4).)";

#endif /* CCOCTREEPY_DOCSTRINGS_HPP_ */
