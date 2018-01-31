#include "Settings.h"

#include <coreplugin/icore.h>

namespace QtCreatorSysTray
{

// -- KEYS --
const QString GROUP("SIGBUILD");
const QString OPT_SYSTRAY_ENABLED("SYSTRAY_ENABLED");
const QString OPT_SYSTRAY_NOTIFY_ENABLED("SYSTRAY_NOTIFY_ENABLED");
const QString OPT_SYSTRAY_NOTIFY_WHEN_ACTIVE("SYSTRAY_NOTIFY_WHEN_ACTIVE");
const QString OPT_AUDIO_ENABLED("AUDIO_ENABLED");
const QString OPT_AUDIO_NOTIFY_WHEN_ACTIVE("AUDIO_NOTIFY_WHEN_ACTIVE");

// -- DEFAULTS --
const bool DEF_SYSTRAY_ENABLED						= true;
const bool DEF_SYSTRAY_NOTIFY_ENABLED				= true;
const bool DEF_SYSTRAY_NOTIFY_WHEN_ACTIVE			= true;
const bool DEF_AUDIO_ENABLED						= true;
const bool DEF_AUDIO_NOTIFY_WHEN_ACTIVE				= true;

Settings::Settings()
	: mOptSystrayEnabled(DEF_SYSTRAY_ENABLED)
{
}

void Settings::Load()
{
	QSettings * globalSettings = Core::ICore::settings();

	globalSettings->beginGroup(GROUP);

	// -- SYSTRAY --
	mOptSystrayEnabled = globalSettings->value(	OPT_SYSTRAY_ENABLED,
												DEF_SYSTRAY_ENABLED).toBool();

	mOptSystrayNotifyEnabled = globalSettings->value(	OPT_SYSTRAY_NOTIFY_ENABLED,
														DEF_SYSTRAY_NOTIFY_ENABLED).toBool();

	mOptSystrayNotifyWhenActive	= globalSettings->value(OPT_SYSTRAY_NOTIFY_WHEN_ACTIVE,
														DEF_SYSTRAY_NOTIFY_WHEN_ACTIVE).toBool();

	// -- AUDIO --
	mOptAudioEnabled = globalSettings->value(	OPT_AUDIO_ENABLED,
												DEF_AUDIO_ENABLED).toBool();

	mOptAudioNotifyWhenActive	= globalSettings->value(OPT_AUDIO_NOTIFY_WHEN_ACTIVE,
														DEF_AUDIO_NOTIFY_WHEN_ACTIVE).toBool();

	globalSettings->endGroup();
}

void Settings::Save()
{
	QSettings * globalSettings = Core::ICore::settings();

	globalSettings->beginGroup(GROUP);

	// -- SYSTRAY --
	globalSettings->setValue(OPT_SYSTRAY_ENABLED, mOptSystrayEnabled);

	globalSettings->setValue(OPT_SYSTRAY_NOTIFY_ENABLED, mOptSystrayNotifyEnabled);

	globalSettings->setValue(OPT_SYSTRAY_NOTIFY_ENABLED, mOptSystrayNotifyWhenActive);

	// -- AUDIO --
	globalSettings->setValue(OPT_AUDIO_ENABLED, mOptAudioEnabled);

	globalSettings->setValue(OPT_AUDIO_NOTIFY_WHEN_ACTIVE, mOptAudioNotifyWhenActive);

	globalSettings->endGroup();
}

} // namespace QtCreatorSysTray
