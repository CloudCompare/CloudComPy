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

#include <QString>
#include <vector>
#include <QtCore>
#include <QtConcurrentRun>
#include <QApplication>

#include <ccPointCloud.h>
#include <ccScalarField.h>
#include <ccMesh.h>

#include <qCork.h>

#include "pyccTrace.h"
#include "Cork_DocStrings.hpp"

void initTrace_Cork()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

PYBIND11_MODULE(_Cork, m11)
{
    m11.doc() = Cork_doc;

    py::enum_<qCork::CSG_OPERATION>(m11, "CSG_OPERATION")
        .value("UNION", qCork::CSG_OPERATION::UNION)
        .value("INTERSECT", qCork::CSG_OPERATION::INTERSECT)
        .value("DIFF", qCork::CSG_OPERATION::DIFF)
        .value("SYM_DIFF", qCork::CSG_OPERATION::SYM_DIFF)
        .export_values();


    py::class_<qCork>(m11, "Cork", Cork_Cork_doc)
        .def_static("compute", &qCork::compute,
            ::py::arg("meshA"),
            ::py::arg("meshB"),
            ::py::arg("operation"),
            Cork_compute_doc);
        ;

    m11.def("initTrace_Cork", &initTrace_Cork, Cork_initTrace_Cork_doc);
}

