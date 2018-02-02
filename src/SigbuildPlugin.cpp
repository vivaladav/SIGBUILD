#include "SigbuildPlugin.h"

#include "OptionsPageMain.h"
#include "Settings.h"

#include <coreplugin/icore.h>
#include <coreplugin/mainwindow.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/project.h>

#include <QAction>
#include <QCoreApplication>
#include <QDateTime>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QSoundEffect>
#include <QSound>
#include <QSystemTrayIcon>

namespace Sigbuild {

SigbuildPlugin::SigbuildPlugin()
	: mSettings(new Settings)
{
}

SigbuildPlugin::~SigbuildPlugin()
{
	DestroySystrayIcon();

	DestroySounds();

	delete mSettings;
}

bool SigbuildPlugin::initialize(const QStringList & arguments, QString * errorString)
{
	Q_UNUSED(arguments)
	Q_UNUSED(errorString)

	// -- OPTIONS PAGE --
	mSettings->Load();

	mOptionsPage = new OptionsPageMain(mSettings);
	addAutoReleasedObject(mOptionsPage);

	connect(mOptionsPage, &OptionsPageMain::SettingsChanged, this, &SigbuildPlugin::OnSettingsChanged);

	// -- CREATE ICON --
	if(mSettings->IsSystrayEnabled())
		CreateSystrayIcon();

	// -- CREATE SFXs --
	if(mSettings->IsAudioEnabled())
		CreateSounds();

	// -- SIGNALS HANDLING --
	// build start
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildStateChanged,
			this, [this](ProjectExplorer::Project * pro)
	{
		Q_UNUSED(pro)

		// only log first state start time
		if(0 == mTimeBuildStart)
			mTimeBuildStart = QDateTime::currentMSecsSinceEpoch();
	});


	// build end
	connect(ProjectExplorer::BuildManager::instance(), &ProjectExplorer::BuildManager::buildQueueFinished,
			this, &SigbuildPlugin::OnBuildFinished);



	return true;
}

void SigbuildPlugin::extensionsInitialized()
{

}

ExtensionSystem::IPlugin::ShutdownFlag SigbuildPlugin::aboutToShutdown()
{
	return SynchronousShutdown;
}

void SigbuildPlugin::OnBuildFinished(bool res)
{
	QMainWindow * window = qobject_cast<QMainWindow *>(Core::ICore::mainWindow());

	// build time in seconds
	int buildTime = (QDateTime::currentMSecsSinceEpoch() - mTimeBuildStart) / 1000;
	mTimeBuildStart = 0;

	// notification time in milliseconds
	const int NOTIFY_TIME = mSettings->GetSystrayNotificationTime() * 1000;

	const bool SHOW_MSG		=	mSettings->IsSystrayEnabled() &&
								mSettings->IsSystrayNotificationEnabled() &&
								(mSettings->ShowSystrayNotificationWhenActive() || !window->isActiveWindow()) &&
								buildTime >= mSettings->GetSystrayMinBuildTime();

	const bool PLAY_AUDIO	=	mSettings->IsAudioEnabled() &&
								(mSettings->PlayAudioNotificationWhenActive() || !window->isActiveWindow()) &&
								buildTime >= mSettings->GetAudioMinBuildtime();

	if(res)
	{
		if(mTrayIcon && SHOW_MSG)
			mTrayIcon->showMessage("Qt Creator", "build succesful! \\o/", QSystemTrayIcon::Information, NOTIFY_TIME);

		if(mSoundSuccess && PLAY_AUDIO)
		{
			mSoundSuccess->setVolume(mSettings->GetAudioVolumeAsReal());
			mSoundSuccess->play();
		}
	}
	else
	{
		if(mTrayIcon && SHOW_MSG)
			mTrayIcon->showMessage("Qt Creator", "build failed! :-(", QSystemTrayIcon::Critical, NOTIFY_TIME);

		if(mSoundFail && PLAY_AUDIO)
		{
			mSoundFail->setVolume(mSettings->GetAudioVolumeAsReal());
			mSoundFail->play();
		}
	}
}

void SigbuildPlugin::OnSettingsChanged()
{
	// -- SYSTRAY --
	if(mSettings->IsSystrayEnabled())
	{
		if(!mTrayIcon)
			CreateSystrayIcon();
	}
	else
	{
		if(mTrayIcon)
			DestroySystrayIcon();
	}

	// -- AUDIO --
	if(mSettings->IsAudioEnabled())
	{
		if(!mSoundFail && !mSoundSuccess)
			CreateSounds();
	}
	else
	{
		if(mSoundFail && mSoundSuccess)
			DestroySounds();
	}
}

void SigbuildPlugin::CreateSystrayIcon()
{
	// -- CREATE SYSTRAY ICON --
	QIcon icon(":/img/icon.png");
	mTrayIcon = new QSystemTrayIcon(icon);

	// -- CREATE CONTEXT MENU --
	mTrayMenu = new QMenu();

	QAction * actionQuit = new QAction("E&xit", mTrayMenu);
	connect(actionQuit, &QAction::triggered, qApp, &QCoreApplication::quit);
	mTrayMenu->addAction(actionQuit);

	mTrayIcon->setContextMenu(mTrayMenu);

	// SHOW ICON
	mTrayIcon->show();
}

void SigbuildPlugin::DestroySystrayIcon()
{
	delete mTrayMenu;
	mTrayMenu = nullptr;

	delete mTrayIcon;
	mTrayIcon = nullptr;
}

void SigbuildPlugin::CreateSounds()
{
	mSoundSuccess = new QSoundEffect();
	mSoundSuccess->setVolume(mSettings->GetAudioVolumeAsReal());
	mSoundSuccess->setSource(QUrl("qrc:/audio/success_short.wav"));

	mSoundFail = new QSoundEffect();
	mSoundFail->setVolume(mSettings->GetAudioVolumeAsReal());
	mSoundFail->setSource(QUrl("qrc:/audio/fail_short.wav"));
}

void SigbuildPlugin::DestroySounds()
{
	delete mSoundSuccess;
	mSoundSuccess = nullptr;

	delete mSoundFail;
	mSoundFail = nullptr;
}

} // namespace Sigbuild
