//##########################################################################
//#                                                                        #
//#                                boost.Python                            #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Library General Public License as       #
//#  published by the Free Software Foundation; version 2 or later of the  #
//#  License.                                                              #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          Copyright 2021 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

#ifndef CLOUDCOMPY_DOCSTRINGS_HPP_
#define CLOUDCOMPY_DOCSTRINGS_HPP_

const char* cloudComPy_doc= R"(
cloudCompPy is the Python module interfacing cloudCompare library
Python3 access to cloudCompare objects is done like this:
import cloudComPy as cc 
cc.initCC  # to do once before using plugins
cloud = cc.loadPointCloud("/home/paul/projets/CloudCompare/data/altiXYZ/RGEALTI_FXX_0845_6446_MNT_LAMB93_IGN69.xyz"))";

const char* cloudComPy_initCC_doc= R"(
Should be done once before using plugins!)";

const char* cloudComPy_loadPointCloud_doc= R"(
Load a 3D cloud from a file
parameters:
- filename
- shift mode from (CC_SHIFT_MODE.AUTO, CC_SHIFT_MODE.XYZ),  optional, default AUTO
    CC_SHIFT_MODE.AUTO: automatic shift of coordinates
    CC_SHIFT_MODE.XYZ:  coordinates shift given by x, y, z parameters
- skip: optional parameter not used yet! default 0
- x, y, z: optional shift values for coordinates (mode XYZ),  default 0
return a ccPointCloud object.
Usage: see ccPointCloud doc.)";

const char* cloudComPy_loadPolyline_doc= R"(
Load a polyline from a file
parameters:
- filename
- shift mode from (AUTO, XYZ),  optional, default AUTO
  AUTO: automatic shift of coordinates
  XYZ:  coordinates shift given by x, y, z parameters
- skip: optional parameter not used yet! default 0
- x, y, z: optional shift values for coordinates (mode XYZ),  default 0
return a ccPolyline object.
Usage: see ccPolyline doc.)";

const char* cloudComPy_SavePointCloud_doc= R"(
Save a 3D cloud in a file
parameters:
- cloud: ccPointCloud
- filename
return 0 or I/O error.)";

const char* cloudComPy_SaveEntities_doc= R"(
Save a list of entities (cloud, meshes, primitives...) in a file: use bin format!
parameters:
- list of entities
- filename
return 0 or I/O error.)";

const char* cloudComPy_computeCurvature_doc= R"("
Compute the curvature on a list of point clouds (create a scalarField)
parameters:
- CurvatureType from cloudCompare.GAUSSIAN_CURV, cloudCompare.MEAN_CURV, cloudCompare.NORMAL_CHANGE_RATE
- radius: try value obtained by GetPointCloudRadius
- list of clouds)";

const char* cloudComPy_filterBySFValue_doc= R"("
Create a new point cloud by filtering points using the current out ScalarField (see cloud.setCurrentOutScalarField).
Keep the points whose ScalarField value is between the min and max parameters.
parameters:
- minimum value
- maximum value
- cloud: ccPointCloud
return a ccPointCloud object.)";

const char* cloudComPy_GetPointCloudRadius_doc= R"("
Compute an estimate radius to use in computeCurvature
parameters:
- list of clouds
- number of nodes wanted within the radius
return estimated radius)";

const char* cloudComPy_getScalarType_doc= R"(
Get the scalar type used in cloudCompare under the form defined in Numpy: 'float32' or 'float64'
)";


#endif /* CLOUDCOMPY_DOCSTRINGS_HPP_ */
