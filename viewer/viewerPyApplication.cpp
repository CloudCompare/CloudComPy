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

#include <QtGlobal>

#ifdef Q_OS_MAC
#include <QFileOpenEvent>
#endif

#include "viewerPy.h"
#include "viewerPyApplication.h"
#include <pyccTrace.h>


viewerPyApplication::viewerPyApplication( int &argc, char **argv, bool isCommandLine, QString appPath )
	: ccApplicationBase( argc, argv, isCommandLine, QStringLiteral( "1.40.0" ), appPath)
{
	mViewer=nullptr;
	setApplicationName( "CloudCompareViewer" );
	CCTRACE("viewerPyApplication");
}

void viewerPyApplication::setViewer(viewerPy *inViewer)
{
    CCTRACE("setViewer");
	mViewer = inViewer;
}

bool viewerPyApplication::event(QEvent *inEvent)
{
#ifdef Q_OS_MAC
	switch ( inEvent->type() )
	{
		case QEvent::FileOpen:
		{			
			if ( mViewer == nullptr )
			{
				return false;
			}
			
			mViewer->addToDB( { static_cast<QFileOpenEvent *>(inEvent)->file() } );
			return true;
		}
			
		default:
			break;
	}
#endif

	return ccApplicationBase::event( inEvent );
}
