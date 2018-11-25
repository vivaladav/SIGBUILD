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
#include <QTimer>

namespace Sigbuild {

// ==== CONSTRUCTOR / DESTRUCTOR ====

SigbuildPlugin::SigbuildPlugin()
	: mSettings(new Settings)
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
	{
		mTimeBuildStart = QDateTime::currentMSecsSinceEpoch();

		// disable "last build" action while building
		if(mTrayIcon)
			mActionShowLastBuild->setEnabled(false);
	}
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
	const int NOTIFY_TIME_MS =	mSettings->GetSystrayNotificationTime() * 1000;

	const bool SHOW_MSG		=	mTrayIcon &&
								mSettings->IsSystrayEnabled() &&
								mSettings->IsSystrayNotificationEnabled() &&
								(mSettings->ShowSystrayNotificationWhenActive() || !window->isActiveWindow()) &&
								BUILD_TIME_SECS >= mSettings->GetSystrayMinBuildTime();

	const bool PLAY_AUDIO	=	mSettings->IsAudioEnabled() &&
								(mSettings->PlayAudioNotificationWhenActive() || !window->isActiveWindow()) &&
								BUILD_TIME_SECS >= mSettings->GetAudioMinBuildtime();

	if(res)
	{
		SetBuildState(BuildState::OK);

		if(SHOW_MSG)
		{
			mMsgNotification = QString("build succesful! \\o/\n\nBUILD TIME: %1").arg(BUILD_TIME_STR);
			mTrayIcon->showMessage("SIGBUILD", mMsgNotification, QSystemTrayIcon::Information, NOTIFY_TIME_MS);
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

		if(SHOW_MSG)
		{
			mMsgNotification = QString("build failed! :-(\n\nBUILD TIME: %1").arg(BUILD_TIME_STR);
			mTrayIcon->showMessage("SIGBUILD", mMsgNotification, QSystemTrayIcon::Critical, NOTIFY_TIME_MS);
		}

		if(mSoundFail && PLAY_AUDIO)
		{
			mSoundFail->setVolume(mSettings->GetAudioVolumeAsReal());
			mSoundFail->play();
		}
	}

	// re-enable "last build" action after showing notification or immediately if not showing them
	if(mTrayIcon)
	{
		if(mSettings->IsSystrayNotificationEnabled())
		{
			QTimer::singleShot(NOTIFY_TIME_MS, [this]
			{
				mActionShowLastBuild->setEnabled(true);
			});
		}
		else
			mActionShowLastBuild->setEnabled(true);
	}
}

void SigbuildPlugin::OnSettingsChanged()
{
	// -- SYSTRAY --
	if(mSettings->IsSystrayEnabled())
	{
		if(!mTrayIcon)
			CreateSystrayIcon();

		mActionToggleNotifySystray->setChecked(mSettings->IsSystrayNotificationEnabled());
	}
	else
	{
		if(mTrayIcon)
			DestroySystrayIcon();
	}

	// -- AUDIO --
	mActionToggleNotifyAudio->setChecked(mSettings->IsAudioEnabled());

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

void SigbuildPlugin::OnActionShowLastBuild()
{
	const QSystemTrayIcon::MessageIcon ICON =	BuildState::OK == mBuildState ?
												QSystemTrayIcon::Information :
												QSystemTrayIcon::Critical;
	const int NOTIFY_TIME_MS = mSettings->GetSystrayNotificationTime() * 1000;

	mTrayIcon->showMessage("SIGBUILD", mMsgNotification, ICON, NOTIFY_TIME_MS);
}

void SigbuildPlugin::OnActionToggleNotifySystray(bool checked)
{
	mSettings->SetSystrayNotificationEnabled(checked);

	mSettings->Save();

	OnSettingsChanged();
}

void SigbuildPlugin::OnActionToggleNotifyAudio(bool checked)
{
	mSettings->SetAudioEnabled(checked);

	mSettings->Save();

	OnSettingsChanged();
}

// ==== PRIVATE FUNCTIONS ====

void SigbuildPlugin::CreateSystrayIcon()
{
	// -- CREATE SYSTRAY ICON --
	mTrayIcon = new QSystemTrayIcon(*mIconStates[static_cast<int>(mBuildState)]);
	mTrayIcon->setToolTip("SIGBUILD");

	// -- CREATE CONTEXT MENU --
	mTrayMenu = new QMenu();
	mTrayIcon->setContextMenu(mTrayMenu);

	// SHOW LAST BUILD
	mActionShowLastBuild = new QAction("Last build", mTrayMenu);
	mActionShowLastBuild->setEnabled(false);
	mTrayMenu->addAction(mActionShowLastBuild);

	connect(mActionShowLastBuild, &QAction::triggered, this, &SigbuildPlugin::OnActionShowLastBuild);

	// -----
	mTrayMenu->addSeparator();

	// TOGGLE SYSTAY NOTIFICATION
	mActionToggleNotifySystray = new QAction("Systray notification", mTrayMenu);
	mActionToggleNotifySystray->setCheckable(true);
	mActionToggleNotifySystray->setChecked(mSettings->IsSystrayNotificationEnabled());
	mTrayMenu->addAction(mActionToggleNotifySystray);

	connect(mActionToggleNotifySystray, &QAction::toggled, this, &SigbuildPlugin::OnActionToggleNotifySystray);

	// TOGGLE AUDIO NOTIFICATION
	mActionToggleNotifyAudio = new QAction("Audio notification", mTrayMenu);
	mActionToggleNotifyAudio->setCheckable(true);
	mActionToggleNotifyAudio->setChecked(mSettings->IsAudioEnabled());
	mTrayMenu->addAction(mActionToggleNotifyAudio);

	connect(mActionToggleNotifyAudio, &QAction::toggled, this, &SigbuildPlugin::OnActionToggleNotifyAudio);

	// -----
	mTrayMenu->addSeparator();

	// EXIT
	QAction * action = new QAction("Exit", mTrayMenu);
	mTrayMenu->addAction(action);

	connect(action, &QAction::triggered, Core::ICore::mainWindow(), &QMainWindow::close);

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
