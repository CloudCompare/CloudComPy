#ifndef VIEWERPYAPPLICATION_H
#define VIEWERPYAPPLICATION_H

//##########################################################################
//#                                                                        #
//#                              CLOUDCOMPARE                              #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 or later of the License.      #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#          COPYRIGHT: CloudCompare project                               #
//#                                                                        #
//##########################################################################

//Common
#include <ccApplicationBase.h>
#include <QString>

class viewerPy;

class viewerPyApplication : public ccApplicationBase
{
	Q_OBJECT
	
public:
	viewerPyApplication( int &argc, char **argv, bool isCommandLine, QString appPath );

	void  setViewer( viewerPy *inViewer );
	viewerPy* getViewer() { return mViewer; };
	
protected:
	bool event( QEvent *inEvent ) override;
	
private:
	viewerPy *mViewer;
};

#endif
