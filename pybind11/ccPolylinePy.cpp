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
#include "pyccTrace.h"

#include <ccPolyline.h>
#include <Polyline.h>
#include <ccShiftedObject.h>
#include <GenericIndexedCloudPersist.h>
#include <ccPointCloud.h>
#include "ccPolylinePy_DocStrings.hpp"

#include <vector>

//! from ccSectionExtractionTool::generateOrthoSections
std::vector<ccPolyline*> generateOrthoSections(ccPolyline& self, double orthoSectionStep, double orthoSectionWidth, int vertDim = 2)
{
    std::vector<ccPolyline*> orthoPolys;
    //compute poyline length
    unsigned vertCount = self.size();
    if (vertCount < 2)
    {
        CCTRACE("[ccSectionExtractionTool] Invalid polyline");
        return orthoPolys;
    }

    PointCoordinateType length = self.computeLength();

    //normal to the plane
    if (vertDim < 0 || vertDim >2)
    {
        CCTRACE("[ccSectionExtractionTool] Invalid vertical direction");
        return orthoPolys;
    }
    CCVector3 N(0, 0, 0);
    N.u[vertDim] = 1.0;

    //curvilinear position
    double s = 0;
    //current length
    double l = 0;
    unsigned maxCount = vertCount;
    if (!self.isClosed())
        maxCount--;
    unsigned polyIndex = 0;
    for (unsigned i = 0; i < maxCount; ++i)
    {
        const CCVector3* A = self.getPoint(i);
        const CCVector3* B = self.getPoint((i + 1) % vertCount);
        CCVector3 AB = (*B - *A);
        AB.u[vertDim] = 0;
        CCVector3 nAB = AB.cross(N);
        nAB.normalize();

        double lAB = (*B - *A).norm();
        while (s < l + lAB)
        {
            double s_local = s - l;
            assert(s_local < lAB);

            //create orhogonal polyline
            ccPointCloud* vertices = new ccPointCloud("vertices");
            ccPolyline* orthoPoly = new ccPolyline(vertices);
            orthoPoly->addChild(vertices);
            if (vertices->reserve(2) && orthoPoly->reserve(2))
            {
                //intersection point
                CCVector3 I = *A + AB * (s_local / lAB);
                CCVector3 I1 = I + nAB * static_cast<PointCoordinateType>(orthoSectionWidth / 2);
                CCVector3 I2 = I - nAB * static_cast<PointCoordinateType>(orthoSectionWidth / 2);

                vertices->addPoint(I1);
                orthoPoly->addPointIndex(0);
                vertices->addPoint(I2);
                orthoPoly->addPointIndex(1);

                orthoPoly->setClosed(false);
                orthoPoly->set2DMode(false);
                orthoPoly->copyGlobalShiftAndScale(self);

                //set default display style
                vertices->setEnabled(false);
                orthoPoly->showColors(true);
                orthoPoly->setName(QString("%1.%2").arg(self.getName()).arg(++polyIndex));

                //add meta data (for Mascaret export)
                {
                    orthoPoly->setMetaData(ccPolyline::MetaKeyUpDir(), QVariant(vertDim));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyAbscissa(), QVariant(s));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyPrefixCenter() + ".x",
                                           QVariant(static_cast<double>(I.x)));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyPrefixCenter() + ".y",
                                           QVariant(static_cast<double>(I.y)));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyPrefixCenter() + ".z",
                                           QVariant(static_cast<double>(I.z)));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyPrefixDirection() + ".x",
                                           QVariant(static_cast<double>(nAB.x)));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyPrefixDirection() + ".y",
                                           QVariant(static_cast<double>(nAB.y)));
                    orthoPoly->setMetaData(ccPolyline::MetaKeyPrefixDirection() + ".z",
                                           QVariant(static_cast<double>(nAB.z)));
                }

                orthoPolys.push_back(orthoPoly);
            }
            else
            {
                delete orthoPoly;
                orthoPoly = nullptr;
                CCTRACE("Not enough memory!");
                //early stop
                i = maxCount;
                break;
            }

            s += orthoSectionStep;
        }

        l += lAB;
    }
    return orthoPolys;
}


void export_ccPolyline(py::module &m0)
{

	py::class_<CCCoreLib::Polyline, CCCoreLib::ReferenceCloud,
               std::unique_ptr<CCCoreLib::Polyline, py::nodelete>> (m0, "Polyline",
			   ccPolylinePy_Polyline_doc)
		 .def(py::init<CCCoreLib::GenericIndexedCloudPersist*>())
         .def("isClosed", &ccPolyline::isClosed, ccPolylinePy_isClosed_doc)
	     .def("setClosed", &ccPolyline::setClosed, ccPolylinePy_setClosed_doc)
		 ;

    py::class_<ccPolyline,CCCoreLib::Polyline, ccShiftedObject,
               std::unique_ptr<ccPolyline, py::nodelete> >(m0, "ccPolyline",
    	       ccPolylinePy_ccPolyline_doc)
        .def(py::init<CCCoreLib::GenericIndexedCloudPersist*>())
        .def("computeLength", &ccPolyline::computeLength, ccPolylinePy_computeLength_doc)
        .def("getName", &ccPolyline::getName, ccPolylinePy_getName_doc)
        .def("is2DMode", &ccPolyline::is2DMode, ccPolylinePy_is2DMode_doc)
        .def("segmentCount", &ccPolyline::segmentCount, ccPolylinePy_segmentCount_doc)
        .def("set2DMode", &ccPolyline::set2DMode, ccPolylinePy_set2DMode_doc)
        .def("setName", &ccPolyline::setName, ccPolylinePy_setName_doc)
        .def("size", &ccPolyline::size, ccPolylinePy_size_doc)
        .def("smoothChaikin", &ccPolyline::smoothChaikin,
             ccPolylinePy_smoothChaikin_doc, py::return_value_policy::reference)
        .def("generateOrthoSections", &generateOrthoSections,
             py::arg("orthoSectionStep"),
             py::arg("orthoSectionWidth"),
             py::arg("vertDim")=2,
             ccPolyline_generateOrthoSections_doc)
        ;
}
