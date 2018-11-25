#include "OptionsPageMainWidget.h"

#include "Settings.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
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

	const bool AUDIO_STATUS = settings->IsAudioEnabled();

	// -- audio on/off --
	mAudioEnabled = new QCheckBox(tr("Enable audio notifications"), box);
	mAudioEnabled->setChecked(AUDIO_STATUS);
	layoutBox->addWidget(mAudioEnabled);

	connect(mAudioEnabled, &QCheckBox::stateChanged, this, &OptionsPageMainWidget::OnAudioStateChanged);

	// -- audio when open --
	mAudioNotifyWhenActive = new QCheckBox(tr("Play audio notifications when Qt Creator is active"), box);
	mAudioNotifyWhenActive->setChecked(settings->PlayAudioNotificationWhenActive());
	mAudioNotifyWhenActive->setEnabled(AUDIO_STATUS);
	layoutBox->addWidget(mAudioNotifyWhenActive);

	// -- audio volume --
	layoutRow = new QHBoxLayout;
	layoutBox->addLayout(layoutRow);

	mAudioVolume = new QSpinBox(box);
	mAudioVolume->setMinimum(Settings::AUDIO_VOL_MIN);
	mAudioVolume->setMaximum(Settings::AUDIO_VOL_MAX);
	mAudioVolume->setValue(settings->GetAudioVolume());
	mAudioVolume->setSingleStep(10);
	mAudioVolume->setMaximumWidth(50);
	mAudioVolume->setEnabled(AUDIO_STATUS);
	layoutRow->addWidget(mAudioVolume);

	mAudioVolumeLabel = new QLabel(tr("Notification volume"), box);
	mAudioVolumeLabel->setEnabled(AUDIO_STATUS);
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
	mAudioMinBuildTime->setEnabled(AUDIO_STATUS);
	layoutRow->addWidget(mAudioMinBuildTime);

	mAudioMinBuildTimeLabel = new QLabel(tr("Minimum build time (in seconds) to play an audio notification"), box);
	mAudioMinBuildTimeLabel->setEnabled(AUDIO_STATUS);
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
	settings.SetAudioVolume(mAudioVolume->value());
	settings.SetAudioMinBuildTime(mAudioMinBuildTime->value());

	return settings;
}

// ===== PRIVATE SIGNALS =====
void OptionsPageMainWidget::OnSystrayStateChanged()
{
	const bool SYSTRAY_ENABLED			= mSystrayEnabled->isChecked();
	const bool SYSTRAY_NOTIFY_ENABLED	= mSystrayNotifyEnabled->isChecked();
	const bool SYSTRAY_OPT_STATUS		= SYSTRAY_ENABLED && SYSTRAY_NOTIFY_ENABLED;

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
	mAudioVolume->setEnabled(STATUS);
	mAudioVolumeLabel->setEnabled(STATUS);
	mAudioMinBuildTime->setEnabled(STATUS);
	mAudioMinBuildTimeLabel->setEnabled(STATUS);
}

} // namespace Sigbuild
