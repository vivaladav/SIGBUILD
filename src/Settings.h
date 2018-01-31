#pragma once

namespace QtCreatorSysTray
{

class Settings
{
public:
	Settings();

	void Load();
	void Save();

	// -- SYSTRAY --
	bool IsSystrayEnabled() const;
	void SetSystrayEnabled(bool val);

	bool IsSystrayNotificationEnabled() const;
	void SetSystrayNotificationEnabled(bool val);

	bool ShowSystrayNotificationWhenActive() const;
	void SetSystrayNotificationWhenActive(bool val);

	// -- AUDIO --
	bool IsAudioEnabled() const;
	void SetAudioEnabled(bool val);

	bool PlayAudioNotificationWhenActive() const;
	void SetAudioNotificationWhenActive(bool val);

	int GetAudioVolume() const;
	double GetAudioVolumeAsReal() const;
	void SetAudioVolume(int vol);

	// -- OPERATORS --
	bool operator==(const Settings & other) const;
	bool operator!=(const Settings & other) const;

public:
	static const int AUDIO_VOL_MIN = 0;
	static const int AUDIO_VOL_MAX = 100;

private:
	// -- SYSTRAY --
	bool mOptSystrayEnabled;
	bool mOptSystrayNotifyEnabled;
	bool mOptSystrayNotifyWhenActive;

	// -- AUDIO --
	bool mOptAudioEnabled;
	bool mOptAudioNotifyWhenActive;
	int mOptAudioVolume;
};

// -- SYSTRAY --
inline bool Settings::IsSystrayEnabled() const { return mOptSystrayEnabled; }
inline void Settings::SetSystrayEnabled(bool val) { mOptSystrayEnabled = val; }

inline bool Settings::IsSystrayNotificationEnabled() const { return mOptSystrayNotifyEnabled; }
inline void Settings::SetSystrayNotificationEnabled(bool val) { mOptSystrayNotifyEnabled = val; }

inline bool Settings::ShowSystrayNotificationWhenActive() const { return mOptSystrayNotifyWhenActive; }
inline void Settings::SetSystrayNotificationWhenActive(bool val) { mOptSystrayNotifyWhenActive = val; }

// -- AUDIO --
inline bool Settings::IsAudioEnabled() const { return mOptAudioEnabled; }
inline void Settings::SetAudioEnabled(bool val) { mOptAudioEnabled = val; }

inline bool Settings::PlayAudioNotificationWhenActive() const { return mOptAudioNotifyWhenActive; }
inline void Settings::SetAudioNotificationWhenActive(bool val) { mOptAudioNotifyWhenActive = val; }

inline int Settings::GetAudioVolume() const { return mOptAudioVolume; }
inline double Settings::GetAudioVolumeAsReal() const
{
	return static_cast<double>(mOptAudioVolume) / static_cast<double>(AUDIO_VOL_MAX);
}

// -- OPERATORS --
inline bool Settings::operator==(const Settings & other) const
{
	return	mOptSystrayEnabled == other.mOptSystrayEnabled &&
			mOptSystrayNotifyEnabled == other.mOptSystrayNotifyEnabled &&
			mOptSystrayNotifyWhenActive == other.mOptSystrayNotifyWhenActive &&
			mOptAudioEnabled == other.mOptAudioEnabled &&
			mOptAudioNotifyWhenActive == other.mOptAudioNotifyWhenActive &&
			mOptAudioVolume == other.mOptAudioVolume;
}

inline bool Settings::operator!=(const Settings & other) const
{
	return !(*this == other);
}

} // namespace QtCreatorSysTray
