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
#include <ccPolyline.h>
#include <ccScalarField.h>
#include <ccMesh.h>

#include <qSRA.h>
#include <profileLoader.h>
#include <distanceMapGenerationTool.h>

#include <ccColorScalesManager.h>

#include "pyccTrace.h"
#include "SRA_DocStrings.hpp"

void initTrace_SRA()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

// from qSRA::loadProfile()
ccPolyline* loadProfilePy(const char* filename, int axisDim=2, bool isAbsoluteHeightValues=false)
{

    //load profile as a (2D) polyline
    CCVector3 origin(0, 0, 0);
    ccPolyline* polyline = ProfileLoader::Load(QString(filename), origin);
    if (!polyline)
    {
        CCTRACE("Failed to load file " << filename);
        return nullptr;
    }

    //DGM: the following works only because the axis is colinear with X, Y or Z!
    PointCoordinateType heightShift = 0;
    if (isAbsoluteHeightValues)
    {
        heightShift = -origin.u[axisDim];
    }

    //apply a visual transformation to see the polyline in the right place
    {
        ccGLMatrix trans;
        CCVector3 T = origin;
        T.u[axisDim] += heightShift;
        trans.setTranslation(T);
        float* mat = trans.data();
        switch (axisDim)
        {
        case 0: //X
            //invert X and Y
            mat[0] = 0; mat[1] = 1;
            mat[4] = 1; mat[5] = 0;
            break;
            //case 1: //Y
        case 2: //Z
            //invert Z and Y
            mat[5] = 0; mat[6] = 1;
            mat[9] = 1; mat[10] = 0;
            break;
        default:
            //nothing to do
            break;
        }
        polyline->set2DMode(false);
        polyline->setGLTransformation(trans);
    }

//    //set meta-data
    DistanceMapGenerationTool::SetPoylineOrigin(polyline, origin);
    DistanceMapGenerationTool::SetPoylineRevolDim(polyline, axisDim);
    DistanceMapGenerationTool::SetPolylineHeightShift(polyline, heightShift);


    CCTRACE("[qSRA] File " << filename << " successfully loaded")
    return polyline;
}

QSharedPointer<DistanceMapGenerationTool::Map> generateMap(
    ccPointCloud* cloud,
    ccPolyline* profile,
    ccScalarField* sf,
    double angStep_rad,
    bool ccw,
    bool isConical,
    double yStep,
    double yMin,
    double yMax,
    DistanceMapGenerationTool::FillStrategyType fillSt,
    DistanceMapGenerationTool::EmptyCellFillOption fillOpt)
{
    //profile parameters
    DistanceMapGenerationTool::ProfileMetaData profileDesc;
    if (!DistanceMapGenerationTool::GetPoylineMetaData(profile, profileDesc))
    {
        CCTRACE("Failed to get polyline meta data")
        return QSharedPointer<DistanceMapGenerationTool::Map>(nullptr);
    }
    //compute transformation from cloud to the surface (of revolution)
    ccGLMatrix cloudToSurface = profileDesc.computeCloudToSurfaceOriginTrans();

    //generate map
    return DistanceMapGenerationTool::CreateMap(cloud, sf, cloudToSurface, profileDesc.revolDim, angStep_rad, yStep,
                                                yMin, yMax, isConical, ccw, fillSt, fillOpt, nullptr);
}

ccPointCloud* exportMapAsCloudPy(
    ccPointCloud* cloud,
    ccPolyline* profile,
    ccScalarField* sf,
    double angStep_deg,
    double yStep,
    double yMin,
    double yMax,
    bool isConical = false,
    bool ccw = false,
    DistanceMapGenerationTool::FillStrategyType fillSt = DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_AVG_DIST,
    DistanceMapGenerationTool::EmptyCellFillOption fillOpt = DistanceMapGenerationTool::EmptyCellFillOption::FILL_INTERPOLATE,
    double baseRadius = 1.0)
{
    double angStep_rad = angStep_deg * M_PI/180.0;
    QSharedPointer<DistanceMapGenerationTool::Map> map = generateMap(cloud, profile, sf, angStep_rad,
                                                                     ccw, isConical, yStep, yMin, yMax,
                                                                     fillSt, fillOpt);
    ccPointCloud* clmap = DistanceMapGenerationTool::ConvertMapToCloud(map,profile,baseRadius);
    return clmap;
}

ccMesh* exportMapAsMeshPy(
    ccPointCloud* cloud,
    ccPolyline* profile,
    ccScalarField* sf,
    double angStep_deg,
    double yStep,
    double yMin,
    double yMax,
    bool isConical = false,
    bool ccw = false,
    DistanceMapGenerationTool::FillStrategyType fillSt = DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_AVG_DIST,
    DistanceMapGenerationTool::EmptyCellFillOption fillOpt = DistanceMapGenerationTool::EmptyCellFillOption::FILL_INTERPOLATE,
    double baseRadius = 1.0,
    ccColorScalesManager::DEFAULT_SCALES colScale = ccColorScalesManager::BGYR,
    int colScaleSteps = 256)
{
    double angStep_rad = angStep_deg * M_PI/180.0;
    QSharedPointer<DistanceMapGenerationTool::Map> map = generateMap(cloud, profile, sf, angStep_rad,
                                                                     ccw, isConical, yStep, yMin, yMax,
                                                                     fillSt, fillOpt);
    //profile parameters
    DistanceMapGenerationTool::ProfileMetaData profileDesc;
    if (!DistanceMapGenerationTool::GetPoylineMetaData(profile, profileDesc))
    {
        CCTRACE("problem GetPoylineMetaData");
        return nullptr;
    }

    ccColorScale::Shared colorScale = ccColorScalesManager::GetDefaultScale(colScale);

    //create new texture QImage
    QImage mapImage = DistanceMapGenerationTool::ConvertMapToImage(map, colorScale, colScaleSteps);
    if (mapImage.isNull())
    {
        CCTRACE("problem ConvertMapToImage");
         return nullptr;
    }

    //compute transformation from cloud to the profile (origin)
    ccGLMatrix cloudToProfile = profileDesc.computeCloudToProfileOriginTrans();
    ccMesh* mesh = DistanceMapGenerationTool::ConvertProfileToMesh(profile, cloudToProfile, map->counterclockwise, map->xSteps, mapImage);
    if (mesh)
    {
        mesh->setDisplay_recursive(cloud->getDisplay());
        mesh->setName(cloud->getName()+QString(".map(%1,%2)").arg(map->xSteps).arg(map->ySteps));
    }
    else
    {
        CCTRACE("Failed to generate mesh! Not enough memory?");
    }
    return mesh;
}

