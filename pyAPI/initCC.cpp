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

#include <pyccTrace.h>
#include "initCC.h"
#include <QDir>
#include <QStandardPaths>
#include <QString>

QString initCC::moduleDir = "";

//! find CloudCompare appDir (see pyAPI::pyCC_setupPaths)
void initCC::init(const char* modulePath)
{
    QDir myModule = QString(modulePath); // module path is something like <installPath>/lib/cloudcompare/cloudComPy/__init__.py
    bool ok = myModule.cdUp();
    ok = myModule.cdUp();
    moduleDir = myModule.absolutePath(); //moduleDir should be where the CloudCompare libs and plugins directory are:  <installPath>/lib/cloudcompare
    CCTRACE("moduleDir: " << moduleDir.toStdString());
}

initCC::initCC()
{
}

initCC::initCC(const initCC&)
{
}

