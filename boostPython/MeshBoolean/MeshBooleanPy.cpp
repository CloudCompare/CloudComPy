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
#include <ccOctree.h>
#include <ccOctreeProxy.h>
#include <CloudSamplingTools.h>

#include <qMeshBoolean.h>

#include "pyccTrace.h"
#include "MeshBoolean_DocStrings.hpp"


namespace bp = boost::python;
namespace bnp = boost::python::numpy;

ccPointCloud* computeMeshBoolean( ccPointCloud* cloud,
                          CCVector3d viewPoint,
                          int octreeLevel = 7)
{
    if(octreeLevel < 0 || octreeLevel > CCCoreLib::DgmOctree::MAX_OCTREE_LEVEL)
    {
        CCTRACE("octreeLevel must be between 0 and " << CCCoreLib::DgmOctree::MAX_OCTREE_LEVEL);
        return nullptr;
    }

    //compute octree if cloud hasn't any
    ccOctree::Shared theOctree = cloud->getOctree();
    if (!theOctree)
    {
        theOctree = cloud->computeOctree(nullptr);
    }

    if (!theOctree)
    {
        CCTRACE("Couldn't compute octree!");
        return nullptr;
    }

    //MeshBoolean
    QScopedPointer<CCCoreLib::ReferenceCloud> visibleCells;
    {
        QScopedPointer<CCCoreLib::ReferenceCloud> theCellCenters(
            CCCoreLib::CloudSamplingTools::subsampleCloudWithOctreeAtLevel( cloud,
                                                                            static_cast<unsigned char>(octreeLevel),
                                                                            CCCoreLib::CloudSamplingTools::NEAREST_POINT_TO_CELL_CENTER,
                                                                            nullptr,
                                                                            theOctree.data()) );
        if (!theCellCenters)
        {
            CCTRACE("Error while simplifying point cloud with octree!");
            return nullptr;
        }

        visibleCells.reset(qMeshBoolean::removeHiddenPoints(theCellCenters.data(), viewPoint, 3.5));

        //warning: after this point, visibleCells can't be used anymore as a
        //normal cloud (as it's 'associated cloud' has been deleted).
        //Only its indexes are valid! (they are corresponding to octree cells)
    }

    if (visibleCells)
    {
        //DGM: we generate a new cloud now, instead of playing with the points visiblity! (too confusing for the user)
        /*if (!cloud->isVisibilityTableInstantiated() && !cloud->resetVisibilityArray())
        {
            m_app->dispToConsole("Visibility array allocation failed! (Not enough memory?)",ccMainAppInterface::ERR_CONSOLE_MESSAGE);
            return;
        }

        ccPointCloud::VisibilityTableType* pointsVisibility = cloud->getTheVisibilityArray();
        assert(pointsVisibility);
        pointsVisibility->fill(POINT_HIDDEN);
        */

        CCCoreLib::ReferenceCloud visiblePoints(theOctree->associatedCloud());

        unsigned visiblePointCount = 0;
        unsigned visibleCellsCount = visibleCells->size();

        CCCoreLib::DgmOctree::cellIndexesContainer cellIndexes;
        if (!theOctree->getCellIndexes(static_cast<unsigned char>(octreeLevel), cellIndexes))
        {
            CCTRACE("Couldn't fetch the list of octree cell indexes! (Not enough memory?)");
            return nullptr;
        }

        for (unsigned i = 0; i < visibleCellsCount; ++i)
        {
            //cell index
            unsigned index = visibleCells->getPointGlobalIndex(i);

            //points in this cell...
            CCCoreLib::ReferenceCloud Yk(theOctree->associatedCloud());
            theOctree->getPointsInCellByCellIndex(&Yk, cellIndexes[index], static_cast<unsigned char>(octreeLevel));
            //...are all visible
            if (!visiblePoints.add(Yk))
            {
                CCTRACE("Not enough memory!");
                return nullptr;
            }
        }

        visibleCells.reset(nullptr);

        CCTRACE("[MeshBoolean] Visible points: " << visiblePointCount);

        if (visiblePoints.size() == cloud->size())
        {
            CCTRACE("No points were removed!");
            return nullptr;
        }
        else
        {
            //create cloud from visibility selection
            ccPointCloud* newCloud = cloud->partialClone(&visiblePoints);
            if (newCloud)
            {
                newCloud->setName(cloud->getName() + QString(".visible_points"));
            }
            else
            {
                CCTRACE("Not enough memory!");
            }
            return newCloud;
        }
    }
    return nullptr;
}

BOOST_PYTHON_FUNCTION_OVERLOADS(computeMeshBoolean_overloads, computeMeshBoolean, 2, 3)


BOOST_PYTHON_MODULE(_MeshBoolean)
{
    using namespace boost::python;

    scope().attr("__doc__") = MeshBoolean_doc;

    def("computeMeshBoolean", computeMeshBoolean,
        computeMeshBoolean_overloads(
        (arg("cloud"), arg("viewPoint"), arg("octreeLevel")=7),
        MeshBoolean_computeMeshBoolean_doc)[return_value_policy<reference_existing_object>()]);

}