PYBIND11_MODULE(_SRA, m8)
{
    m8.doc() = SRA_doc;

    py::enum_<DistanceMapGenerationTool::FillStrategyType>(m8, "FillStrategyType")
        .value("FILL_STRAT_MIN_DIST", DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_MIN_DIST)
        .value("FILL_STRAT_AVG_DIST", DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_AVG_DIST)
        .value("FILL_STRAT_MAX_DIST", DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_MAX_DIST)
        .value("INVALID_STRATEGY_TYPE", DistanceMapGenerationTool::FillStrategyType::INVALID_STRATEGY_TYPE)
        .export_values();

    py::enum_<DistanceMapGenerationTool::EmptyCellFillOption>(m8, "EmptyCellFillOption")
        .value("LEAVE_EMPTY", DistanceMapGenerationTool::EmptyCellFillOption::LEAVE_EMPTY)
        .value("FILL_WITH_ZERO", DistanceMapGenerationTool::EmptyCellFillOption::FILL_WITH_ZERO)
        .value("FILL_INTERPOLATE", DistanceMapGenerationTool::EmptyCellFillOption::FILL_INTERPOLATE)
        .export_values();

    py::enum_<ccColorScalesManager::DEFAULT_SCALES>(m8, "DEFAULT_SCALES")
        .value("BGYR", ccColorScalesManager::DEFAULT_SCALES::BGYR)
        .value("GREY", ccColorScalesManager::DEFAULT_SCALES::GREY)
        .value("BWR", ccColorScalesManager::DEFAULT_SCALES::BWR)
        .value("RY", ccColorScalesManager::DEFAULT_SCALES::RY)
        .value("RW", ccColorScalesManager::DEFAULT_SCALES::RW)
        .value("ABS_NORM_GREY", ccColorScalesManager::DEFAULT_SCALES::ABS_NORM_GREY)
        .value("HSV_360_DEG", ccColorScalesManager::DEFAULT_SCALES::HSV_360_DEG)
        .value("VERTEX_QUALITY", ccColorScalesManager::DEFAULT_SCALES::VERTEX_QUALITY)
        .value("DIP_BRYW", ccColorScalesManager::DEFAULT_SCALES::DIP_BRYW)
        .value("DIP_DIR_REPEAT", ccColorScalesManager::DEFAULT_SCALES::DIP_DIR_REPEAT)
        .value("VIRIDIS", ccColorScalesManager::DEFAULT_SCALES::VIRIDIS)
        .value("BROWN_YELLOW", ccColorScalesManager::DEFAULT_SCALES::BROWN_YELLOW)
        .value("YELLOW_BROWN", ccColorScalesManager::DEFAULT_SCALES::YELLOW_BROWN)
        .value("TOPO_LANDSERF", ccColorScalesManager::DEFAULT_SCALES::TOPO_LANDSERF)
        .value("HIGH_CONTRAST", ccColorScalesManager::DEFAULT_SCALES::HIGH_CONTRAST)
        .export_values();

    py::class_<qSRA>(m8, "qSRA", SRA_qSRA_doc)
        .def(py::init<>())
        .def("doComputeRadialDists", &qSRA::doComputeRadialDists, SRA_qSRA_doComputeRadialDists_doc)
        ;

    m8.def("loadProfile", &loadProfilePy, SRA_qSRA_loadProfile_doc);

    m8.def("exportMapAsCloud", &exportMapAsCloudPy,
           py::arg("cloud"), py::arg("profile"), py::arg("sf"),
           py::arg("angStep_deg"), py::arg("yStep"), py::arg("yMin"), py::arg("yMax"),
           py::arg("isConical")=false, py::arg("ccw")=false,
           py::arg("fillSt")=DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_AVG_DIST,
           py::arg("fillOpt")=DistanceMapGenerationTool::EmptyCellFillOption::FILL_INTERPOLATE,
           py::arg("baseRadius")=1.0,
           SRA_exportMapAsCloud_doc);

    m8.def("exportMapAsMesh", &exportMapAsMeshPy,
           py::arg("cloud"), py::arg("profile"), py::arg("sf"),
           py::arg("angStep_deg"), py::arg("yStep"), py::arg("yMin"), py::arg("yMax"),
           py::arg("isConical")=false, py::arg("ccw")=false,
           py::arg("fillSt")=DistanceMapGenerationTool::FillStrategyType::FILL_STRAT_AVG_DIST,
           py::arg("fillOpt")=DistanceMapGenerationTool::EmptyCellFillOption::FILL_INTERPOLATE,
           py::arg("baseRadius")=1.0,
           py::arg("colScale")=ccColorScalesManager::BGYR,
           py::arg("colScaleSteps")=256,
           SRA_exportMapAsMesh_doc);

   m8.def("initTrace_SRA", &initTrace_SRA, SRA_initTrace_SRA_doc);
}

