#include "qtcreatorsystrayplugin.h"

#include <coreplugin/icore.h>
#include <coreplugin/mainwindow.h>
#include <projectexplorer/buildmanager.h>

#include <QAction>
#include <QCoreApplication>
#include <QIcon>
#include <QMenu>
#include <QSystemTrayIcon>

namespace QtCreatorSysTray {

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
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildQueueFinished, this, [=](bool res)
	{
		if(res)
			mTrayIcon->showMessage("Qt Creator", "build succesful! \\o/", QSystemTrayIcon::Information, mTimeNotification);
		else
			mTrayIcon->showMessage("Qt Creator", "build failed! :-(", QSystemTrayIcon::Critical, mTimeNotification);
	});

	return true;
}

void QtCreatorSysTrayPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag QtCreatorSysTrayPlugin::aboutToShutdown()
{
	return SynchronousShutdown;
}

} // namespace QtCreatorSysTray
