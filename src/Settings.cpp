#include "Settings.h"

#include <coreplugin/icore.h>

namespace Sigbuild
{

// -- KEYS --
const QString GROUP("SIGBUILD");
const QString OPT_SYSTRAY_ENABLED("SYSTRAY_ENABLED");
const QString OPT_SYSTRAY_NOTIFY_ENABLED("SYSTRAY_NOTIFY_ENABLED");
const QString OPT_SYSTRAY_NOTIFY_WHEN_ACTIVE("SYSTRAY_NOTIFY_WHEN_ACTIVE");
const QString OPT_SYSTRAY_MIN_BUILD_TIME("SYSTRAY_MIN_BUILD_TIME");
const QString OPT_SYSTRAY_NOTIFY_TIME("SYSTRAY_NOTIFY_TIME");
const QString OPT_AUDIO_ENABLED("AUDIO_ENABLED");
const QString OPT_AUDIO_NOTIFY_WHEN_ACTIVE("AUDIO_NOTIFY_WHEN_ACTIVE");
const QString OPT_AUDIO_VOLUME("AUDIO_VOLUME");
const QString OPT_AUDIO_MIN_BUILD_TIME("AUDIO_MIN_BUILD_TIME");

// -- DEFAULTS --
const bool DEF_SYSTRAY_ENABLED						= true;
const bool DEF_SYSTRAY_NOTIFY_ENABLED				= true;
const bool DEF_SYSTRAY_NOTIFY_WHEN_ACTIVE			= true;
const int DEF_SYSTRAY_MIN_BUILD_TIME				= 0;
const int DEF_SYSTRAY_NOTIFY_TIME					= 5;
const bool DEF_AUDIO_ENABLED						= true;
const bool DEF_AUDIO_NOTIFY_WHEN_ACTIVE				= true;
const int DEF_AUDIO_VOLUME							= Settings::AUDIO_VOL_MAX;
const int DEF_AUDIO_MIN_BUILD_TIME					= 0;

Settings::Settings()
	: mOptSystrayEnabled(DEF_SYSTRAY_ENABLED)
	, mOptSystrayNotifyEnabled(DEF_SYSTRAY_NOTIFY_ENABLED)
	, mOptSystrayNotifyWhenActive(DEF_SYSTRAY_NOTIFY_WHEN_ACTIVE)
	, mOptSystrayMinBuildTime(DEF_SYSTRAY_MIN_BUILD_TIME)
	, mOptAudioEnabled(DEF_AUDIO_ENABLED)
	, mOptAudioNotifyWhenActive(DEF_AUDIO_NOTIFY_WHEN_ACTIVE)
	, mOptAudioVolume(DEF_AUDIO_VOLUME)
	, mOptAudioMinBuildTime(DEF_AUDIO_MIN_BUILD_TIME)
{
}

void Settings::Load()
{
	QSettings * globalSettings = Core::ICore::settings();

	globalSettings->beginGroup(GROUP);

	// -- SYSTRAY --
	mOptSystrayEnabled = globalSettings->value(	OPT_SYSTRAY_ENABLED, DEF_SYSTRAY_ENABLED).toBool();

	mOptSystrayNotifyEnabled = globalSettings->value(OPT_SYSTRAY_NOTIFY_ENABLED, DEF_SYSTRAY_NOTIFY_ENABLED).toBool();

	mOptSystrayNotifyWhenActive	= globalSettings->value(OPT_SYSTRAY_NOTIFY_WHEN_ACTIVE,
														DEF_SYSTRAY_NOTIFY_WHEN_ACTIVE).toBool();

	mOptSystrayMinBuildTime = globalSettings->value(OPT_SYSTRAY_MIN_BUILD_TIME, DEF_SYSTRAY_MIN_BUILD_TIME).toInt();

	mOptSystrayNotifyTime = globalSettings->value(OPT_SYSTRAY_NOTIFY_TIME, DEF_SYSTRAY_NOTIFY_TIME).toInt();

	// -- AUDIO --
	mOptAudioEnabled = globalSettings->value(OPT_AUDIO_ENABLED, DEF_AUDIO_ENABLED).toBool();

	mOptAudioNotifyWhenActive = globalSettings->value(	OPT_AUDIO_NOTIFY_WHEN_ACTIVE,
														DEF_AUDIO_NOTIFY_WHEN_ACTIVE).toBool();

	mOptAudioVolume = globalSettings->value(OPT_AUDIO_VOLUME, DEF_AUDIO_VOLUME).toInt();

	mOptAudioMinBuildTime = globalSettings->value(OPT_AUDIO_MIN_BUILD_TIME, DEF_AUDIO_MIN_BUILD_TIME).toInt();



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

	globalSettings->setValue(OPT_SYSTRAY_MIN_BUILD_TIME, mOptSystrayMinBuildTime);

	globalSettings->setValue(OPT_SYSTRAY_NOTIFY_TIME, mOptSystrayNotifyTime);

	// -- AUDIO --
	globalSettings->setValue(OPT_AUDIO_ENABLED, mOptAudioEnabled);

	globalSettings->setValue(OPT_AUDIO_NOTIFY_WHEN_ACTIVE, mOptAudioNotifyWhenActive);

	globalSettings->setValue(OPT_AUDIO_VOLUME, mOptAudioVolume);

	globalSettings->setValue(OPT_AUDIO_MIN_BUILD_TIME, mOptAudioMinBuildTime);

	globalSettings->endGroup();
}

void Settings::SetSystrayMinBuildTime(int val)
{
	// clamp value
	if(val < SYSTRAY_BUILD_TIME_MIN)
		val = SYSTRAY_BUILD_TIME_MIN;
	else if(val > SYSTRAY_BUILD_TIME_MAX)
		val = SYSTRAY_BUILD_TIME_MAX;

	// set
	mOptSystrayMinBuildTime = val;
}

void Settings::SetSystrayNotificationTime(int val)
{
	// clamp value
	if(val < SYSTRAY_NOTIFICATION_TIME_MIN)
		val = SYSTRAY_NOTIFICATION_TIME_MIN;
	else if(val > SYSTRAY_NOTIFICATION_TIME_MAX)
		val = SYSTRAY_NOTIFICATION_TIME_MAX;

	// set
	mOptSystrayNotifyTime = val;
}

void Settings::SetAudioVolume(int val)
{
	// clamp value
	if(val < AUDIO_VOL_MIN)
		val = AUDIO_VOL_MIN;
	else if(val > AUDIO_VOL_MAX)
		val = AUDIO_VOL_MAX;

	// set
	mOptAudioVolume = val;
}

void Settings::SetAudioMinBuildTime(int val)
{
	// clamp value
	if(val < AUDIO_BUILD_TIME_MIN)
		val = AUDIO_BUILD_TIME_MIN;
	else if(val > AUDIO_BUILD_TIME_MAX)
		val = AUDIO_BUILD_TIME_MAX;

	// set
	mOptAudioMinBuildTime = val;
}

} // namespace Sigbuild
