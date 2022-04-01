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


namespace bp = boost::python;
namespace bnp = boost::python::numpy;

void initTrace_RANSAC_SD()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

bp::tuple computeRANSAC_SD(ccPointCloud* ccPC,
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
            ccHObject* child = objsFound->getChild(i);
            if (child)
            {
                int nbch = child->getChildrenNumber();
                CCTRACE("nbch " << nbch);
                if (nbch)
                {
                    ccHObject* grandChild = child->getChild(0);
                    if (grandChild)
                    {
                        CCTRACE(grandChild->getName().toStdString());
                        child->detachAllChildren();
                        ccMesh* prim = ccHObjectCaster::ToMesh(grandChild);
                        meshes.push_back(prim);
                    }
                    else meshes.push_back(nullptr);
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
    bp::tuple res = bp::make_tuple(meshes, clouds);
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

BOOST_PYTHON_MODULE(_RANSAC_SD)
{
    using namespace boost::python;

    scope().attr("__doc__") = RANSAC_SD_doc;

    enum_<qRansacSD::RANSAC_PRIMITIVE_TYPES>("RANSAC_PRIMITIVE_TYPES")
        .value("RPT_PLANE", qRansacSD::RPT_PLANE )
        .value("RPT_SPHERE", qRansacSD::RPT_SPHERE )
        .value("RPT_CYLINDER", qRansacSD::RPT_CYLINDER )
        .value("RPT_CONE", qRansacSD::RPT_CONE )
        .value("RPT_TORUS", qRansacSD::RPT_TORUS )
        ;

    class_<qRansacSD::RansacParams>("RansacParams", RANSAC_SD_RansacParams_doc)
        .def_readwrite("epsilon", &qRansacSD::RansacParams::epsilon, RANSAC_SD_RansacParams_doc)
        .def_readwrite("bitmapEpsilon", &qRansacSD::RansacParams::bitmapEpsilon, RANSAC_SD_RansacParams_doc)
        .def_readwrite("supportPoints", &qRansacSD::RansacParams::supportPoints, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxNormalDev_deg", &qRansacSD::RansacParams::maxNormalDev_deg, RANSAC_SD_RansacParams_doc)
        .def_readwrite("probability", &qRansacSD::RansacParams::probability, RANSAC_SD_RansacParams_doc)
        .def_readwrite("randomColor", &qRansacSD::RansacParams::randomColor, RANSAC_SD_RansacParams_doc)
        .def_readwrite("createCloudFromLeftOverPoints", &qRansacSD::RansacParams::createCloudFromLeftOverPoints, RANSAC_SD_RansacParams_doc)
        .def_readwrite("allowSimplification", &qRansacSD::RansacParams::allowSimplification, RANSAC_SD_RansacParams_doc)
        .def_readwrite("allowFitting", &qRansacSD::RansacParams::allowFitting, RANSAC_SD_RansacParams_doc)
        .def_readwrite("minSphereRadius", &qRansacSD::RansacParams::minSphereRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxSphereRadius", &qRansacSD::RansacParams::maxSphereRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("minCylinderRadius", &qRansacSD::RansacParams::minCylinderRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxCylinderRadius", &qRansacSD::RansacParams::maxCylinderRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxCylinderLength", &qRansacSD::RansacParams::maxCylinderLength, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxConeRadius", &qRansacSD::RansacParams::maxConeRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxConeAngle_deg", &qRansacSD::RansacParams::maxConeAngle_deg, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxConeLength", &qRansacSD::RansacParams::maxConeLength, RANSAC_SD_RansacParams_doc)
        .def_readwrite("minTorusMinorRadius", &qRansacSD::RansacParams::minTorusMinorRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("minTorusMajorRadius", &qRansacSD::RansacParams::minTorusMajorRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxTorusMinorRadius", &qRansacSD::RansacParams::maxTorusMinorRadius, RANSAC_SD_RansacParams_doc)
        .def_readwrite("maxTorusMajorRadius", &qRansacSD::RansacParams::maxTorusMajorRadius, RANSAC_SD_RansacParams_doc)
        .def("setPrimEnabled", setPrimEnabled, RANSAC_SD_RansacParams_doc)
        .def("getPrimEnabled", getPrimEnabled, RANSAC_SD_RansacParams_doc)
        .def("optimizeForCloud", optimizeForCloud, RANSAC_SD_RansacParams_doc)
        ;

    def("computeRANSAC_SD", computeRANSAC_SD, RANSAC_SD_computeRANSAC_SD_doc);

    def("initTrace_RANSAC_SD", initTrace_RANSAC_SD, RANSAC_SD_initTrace_RANSAC_SD_doc);

}
