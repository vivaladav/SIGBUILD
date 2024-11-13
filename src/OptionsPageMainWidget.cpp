#include "OptionsPageMainWidget.h"

#include "Settings.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>

namespace Sigbuild
{

OptionsPageMainWidget::OptionsPageMainWidget(const Settings * settings)
{
    QVBoxLayout * layout = new QVBoxLayout(this);

    // == SYSTRAY BOX ==
    QGroupBox * box = new QGroupBox(tr("Systray"), this);
    layout->addWidget(box);

    QVBoxLayout * layoutBox = new QVBoxLayout(box);

    const bool SYSTRAY_ENABLED			= settings->IsSystrayEnabled();
    const bool SYSTRAY_NOTIFY_ENABLED	= settings->IsSystrayNotificationEnabled();
    const bool SYSTRAY_OPT_STATUS		= SYSTRAY_ENABLED && SYSTRAY_NOTIFY_ENABLED;

    // -- systray on/off --
    mSystrayEnabled = new QCheckBox(tr("Enable systray icon"), box);
    mSystrayEnabled->setChecked(SYSTRAY_ENABLED);
    layoutBox->addWidget(mSystrayEnabled);

    connect(mSystrayEnabled, &QCheckBox::stateChanged, this, &OptionsPageMainWidget::OnSystrayStateChanged);

    // -- systray notifications on/off
    mSystrayNotifyEnabled = new QCheckBox(tr("Enable systray notifications"), box);
    mSystrayNotifyEnabled->setChecked(SYSTRAY_NOTIFY_ENABLED);
    mSystrayNotifyEnabled->setEnabled(SYSTRAY_ENABLED);
    layoutBox->addWidget(mSystrayNotifyEnabled);

    connect(mSystrayNotifyEnabled, &QCheckBox::stateChanged, this, &OptionsPageMainWidget::OnSystrayStateChanged);

    // -- systray notifications when active --
    mSystrayNotifyWhenActive = new QCheckBox(tr("Show systray notifications when Qt Creator is active"), box);
    mSystrayNotifyWhenActive->setChecked(settings->ShowSystrayNotificationWhenActive());
    mSystrayNotifyWhenActive->setEnabled(SYSTRAY_OPT_STATUS);
    layoutBox->addWidget(mSystrayNotifyWhenActive);

    // -- systray notification min build time --
    QHBoxLayout * layoutRow = new QHBoxLayout;
    layoutBox->addLayout(layoutRow);

    mSystrayMinBuildTime = new QSpinBox(box);
    mSystrayMinBuildTime->setMinimum(Settings::SYSTRAY_BUILD_TIME_MIN);
    mSystrayMinBuildTime->setMaximum(Settings::SYSTRAY_BUILD_TIME_MAX);
    mSystrayMinBuildTime->setValue(settings->GetSystrayMinBuildTime());
    mSystrayMinBuildTime->setSingleStep(1);
    mSystrayMinBuildTime->setMaximumWidth(50);
    mSystrayMinBuildTime->setEnabled(SYSTRAY_OPT_STATUS);
    layoutRow->addWidget(mSystrayMinBuildTime);

    mSystrayMinBuildTimeLabel = new QLabel(tr("Minimum build time (in seconds) to display a systray notification"), box);
    mSystrayMinBuildTimeLabel->setEnabled(SYSTRAY_ENABLED && SYSTRAY_NOTIFY_ENABLED);
    layoutRow->addWidget(mSystrayMinBuildTimeLabel);

    // -- systray notification length time --
    layoutRow = new QHBoxLayout;
    layoutBox->addLayout(layoutRow);

    mSystrayNotifyTime = new QSpinBox(box);
    mSystrayNotifyTime->setMinimum(Settings::SYSTRAY_NOTIFICATION_TIME_MIN);
    mSystrayNotifyTime->setMaximum(Settings::SYSTRAY_NOTIFICATION_TIME_MAX);
    mSystrayNotifyTime->setValue(settings->GetSystrayNotificationTime());
    mSystrayNotifyTime->setSingleStep(1);
    mSystrayNotifyTime->setMaximumWidth(50);
    mSystrayNotifyTime->setEnabled(SYSTRAY_OPT_STATUS);
    layoutRow->addWidget(mSystrayNotifyTime);

    mSystrayNotifyTimeLabel = new QLabel(tr("Time (in seconds) the systray notification will be visible"), box);
    mSystrayNotifyTimeLabel->setEnabled(SYSTRAY_ENABLED && SYSTRAY_NOTIFY_ENABLED);
    layoutRow->addWidget(mSystrayNotifyTimeLabel);

    // == AUDIO BOX ==
    box = new QGroupBox(tr("Audio"), this);
    layout->addWidget(box);

    layoutBox = new QVBoxLayout(box);

    const bool audioEnabled = settings->IsAudioEnabled();

    // -- audio on/off --
    mAudioEnabled = new QCheckBox(tr("Enable audio notifications"), box);
    mAudioEnabled->setChecked(audioEnabled);
    layoutBox->addWidget(mAudioEnabled);

    connect(mAudioEnabled, &QCheckBox::stateChanged, this, &OptionsPageMainWidget::OnAudioStateChanged);

    // -- audio when open --
    mAudioNotifyWhenActive = new QCheckBox(tr("Play audio notifications when Qt Creator is active"), box);
    mAudioNotifyWhenActive->setChecked(settings->PlayAudioNotificationWhenActive());
    mAudioNotifyWhenActive->setEnabled(audioEnabled);
    layoutBox->addWidget(mAudioNotifyWhenActive);

    // -- audio custom sounds --
    mAudioCustomSounds = new QCheckBox(tr("Use custom sounds"), box);
    mAudioCustomSounds->setChecked(settings->UseCustomSounds());
    mAudioCustomSounds->setEnabled(audioEnabled);
    layoutBox->addWidget(mAudioCustomSounds);

    // success sound
    const int MIN_COL0_W = 200;
    auto layoutGridRow = new QGridLayout;
    layoutBox->addLayout(layoutGridRow);
    layoutGridRow->setEnabled(false);
    layoutGridRow->setColumnMinimumWidth(0, MIN_COL0_W);

    mCustomSoundSuccessLabel = new QLabel(tr("Success sound:"));
    layoutGridRow->addWidget(mCustomSoundSuccessLabel, 0, 0);
    mCustomSoundSuccessLine = new QLineEdit;
    mCustomSoundSuccessLine->setReadOnly(true);
    layoutGridRow->addWidget(mCustomSoundSuccessLine, 0, 1);
    mCustomSoundSuccessButton = new QPushButton(tr("Select"));
    layoutGridRow->addWidget(mCustomSoundSuccessButton, 0, 2);

    connect(mCustomSoundSuccessButton, &QPushButton::clicked, this, [this]
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("select a success sound"),
                                                        QString(), tr("Audio (*.ogg *.mp3 *.wav)"));
        mCustomSoundSuccessLine->setText(fileName);
    });

    // fail sound
    layoutGridRow = new QGridLayout;
    layoutBox->addLayout(layoutGridRow);
    layoutGridRow->setEnabled(false);
    layoutGridRow->setColumnMinimumWidth(0, MIN_COL0_W);

    mCustomSoundFailLabel = new QLabel(tr("Fail sound:"));
    layoutGridRow->addWidget(mCustomSoundFailLabel, 0, 0);
    mCustomSoundFailLine = new QLineEdit;
    mCustomSoundFailLine->setReadOnly(true);
    layoutGridRow->addWidget(mCustomSoundFailLine, 0, 1);
    mCustomSoundFailButton = new QPushButton(tr("Select"));
    layoutGridRow->addWidget(mCustomSoundFailButton, 0, 2);

    SetCustomAudioControlsVisible(false);

    connect(mAudioCustomSounds, &QCheckBox::stateChanged,
            this, &OptionsPageMainWidget::SetCustomAudioControlsVisible);


    // -- audio volume --
    layoutRow = new QHBoxLayout;
    layoutBox->addLayout(layoutRow);

    mAudioVolume = new QSpinBox(box);
    mAudioVolume->setMinimum(Settings::AUDIO_VOL_MIN);
    mAudioVolume->setMaximum(Settings::AUDIO_VOL_MAX);
    mAudioVolume->setValue(settings->GetAudioVolume());
    mAudioVolume->setSingleStep(10);
    mAudioVolume->setMaximumWidth(50);
    mAudioVolume->setEnabled(audioEnabled);
    layoutRow->addWidget(mAudioVolume);

    mAudioVolumeLabel = new QLabel(tr("Notification volume"), box);
    mAudioVolumeLabel->setEnabled(audioEnabled);
    layoutRow->addWidget(mAudioVolumeLabel);

    // -- systray notification min build time --
    layoutRow = new QHBoxLayout;
    layoutBox->addLayout(layoutRow);

    mAudioMinBuildTime = new QSpinBox(box);
    mAudioMinBuildTime->setMinimum(Settings::AUDIO_BUILD_TIME_MIN);
    mAudioMinBuildTime->setMaximum(Settings::AUDIO_BUILD_TIME_MAX);
    mAudioMinBuildTime->setValue(settings->GetAudioMinBuildtime());
    mAudioMinBuildTime->setSingleStep(1);
    mAudioMinBuildTime->setMaximumWidth(50);
    mAudioMinBuildTime->setEnabled(audioEnabled);
    layoutRow->addWidget(mAudioMinBuildTime);

    mAudioMinBuildTimeLabel = new QLabel(tr("Minimum build time (in seconds) to play an audio notification"), box);
    mAudioMinBuildTimeLabel->setEnabled(audioEnabled);
    layoutRow->addWidget(mAudioMinBuildTimeLabel);

    // == VERTICAL SPACER ==`
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

