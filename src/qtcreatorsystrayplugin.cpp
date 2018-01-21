#include "qtcreatorsystrayplugin.h"

#include "OptionsPageMain.h"

#include <coreplugin/icore.h>
#include <coreplugin/mainwindow.h>
#include <projectexplorer/buildmanager.h>

#include <QAction>
#include <QCoreApplication>
#include <QIcon>
#include <QMenu>
#include <QSoundEffect>
#include <QSound>
#include <QSystemTrayIcon>

namespace QtCreatorSysTray {

QtCreatorSysTrayPlugin::QtCreatorSysTrayPlugin()
{
}

QtCreatorSysTrayPlugin::~QtCreatorSysTrayPlugin()
{
	delete mTrayMenu;
	delete mTrayIcon;

	delete mSoundSuccess;
	delete mSoundFail;
}

bool QtCreatorSysTrayPlugin::initialize(const QStringList & arguments, QString * errorString)
{
	Q_UNUSED(arguments)
	Q_UNUSED(errorString)

	// -- OPTIONS PAGE --
	mOptionsPage = new OptionsPageMain();
	addAutoReleasedObject(mOptionsPage);

	// -- CREATE ICON --
	QIcon icon(":/img/qtcreator-logo-64.png");
	mTrayIcon = new QSystemTrayIcon(icon);

	// -- CREATE CONTEXT MENU --
	mTrayMenu = new QMenu();

	QAction * actionQuit = new QAction("E&xit", mTrayMenu);
	connect(actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
	mTrayMenu->addAction(actionQuit);

	mTrayIcon->setContextMenu(mTrayMenu);

	// -- CREATE SFXs --
	mSoundSuccess = new QSoundEffect();
	mSoundSuccess->setSource(QUrl("qrc:/audio/success_short.wav"));

	mSoundFail = new QSoundEffect();
	mSoundFail->setSource(QUrl("qrc:/audio/fail_short.wav"));

	// get notified of builds
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildQueueFinished, this, [=](bool res)
	{
		if(res)
		{
			mTrayIcon->showMessage("Qt Creator", "build succesful! \\o/", QSystemTrayIcon::Information, mTimeNotification);

			mSoundSuccess->play();
		}
		else
		{
			mTrayIcon->showMessage("Qt Creator", "build failed! :-(", QSystemTrayIcon::Critical, mTimeNotification);

			mSoundFail->play();
		}
	});

	// SHOW ICON
	mTrayIcon->show();

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
