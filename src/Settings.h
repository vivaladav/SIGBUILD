#pragma once

#include <QString>

namespace Sigbuild
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

    int GetSystrayMinBuildTime() const;
    void SetSystrayMinBuildTime(int val);

    int GetSystrayNotificationTime() const;
    void SetSystrayNotificationTime(int val);

    // -- AUDIO --
    bool IsAudioEnabled() const;
    void SetAudioEnabled(bool val);

    bool PlayAudioNotificationWhenActive() const;
    void SetAudioNotificationWhenActive(bool val);

    bool UseCustomSounds() const;
    void SetUseCustomSounds(bool val);
    QString GetCustomSuccessSound() const;
    void SetCustomSuccessSound(QString filename);
    QString GetCustomFailSound() const;
    void SetCustomFailSound(QString filename);

    int GetAudioVolume() const;
    double GetAudioVolumeAsReal() const;
    void SetAudioVolume(int val);

    int GetAudioMinBuildtime() const;
    void SetAudioMinBuildTime(int val);

    // -- OPERATORS --
    bool operator==(const Settings & other) const;
    bool operator!=(const Settings & other) const;

public:
    static const int SYSTRAY_BUILD_TIME_MIN = 0;
    static const int SYSTRAY_BUILD_TIME_MAX = 60;

    static const int SYSTRAY_NOTIFICATION_TIME_MIN = 1;
    static const int SYSTRAY_NOTIFICATION_TIME_MAX = 60;

    static const int AUDIO_VOL_MIN = 0;
    static const int AUDIO_VOL_MAX = 100;

    static const int AUDIO_BUILD_TIME_MIN = 0;
    static const int AUDIO_BUILD_TIME_MAX = 60;

private:
    // -- SYSTRAY --
    bool mOptSystrayEnabled;
    bool mOptSystrayNotifyEnabled;
    bool mOptSystrayNotifyWhenActive;
    int mOptSystrayMinBuildTime;
    int mOptSystrayNotifyTime;

    // -- AUDIO --
    bool mOptAudioEnabled;
    bool mOptAudioNotifyWhenActive;
    bool mOptAudioCustomSounds;
    QString mOptAudioCustomSuccessSound;
    QString mOptAudioCustomFailSound;
    int mOptAudioVolume;
    int mOptAudioMinBuildTime;
};

// -- SYSTRAY --
inline bool Settings::IsSystrayEnabled() const { return mOptSystrayEnabled; }
inline void Settings::SetSystrayEnabled(bool val) { mOptSystrayEnabled = val; }

inline bool Settings::IsSystrayNotificationEnabled() const { return mOptSystrayNotifyEnabled; }
inline void Settings::SetSystrayNotificationEnabled(bool val) { mOptSystrayNotifyEnabled = val; }

inline bool Settings::ShowSystrayNotificationWhenActive() const { return mOptSystrayNotifyWhenActive; }
inline void Settings::SetSystrayNotificationWhenActive(bool val) { mOptSystrayNotifyWhenActive = val; }

inline int Settings::GetSystrayMinBuildTime() const { return mOptSystrayMinBuildTime; }

inline int Settings::GetSystrayNotificationTime() const { return mOptSystrayNotifyTime; }

// -- AUDIO --
inline bool Settings::IsAudioEnabled() const { return mOptAudioEnabled; }
inline void Settings::SetAudioEnabled(bool val) { mOptAudioEnabled = val; }

inline bool Settings::PlayAudioNotificationWhenActive() const { return mOptAudioNotifyWhenActive; }
inline void Settings::SetAudioNotificationWhenActive(bool val) { mOptAudioNotifyWhenActive = val; }

inline bool Settings::UseCustomSounds() const { return mOptAudioCustomSounds; }
inline void Settings::SetUseCustomSounds(bool val) { mOptAudioCustomSounds = val; }
inline QString Settings::GetCustomSuccessSound() const { return mOptAudioCustomSuccessSound; }
inline void Settings::SetCustomSuccessSound(QString filename) { mOptAudioCustomSuccessSound = filename; }
inline QString Settings::GetCustomFailSound() const { return mOptAudioCustomFailSound; }
inline void Settings::SetCustomFailSound(QString filename) { mOptAudioCustomFailSound = filename; }

inline int Settings::GetAudioVolume() const { return mOptAudioVolume; }
inline double Settings::GetAudioVolumeAsReal() const
{
    return static_cast<double>(mOptAudioVolume) / static_cast<double>(AUDIO_VOL_MAX);
}

inline int Settings::GetAudioMinBuildtime() const { return mOptAudioMinBuildTime; }

// -- OPERATORS --
inline bool Settings::operator==(const Settings & other) const
{
    return  mOptSystrayEnabled == other.mOptSystrayEnabled &&
            mOptSystrayNotifyEnabled == other.mOptSystrayNotifyEnabled &&
            mOptSystrayNotifyWhenActive == other.mOptSystrayNotifyWhenActive &&
            mOptSystrayMinBuildTime == other.mOptSystrayMinBuildTime &&
            mOptSystrayNotifyTime == other.mOptSystrayNotifyTime &&
            mOptAudioEnabled == other.mOptAudioEnabled &&
            mOptAudioNotifyWhenActive == other.mOptAudioNotifyWhenActive &&
            mOptAudioVolume == other.mOptAudioVolume &&
            mOptAudioMinBuildTime == other.mOptAudioMinBuildTime &&
            mOptAudioCustomSounds == other.mOptAudioCustomSounds &&
            mOptAudioCustomSuccessSound == other.mOptAudioCustomSuccessSound &&
            mOptAudioCustomFailSound == other.mOptAudioCustomFailSound;
}

inline bool Settings::operator!=(const Settings & other) const
{
    return !(*this == other);
}

} // namespace Sigbuild
