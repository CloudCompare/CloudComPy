//##########################################################################
//#                                                                        #
//#                   CLOUDCOMPARE LIGHT VIEWER                            #
//#                                                                        #
//#  This project has been initiated under funding from ANR/CIFRE          #
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
//#      +++ COPYRIGHT: EDF R&D + TELECOM ParisTech (ENST-TSI) +++         #
//#                                                                        #
//##########################################################################

#include "viewerPy.h"
#include "viewerPyApplication.h"
#include <pyccTrace.h>

//Qt
#include <QMessageBox>

//qCC_glWindow
#include <ccGLWindowInterface.h>

//common dialogs
#include <ccCameraParamEditDlg.h>
#include <ccDisplayOptionsDlg.h>
#include <ccStereoModeDlg.h>

//qCC_db
#include <ccGenericMesh.h>
#include <ccHObjectCaster.h>
#include <ccPointCloud.h>

//plugins
#include "ccGLPluginInterface.h"
#include "ccIOPluginInterface.h"
#include "ccPluginManager.h"

//3D mouse handler
#ifdef CC_3DXWARE_SUPPORT
#include "Mouse3DInput.h"
#endif

//Gamepads
#ifdef CC_GAMEPAD_SUPPORT
#include "ccGamepadManager.h"
#endif

//Camera parameters dialog
static ccCameraParamEditDlg* s_cpeDlg = nullptr;

