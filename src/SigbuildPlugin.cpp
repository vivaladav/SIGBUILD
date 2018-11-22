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

// ==== CONSTRUCTOR / DESTRUCTOR ====

SigbuildPlugin::SigbuildPlugin()
	: mSettings(new Settings)
	, mBuildState(BuildState::OK)
{
	mIconStates[static_cast<int>(BuildState::BUILDING)] = new QIcon(":/img/icon_building.png");
	mIconStates[static_cast<int>(BuildState::FAILED)] = new QIcon(":/img/icon_fail.png");
	mIconStates[static_cast<int>(BuildState::OK)] = new QIcon(":/img/icon.png");
}

SigbuildPlugin::~SigbuildPlugin()
{
	DestroySystrayIcon();

	for(int i = 0; i < NUM_BUILD_STATES; ++i)
		delete mIconStates[i];

	DestroySounds();

	delete mSettings;
}

// ==== PUBLIC FUNCTIONS ====

bool SigbuildPlugin::initialize(const QStringList & arguments, QString * errorString)
{
	Q_UNUSED(arguments)
	Q_UNUSED(errorString)

	// -- OPTIONS PAGE --
	mSettings->Load();

	mOptionsPage = new OptionsPageMain(mSettings, this);

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
			this, &SigbuildPlugin::OnBuildStateChanged);

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

// ==== PRIVATE SLOTS ====
void SigbuildPlugin::OnBuildStateChanged(ProjectExplorer::Project * pro)
{
	Q_UNUSED(pro)

	SetBuildState(BuildState::BUILDING);

	// only log first state start time
	if(0 == mTimeBuildStart)
		mTimeBuildStart = QDateTime::currentMSecsSinceEpoch();
}

void SigbuildPlugin::OnBuildFinished(bool res)
{
	QMainWindow * window = qobject_cast<QMainWindow *>(Core::ICore::mainWindow());

	// build time
	const int BUILD_TIME_SECS = roundf((QDateTime::currentMSecsSinceEpoch() - mTimeBuildStart) / 1000.0f);
	QTime buildTime(0, 0, 0, 0);
	buildTime = buildTime.addSecs(BUILD_TIME_SECS);
	const QString BUILD_TIME_STR = buildTime.toString("hh:mm:ss");

	// reset build timer
	mTimeBuildStart = 0;

	// notification time in milliseconds
	const int NOTIFY_TIME = mSettings->GetSystrayNotificationTime() * 1000;

	const bool SHOW_MSG		=	mSettings->IsSystrayEnabled() &&
								mSettings->IsSystrayNotificationEnabled() &&
								(mSettings->ShowSystrayNotificationWhenActive() || !window->isActiveWindow()) &&
								BUILD_TIME_SECS >= mSettings->GetSystrayMinBuildTime();

	const bool PLAY_AUDIO	=	mSettings->IsAudioEnabled() &&
								(mSettings->PlayAudioNotificationWhenActive() || !window->isActiveWindow()) &&
								BUILD_TIME_SECS >= mSettings->GetAudioMinBuildtime();

	if(res)
	{
		SetBuildState(BuildState::OK);

		if(mTrayIcon && SHOW_MSG)
		{
			const QString MSG = QString("build succesful! \\o/\n\nBUILD TIME: %1").arg(BUILD_TIME_STR);
			mTrayIcon->showMessage("SIGBUILD", MSG, QSystemTrayIcon::Information, NOTIFY_TIME);
		}

		if(mSoundSuccess && PLAY_AUDIO)
		{
			mSoundSuccess->setVolume(mSettings->GetAudioVolumeAsReal());
			mSoundSuccess->play();
		}
	}
	else
	{
		SetBuildState(BuildState::FAILED);

		if(mTrayIcon && SHOW_MSG)
		{
			const QString MSG = QString("build failed! :-(\n\nBUILD TIME: %1").arg(BUILD_TIME_STR);
			mTrayIcon->showMessage("SIGBUILD", MSG, QSystemTrayIcon::Critical, NOTIFY_TIME);
		}

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

// ==== PRIVATE FUNCTIONS ====

void SigbuildPlugin::CreateSystrayIcon()
{
	// -- CREATE SYSTRAY ICON --
	mTrayIcon = new QSystemTrayIcon(*mIconStates[static_cast<int>(mBuildState)]);
	mTrayIcon->setToolTip("SIGBUILD");

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

void SigbuildPlugin::SetBuildState(BuildState state)
{
	if(mTrayIcon && state != mBuildState)
		mTrayIcon->setIcon(*mIconStates[static_cast<int>(state)]);

	mBuildState = state;
}

} // namespace Sigbuild