Settings OptionsPageMainWidget::GenerateSettings() const
{
    Settings settings;

    // -- SYSTRAY --
    settings.SetSystrayEnabled(mSystrayEnabled->isChecked());
    settings.SetSystrayNotificationEnabled(mSystrayNotifyEnabled->isChecked());
    settings.SetSystrayNotificationWhenActive(mSystrayNotifyWhenActive->isChecked());
    settings.SetSystrayMinBuildTime(mSystrayMinBuildTime->value());
    settings.SetSystrayNotificationTime(mSystrayNotifyTime->value());

    // -- AUDIO --
    settings.SetAudioEnabled(mAudioEnabled->isChecked());
    settings.SetAudioNotificationWhenActive(mAudioNotifyWhenActive->isChecked());
    settings.SetUseCustomSounds(mAudioCustomSounds->isChecked());
    settings.SetAudioVolume(mAudioVolume->value());
    settings.SetAudioMinBuildTime(mAudioMinBuildTime->value());

    return settings;
}

// ===== PRIVATE SIGNALS =====
void OptionsPageMainWidget::OnSystrayStateChanged()
{
    const bool SYSTRAY_ENABLED          = mSystrayEnabled->isChecked();
    const bool SYSTRAY_NOTIFY_ENABLED   = mSystrayNotifyEnabled->isChecked();
    const bool SYSTRAY_OPT_STATUS       = SYSTRAY_ENABLED && SYSTRAY_NOTIFY_ENABLED;

    mSystrayNotifyEnabled->setEnabled(SYSTRAY_ENABLED);
    mSystrayNotifyWhenActive->setEnabled(SYSTRAY_OPT_STATUS);
    mSystrayMinBuildTime->setEnabled(SYSTRAY_OPT_STATUS);
    mSystrayMinBuildTimeLabel->setEnabled(SYSTRAY_OPT_STATUS);
    mSystrayNotifyTime->setEnabled(SYSTRAY_OPT_STATUS);
    mSystrayNotifyTimeLabel->setEnabled(SYSTRAY_OPT_STATUS);
}

void OptionsPageMainWidget::OnAudioStateChanged()
{
    const bool STATUS = mAudioEnabled->isChecked();

    mAudioNotifyWhenActive->setEnabled(STATUS);
    mAudioCustomSounds->setEnabled(STATUS);
    mAudioVolume->setEnabled(STATUS);
    mAudioVolumeLabel->setEnabled(STATUS);
    mAudioMinBuildTime->setEnabled(STATUS);
    mAudioMinBuildTimeLabel->setEnabled(STATUS);

    mCustomSoundSuccessLabel->setEnabled(STATUS);
    mCustomSoundSuccessLine->setEnabled(STATUS);
    mCustomSoundFailLabel->setEnabled(STATUS);
    mCustomSoundFailLine->setEnabled(STATUS);
}

void OptionsPageMainWidget::SetCustomAudioControlsVisible(bool visible)
{
    mCustomSoundSuccessLabel->setVisible(visible);
    mCustomSoundSuccessLine->setVisible(visible);
    mCustomSoundSuccessButton->setVisible(visible);
    mCustomSoundFailLabel->setVisible(visible);
    mCustomSoundFailLine->setVisible(visible);
    mCustomSoundFailButton->setVisible(visible);
}

} // namespace Sigbuild
