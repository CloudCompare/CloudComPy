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
#include <CloudSamplingTools.h>

#include <qCanupoProcess.h>

#include "pyccTrace.h"
#include "Canupo_DocStrings.hpp"

void initTrace_Canupo()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

enum CORE_CLOUD_SOURCES
{
    ORIGINAL,
    OTHER,
    SUBSAMPLED,
    MSC_FILE
};

//! from qCanupoPlugin::doClassifyAction
bool ClassifyPy(
    ccPointCloud* cloud,
    QString classifierFilename,
    ccPointCloud* corePointCloud = nullptr,
    CORE_CLOUD_SOURCES coreSource = ORIGINAL,
    QString MscFilename = "",
    double confidenceThreshold = 0.,
    bool generateAdditionalSF = false,
    bool generateRoughnessSF = false,
    int maxThreadCount = 0,
    bool useActiveSFForConfidence = false,
    PointCoordinateType samplingDist = 0.0)
{
    //store parameters
    qCanupoProcess::ClassifyParams params;
    {
        params.confidenceThreshold = confidenceThreshold;
        params.generateAdditionalSF = generateAdditionalSF;
        params.generateRoughnessSF = generateRoughnessSF;
        params.maxThreadCount = maxThreadCount;
        params.useActiveSFForConfidence = useActiveSFForConfidence;
        params.samplingDist = 0;
    }

    CorePointDescSet corePointsDescriptors; //core point descriptors
    ccPointCloud* realCorePoints = nullptr; //the core point cloud (as a real point cloud, if available)
    CCCoreLib::GenericIndexedCloudPersist* corePoints = nullptr; //the core points, potentially as references!

    switch (coreSource)
    {
    case ORIGINAL:
        realCorePoints = cloud;
        corePoints = realCorePoints;
        break;
    case OTHER:
    {
        realCorePoints = corePointCloud;
        corePoints = realCorePoints;
        if (!corePoints)
        {
            CCTRACE("corePointCloud null and OTHER core source specified!")
            return false;
        }
    }
    break;

    case SUBSAMPLED:
    {
        if (samplingDist <= 0)
        {
            CCTRACE("samplingDist <=0 and SUBSAMPLED core source specified!")
            return false;
        }
        CCCoreLib::CloudSamplingTools::SFModulationParams modParams(false);
        CCCoreLib::ReferenceCloud* refCloud = CCCoreLib::CloudSamplingTools::resampleCloudSpatially(cloud,
                                                                                            samplingDist,
                                                                                            modParams,
                                                                                            nullptr,
                                                                                            nullptr);
        if (!refCloud)
        {
            CCTRACE("Failed to compute sub-sampled core points!");
            return false;
        }

        params.samplingDist = samplingDist;

        //try to convert the references into a real point cloud (not mandatory though!)
        realCorePoints = cloud->partialClone(refCloud);
        if (realCorePoints)
        {
            realCorePoints->setName(cloud->getName() + QString(".core points (subsampled @ %1)").arg(samplingDist));
            cloud->addChild(realCorePoints);
            corePoints = realCorePoints;
        }
        else
        {
            CCTRACE("Can't save subsampled cloud (not enough memory)!");
            return false;
        }

        delete refCloud;
        refCloud = nullptr;
    }
    break;

    case MSC_FILE:
    {
        realCorePoints = new ccPointCloud("MSC core points");
        QString error;
        if (!corePointsDescriptors.loadFromMSC(MscFilename, error, realCorePoints))
        {
            //failed to read the input MSC file
            if (realCorePoints)
            {
                delete realCorePoints;
                realCorePoints = nullptr;
            }
            CCTRACE(error.toStdString());
            return false;
        }
        else if (!error.isNull())
        {
            //then it's just a warning
            CCTRACE(error.toStdString());
        }

        assert(realCorePoints);
        cloud->addChild(realCorePoints);
        corePoints = realCorePoints;
    }
    break;

    default:
    {
        assert(false);
        CCTRACE("Internal error: no core point source specified?!");
        return false;
    }
    }

    assert(corePoints);

    qCanupoProcess::Classify(classifierFilename, params, cloud, corePoints, corePointsDescriptors, realCorePoints, nullptr, nullptr);

    //dispose of the 'virtual' core points (if any)
    if (corePoints != realCorePoints)
    {
        delete corePoints;
        corePoints = nullptr;
    }
    return true;
}

PYBIND11_MODULE(_Canupo, m9)
{
    m9.doc() = Canupo_doc;

    py::enum_<CORE_CLOUD_SOURCES>(m9, "CORE_CLOUD_SOURCES")
                .value("ORIGINAL", CORE_CLOUD_SOURCES::ORIGINAL)
                .value("OTHER", CORE_CLOUD_SOURCES::OTHER)
                .value("SUBSAMPLED", CORE_CLOUD_SOURCES::SUBSAMPLED)
                .value("MSC_FILE", CORE_CLOUD_SOURCES::MSC_FILE)
        .export_values();

    m9.def("Classify", &ClassifyPy,
            py::arg("cloud"), py::arg("classifierFilename"),
            py::arg("corePointCloud")=nullptr,
            py::arg("coreSource")=ORIGINAL,
            py::arg("MscFilename")="",
            py::arg("confidenceThreshold")=0.,
            py::arg("generateAdditionalSF")=false,
            py::arg("generateRoughnessSF")=false,
            py::arg("maxThreadCount")=0,
            py::arg("useActiveSFForConfidence")=false,
            py::arg("samplingDist")=0.,
            Canupo_Classify_doc);

    m9.def("initTrace_Canupo", initTrace_Canupo, Canupo_initTrace_Canupo_doc);
}

