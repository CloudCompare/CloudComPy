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
//#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#include <pyccTrace.h>
#include "initCC.h"
#include <QDir>
#include <QStandardPaths>
#include <QString>

QString initCC::moduleDir = "";

void initCC::init(const char* modulePath)
{
    QDir myModule = QString(modulePath);
    bool ok = myModule.cdUp();
    moduleDir = myModule.absolutePath();
    CCTRACE("moduleDir: " << moduleDir.toStdString());
}

initCC::initCC()
{
}

initCC::initCC(const initCC&)
{
}

