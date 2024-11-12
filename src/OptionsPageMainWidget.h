#pragma once

#include <QWidget>

class QCheckBox;
class QLabel;
class QLineEdit;
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
    void SetCustomAudioControlsVisible(bool visible);

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
    QCheckBox * mAudioCustomSounds = nullptr;
    QLabel * mCustomSoundSuccessLabel = nullptr;
    QLineEdit * mCustomSoundSuccessLine = nullptr;
    QLabel * mCustomSoundFailLabel = nullptr;
    QLineEdit * mCustomSoundFailLine = nullptr;
    QSpinBox * mAudioVolume = nullptr;
    QLabel * mAudioVolumeLabel = nullptr;
    QSpinBox * mAudioMinBuildTime = nullptr;
    QLabel * mAudioMinBuildTimeLabel = nullptr;
};

} // namespace Sigbuild
