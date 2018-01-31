#pragma once

#include <QWidget>

class QCheckBox;

namespace QtCreatorSysTray
{

class Settings;

class OptionsPageMainWidget : public QWidget
{
	Q_OBJECT

public:
	explicit OptionsPageMainWidget(const Settings * settings);

	Settings GenerateSettings() const;

private:
	// -- SYSTRAY BOX --
	QCheckBox * mSystrayEnabled = nullptr;
	QCheckBox * mSystrayNotifyEnabled = nullptr;
	QCheckBox * mSystrayNotifyWhenActive = nullptr;

	// -- AUDIO BOX --
	QCheckBox * mAudioEnabled = nullptr;
	QCheckBox * mAudioNotifyWhenActive = nullptr;
};

} // namespace QtCreatorSysTray
