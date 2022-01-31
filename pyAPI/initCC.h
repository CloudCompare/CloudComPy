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

#ifndef CLOUDCOMPY_PYAPI_INITCC_H_
#define CLOUDCOMPY_PYAPI_INITCC_H_

#include <QString>

//! class with only a static method for initialisation in a specific generated sip source (see cloudCompare.sip)

class initCC
{
public:
    //! get the runtime path of the python module cloudCompare, required to find the plugins
    static void init(const char* modulePath);

    //! runtime path of the python module cloudCompare, required to find the plugins
    static QString moduleDir;

private:
    initCC();
    initCC(const initCC&);
};

#endif /* CLOUDCOMPY_PYAPI_INITCC_H_ */
