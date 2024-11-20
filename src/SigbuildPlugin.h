#pragma once

#include "SigbuildDefines.h"

#include <extensionsystem/iplugin.h>

#include <QPixmap>
#include <QVector>

class QAction;
class QIcon;
class QMenu;
class QPixmap;
class QSoundEffect;
class QSystemTrayIcon;
class QTimer;

namespace ProjectExplorer { class Project; }

namespace Sigbuild
{

class BuildData;
class OptionsPageMain;
class Settings;

class SigbuildPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "SIGBUILD.json")

public:
    SigbuildPlugin();
    ~SigbuildPlugin();

    bool initialize(const QStringList & arguments, QString * errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private slots:
    void OnBuildStateChanged(ProjectExplorer::Project * pro);
    void OnBuildFinished(bool res);

    void OnSettingsChanged();

    void OnActionShowLastBuild();
    void OnActionShowSessionBuilds();
    void OnActionToggleNotifySystray(bool checked);
    void OnActionToggleNotifyAudio(bool checked);

    void OnBuildUpdate();

private:
    void CreateSystrayIcon();
    void DestroySystrayIcon();

    void CreateSounds();
    void UpdateSounds();
    void DestroySounds();

    void SetBuildState(BuildState state);

    int GetBuildTimeSecs(qint64 nowMs) const;
    int GetBuildTimeSecs(qint64 startMs, qint64 endMs) const;
    QString GetBuildTimeStr(const int buildTimeSecs) const;

private:
    QVector<BuildData *> mBuildsData;

    QIcon * mIconStates[NUM_BUILD_STATES];
    QVector<QPixmap> mIconDialogStates;

    Settings * mSettings = nullptr;

    OptionsPageMain * mOptionsPage = nullptr;

    QMenu * mTrayMenu = nullptr;
    QAction * mActionShowLastBuild = nullptr;
    QAction * mActionShowSessionBuilds = nullptr;
    QAction * mActionToggleNotifySystray = nullptr;
    QAction * mActionToggleNotifyAudio = nullptr;
    QSystemTrayIcon * mTrayIcon  = nullptr;

    QSoundEffect * mSoundSuccess = nullptr;
    QSoundEffect * mSoundFail = nullptr;

    quint64 mTimeBuildStart = 0;
    quint64 mTimeLastBuildStart = 0;
    quint64 mTimeLastBuildEnd = 0;

    BuildState mBuildState = BuildState::OK;
    BuildState mLastBuildState = BuildState::OK;

    QString mMsgNotification;
    QString mCurrentProject;
    QString mLastBuildProject;

    QTimer * mTimerBuildUpdater = nullptr;
};

} // namespace Sigbuild
