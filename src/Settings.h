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
	void SetSystrayEnabled(bool val);
	bool IsSystrayEnabled() const;

	void SetSystrayNotificationEnabled(bool val);
	bool IsSystrayNotificationEnabled() const;

	void SetSystrayNotificationWhenActive(bool val);
	bool ShowSystrayNotificationWhenActive() const;

	// -- AUDIO --
	void SetAudioEnabled(bool val);
	bool IsAudioEnabled() const;

	void SetAudioNotificationWhenActive(bool val);
	bool PlayAudioNotificationWhenActive() const;

	// -- OPERATORS --
	bool operator==(const Settings & other) const;
	bool operator!=(const Settings & other) const;

private:
	// -- SYSTRAY --
	bool mOptSystrayEnabled;
	bool mOptSystrayNotifyEnabled;
	bool mOptSystrayNotifyWhenActive;

	// -- AUDIO --
	bool mOptAudioEnabled;
	bool mOptAudioNotifyWhenActive;
};

// -- SYSTRAY --
inline void Settings::SetSystrayEnabled(bool val) { mOptSystrayEnabled = val; }
inline bool Settings::IsSystrayEnabled() const { return mOptSystrayEnabled; }

inline void Settings::SetSystrayNotificationEnabled(bool val) { mOptSystrayNotifyEnabled = val; }
inline bool Settings::IsSystrayNotificationEnabled() const { return mOptSystrayNotifyEnabled; }

inline void Settings::SetSystrayNotificationWhenActive(bool val) { mOptSystrayNotifyWhenActive = val; }
inline bool Settings::ShowSystrayNotificationWhenActive() const { return mOptSystrayNotifyWhenActive; }

// -- AUDIO --
inline void Settings::SetAudioEnabled(bool val) { mOptAudioEnabled = val; }
inline bool Settings::IsAudioEnabled() const { return mOptAudioEnabled; }

inline void Settings::SetAudioNotificationWhenActive(bool val) { mOptAudioNotifyWhenActive = val; }
inline bool Settings::PlayAudioNotificationWhenActive() const { return mOptAudioNotifyWhenActive; }

// -- OPERATORS --
inline bool Settings::operator==(const Settings & other) const
{
	return	mOptSystrayEnabled == other.mOptSystrayEnabled &&
			mOptSystrayNotifyEnabled == other.mOptSystrayNotifyEnabled &&
			mOptSystrayNotifyWhenActive == other.mOptSystrayNotifyWhenActive &&
			mOptAudioEnabled == other.mOptAudioEnabled &&
			mOptAudioNotifyWhenActive == other.mOptAudioNotifyWhenActive;
}

inline bool Settings::operator!=(const Settings & other) const
{
	return !(*this == other);
}

} // namespace QtCreatorSysTray
