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

#include <PCV.h>
#include <PCVCommand.h>

#include "pyccTrace.h"
#include "PCV_DocStrings.hpp"

void initTrace_PCV()
{
#ifdef _PYTHONAPI_DEBUG_
    ccLogTrace::settrace();
#endif
}

bool computeShadeVIS(std::vector<ccHObject*> clouds,
                        ccPointCloud* cloudWithNormals = nullptr,
                        int rayCount = 256,
                        int resolution = 1024,
                        bool is360 = false,
                        bool isClosedMesh = false)
{
    CCTRACE("computeShadeVIS");
    std::vector<CCVector3> rays;
    if (cloudWithNormals)
    {
        if (cloudWithNormals->hasNormals())
        {
            unsigned count = cloudWithNormals->size();
            try
            {
                rays.resize(count);
            }
            catch (std::bad_alloc)
            {
                CCTRACE("Not enough memory to generate the set of rays");
                return false;
            }
            for (unsigned i = 0; i < count; ++i)
            {
                rays[i] = CCVector3(cloudWithNormals->getPointNormal(i));
            }
        }
        else
        {
            CCTRACE("cloudWithNormals should have normals!");
            return false;
        }
    }
    else
    {
        CCTRACE("cloudWithNormals not provided, compute sample rays on a sphere");
        if (!PCV::GenerateRays(rayCount, rays, is360))
        {
            CCTRACE("Failed to generate the set of rays");
            return false;
        }
    }
    if (rays.empty())
    {
        CCTRACE("No ray was generated?!");
        return false;
    }

    ccHObject::Container candidates;
    bool hasMeshes = false;
    for (ccHObject* obj : clouds)
    {
        if (obj->isA(CC_TYPES::POINT_CLOUD))
        {
            //we need a real point cloud
            candidates.push_back(obj);
        }
        else if (obj->isKindOf(CC_TYPES::MESH))
        {
            ccGenericMesh* mesh = ccHObjectCaster::ToGenericMesh(obj);
            if (mesh->getAssociatedCloud() && mesh->getAssociatedCloud()->isA(CC_TYPES::POINT_CLOUD))
            {
                //we need a mesh with a real point cloud
                candidates.push_back(obj);
                hasMeshes = true;
            }
        }
    }

    return PCVCommand::Process(candidates, rays, isClosedMesh, resolution);
}

PYBIND11_MODULE(_PCV, m2)
{
    m2.doc() = PCV_doc;

    m2.def("computeShadeVIS", computeShadeVIS,
        py::arg("clouds"), py::arg("cloudWithNormals")=nullptr, py::arg("rayCount")=256, py::arg("resolution")=1024,
        py::arg("is360")=false, py::arg("isClosedMesh")=false, PCV_computeShadeVIS_doc);
    m2.def("initTrace_PCV", initTrace_PCV, PCV_initTrace_PCV_doc);
}
