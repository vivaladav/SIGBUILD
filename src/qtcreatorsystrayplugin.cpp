#include "qtcreatorsystrayplugin.h"

#include <coreplugin/icore.h>
#include <coreplugin/mainwindow.h>
#include <coreplugin/progressmanager/progressmanager.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/task.h>

#include <QAction>
#include <QCoreApplication>
#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>

#include <QDebug>
#include <QStatusBar>

namespace QtCreatorSysTray {
namespace Internal {

QtCreatorSysTrayPlugin::QtCreatorSysTrayPlugin()
{
	// -- CREATE ICON --
	QIcon icon(":/img/qtcreator-logo-64.png");
	mTrayIcon = new QSystemTrayIcon(icon, Core::ICore::mainWindow());

	// -- CREATE CONTEXT MENU --
	mTrayMenu = new QMenu();

	QAction * actionQuit = new QAction("E&xit");
	connect(actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
	mTrayMenu->addAction(actionQuit);

	mTrayIcon->setContextMenu(mTrayMenu);

	connect(mTrayIcon, &QSystemTrayIcon::messageClicked, this, &QtCreatorSysTrayPlugin::onMessageClicked);

	// SHOW
	mTrayIcon->show();
}

QtCreatorSysTrayPlugin::~QtCreatorSysTrayPlugin()
{
	delete mTrayMenu;
}

bool QtCreatorSysTrayPlugin::initialize(const QStringList & arguments, QString * errorString)
{
	Q_UNUSED(arguments)
	Q_UNUSED(errorString)

	// get notified of builds
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildQueueFinished, [=](bool res)
	{
		const int TIME = 2500;

		if(res)
			mTrayIcon->showMessage("Qt Creator", "build succesful! \\o/", QSystemTrayIcon::Information, TIME);
		else
			mTrayIcon->showMessage("Qt Creator", "build failed! :-(", QSystemTrayIcon::Critical, TIME);
	});


	/*
	// get notified of builds
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::taskAdded, [=](const ProjectExplorer::Task & task)
	{
		qDebug() << "TASK " << task.description;
	});
	*/

	// get notified of builds
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildStateChanged, [=](ProjectExplorer::Project * pro)
	{
		qDebug() << "BUILD STATE CHANGED";
		ProjectExplorer::BuildConfiguration * buildConf = pro->activeTarget()->activeBuildConfiguration();
		qDebug() << "buildconf: " <<  buildConf;

		QList<Core::Id> knownStepList = buildConf->knownStepLists();
		qDebug() << "knownStepList: " << knownStepList;

		qDebug() << "";

		for(Core::Id cid : knownStepList)
		{
			ProjectExplorer::BuildStepList * stepList = buildConf->stepList(cid);
			qDebug() << "stepList: " << stepList->displayName();

			QList<ProjectExplorer::BuildStep *> steps = stepList->steps();

			for(ProjectExplorer::BuildStep * step : steps)
				qDebug() << step->displayName() << " - RUNNING " << ProjectExplorer::BuildManager::instance()->isBuilding(step);
			qDebug() << "";
		}

		qDebug() << "====================";
	});

	/*
	// task started
	connect(Core::ProgressManager::instance(), &Core::ProgressManager::taskStarted, [=](Core::Id type)
	{
		qDebug() << "taskStarted: " << type;
	});

	// task started
	connect(Core::ProgressManager::instance(), &Core::ProgressManager::allTasksFinished, [=](Core::Id type)
	{
		qDebug() << "allTasksFinished: " << type;
	});
	*/

	return true;
}

void QtCreatorSysTrayPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag QtCreatorSysTrayPlugin::aboutToShutdown()
{
	return SynchronousShutdown;
}

void QtCreatorSysTrayPlugin::onMessageClicked()
{
	qDebug() << "QtCreatorSysTrayPlugin::onMessageClicked()";

	//(Core::ICore::mainWindow()->isVisible())
	Core::ICore::mainWindow()->raise();
}

} // namespace Internal
} // namespace QtCreatorSysTray
