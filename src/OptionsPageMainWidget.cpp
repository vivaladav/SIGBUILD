#include "OptionsPageMainWidget.h"

#include "Settings.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>

namespace QtCreatorSysTray
{

OptionsPageMainWidget::OptionsPageMainWidget(const Settings * settings)
{
	QVBoxLayout * layout = new QVBoxLayout(this);

	// == SYSTRAY BOX ==
	QGroupBox * box = new QGroupBox(tr("Systray"), this);
	layout->addWidget(box);

	QVBoxLayout * layoutBox = new QVBoxLayout(box);

	// -- systray on/off --
	mSystrayEnabled = new QCheckBox(tr("Enable systray icon"), box);
	mSystrayEnabled->setChecked(settings->IsSystrayEnabled());
	layoutBox->addWidget(mSystrayEnabled);

	// -- systray notifications on/off
	mSystrayNotifyEnabled = new QCheckBox(tr("Enable systray notifications"), box);
	mSystrayNotifyEnabled->setChecked(settings->IsSystrayNotificationEnabled());
	layoutBox->addWidget(mSystrayNotifyEnabled);

	// -- systray notifications when active --
	mSystrayNotifyWhenActive = new QCheckBox(tr("Show systray notifications when Qt Creator is active"), box);
	mSystrayNotifyWhenActive->setChecked(settings->ShowSystrayNotificationWhenActive());
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
	layoutRow->addWidget(mSystrayMinBuildTime);

	QLabel * label = new QLabel(tr("Minimum build time (in seconds) to display a systray notification"), box);
	layoutRow->addWidget(label);

	// -- systray notification length time --
	layoutRow = new QHBoxLayout;
	layoutBox->addLayout(layoutRow);

	mSystrayNotifyTime = new QSpinBox(box);
	mSystrayNotifyTime->setMinimum(Settings::SYSTRAY_NOTIFICATION_TIME_MIN);
	mSystrayNotifyTime->setMaximum(Settings::SYSTRAY_NOTIFICATION_TIME_MAX);
	mSystrayNotifyTime->setValue(settings->GetSystrayNotificationTime());
	mSystrayNotifyTime->setSingleStep(1);
	mSystrayNotifyTime->setMaximumWidth(50);
	layoutRow->addWidget(mSystrayNotifyTime);

	label = new QLabel(tr("Time (in seconds) the systray notification will be visible"), box);
	layoutRow->addWidget(label);

	// == AUDIO BOX ==
	box = new QGroupBox(tr("Audio"), this);
	layout->addWidget(box);

	layoutBox = new QVBoxLayout(box);

	// -- audio on/off --
	mAudioEnabled = new QCheckBox(tr("Enable audio notifications"), box);
	mAudioEnabled->setChecked(settings->IsAudioEnabled());
	layoutBox->addWidget(mAudioEnabled);

	// -- audio when open --
	mAudioNotifyWhenActive = new QCheckBox(tr("Play audio notifications when Qt Creator is active"), box);
	mAudioNotifyWhenActive->setChecked(settings->PlayAudioNotificationWhenActive());
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
	layoutRow->addWidget(mAudioVolume);

	label = new QLabel(tr("Notification volume"), box);
	layoutRow->addWidget(label);

	// -- systray notification min build time --
	layoutRow = new QHBoxLayout;
	layoutBox->addLayout(layoutRow);

	mAudioMinBuildTime = new QSpinBox(box);
	mAudioMinBuildTime->setMinimum(Settings::AUDIO_BUILD_TIME_MIN);
	mAudioMinBuildTime->setMaximum(Settings::AUDIO_BUILD_TIME_MAX);
	mAudioMinBuildTime->setValue(settings->GetAudioMinBuildtime());
	mAudioMinBuildTime->setSingleStep(1);
	mAudioMinBuildTime->setMaximumWidth(50);
	layoutRow->addWidget(mAudioMinBuildTime);

	label = new QLabel(tr("Minimum build time (in seconds) to play an audio notification"), box);
	layoutRow->addWidget(label);

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

} // namespace QtCreatorSysTray
