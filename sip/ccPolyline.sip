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

// sip implementation of ccPolyline


class ccPolyline
{
%TypeHeaderCode
#include <pyccTrace.h>
#include <ccNumpyIncludes.h>
#include <ccPolyline.h>
#include <PyScalarType.h>
%End

%Docstring
A simple polyline
The polyline is considered as a cloud of points
(in a specific order) with a open/closed state
information.
%End

public:
    QString getName() const;

    unsigned size() const;

    double computeLength() const [PointCoordinateType () ];

    unsigned segmentCount() const;

    bool isClosed() const;

    void setClosed(bool state);

    void set2DMode(bool state);

    bool is2DMode() const;

private:
    ccPolyline();

    ccPolyline(const ccPolyline& poly);

};
