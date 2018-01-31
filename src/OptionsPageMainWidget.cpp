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

	QSpinBox * spin = new QSpinBox(box);
	spin->setEnabled(false);
	spin->setMinimum(0);
	spin->setMaximum(60);
	spin->setSingleStep(5);
	spin->setMaximumWidth(50);
	layoutRow->addWidget(spin);

	QLabel * label = new QLabel(tr("Minimum build time (in seconds) to display a systray notification"), box);
	label->setEnabled(false);
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

	spin = new QSpinBox(box);
	spin->setEnabled(false);
	spin->setMinimum(0);
	spin->setMaximum(60);
	spin->setSingleStep(5);
	spin->setMaximumWidth(50);
	layoutRow->addWidget(spin);

	label = new QLabel(tr("Minimum build time (in seconds) to play an audio notification"), box);
	label->setEnabled(false);
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

	// -- AUDIO --
	settings.SetAudioEnabled(mAudioEnabled->isChecked());

	settings.SetAudioNotificationWhenActive(mAudioNotifyWhenActive->isChecked());

	settings.SetAudioVolume(mAudioVolume->value());

	return settings;
}

} // namespace QtCreatorSysTray
