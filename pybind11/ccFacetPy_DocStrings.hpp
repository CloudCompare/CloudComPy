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

#ifndef CCFACETPY_DOCSTRINGS_HPP_
#define CCFACETPY_DOCSTRINGS_HPP_

const char* ccFacetPy_ccFacet_doc= R"(
Facet: Composite object: point cloud + 2D1/2 contour polyline + 2D1/2 surface mesh)";

const char* ccFacetPy_Create_doc= R"(
Creates a facet from a set of points.

The facet boundary can either be the convex hull (maxEdgeLength = 0)
or the concave hull (maxEdgeLength > 0).

:param GenericIndexedCloudPersist cloud: cloud from which to create the facet
:param int,optional maxEdgeLength: max edge length (if possible - ignored if 0), default 0.
:param bool,optional transferOwnership: if true and the input cloud is a ccPointCloud,
    it will be 'kept' as 'origin points'. default False.
:param tuple(float),optional planeEquation: to input a custom plane equation: 
    [a, b, c, d] as 'ax+by+cz=d', default []

:return: a facet (or 0 if an error occurred)
:rtype: ccFacet
)";

const char* ccFacetPy_getNormal_doc=R"(
Returns the facet normal.

:return: the facet normal
:rtype: tuple
)";

const char* ccFacetPy_getRMS_doc=R"(
Returns associated RMS (Root Mean Square).

:return: RMS (Root Mean Square)
:rtype: double
)";

const char* ccFacetPy_getSurface_doc=R"(
Returns associated surface.

:return: surface
:rtype: double
)";

const char* ccFacetPy_getPlaneEquation_doc=R"(
Returns plane equation.

:return: plane equation : [a, b, c, d] as 'ax+by+cz=d'
:rtype: tuple)";

const char* ccFacetPy_invertNormal_doc=R"(
Inverts the facet normal.
)";

const char* ccFacetPy_getCenter_doc=R"(
Returns the facet center.

:return: facet center coordinates
:rtype: tuple
)";

const char* ccFacetPy_getPolygon_doc=R"(
Returns polygon mesh (if any).

:return: polygon mesh or None
:rtype: ccMesh
)";

const char* ccFacetPy_getContour_doc=R"(
Returns contour polyline (if any).

:return: contour polyline or None
:rtype: ccPolyline
)";

const char* ccFacetPy_getContourVertices_doc=R"(
Returns contour vertices (if any).

:return: contour vertices or None
:rtype: ccPointCloud
)";

const char* ccFacetPy_getOriginPoints_doc=R"(
Returns origin points (if any).

:return: origin points or None
:rtype: ccPointCloud
)";

const char* ccFacetPy_setPolygon_doc=R"(
Sets polygon mesh.

:param ccMesh polygon: polygon mesh
)";

const char* ccFacetPy_setContour_doc=R"(
Sets contour polyline.

:param ccPolyline contour: contour polyline
)";

const char* ccFacetPy_setContourVertices_doc=R"(
Sets contour vertices.

:param ccPointCloud vertices: contour vertices
)";

const char* ccFacetPy_setOriginPoints_doc=R"(
Sets origin points.

:param ccPointCloud origin: origin points
)";

const char* ccFacetPy_clone_doc=R"(
Clones this facet.

:return: facet clone
:rtype: ccFacet
)";

#endif /* CCFACETPY_DOCSTRINGS_HPP_ */
