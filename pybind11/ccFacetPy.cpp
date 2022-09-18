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

#include <ccFacet.h>
#include <ccHObject.h>
#include <ccPointCloud.h>
#include <ccMesh.h>
#include <ccPolyline.h>
#include <GenericProgressCallback.h>

#include "ccFacetPy_DocStrings.hpp"
#include <vector>
#include "pyccTrace.h"

ccFacet* CreatePy(CCCoreLib::GenericIndexedCloudPersist* cloud,
                  PointCoordinateType maxEdgeLength = 0,
                  bool transferOwnership = false,
                  std::vector<double> planeEquation = std::vector<double>{})
{
    float* pe = nullptr;
    if (planeEquation.size() == 4)
    {
        float eq[4] = {(float)planeEquation[0], (float)planeEquation[1], (float)planeEquation[2], (float)planeEquation[3]};
        pe = eq;
        for (int i=0; i<4; i++)
            CCTRACE("  - "<< pe[i]);
    }
    return ccFacet::Create(cloud, maxEdgeLength, transferOwnership, pe);
}

CCVector3 getCenterPy(ccFacet& self)
{
    CCVector3 center = self.getCenter();
    return center;
}

ccPolyline* getContourPy(ccFacet& self)
{
    return self.getContour();
}

ccPointCloud* getContourVerticesPy(ccFacet& self)
{
    return self.getContourVertices();
}

ccPointCloud* getOriginPointsPy(ccFacet& self)
{
    return self.getOriginPoints();
}

std::vector<double> getPlaneEquationPy(ccFacet& self)
{
    const PointCoordinateType* coefs = self.getPlaneEquation();
    std::vector<double> vec = {coefs[0], coefs[1], coefs[2], coefs[3]};
    return vec;
}

ccMesh* getPolygonPy(ccFacet& self)
{
    return self.getPolygon();
}

void export_ccFacet(py::module &m0)
{
    py::class_<ccFacet, ccHObject >(m0, "ccFacet", ccFacetPy_ccFacet_doc) // no_init
        .def_static("Create",
             &CreatePy,
             py::arg("cloud"), py::arg("maxEdgeLength")=0,
             py::arg("transferOwnership")=false, py::arg("planeEquation")=std::vector<double>{},
                     ccFacetPy_Create_doc,  py::return_value_policy::reference)
        .def("getNormal", &ccFacet::getNormal, ccFacetPy_getNormal_doc)
        .def("getRMS", &ccFacet::getRMS, ccFacetPy_getRMS_doc)
        .def("getSurface", &ccFacet::getSurface, ccFacetPy_getSurface_doc)
        .def("getPlaneEquation", &getPlaneEquationPy, ccFacetPy_getPlaneEquation_doc)
        .def("invertNormal", &ccFacet::invertNormal, ccFacetPy_invertNormal_doc)
        .def("getCenter", &getCenterPy, ccFacetPy_getCenter_doc)
        .def("getPolygon", &getPolygonPy, py::return_value_policy::reference, ccFacetPy_getPolygon_doc)
        .def("getContour", &getContourPy, py::return_value_policy::reference, ccFacetPy_getContour_doc)
        .def("getContourVertices", &getContourVerticesPy, py::return_value_policy::reference, ccFacetPy_getContourVertices_doc)
        .def("getOriginPoints", &getOriginPointsPy, py::return_value_policy::reference, ccFacetPy_getOriginPoints_doc)
        .def("setPolygon", &ccFacet::setPolygon, ccFacetPy_setPolygon_doc)
        .def("setContour", &ccFacet::setContour, ccFacetPy_setContour_doc)
        .def("setContourVertices", &ccFacet::setContourVertices, ccFacetPy_setContourVertices_doc)
        .def("setOriginPoints", &ccFacet::setOriginPoints, ccFacetPy_setOriginPoints_doc)
        .def("clone", &ccFacet::clone, py::return_value_policy::reference, ccFacetPy_clone_doc)
        ;
}


