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

#include <ccPointCloud.h>
#include <ccMesh.h>
#include <ccHObject.h>
#include <ccHObjectCaster.h>

#include <qRANSAC_SD.h>

#include "pyccTrace.h"
#include "RANSAC_SD_DocStrings.hpp"

void initTrace_RANSAC_SD()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

py::tuple computeRANSAC_SD(ccPointCloud* ccPC,
                           const qRansacSD::RansacParams& param)
{
    CCTRACE("computeRANSAC_SD");
    std::vector<ccMesh*> meshes;
    std::vector<ccPointCloud*> clouds;
    ccHObject* objsFound = qRansacSD::executeRANSAC(ccPC, param, true);
    if (objsFound)
    {
        unsigned int nbChildren = objsFound->getChildrenNumber();
        CCTRACE("found " << nbChildren << " shapes");
        for (int i=0; i<nbChildren; i++)
        {
            CCTRACE(" --- shape n°: " << i << " ---");
            ccHObject* child = objsFound->getChild(i);
            if (child)
            {
                CCTRACE("child name: " << child->getName().toStdString() << " type: " << child->getClassID());
                int nbch = child->getChildrenNumber();
                CCTRACE("nbch " << nbch);
                if (nbch)
                {
                    bool meshFound = false;
                    for (int j=0; j<nbch; j++)
                    {
                        ccHObject* grandChild = child->getChild(j);
                        if (grandChild)
                        {
                            CCTRACE(grandChild->getName().toStdString() << " type: " << grandChild->getClassID());
                            ccMesh* prim = ccHObjectCaster::ToMesh(grandChild);
                            if (prim)
                            {
                                CCTRACE("mesh name: " << prim->getName().toStdString());
                                meshes.push_back(prim);
                                meshFound = true;
                                child->detachAllChildren();
                                break;
                            }
                        }
                    }
                    if (!meshFound)
                        meshes.push_back(nullptr);
                }
                else meshes.push_back(nullptr);
                ccPointCloud* pc = ccHObjectCaster::ToPointCloud(child);
                CCTRACE("pc name:" << pc->getName().toStdString());
                clouds.push_back(pc);
            }
        }
        objsFound->detachAllChildren();
        CCTRACE("found " << clouds.size() << " separate clouds");
    }
    else
    {
        CCTRACE("Nothing found by RANSAC_SD!");
    }
    py::tuple res = py::make_tuple(meshes, clouds);
    return res;
}

void setPrimEnabled(qRansacSD::RansacParams& self, qRansacSD::RANSAC_PRIMITIVE_TYPES rpt, bool isEnabled )
{
    self.primEnabled[rpt] = isEnabled;
}

bool getPrimEnabled(qRansacSD::RansacParams& self, qRansacSD::RANSAC_PRIMITIVE_TYPES rpt )
{
    return self.primEnabled[rpt];
}

void optimizeForCloud(qRansacSD::RansacParams& self, ccPointCloud* pc)
{
    CCVector3 bbMin, bbMax;
    pc->getBoundingBox(bbMin, bbMax);
    CCVector3 diff = bbMax - bbMin;
    float scale = std::max(std::max(diff[0], diff[1]), diff[2]);
    self.epsilon = 0.005 * scale;
    self.bitmapEpsilon = 0.01 * scale;
}

