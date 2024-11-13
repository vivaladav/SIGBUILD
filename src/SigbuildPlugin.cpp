#include "SigbuildPlugin.h"

#include "BuildData.h"
#include "DialogLastBuild.h"
#include "DialogSessionBuilds.h"
#include "OptionsPageMain.h"
#include "Settings.h"

#include <coreplugin/icore.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/projecttree.h>

#include <QAction>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDateTime>
#include <QIcon>
#include <QMainWindow>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QMenu>
#include <QSoundEffect>
#include <QSystemTrayIcon>
#include <QTimer>

#include <cmath>

namespace Sigbuild
{

// ==== CONSTRUCTOR / DESTRUCTOR ====

SigbuildPlugin::SigbuildPlugin()
    : mSettings(new Settings)
{
    const int ICON_SIZE = 16;

    mIconDialogStates.reserve(NUM_BUILD_STATES);

    int ind = static_cast<int>(BuildState::BUILDING);
    mIconStates[ind] = new QIcon(":/img/icon_building.png");
    mIconDialogStates.push_back(mIconStates[ind]->pixmap(ICON_SIZE, ICON_SIZE));

    ind = static_cast<int>(BuildState::FAILED);
    mIconStates[ind] = new QIcon(":/img/icon_fail.png");
    mIconDialogStates.push_back(mIconStates[ind]->pixmap(ICON_SIZE, ICON_SIZE));

    ind = static_cast<int>(BuildState::OK);
    mIconStates[ind] = new QIcon(":/img/icon.png");
    mIconDialogStates.push_back(mIconStates[ind]->pixmap(ICON_SIZE, ICON_SIZE));
}

SigbuildPlugin::~SigbuildPlugin()
{
    DestroySystrayIcon();

    for(int i = 0; i < NUM_BUILD_STATES; ++i)
        delete mIconStates[i];

    DestroySounds();

    const int NUM_DATA = mBuildsData.size();

    for(int i = 0;  i < NUM_DATA; ++i)
        delete mBuildsData[i];

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
    SetBuildState(BuildState::BUILDING);

    // only log first state start time
    if(0 == mTimeBuildStart)
    {
        mTimeBuildStart = QDateTime::currentMSecsSinceEpoch();

        ProjectExplorer::Node * node = ProjectExplorer::ProjectTree::currentNode();

        if(node)
        {
            // if there's a project node we are in a sub-project
            ProjectExplorer::ProjectNode * prjNode = node->parentProjectNode();

            if(prjNode)
                mCurrentProject = QString("%1").arg(prjNode->displayName());
            else
                mCurrentProject = pro->displayName();
        }
        else
            mCurrentProject = QString("???");

        // show build status with updates
        if(mTrayIcon)
            mTimerBuildUpdater->start();
    }
}

void SigbuildPlugin::OnBuildFinished(bool res)
{
    QMainWindow * window = qobject_cast<QMainWindow *>(Core::ICore::mainWindow());

    mLastBuildProject = mCurrentProject;

    // build time
    mTimeLastBuildStart = mTimeBuildStart;
    mTimeLastBuildEnd = QDateTime::currentMSecsSinceEpoch();
    const int BUILD_TIME_SECS = GetBuildTimeSecs(mTimeLastBuildEnd);
    const QString BUILD_TIME_STR = GetBuildTimeStr(BUILD_TIME_SECS);

    // reset build time
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
        mLastBuildState = BuildState::OK;

        if(SHOW_MSG)
        {
            mMsgNotification = QString("build succesful! \\o/\n\nBUILD TIME: %2").arg(BUILD_TIME_STR);
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
        mLastBuildState = BuildState::FAILED;

        if(SHOW_MSG)
        {
            mMsgNotification = QString("build failed! :-(\n\nBUILD TIME: %2").arg(BUILD_TIME_STR);
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
        // restore default tooltip
        mTimerBuildUpdater->stop();
        mTrayIcon->setToolTip("SIGBUILD");

        // enable last build menu entry after the first finished build
        if(!mActionShowLastBuild->isEnabled())
        {
            mActionShowLastBuild->setEnabled(true);
            mActionShowSessionBuilds->setEnabled(true);
        }
    }

    mBuildsData.push_back(new BuildData(mLastBuildProject, mTimeLastBuildStart, mTimeLastBuildEnd, mLastBuildState));
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

        UpdateSounds();

        // if(mSoundFail != nullptr && mSoundSuccess != nullptr)
        //     DestroySounds();

        // CreateSounds();
    }
    else
    {
        if(mSoundFail && mSoundSuccess)
            DestroySounds();
    }
}

void SigbuildPlugin::OnActionShowLastBuild()
{
    QMainWindow * window = qobject_cast<QMainWindow *>(Core::ICore::mainWindow());

    DialogLastBuild * d = new DialogLastBuild(	mLastBuildProject,
                                                QDateTime::fromMSecsSinceEpoch(mTimeLastBuildStart).toString("dd-MM-yyyy HH:mm:ss"),
                                                QDateTime::fromMSecsSinceEpoch(mTimeLastBuildEnd).toString("dd-MM-yyyy HH:mm:ss"),
                                                GetBuildTimeStr(GetBuildTimeSecs(mTimeLastBuildStart, mTimeLastBuildEnd)),
                                                mIconDialogStates[static_cast<int>(mLastBuildState)],
                                                window	);

    d->show();
    d->raise();
    // this to not let the user resize the dialog
    d->setFixedSize(d->size());

    // schedule self-destructionwhen closing
    connect(d, &QDialog::finished, d, &QDialog::deleteLater);
}

void SigbuildPlugin::OnActionShowSessionBuilds()
{
    QMainWindow * window = qobject_cast<QMainWindow *>(Core::ICore::mainWindow());

    DialogSessionBuilds * dialog = new DialogSessionBuilds(mBuildsData, mIconDialogStates, window);
    dialog->show();
    dialog->raise();

    // schedule self-destructionwhen closing
    connect(dialog, &QDialog::finished, dialog, &QDialog::deleteLater);
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

void SigbuildPlugin::OnBuildUpdate()
{
    if(!mTrayIcon)
        return ;

    const int BUILD_TIME_SECS = GetBuildTimeSecs(QDateTime::currentMSecsSinceEpoch());
    mTrayIcon->setToolTip(GetBuildTimeStr(BUILD_TIME_SECS));
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
    mActionShowLastBuild = new QAction("Last build");
    mActionShowLastBuild->setEnabled(false);
    mTrayMenu->addAction(mActionShowLastBuild);

    connect(mActionShowLastBuild, &QAction::triggered, this, &SigbuildPlugin::OnActionShowLastBuild);

    // SHOW SESSION BUILDS
    mActionShowSessionBuilds = new QAction("Session builds");
    mActionShowSessionBuilds->setEnabled(false);
    mTrayMenu->addAction(mActionShowSessionBuilds);

    connect(mActionShowSessionBuilds, &QAction::triggered, this, &SigbuildPlugin::OnActionShowSessionBuilds);

    // -----
    mTrayMenu->addSeparator();

    // TOGGLE SYSTAY NOTIFICATION
    mActionToggleNotifySystray = new QAction("Systray notification");
    mActionToggleNotifySystray->setCheckable(true);
    mActionToggleNotifySystray->setChecked(mSettings->IsSystrayNotificationEnabled());
    mTrayMenu->addAction(mActionToggleNotifySystray);

    connect(mActionToggleNotifySystray, &QAction::toggled, this, &SigbuildPlugin::OnActionToggleNotifySystray);

    // TOGGLE AUDIO NOTIFICATION
    mActionToggleNotifyAudio = new QAction("Audio notification");
    mActionToggleNotifyAudio->setCheckable(true);
    mActionToggleNotifyAudio->setChecked(mSettings->IsAudioEnabled());
    mTrayMenu->addAction(mActionToggleNotifyAudio);

    connect(mActionToggleNotifyAudio, &QAction::toggled, this, &SigbuildPlugin::OnActionToggleNotifyAudio);

    // -----
    mTrayMenu->addSeparator();

    // EXIT
    QAction * action = new QAction("Exit");
    mTrayMenu->addAction(action);

    connect(action, &QAction::triggered, Core::ICore::mainWindow(), &QMainWindow::close);

    // SHOW ICON
    mTrayIcon->show();

    // -- create build update timer --
    mTimerBuildUpdater = new QTimer;
    mTimerBuildUpdater->setInterval(1000);
    connect(mTimerBuildUpdater, &QTimer::timeout, this, &SigbuildPlugin::OnBuildUpdate);
}

void SigbuildPlugin::DestroySystrayIcon()
{
    mTimerBuildUpdater->stop();
    delete mTimerBuildUpdater;
    mTimerBuildUpdater = nullptr;

    delete mTrayMenu;
    mTrayMenu = nullptr;

    delete mTrayIcon;
    mTrayIcon = nullptr;
}

void SigbuildPlugin::CreateSounds()
{
    mSoundSuccess = new QSoundEffect;
    mSoundSuccess->setVolume(mSettings->GetAudioVolumeAsReal());

    connect(mSoundSuccess, &QSoundEffect::sourceChanged, this, [this]
    {
        qDebug() << "SigbuildPlugin::CreateSounds - SUCCESS SOURCE CHANGED: " << mSoundSuccess->source();
    });

    connect(mSoundSuccess, &QSoundEffect::statusChanged, this, [this]
    {
        qDebug() << "SigbuildPlugin::CreateSounds - SUCCESS SOUND STATUS CHANGED: " << mSoundSuccess->status();
        qDebug() << "SigbuildPlugin::CreateSounds - SUCCESS SOURCE: " << mSoundSuccess->source();
    });

    mSoundFail = new QSoundEffect;
    mSoundFail->setVolume(mSettings->GetAudioVolumeAsReal());

    connect(mSoundFail, &QSoundEffect::sourceChanged, this, [this]
    {
        qDebug() << "SigbuildPlugin::CreateSounds - FAIL SOURCE CHANGED: " << mSoundFail->source();
    });

    connect(mSoundFail, &QSoundEffect::statusChanged, this, [this]
    {
        qDebug() << "SigbuildPlugin::CreateSounds - FAIL SOUND STATUS CHANGED: " << mSoundFail->status();
        qDebug() << "SigbuildPlugin::CreateSounds - FAIL SOURCE: " << mSoundFail->source();
    });

    UpdateSounds();
}

void SigbuildPlugin::UpdateSounds()
{
    qDebug() << "----- SigbuildPlugin::UpdateSounds() - START -----";

    const QUrl DEF_SOUND_SUCCESS("qrc:/audio/success_short.wav");
    const QUrl DEF_SOUND_FAIL("qrc:/audio/fail_short.wav");

    if(mSettings->UseCustomSounds())
    {
        const QString successSound = mSettings->GetCustomSuccessSound();

        if(!successSound.isEmpty())
            mSoundSuccess->setSource(QUrl::fromLocalFile(successSound));
        else
            mSoundSuccess->setSource(DEF_SOUND_SUCCESS);

        const QString failSound = mSettings->GetCustomFailSound();

        if(!failSound.isEmpty())
            mSoundFail->setSource(QUrl::fromLocalFile(failSound));
        else
            mSoundFail->setSource(DEF_SOUND_FAIL);

        qDebug() << "SigbuildPlugin::UpdateSounds() - CUSTOM";
    }
    else
    {
        mSoundSuccess->setSource(DEF_SOUND_SUCCESS);
        mSoundFail->setSource(DEF_SOUND_FAIL);

        qDebug() << "SigbuildPlugin::UpdateSounds() - DEFAULT";
    }

    qDebug() << "SigbuildPlugin::UpdateSounds() - SUCCESS:" << mSoundSuccess->source();
    qDebug() << "SigbuildPlugin::UpdateSounds() - FAIL:" << mSoundFail->source();

    qDebug() << "----- SigbuildPlugin::UpdateSounds() - END -----";
}

void SigbuildPlugin::DestroySounds()
{
    qDebug() << "SigbuildPlugin::DestroySounds";

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

int SigbuildPlugin::GetBuildTimeSecs(qint64 nowMs) const
{
    return GetBuildTimeSecs(mTimeBuildStart, nowMs);
}

int SigbuildPlugin::GetBuildTimeSecs(qint64 startMs, qint64 endMs) const
{
    return roundf((endMs - startMs) / 1000.0f);
}

QString SigbuildPlugin::GetBuildTimeStr(const int buildTimeSecs) const
{
    QTime buildTime(0, 0, 0, 0);
    buildTime = buildTime.addSecs(buildTimeSecs);
    return buildTime.toString("hh:mm:ss");
}

} // namespace Sigbuild
