#pragma once

#include <QWidget>

class QCheckBox;
class QLabel;
class QSpinBox;

namespace Sigbuild
{

class Settings;

class OptionsPageMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsPageMainWidget(const Settings * settings);

    Settings GenerateSettings() const;

private slots:
    void OnSystrayStateChanged();
    void OnAudioStateChanged();

private:
    // -- SYSTRAY BOX --
    QCheckBox * mSystrayEnabled = nullptr;
    QCheckBox * mSystrayNotifyEnabled = nullptr;
    QCheckBox * mSystrayNotifyWhenActive = nullptr;
    QSpinBox * mSystrayMinBuildTime = nullptr;
    QLabel * mSystrayMinBuildTimeLabel = nullptr;
    QSpinBox * mSystrayNotifyTime = nullptr;
    QLabel * mSystrayNotifyTimeLabel = nullptr;

    // -- AUDIO BOX --
    QCheckBox * mAudioEnabled = nullptr;
    QCheckBox * mAudioNotifyWhenActive = nullptr;
    QSpinBox * mAudioVolume = nullptr;
    QLabel * mAudioVolumeLabel = nullptr;
    QSpinBox * mAudioMinBuildTime = nullptr;
    QLabel * mAudioMinBuildTimeLabel = nullptr;
};

} // namespace Sigbuild