PYBIND11_MODULE(_RANSAC_SD, m5)
{
    m5.doc() = RANSAC_SD_doc;

    py::enum_<qRansacSD::RANSAC_PRIMITIVE_TYPES>(m5, "RANSAC_PRIMITIVE_TYPES")
        .value("RPT_PLANE", qRansacSD::RPT_PLANE )
        .value("RPT_SPHERE", qRansacSD::RPT_SPHERE )
        .value("RPT_CYLINDER", qRansacSD::RPT_CYLINDER )
        .value("RPT_CONE", qRansacSD::RPT_CONE )
        .value("RPT_TORUS", qRansacSD::RPT_TORUS )
        .export_values();
        ;

    py::class_<qRansacSD::RansacParams>(m5, "RansacParams", RANSAC_SD_RansacParams_doc)
        .def(py::init<>())
        .def_readwrite("epsilon", &qRansacSD::RansacParams::epsilon,
                       RANSAC_SD_RansacParams_epsilon_doc)
        .def_readwrite("bitmapEpsilon", &qRansacSD::RansacParams::bitmapEpsilon,
                       RANSAC_SD_RansacParams_bitmapEpsilon_doc)
        .def_readwrite("supportPoints", &qRansacSD::RansacParams::supportPoints,
                       RANSAC_SD_RansacParams_supportPoints_doc)
        .def_readwrite("maxNormalDev_deg", &qRansacSD::RansacParams::maxNormalDev_deg,
                       RANSAC_SD_RansacParams_maxNormalDev_deg_doc)
        .def_readwrite("probability", &qRansacSD::RansacParams::probability,
                       RANSAC_SD_RansacParams_probability_doc)
        .def_readwrite("randomColor", &qRansacSD::RansacParams::randomColor,
                       RANSAC_SD_RansacParams_randomColor_doc)
        .def_readwrite("createCloudFromLeftOverPoints", &qRansacSD::RansacParams::createCloudFromLeftOverPoints,
                       RANSAC_SD_RansacParams_createCloudFromLeftOverPoints_doc)
        .def_readwrite("allowSimplification", &qRansacSD::RansacParams::allowSimplification,
                       RANSAC_SD_RansacParams_allowSimplification_doc)
        .def_readwrite("allowFitting", &qRansacSD::RansacParams::allowFitting,
                       RANSAC_SD_RansacParams_allowFitting_doc)
        .def_readwrite("minSphereRadius", &qRansacSD::RansacParams::minSphereRadius,
                       RANSAC_SD_RansacParams_minSphereRadius_doc)
        .def_readwrite("maxSphereRadius", &qRansacSD::RansacParams::maxSphereRadius,
                       RANSAC_SD_RansacParams_maxSphereRadius_doc)
        .def_readwrite("minCylinderRadius", &qRansacSD::RansacParams::minCylinderRadius,
                       RANSAC_SD_RansacParams_minCylinderRadius_doc)
        .def_readwrite("maxCylinderRadius", &qRansacSD::RansacParams::maxCylinderRadius,
                       RANSAC_SD_RansacParams_maxCylinderRadius_doc)
        .def_readwrite("maxCylinderLength", &qRansacSD::RansacParams::maxCylinderLength,
                       RANSAC_SD_RansacParams_maxCylinderLength_doc)
        .def_readwrite("maxConeRadius", &qRansacSD::RansacParams::maxConeRadius,
                       RANSAC_SD_RansacParams_maxConeRadius_doc)
        .def_readwrite("maxConeAngle_deg", &qRansacSD::RansacParams::maxConeAngle_deg,
                       RANSAC_SD_RansacParams_maxConeAngle_deg_doc)
        .def_readwrite("maxConeLength", &qRansacSD::RansacParams::maxConeLength,
                       RANSAC_SD_RansacParams_maxConeLength_doc)
        .def_readwrite("minTorusMinorRadius", &qRansacSD::RansacParams::minTorusMinorRadius,
                       RANSAC_SD_RansacParams_minTorusMinorRadius_doc)
        .def_readwrite("minTorusMajorRadius", &qRansacSD::RansacParams::minTorusMajorRadius,
                       RANSAC_SD_RansacParams_minTorusMajorRadius_doc)
        .def_readwrite("maxTorusMinorRadius", &qRansacSD::RansacParams::maxTorusMinorRadius,
                       RANSAC_SD_RansacParams_maxTorusMinorRadius_doc)
        .def_readwrite("maxTorusMajorRadius", &qRansacSD::RansacParams::maxTorusMajorRadius,
                       RANSAC_SD_RansacParams_maxTorusMajorRadius_doc)
        .def("setPrimEnabled", setPrimEnabled, RANSAC_SD_RansacParams_setPrimEnabled_doc)
        .def("getPrimEnabled", getPrimEnabled, RANSAC_SD_RansacParams_getPrimEnabled_doc)
        .def("optimizeForCloud", optimizeForCloud, RANSAC_SD_RansacParams_optimizeForCloud_doc)
        ;

    m5.def("computeRANSAC_SD", computeRANSAC_SD, RANSAC_SD_computeRANSAC_SD_doc);

    m5.def("initTrace_RANSAC_SD", initTrace_RANSAC_SD, RANSAC_SD_initTrace_RANSAC_SD_doc);
}
