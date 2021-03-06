//##########################################################################
//#                                                                        #
//#                                PYCC                                    #
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
//#          Copyright 2020 Paul RASCLE www.openfields.fr                  #
//#                                                                        #
//##########################################################################

// sip implementation of pyCC

//! Typical I/O filter errors
enum CC_FILE_ERROR
{
    CC_FERR_NO_ERROR,
    CC_FERR_BAD_ARGUMENT,
    CC_FERR_UNKNOWN_FILE,
    CC_FERR_WRONG_FILE_TYPE,
    CC_FERR_WRITING,
    CC_FERR_READING,
    CC_FERR_NO_SAVE,
    CC_FERR_NO_LOAD,
    CC_FERR_BAD_ENTITY_TYPE,
    CC_FERR_CANCELED_BY_USER,
    CC_FERR_NOT_ENOUGH_MEMORY,
    CC_FERR_MALFORMED_FILE,
    CC_FERR_CONSOLE_ERROR,
    CC_FERR_BROKEN_DEPENDENCY_ERROR,
    CC_FERR_FILE_WAS_WRITTEN_BY_UNKNOWN_PLUGIN,
    CC_FERR_THIRD_PARTY_LIB_FAILURE,
    CC_FERR_THIRD_PARTY_LIB_EXCEPTION,
    CC_FERR_NOT_IMPLEMENTED,
};

enum CC_SHIFT_MODE
{
    AUTO = 0, XYZ = 1
};

enum CurvatureType
{
    GAUSSIAN_CURV = 1,
    MEAN_CURV,
    NORMAL_CHANGE_RATE
};

ccPolyline* loadPolyline(const char* filename,
                                CC_SHIFT_MODE mode=AUTO,
                                int skip=0,
                                double x=0,
                                double y=0,
                                double z=0);
%Docstring
Load a polyline from a file
parameters:
- filename
- shift mode from (AUTO, XYZ),  optional, default AUTO
  AUTO: automatic shift of coordinates
  XYZ:  coordinates shift given by x, y, z parameters
- skip optional parameter not used yet! default 0
- x, y, z:: optional shift values for coordinates (mode XYZ),  default 0
return a ccPolyline object.
Usage: see ccPolyline doc.
%End

ccPointCloud* loadPointCloud(const char* filename,
                                CC_SHIFT_MODE mode=AUTO,
                                int skip=0,
                                double x=0,
                                double y=0,
                                double z=0);
%Docstring
Load a 3D cloud from a file
parameters:
- filename
- shift mode from (AUTO, XYZ),  optional, default AUTO
  AUTO: automatic shift of coordinates
  XYZ:  coordinates shift given by x, y, z parameters
- skip optional parameter not used yet! default 0
- x, y, z:: optional shift values for coordinates (mode XYZ),  default 0
return a ccPointCloud object.
Usage: see ccPointCloud doc.
%End

CC_FILE_ERROR SavePointCloud(ccPointCloud* cloud,
                                const QString& filename);
%Docstring
Save a 3D cloud in a file
parameters:
- cloud: ccPointCloud
- filename
return 0 or I/O error.
%End

bool computeCurvature(CurvatureType option, double radius, QList<ccPointCloud*>);
%Docstring
Compute the curvature on a list of point clouds (create a scalarField)
parameters:
- CurvatureType from cloudCompare.GAUSSIAN_CURV, cloudCompare.MEAN_CURV, cloudCompare.NORMAL_CHANGE_RATE
- radius: try value obtained by GetPointCloudRadius
- list of clouds
%End

ccPointCloud* filterBySFValue(double minVal, double maxVal, ccPointCloud* cloud);
%Docstring
Create a new point cloud by filtering points using the current out ScalarField (see cloud.setCurrentOutScalarField).
Keep the points whose ScalarField value is between the min and max parameters.
parameters:
- minimum value
- maximum value
- cloud: ccPointCloud
return a ccPointCloud object.
%End

double GetPointCloudRadius(QList<ccPointCloud*> clouds, unsigned knn = 12);
%Docstring
Compute an estimate radius to use in computeCurvature
parameters:
- list of clouds
- number of nodes wanted within the radius
return estimated radius
%End

%Exception std::exception(SIP_Exception) /PyName=StdException, default/
{
%TypeHeaderCode
#include <exception>
%End
%RaiseCode
    const char *detail = sipExceptionRef.what();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_std_exception, detail);
    SIP_UNBLOCK_THREADS
%End
};

