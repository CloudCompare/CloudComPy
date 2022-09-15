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

#ifndef CCOCTREEPY_HPP_
#define CCOCTREEPY_HPP_

#include <CCGeom.h>
#include <DgmOctree.h>

struct PointDescriptor_persistent_py
{
    const CCVector3 point;
    unsigned pointIndex;
    double squareDistd;
    PointDescriptor_persistent_py();
    PointDescriptor_persistent_py(const CCCoreLib::DgmOctree::PointDescriptor& pt);
};

void export_ccOctree();

#endif