viewerPy::viewerPy(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	, m_glWindow(nullptr)
	, m_selectedObject(nullptr)
	, m_3dMouseInput(nullptr)
    , m_gamepadManager(nullptr)
    , m_filename("render.png")
{
    CCTRACE("viewerPy");
	ui.setupUi(this);

#ifdef Q_OS_LINUX
	 //we reset the whole stylesheet but we keep the StatusBar style
	setStyleSheet(QString());	
	setStyleSheet("QStatusBar{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 rgb(200,200,200), stop:1 rgb(255,255,255));}");
#endif
	
	setWindowTitle(QString("viewerPy v%1").arg(ccApp->versionLongStr( false )));

	//insert GL window in a vertical layout
	{
		QVBoxLayout* verticalLayout = new QVBoxLayout(ui.GLframe);
		verticalLayout->setSpacing(0);
		const int margin = 10;
		verticalLayout->setContentsMargins(margin, margin, margin, margin);

        bool stereoMode = ccGLWindowInterface::TestStereoSupport();

		QWidget* glWidget = nullptr;
        ccGLWindowInterface::Create(m_glWindow, glWidget, stereoMode);
		assert(m_glWindow && glWidget);

		verticalLayout->addWidget(glWidget);
	}

	updateGLFrameGradient();

	m_glWindow->setRectangularPickingAllowed(false); //multiple entities picking not supported

	//UI/display synchronization
	ui.actionFullScreen->setChecked(false);
	ui.menuSelected->setEnabled(false);
	reflectLightsState();
	reflectPerspectiveState();
	reflectPivotVisibilityState();

#ifdef CC_3DXWARE_SUPPORT
	enable3DMouse(true);
#else
	ui.actionEnable3DMouse->setEnabled(false);
#endif

#ifdef CC_GAMEPAD_SUPPORT
    m_gamepadManager = new ccGamepadManager(this, this);
    ui.menuOptions->insertMenu(ui.menu3DMouse->menuAction(), m_gamepadManager->menu());
#endif

    //Signals & slots connection
    connect(m_glWindow->signalEmitter(),            &ccGLWindowSignalEmitter::filesDropped,             this,   qOverload<QStringList>(&viewerPy::addToDB), Qt::QueuedConnection);
    connect(m_glWindow->signalEmitter(),            &ccGLWindowSignalEmitter::entitySelectionChanged,   this,   &viewerPy::selectEntity);
    connect(m_glWindow->signalEmitter(),            &ccGLWindowSignalEmitter::exclusiveFullScreenToggled,this,  &viewerPy::onExclusiveFullScreenToggled);

	//"Options" menu
	connect(ui.actionDisplayParameters,				&QAction::triggered,					this,	&viewerPy::showDisplayParameters);
	connect(ui.actionEditCamera,					&QAction::triggered,					this,	&viewerPy::doActionEditCamera);
	//"Display > Standard views" menu
	connect(ui.actionSetViewTop,					&QAction::triggered,					this,	&viewerPy::setTopView);
	connect(ui.actionSetViewBottom,					&QAction::triggered,					this,	&viewerPy::setBottomView);
	connect(ui.actionSetViewFront,					&QAction::triggered,					this,	&viewerPy::setFrontView);
	connect(ui.actionSetViewBack,					&QAction::triggered,					this,	&viewerPy::setBackView);
	connect(ui.actionSetViewLeft,					&QAction::triggered,					this,	&viewerPy::setLeftView);
	connect(ui.actionSetViewRight,					&QAction::triggered,					this,	&viewerPy::setRightView);
	connect(ui.actionSetViewIso1,					&QAction::triggered,					this,	&viewerPy::setIsoView1);
	connect(ui.actionSetViewIso2,					&QAction::triggered,					this,	&viewerPy::setIsoView2);

	//"Options > Perspective" menu
	connect(ui.actionSetOrthoView,					&QAction::triggered,					this,	&viewerPy::setOrthoView);
	connect(ui.actionSetCenteredPerspectiveView,	&QAction::triggered,					this,	&viewerPy::setCenteredPerspectiveView);
	connect(ui.actionSetViewerPerspectiveView,		&QAction::triggered,					this,	&viewerPy::setViewerPerspectiveView);
	//"Options > Rotation symbol" menu
	connect(ui.actionSetPivotAlwaysOn,				&QAction::triggered,					this,	&viewerPy::setPivotAlwaysOn);
	connect(ui.actionSetPivotRotationOnly,			&QAction::triggered,					this,	&viewerPy::setPivotRotationOnly);
	connect(ui.actionSetPivotOff,					&QAction::triggered,					this,	&viewerPy::setPivotOff);
	//"Options > 3D mouse" menu
	connect(ui.actionEnable3DMouse,					&QAction::toggled,						this,	&viewerPy::enable3DMouse);
	//"Display > Lights & Materials" menu
	connect(ui.actionToggleSunLight,				&QAction::toggled,						this,	&viewerPy::toggleSunLight);
	connect(ui.actionToggleCustomLight,				&QAction::toggled,						this,	&viewerPy::toggleCustomLight);
	//"Options" menu
	connect(ui.actionGlobalZoom,					&QAction::triggered,					this,	&viewerPy::setGlobalZoom);
	connect(ui.actionEnableStereo,					&QAction::toggled,						this,	&viewerPy::toggleStereoMode);
	connect(ui.actionFullScreen,					&QAction::toggled,						this,	&viewerPy::toggleFullScreen);
	connect(ui.actionLockRotationVertAxis,			&QAction::triggered,					this,   &viewerPy::toggleRotationAboutVertAxis);

	//"Options > Selected" menu
	connect(ui.actionShowColors,					&QAction::toggled,						this,	&viewerPy::toggleColorsShown);
	connect(ui.actionShowNormals,					&QAction::toggled,						this,	&viewerPy::toggleNormalsShown);
	connect(ui.actionShowMaterials,					&QAction::toggled,						this,	&viewerPy::toggleMaterialsShown);
	connect(ui.actionShowScalarField,				&QAction::toggled,						this,	&viewerPy::toggleScalarShown);
	connect(ui.actionShowColorRamp,					&QAction::toggled,						this,	&viewerPy::toggleColorbarShown);
	connect(ui.actionZoomOnSelectedEntity,			&QAction::triggered,					this,	&viewerPy::zoomOnSelectedEntity);
	connect(ui.actionDelete,						&QAction::triggered,					this,	&viewerPy::doActionDeleteSelectedEntity);


	//"Shaders" menu
	connect(ui.actionNoFilter,						&QAction::triggered,					this,	&viewerPy::doDisableGLFilter);

	//"Help" menu
	connect(ui.actionAbout,							&QAction::triggered,					this,	&viewerPy::doActionAbout);
	connect(ui.actionHelpShortcuts,					&QAction::triggered,					this,	&viewerPy::doActionDisplayShortcuts);

	loadPlugins();
}

viewerPy::~viewerPy()
{
	release3DMouse();

#ifdef CC_GAMEPAD_SUPPORT
	delete m_gamepadManager;
	m_gamepadManager = nullptr;
#endif

	if (s_cpeDlg)
	{
		delete s_cpeDlg;
		s_cpeDlg = nullptr;
	}

	ccHObject* currentRoot = m_glWindow->getSceneDB();
	if (currentRoot)
	{
		m_glWindow->setSceneDB(nullptr);
		//m_glWindow->redraw();
		delete currentRoot;
	}
}

void viewerPy::loadPlugins()
{
	ui.menuPlugins->setEnabled(false);

	ccPluginManager::Get().loadPlugins();

	for (ccPluginInterface* plugin : ccPluginManager::Get().pluginList())
	{
		if (plugin == nullptr)
		{
			Q_ASSERT(false);
			continue;
		}

		// is this a GL plugin?
		if (plugin->getType() == CC_GL_FILTER_PLUGIN)
		{
			ccGLPluginInterface* glPlugin = static_cast<ccGLPluginInterface*>(plugin);

			const QString pluginName = glPlugin->getName();

			Q_ASSERT(!pluginName.isEmpty());

			if (pluginName.isEmpty())
			{
				// should be unreachable - we have already checked for this in ccPlugins::Find()
				continue;
			}

			ccLog::Print(QStringLiteral("Plugin name: [%1] (GL filter)").arg(pluginName));

			QAction* action = new QAction(pluginName, this);
			action->setToolTip(glPlugin->getDescription());
			action->setIcon(glPlugin->getIcon());

			// store the plugin's interface pointer in the QAction data so we can access it in doEnableGLFilter()
			QVariant v;
			v.setValue(glPlugin);

			action->setData(v);

			connect(action, &QAction::triggered, this, &viewerPy::doEnableGLFilter);

			ui.menuPlugins->addAction(action);
			ui.menuPlugins->setEnabled(true);
			ui.menuPlugins->setVisible(true);
		}
	}
}

void viewerPy::doDisableGLFilter()
{
	if (m_glWindow)
	{
		m_glWindow->setGlFilter(nullptr);
		m_glWindow->redraw();
	}
}

void viewerPy::doEnableGLFilter()
{
	if (!m_glWindow)
	{
		ccLog::Warning("[GL filter] No active 3D view!");
		return;
	}

	QAction* action = qobject_cast<QAction*>(sender());

	if (action == nullptr)
	{
		Q_ASSERT(false);
		return;
	}

	ccGLPluginInterface	*plugin = action->data().value<ccGLPluginInterface *>();
	if (plugin == nullptr)
	{
		return;
	}
	Q_ASSERT(plugin->getType() == CC_GL_FILTER_PLUGIN);

	ccGlFilter* filter = plugin->getFilter();
	if (filter != nullptr)
	{
		if (m_glWindow->areGLFiltersEnabled())
		{
			m_glWindow->setGlFilter(filter);

			ccLog::Print("Note: go to << Display > Shaders & Filters > No filter >> to disable GL filter");
		}
		else
		{
			ccLog::Error("GL filters not supported");
		}
	}
	else
	{
		ccLog::Error("Can't load GL filter (an error occurred)!");
	}
}

void viewerPy::doActionDeleteSelectedEntity()
{
	ccHObject* currentRoot = m_glWindow->getSceneDB();
	if (!currentRoot)
		return;

	ccHObject::Container toCheck;
	toCheck.push_back(currentRoot);

	while (!toCheck.empty())
	{
		ccHObject* obj = toCheck.back();
		toCheck.pop_back();

		if (obj->isSelected())
		{
			if (obj->getParent())
			{
				obj->getParent()->addDependency(obj,ccHObject::DP_DELETE_OTHER); //we force deletion!
				obj->getParent()->removeChild(obj);
			}
			else
			{
				delete obj;
				obj = nullptr;
			}
		}
		else
		{
			for (unsigned i = 0; i < obj->getChildrenNumber(); ++i)
			{
				toCheck.push_back(obj->getChild(i));
			}
		}
	}

	m_glWindow->redraw();
}

void viewerPy::selectEntity(ccHObject* toSelect)
{
	ccHObject* currentRoot = m_glWindow->getSceneDB();
	if (!currentRoot)
		return;

	currentRoot->setSelected_recursive(false);
	ui.menuSelectSF->clear();
	ui.menuSelected->setEnabled(false);

	if (toSelect)
	{
		toSelect->setSelected(true);

		ui.actionShowColors->blockSignals(true);
		ui.actionShowNormals->blockSignals(true);
		ui.actionShowMaterials->blockSignals(true);
		ui.actionShowScalarField->blockSignals(true);
		ui.actionShowColorRamp->blockSignals(true);

		ui.actionShowColors->setEnabled(toSelect->hasColors());
		ui.actionShowColors->setChecked(toSelect->colorsShown());
		ui.actionShowNormals->setEnabled(toSelect->hasNormals());
		ui.actionShowNormals->setChecked(toSelect->normalsShown());

		if (toSelect->isKindOf(CC_TYPES::MESH))
		{
			ccGenericMesh* mesh = static_cast<ccGenericMesh*>(toSelect);
			ui.actionShowMaterials->setEnabled(mesh->hasMaterials());
			ui.actionShowMaterials->setChecked(mesh->materialsShown());
		}
		else
		{
			ui.actionShowMaterials->setEnabled(false);
			ui.actionShowMaterials->setChecked(false);
		}

		ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(toSelect);
		bool hasSF = (cloud ? cloud->hasScalarFields() : false);
		ui.actionShowScalarField->setEnabled(hasSF);
		ui.actionShowScalarField->setChecked(toSelect->sfShown());
		ui.actionShowColorRamp->setEnabled(hasSF);
		ui.actionShowColorRamp->setChecked(cloud ? cloud->sfColorScaleShown() && cloud->sfShown() : false);

		unsigned sfCount = (cloud ? cloud->getNumberOfScalarFields() : 0);
		ui.menuSelectSF->setEnabled(hasSF && sfCount>1);
		if (hasSF && sfCount > 1)
		{
			int currentSFIndex = cloud->getCurrentDisplayedScalarFieldIndex();
			//ui.menuSelectSF->clear();
			for (unsigned i = 0; i < sfCount; ++i)
			{
				QAction* action = ui.menuSelectSF->addAction(cloud->getScalarFieldName(i));
				action->setData(i);
				action->setCheckable(true);
				if (currentSFIndex == static_cast<int>(i))
					action->setChecked(true);
				connect(action, &QAction::toggled, this, &viewerPy::changeCurrentScalarField);
			}
		}

		ui.menuSelected->setEnabled(true);

		ui.actionShowColors->blockSignals(false);
		ui.actionShowNormals->blockSignals(false);
		ui.actionShowMaterials->blockSignals(false);
		ui.actionShowScalarField->blockSignals(false);
		ui.actionShowColorRamp->blockSignals(false);

		m_selectedObject = toSelect;
	}

	m_glWindow->redraw();
}

bool viewerPy::checkForLoadedEntities()
{
	bool loadedEntities = true;
	m_glWindow->displayNewMessage(QString(),ccGLWindowInterface::SCREEN_CENTER_MESSAGE); //clear (any) message in the middle area

	if (!m_glWindow->getSceneDB())
	{
		m_glWindow->displayNewMessage("Drag & drop files on the 3D window to load them!",ccGLWindowInterface::SCREEN_CENTER_MESSAGE,true,3600);
		loadedEntities = false;
	}

	if (m_glWindow->getDisplayParameters().displayCross != loadedEntities)
	{
		ccGui::ParamStruct params = m_glWindow->getDisplayParameters();
		params.displayCross = loadedEntities;
		m_glWindow->setDisplayParameters(params);
	}

	return loadedEntities;
}

void viewerPy::updateDisplay()
{
	updateGLFrameGradient();

	m_glWindow->redraw();
}

void viewerPy::setBackgroundColor(bool gradient, unsigned char r, unsigned char g, unsigned char b)
{
    static const ccColor::Rgbub backgroundCol(r, g, b);
    ccGui::ParamStruct params = m_glWindow->getDisplayParameters();
    params.drawBackgroundGradient = gradient;
    params.backgroundCol = backgroundCol;
    m_glWindow->setDisplayParameters(params);

    m_glWindow->redraw();
}

void viewerPy::setTextDefaultCol(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    static const ccColor::Rgba foregroundCol(r, g, b, a);
    ccGui::ParamStruct params = m_glWindow->getDisplayParameters();
    params.textDefaultCol = foregroundCol;
    m_glWindow->setDisplayParameters(params);

    m_glWindow->redraw();
}

void viewerPy::setColorScaleShowHistogram(bool showHist)
{
    ccGui::ParamStruct params = m_glWindow->getDisplayParameters();
    params.colorScaleShowHistogram = showHist;
    m_glWindow->setDisplayParameters(params);

    m_glWindow->redraw();
}

void viewerPy::updateGLFrameGradient()
{
	//display parameters
	static const ccColor::Rgbub s_black(0, 0, 0);
	static const ccColor::Rgbub s_white(255, 255, 255);
	bool stereoModeEnabled = m_glWindow->stereoModeIsEnabled();
	const ccColor::Rgbub& bkgCol = stereoModeEnabled ? s_black : m_glWindow->getDisplayParameters().backgroundCol;
	const ccColor::Rgbub& forCol = stereoModeEnabled ? s_white : m_glWindow->getDisplayParameters().pointsDefaultCol;

	QString styleSheet = QString("QFrame{border: 2px solid white; border-radius: 10px; background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 rgb(%1,%2,%3), stop:1 rgb(%4,%5,%6));}")
								.arg(bkgCol.r)
								.arg(bkgCol.g)
								.arg(bkgCol.b)
								.arg(255-forCol.r)
								.arg(255-forCol.g)
								.arg(255-forCol.b);
	
	ui.GLframe->setStyleSheet(styleSheet);
}

ccHObject* viewerPy::addToDB(QStringList filenames)
{
	ccHObject* currentRoot = m_glWindow->getSceneDB();
	if (currentRoot)
	{
		m_selectedObject = nullptr;
		m_glWindow->setSceneDB(nullptr);
		m_glWindow->redraw();
		delete currentRoot;
		currentRoot = nullptr;
	}

	bool scaleAlreadyDisplayed = false;

	FileIOFilter::LoadParameters parameters;
	parameters.alwaysDisplayLoadDialog = false;
	parameters.shiftHandlingMode = ccGlobalShiftManager::NO_DIALOG_AUTO_SHIFT;
	parameters.parentWidget = this;

	const ccOptions& options = ccOptions::Instance();
	FileIOFilter::ResetSesionCounter();

    ccHObject* firstLoadedEntity = nullptr;

	for (int i = 0; i < filenames.size(); ++i)
	{
		CC_FILE_ERROR result = CC_FERR_NO_ERROR;
		ccHObject* newGroup = FileIOFilter::LoadFromFile(filenames[i], parameters, result);

		if (newGroup)
		{
			if (!options.normalsDisplayedByDefault)
			{
				//disable the normals on all loaded clouds!
				ccHObject::Container clouds;
				newGroup->filterChildren(clouds, true, CC_TYPES::POINT_CLOUD);
				for (ccHObject* cloud : clouds)
				{
					if (cloud)
					{
						static_cast<ccGenericPointCloud*>(cloud)->showNormals(false);
					}
				}
			}

			addToDB(newGroup);

			if (!scaleAlreadyDisplayed)
			{
				for (unsigned i = 0; i < newGroup->getChildrenNumber(); ++i)
				{
					ccHObject* ent = newGroup->getChild(i);
					if (ent->isA(CC_TYPES::POINT_CLOUD))
					{
						ccPointCloud* pc = static_cast<ccPointCloud*>(ent);
						if (pc->hasScalarFields())
						{
							pc->setCurrentDisplayedScalarField(0);
							pc->showSFColorsScale(true);
							scaleAlreadyDisplayed=true;
						}
					}
					else if (ent->isKindOf(CC_TYPES::MESH))
					{
						ccGenericMesh* mesh = static_cast<ccGenericMesh*>(ent);
						if (mesh->hasScalarFields())
						{
							mesh->showSF(true);
							scaleAlreadyDisplayed=true;
							ccPointCloud* pc = static_cast<ccPointCloud*>(mesh->getAssociatedCloud());
							pc->showSFColorsScale(true);
						}
					}
				}
			}

            if (!firstLoadedEntity)
            {
                firstLoadedEntity = newGroup;
            }
		}
		
		if (result == CC_FERR_CANCELED_BY_USER)
		{
			//stop importing the file if the user has cancelled the current process!
			break;
		}
	}

	checkForLoadedEntities();

    return firstLoadedEntity;
}

void viewerPy::addToDB(	ccHObject* entity,
						bool updateZoom/*=false*/,
						bool autoExpandDBTree/*=true*/,
						bool checkDimensions/*=false*/,
						bool autoRedraw/*=true*/)
{
	assert(entity && m_glWindow);

	entity->setDisplay_recursive(m_glWindow);

	ccHObject* currentRoot = m_glWindow->getSceneDB();
	if (currentRoot)
	{
		//already a pure 'root'
		if (currentRoot->isA(CC_TYPES::HIERARCHY_OBJECT))
		{
			currentRoot->addChild(entity);
		}
		else
		{
			ccHObject* root = new ccHObject("root");
			root->addChild(currentRoot);
			root->addChild(entity);
			m_glWindow->setSceneDB(root);
		}
	}
	else
	{
		m_glWindow->setSceneDB(entity);
	}

	checkForLoadedEntities();
}

void viewerPy::removeFromDB(ccHObject* obj, bool autoDelete/*=true*/)
{
    ccHObject* currentRoot = m_glWindow->getSceneDB();
    if (currentRoot)
    {
        if (currentRoot == obj)
        {
        	CCTRACE("removeFromDB: currentRoot == obj")
            m_glWindow->setSceneDB(nullptr);
            if (autoDelete)
            {
                delete currentRoot;
            }
        }
        else
        {
        	CCTRACE("removeFromDB: currentRoot != obj")
            currentRoot->removeChild(obj);
        }
    }

    m_glWindow->redraw();
}

void viewerPy::showDisplayParameters()
{
	ccDisplayOptionsDlg clmDlg(this);

	connect(&clmDlg, &ccDisplayOptionsDlg::aspectHasChanged, this, &viewerPy::updateDisplay);

	clmDlg.exec();

	disconnect(&clmDlg, nullptr, nullptr, nullptr);
}

void viewerPy::doActionEditCamera()
{
	if (!s_cpeDlg)
	{
		s_cpeDlg = new ccCameraParamEditDlg(this, nullptr);
		s_cpeDlg->linkWith(m_glWindow);
	}
	s_cpeDlg->show();
}

void viewerPy::doActionRenderToFile(QString filename, bool isInteractive)
{
    m_filename = filename;
    QCoreApplication *coreApp = QCoreApplication::instance();
    viewerPyApplication* app = dynamic_cast<viewerPyApplication*>(coreApp);
    if (app)
    {
        connect(this, SIGNAL(exitRequested()), app, SLOT(closeAllWindows()), Qt::QueuedConnection);
        if (!isInteractive)
        {
            this->renderToFileAndClose();
        } else
        {
            ui.actionResume_Python->setEnabled(true);
            connect(ui.actionResume_Python, &QAction::triggered, this, &viewerPy::renderToFileAndClose);
        }
    }
}

void viewerPy::renderToFileAndClose()
{
    m_glWindow->renderToFile(m_filename);
    CCTRACE("renderToFile done: " << m_filename.toStdString());
    CCTRACE("call app->exit()");
    exitRequested();
}

void viewerPy::reflectPerspectiveState()
{
	if ( m_glWindow == nullptr )
		return;
	
	bool objectCentered = false;
	bool perspectiveEnabled = m_glWindow->getPerspectiveState(objectCentered);

	ui.actionSetOrthoView->setChecked(!perspectiveEnabled);
	ui.actionSetCenteredPerspectiveView->setChecked(perspectiveEnabled && objectCentered);
	ui.actionSetViewerPerspectiveView->setChecked(perspectiveEnabled && !objectCentered);
}

bool viewerPy::checkStereoMode()
{
	if (	m_glWindow
		&&	m_glWindow->getViewportParameters().perspectiveView
		&&	m_glWindow->stereoModeIsEnabled())
	{
		if (QMessageBox::question(this,"Stereo mode", "Stereo-mode only works in perspective mode. Do you want to enable it?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
		{
			return false;
		}
		else
		{
			toggleStereoMode(false);
		}
	}

	return true;
}

void viewerPy::setOrthoView()
{
	if (m_glWindow)
	{
		if (!checkStereoMode())
			return;
		m_glWindow->setPerspectiveState(false,true);
		m_glWindow->redraw();
	}
	reflectPerspectiveState();
}

void viewerPy::setCenteredPerspectiveView()
{
	if (m_glWindow)
	{
		m_glWindow->setPerspectiveState(true,true);
		m_glWindow->redraw();
	}
	reflectPerspectiveState();
}

void viewerPy::setViewerPerspectiveView()
{
	if (m_glWindow)
	{
		m_glWindow->setPerspectiveState(true,false);
		m_glWindow->redraw();
	}
	reflectPerspectiveState();
}

void viewerPy::reflectPivotVisibilityState()
{
	if ( m_glWindow == nullptr )
		return;
	
	ccGLWindowInterface::PivotVisibility vis = m_glWindow->getPivotVisibility();

	ui.actionSetPivotAlwaysOn->setChecked(vis == ccGLWindowInterface::PIVOT_ALWAYS_SHOW);
	ui.actionSetPivotRotationOnly->setChecked(vis == ccGLWindowInterface::PIVOT_SHOW_ON_MOVE);
	ui.actionSetPivotOff->setChecked(vis == ccGLWindowInterface::PIVOT_HIDE);
}

void viewerPy::setPivotAlwaysOn()
{
	if (m_glWindow)
	{
		m_glWindow->setPivotVisibility(ccGLWindowInterface::PIVOT_ALWAYS_SHOW);
		m_glWindow->redraw();
	}
	reflectPivotVisibilityState();
}

void viewerPy::setPivotRotationOnly()
{
	if (m_glWindow)
	{
		m_glWindow->setPivotVisibility(ccGLWindowInterface::PIVOT_SHOW_ON_MOVE);
		m_glWindow->redraw();
	}
	reflectPivotVisibilityState();
}

void viewerPy::setPivotOff()
{
	if (m_glWindow)
	{
		m_glWindow->setPivotVisibility(ccGLWindowInterface::PIVOT_HIDE);
		m_glWindow->redraw();
	}
	reflectPivotVisibilityState();
}

void viewerPy::reflectLightsState()
{
	if ( m_glWindow == nullptr )
		return;
	
	ui.actionToggleSunLight->blockSignals(true);
	ui.actionToggleCustomLight->blockSignals(true);

	ui.actionToggleSunLight->setChecked(m_glWindow->sunLightEnabled());
	ui.actionToggleCustomLight->setChecked(m_glWindow->customLightEnabled());

	ui.actionToggleSunLight->blockSignals(false);
	ui.actionToggleCustomLight->blockSignals(false);
}

void viewerPy::toggleSunLight(bool state)
{
	if (m_glWindow)
		m_glWindow->setSunLight(state);
	reflectLightsState();
}

void viewerPy::toggleCustomLight(bool state)
{
	if (m_glWindow)
		m_glWindow->setCustomLight(state);
	reflectLightsState();
}

void viewerPy::toggleStereoMode(bool state)
{
	if (!m_glWindow)
		return;

	bool isActive = m_glWindow->stereoModeIsEnabled();
	if (isActive == state)
	{
		//nothing to do
		return;
	}

	if (isActive)
	{
		m_glWindow->disableStereoMode();
		if (	m_glWindow->getStereoParams().glassType == ccGLWindowInterface::StereoParams::NVIDIA_VISION
			||	m_glWindow->getStereoParams().glassType == ccGLWindowInterface::StereoParams::GENERIC_STEREO_DISPLAY)
		{
			//disable full screen
			ui.actionFullScreen->setChecked(false);
		}
	}
	else
	{
		//display a parameters dialog
		ccStereoModeDlg smDlg(this);
		smDlg.setParameters(m_glWindow->getStereoParams());
		if (!smDlg.exec())
		{
			//cancelled by the user
			ui.actionEnableStereo->blockSignals(true);
			ui.actionEnableStereo->setChecked(false);
			ui.actionEnableStereo->blockSignals(false);
			return;
		}

        ccGLWindowInterface::StereoParams params = smDlg.getParameters();
        if (!ccGLWindowInterface::StereoSupported() && !params.isAnaglyph())
        {
            ccLog::Error(tr("It seems your graphic card doesn't support Quad Buffered Stereo rendering"));
            //activation of the stereo mode failed: cancel selection
            ui.actionEnableStereo->blockSignals(true);
            ui.actionEnableStereo->setChecked(false);
            ui.actionEnableStereo->blockSignals(false);
            return;
        }

		//force perspective state!
		if (!m_glWindow->getViewportParameters().perspectiveView)
		{
			m_glWindow->setPerspectiveState(true, true);
			reflectPerspectiveState();
		}

		if (	params.glassType == ccGLWindowInterface::StereoParams::NVIDIA_VISION
			||	params.glassType == ccGLWindowInterface::StereoParams::GENERIC_STEREO_DISPLAY)
		{
			//force full screen
			ui.actionFullScreen->setChecked(true);
		}

		if (!m_glWindow->enableStereoMode(params))
		{
			//activation of the stereo mode failed: cancel selection
			ui.actionEnableStereo->blockSignals(true);
			ui.actionEnableStereo->setChecked(false);
			ui.actionEnableStereo->blockSignals(false);
		}
	}

	updateDisplay();
}

void viewerPy::toggleFullScreen(bool state)
{
	if (m_glWindow)
	{
		if (	m_glWindow->stereoModeIsEnabled()
			&&	(	m_glWindow->getStereoParams().glassType == ccGLWindowInterface::StereoParams::NVIDIA_VISION
				||	m_glWindow->getStereoParams().glassType == ccGLWindowInterface::StereoParams::GENERIC_STEREO_DISPLAY)
			)
		{
			//auto disable stereo mode as NVidia Vision only works in full screen mode!
			ui.actionEnableStereo->setChecked(false);
		}

		m_glWindow->toggleExclusiveFullScreen(state);
	}
}

void viewerPy::onExclusiveFullScreenToggled(bool state)
{
	ui.actionFullScreen->blockSignals(true);
	ui.actionFullScreen->setChecked(m_glWindow ? m_glWindow->exclusiveFullScreen() : false);
	ui.actionFullScreen->blockSignals(false);

	if (	!state
		&&	m_glWindow
		&&	m_glWindow->stereoModeIsEnabled()
		&&	(	m_glWindow->getStereoParams().glassType == ccGLWindowInterface::StereoParams::NVIDIA_VISION
			||	m_glWindow->getStereoParams().glassType == ccGLWindowInterface::StereoParams::GENERIC_STEREO_DISPLAY)
		)
	{
		//auto disable stereo mode as NVidia Vision only works in full screen mode!
		ui.actionEnableStereo->setChecked(false);
	}
}

void viewerPy::toggleRotationAboutVertAxis()
{
	if (!m_glWindow)
		return;

	bool wasLocked = m_glWindow->isRotationAxisLocked();
	bool isLocked = !wasLocked;

	m_glWindow->lockRotationAxis(isLocked, CCVector3d(0.0, 0.0, 1.0));

	ui.actionLockRotationVertAxis->blockSignals(true);
	ui.actionLockRotationVertAxis->setChecked(isLocked);
	ui.actionLockRotationVertAxis->blockSignals(false);

	if (isLocked)
	{
		m_glWindow->displayNewMessage(QString("[ROTATION LOCKED]"), ccGLWindowInterface::UPPER_CENTER_MESSAGE, false, 24 * 3600, ccGLWindowInterface::ROTAION_LOCK_MESSAGE);
	}
	else
	{
		m_glWindow->displayNewMessage(QString(), ccGLWindowInterface::UPPER_CENTER_MESSAGE, false, 0, ccGLWindowInterface::ROTAION_LOCK_MESSAGE);
	}
	m_glWindow->redraw();
}

void viewerPy::doActionDisplayShortcuts()
{
	QMessageBox msgBox;
	QString text;
	text += "Shortcuts:\n\n";
	text += "F2 : Set orthographic view\n";
	text += "F3 : Set object-centered perspective\n";
	text += "F4 : Set viewer-based perspective\n";
	text += "F6 : Toggle sun light\n";
	text += "F7 : Toggle custom light\n";
	text += "F8 : Toggle Console display\n";
	text += "F9 : Toggle full screen\n";
	text += "F11: Toggle exclusive full screen\n";
	text += "Z  : Zoom on selected entity\n";
	text += "L  : Lock rotation around Z\n";
	text += "B  : Enter/leave bubble view mode\n";
	text += "DEL: Delete selected entity\n";
	text += "+  : Zoom in\n";
	text += "-  : Zoom out\n";
	text += "\n";
	text += "Shift + C: Toggle color ramp visibility\n";
	text += "Shift + up arrow: activate previous SF\n";
	text += "Shift + down arrow: activate next SF\n";
	text += "\n";
	text += "Ctrl + D: Display parameters\n";
	text += "Ctrl + C: Camera parameters\n";
	text += "\n";
	text += "Left click: Select entity\n";
	//text += "Ctrl + left click: Select multiple entities (toggle)\n";
	//text += "Alt + left button hold: Select multiple entities (rectangular area)\n";
	text += "Shift + left click (on a point/triangle): spawn a label\n";
	text += "Right click (on a label): expand/collapse\n";
	msgBox.setText(text);
	msgBox.exec();
}

void viewerPy::setTopView()
{
	m_glWindow->setView(CC_TOP_VIEW);
}

void viewerPy::setBottomView()
{
	m_glWindow->setView(CC_BOTTOM_VIEW);
}

void viewerPy::setFrontView()
{
	m_glWindow->setView(CC_FRONT_VIEW);
}

void viewerPy::setBackView()
{
	m_glWindow->setView(CC_BACK_VIEW);
}

void viewerPy::setLeftView()
{
	m_glWindow->setView(CC_LEFT_VIEW);
}

void viewerPy::setRightView()
{
	m_glWindow->setView(CC_RIGHT_VIEW);
}

void viewerPy::setIsoView1()
{
	m_glWindow->setView(CC_ISO_VIEW_1);
}

void viewerPy::setIsoView2()
{
	m_glWindow->setView(CC_ISO_VIEW_2);
}

void viewerPy::toggleColorsShown(bool state)
{
	if (!m_selectedObject)
		return;

	m_selectedObject->showColors(state);
	m_glWindow->redraw();
}

void viewerPy::toggleNormalsShown(bool state)
{
	if (!m_selectedObject)
		return;

	m_selectedObject->showNormals(state);
	m_glWindow->redraw();
}

void viewerPy::toggleMaterialsShown(bool state)
{
	if (m_selectedObject && m_selectedObject->isKindOf(CC_TYPES::MESH))
	{
		static_cast<ccGenericMesh*>(m_selectedObject)->showMaterials(state);
		m_glWindow->redraw();
	}
}

void viewerPy::toggleScalarShown(bool state)
{
	if (!m_selectedObject)
		return;

	m_selectedObject->showSF(state);
	m_glWindow->redraw();
}

void viewerPy::toggleColorbarShown(bool state)
{
	if (!m_selectedObject)
		return;

	ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(m_selectedObject);
	if (!cloud)
		return;
	cloud->showSFColorsScale(state);
	m_glWindow->redraw(true, false);
}

void viewerPy::changeCurrentScalarField(bool state)
{
	if (!m_selectedObject)
		return;

	ccPointCloud* cloud = ccHObjectCaster::ToPointCloud(m_selectedObject);
	if (!cloud)
		return;

	QAction* action = qobject_cast<QAction*>(QObject::sender());
	if (!action)
		return;

	//disable all other actions
	const QObjectList& children = ui.menuSelectSF->children();
	for (int i = 0; i < children.size(); ++i)
	{
		QAction* act = static_cast<QAction*>(children[i]);
		act->blockSignals(true);
		act->setChecked(act == action);
		act->blockSignals(false);
	}

	int sfIndex = action->data().toInt();
	if (sfIndex < static_cast<int>(cloud->getNumberOfScalarFields()))
	{
		cloud->setCurrentDisplayedScalarField(sfIndex);
		//when 'setCurrentDisplayedScalarField' is called, scalar field is automatically shown!
		ui.actionShowScalarField->blockSignals(true);
		ui.actionShowScalarField->setChecked(true);
		ui.actionShowScalarField->blockSignals(false);
		m_glWindow->redraw();
	}
}

void viewerPy::setGlobalZoom()
{
	if (m_glWindow)
		m_glWindow->zoomGlobal();
}

void viewerPy::zoomOnSelectedEntity()
{
	if (!m_glWindow || !m_selectedObject)
		return;

	ccBBox box = m_selectedObject->getDisplayBB_recursive(false, m_glWindow);
	m_glWindow->updateConstellationCenterAndZoom(&box);
	m_glWindow->redraw();
}

#include <ui_ccviewerAbout.h>

void viewerPy::doActionAbout()
{
	QDialog aboutDialog(this);

	Ui::AboutDialog ui;
	ui.setupUi(&aboutDialog);
	ui.textEdit->setHtml(ui.textEdit->toHtml().arg(ccApp->versionLongStr( true )));

	aboutDialog.exec();
}

/*** 3D MOUSE SUPPORT ***/

void viewerPy::release3DMouse()
{
#ifdef CC_3DXWARE_SUPPORT
	if (m_3dMouseInput)
	{
		m_3dMouseInput->disconnect(); //disconnect from the driver
		disconnect(m_3dMouseInput); //disconnect from Qt ;)

		delete m_3dMouseInput;
		m_3dMouseInput = 0;
	}
#endif
}

void viewerPy::enable3DMouse(bool state)
{
#ifdef CC_3DXWARE_SUPPORT
	if (m_3dMouseInput)
		release3DMouse();

	if (state)
	{
		m_3dMouseInput = new Mouse3DInput(this);
		if (m_3dMouseInput->connect(this,"viewerPy"))
		{
			QObject::connect(m_3dMouseInput, &Mouse3DInput::sigMove3d,				this,	&viewerPy::on3DMouseMove);
			QObject::connect(m_3dMouseInput, &Mouse3DInput::sigReleased,			this,	&viewerPy::on3DMouseReleased);
			QObject::connect(m_3dMouseInput, &Mouse3DInput::sigOn3dmouseKeyDown,	this,	&viewerPy::on3DMouseKeyDown);
			QObject::connect(m_3dMouseInput, &Mouse3DInput::sigOn3dmouseKeyUp,		this,	&viewerPy::on3DMouseKeyUp);
			QObject::connect(m_3dMouseInput, &Mouse3DInput::sigOn3dmouseCMDKeyDown, this,	&viewerPy::on3DMouseCMDKeyDown);
			QObject::connect(m_3dMouseInput, &Mouse3DInput::sigOn3dmouseCMDKeyUp,	this,	&viewerPy::on3DMouseCMDKeyUp);
		}
		else
		{
			delete m_3dMouseInput;
			m_3dMouseInput = 0;
			
			ccLog::Warning("[3D Mouse] No device found");
			state = false;
		}
	}
	else
	{
		ccLog::Warning("[3D Mouse] Device has been disabled");
	}
#else
	state = false;
#endif

	ui.actionEnable3DMouse->blockSignals(true);
	ui.actionEnable3DMouse->setChecked(state);
	ui.actionEnable3DMouse->blockSignals(false);
}

void viewerPy::on3DMouseKeyUp(int)
{
	//nothing right now
}

// ANY CHANGE/BUG FIX SHOULD BE REFLECTED TO THE EQUIVALENT METHODS IN QCC "MainWindow.cpp" FILE!
void viewerPy::on3DMouseKeyDown(int key)
{
#ifdef CC_3DXWARE_SUPPORT

	switch(key)
	{
	case Mouse3DInput::V3DK_MENU:
		//should be handled by the driver now!
		break;
	case Mouse3DInput::V3DK_FIT:
		{
			if (m_selectedObject)
				zoomOnSelectedEntity();
			else
				setGlobalZoom();
		}
		break;
	case Mouse3DInput::V3DK_TOP:
		setTopView();
		break;
	case Mouse3DInput::V3DK_LEFT:
		setLeftView();
		break;
	case Mouse3DInput::V3DK_RIGHT:
		setRightView();
		break;
	case Mouse3DInput::V3DK_FRONT:
		setFrontView();
		break;
	case Mouse3DInput::V3DK_BOTTOM:
		setBottomView();
		break;
	case Mouse3DInput::V3DK_BACK:
		setBackView();
		break;
	case Mouse3DInput::V3DK_ROTATE:
		//should be handled by the driver now!
		break;
	case Mouse3DInput::V3DK_PANZOOM:
		//should be handled by the driver now!
		break;
	case Mouse3DInput::V3DK_ISO1:
		setIsoView1();
		break;
	case Mouse3DInput::V3DK_ISO2:
		setIsoView2();
		break;
	case Mouse3DInput::V3DK_PLUS:
		//should be handled by the driver now!
		break;
	case Mouse3DInput::V3DK_MINUS:
		//should be handled by the driver now!
		break;
	case Mouse3DInput::V3DK_DOMINANT:
		//should be handled by the driver now!
		break;
	case Mouse3DInput::V3DK_CW:
	case Mouse3DInput::V3DK_CCW:
		{
			if (m_glWindow)
			{
				CCVector3d axis(0,0,-1);
				CCVector3d trans(0,0,0);
				ccGLMatrixd mat;
				double angle = M_PI/2;
				if (key == Mouse3DInput::V3DK_CCW)
					angle = -angle;
				mat.initFromParameters(angle,axis,trans);
				m_glWindow->rotateBaseViewMat(mat);
				m_glWindow->redraw();
			}
		}
		break;
	case Mouse3DInput::V3DK_ESC:
	case Mouse3DInput::V3DK_ALT:
	case Mouse3DInput::V3DK_SHIFT:
	case Mouse3DInput::V3DK_CTRL:
	default:
		ccLog::Warning("[3D mouse] This button is not handled (yet)");
		//TODO
		break;
	}

#endif
}
void viewerPy::on3DMouseCMDKeyUp(int cmd)
{
	//nothing right now
}

void viewerPy::on3DMouseCMDKeyDown(int cmd)
{
#ifdef CC_3DXWARE_SUPPORT
	switch (cmd)
	{
		//ccLog::Print(QString("on3DMouseCMDKeyDown Cmd = %1").arg(cmd));
	case Mouse3DInput::V3DCMD_VIEW_FIT:
	{
		if (m_selectedObject)
			zoomOnSelectedEntity();
		else
			setGlobalZoom();
	}
	break;
	case Mouse3DInput::V3DCMD_VIEW_TOP:
		setTopView();
		break;
	case Mouse3DInput::V3DCMD_VIEW_LEFT:
		setLeftView();
		break;
	case Mouse3DInput::V3DCMD_VIEW_RIGHT:
		setRightView();
		break;
	case Mouse3DInput::V3DCMD_VIEW_FRONT:
		setFrontView();
		break;
	case Mouse3DInput::V3DCMD_VIEW_BOTTOM:
		setBottomView();
		break;
	case Mouse3DInput::V3DCMD_VIEW_BACK:
		setBackView();
		break;
	case Mouse3DInput::V3DCMD_VIEW_ISO1:
		setIsoView1();
		break;
	case Mouse3DInput::V3DCMD_VIEW_ISO2:
		setIsoView2();
		break;
	case Mouse3DInput::V3DCMD_VIEW_ROLLCW:
	case Mouse3DInput::V3DCMD_VIEW_ROLLCCW:
	{
		if (m_glWindow)
		{
			CCVector3d axis(0, 0, -1);
			CCVector3d trans(0, 0, 0);
			ccGLMatrixd mat;
			double angle = M_PI / 2;
			if (cmd == Mouse3DInput::V3DCMD_VIEW_ROLLCCW)
				angle = -angle;
			mat.initFromParameters(angle, axis, trans);
			m_glWindow->rotateBaseViewMat(mat);
			m_glWindow->redraw();
		}
	}
	break;
	case Mouse3DInput::V3DCMD_VIEW_SPINCW:
	case Mouse3DInput::V3DCMD_VIEW_SPINCCW:
	{
		if (m_glWindow)
		{
			CCVector3d axis(0, 1, 0);
			CCVector3d trans(0, 0, 0);
			ccGLMatrixd mat;
			double angle = M_PI / 2;
			if (cmd == Mouse3DInput::V3DCMD_VIEW_SPINCCW)
				angle = -angle;
			mat.initFromParameters(angle, axis, trans);
			m_glWindow->rotateBaseViewMat(mat);
			m_glWindow->redraw();
		}
	}
	case Mouse3DInput::V3DCMD_VIEW_TILTCW:
	case Mouse3DInput::V3DCMD_VIEW_TILTCCW:
	{
		if (m_glWindow)
		{
			CCVector3d axis(1, 0, 0);
			CCVector3d trans(0, 0, 0);
			ccGLMatrixd mat;
			double angle = M_PI / 2;
			if (cmd == Mouse3DInput::V3DCMD_VIEW_TILTCCW)
				angle = -angle;
			mat.initFromParameters(angle, axis, trans);
			m_glWindow->rotateBaseViewMat(mat);
			m_glWindow->redraw();
		}
	}
	break;
	default:
		ccLog::Warning("[3D mouse] This button is not handled (yet)");
		//TODO
		break;
	}
#endif
}

void viewerPy::on3DMouseMove(std::vector<float>& vec)
{
#ifdef CC_3DXWARE_SUPPORT
	if (m_glWindow)
		Mouse3DInput::Apply(vec,m_glWindow);
#endif
}

void viewerPy::on3DMouseReleased()
{
	//active window?
	if (m_glWindow && m_glWindow->getPivotVisibility() == ccGLWindowInterface::PIVOT_SHOW_ON_MOVE)
	{
		//we have to hide the pivot symbol!
		m_glWindow->showPivotSymbol(false);
		m_glWindow->redraw();
	}
}

const ccHObject::Container& viewerPy::getSelectedEntities() const
{
    static ccHObject::Container Empty;
    return Empty;
}

void viewerPy::dispToConsole(QString message, ConsoleMessageLevel level)
{
    printf("%s\n", qPrintable(message));
}

ccHObject* viewerPy::dbRootObject()
{
    return m_glWindow->getSceneDB();
}

void viewerPy::redrawAll(bool only2D/*=false*/)
{
    m_glWindow->redraw(only2D);
}

void viewerPy::refreshAll(bool only2D/*=false*/)
{
    m_glWindow->refresh(only2D);
}

void viewerPy::enableAll()
{
    m_glWindow->asWidget()->setEnabled(true);
}

void viewerPy::disableAll()
{
    m_glWindow->asWidget()->setEnabled(false);
}

void viewerPy::disableAllBut(ccGLWindowInterface* win)
{
    if (win != m_glWindow)
    {
        m_glWindow->asWidget()->setEnabled(false);
    }
}

void viewerPy::setView(CC_VIEW_ORIENTATION view)
{
    m_glWindow->setView(view, true);
}

void viewerPy::setCustomView(const CCVector3d& forward, const CCVector3d& up)
{
	m_glWindow->setCustomView(forward, up);
}

void viewerPy::setCameraPos(const CCVector3d& P)
{
	m_glWindow->setCameraPos(P);
}

void viewerPy::toggleActiveWindowCustomLight()
{
    ui.actionToggleCustomLight->setChecked(!ui.actionToggleCustomLight->isChecked());
}

void viewerPy::toggleActiveWindowSunLight()
{
    ui.actionToggleSunLight->setChecked(!ui.actionToggleSunLight->isChecked());
}

void viewerPy::toggleActiveWindowCenteredPerspective()
{
    if (ui.actionSetCenteredPerspectiveView->isChecked())
    {
        ui.actionSetOrthoView->trigger();
    }
    else
    {
        ui.actionSetCenteredPerspectiveView->trigger();
    }
}

void viewerPy::toggleActiveWindowViewerBasedPerspective()
{
    if (ui.actionSetViewerPerspectiveView->isChecked())
    {
        ui.actionSetOrthoView->trigger();
    }
    else
    {
        ui.actionSetViewerPerspectiveView->trigger();
    }
}

void viewerPy::increasePointSize()
{
    m_glWindow->setPointSize(m_glWindow->getViewportParameters().defaultPointSize + 1);
    m_glWindow->redraw();
}

void viewerPy::decreasePointSize()
{
    m_glWindow->setPointSize(m_glWindow->getViewportParameters().defaultPointSize - 1);
    m_glWindow->redraw();
}

ccUniqueIDGenerator::Shared viewerPy::getUniqueIDGenerator()
{
    return ccObject::GetUniqueIDGenerator();
}

