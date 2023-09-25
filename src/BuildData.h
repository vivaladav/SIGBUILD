#pragma once

#include <QString>

namespace Sigbuild
{

enum class BuildState;

class BuildData
{
public:
    BuildData(const QString & project, quint64 start, quint64 end, BuildState state);

    const QString & GetProject() const;
    quint64 GetTimeStart() const;
    quint64 GetTimeEnd() const;
    BuildState GetState() const;

private:
    QString mProject;

    quint64 mTimeStart;
    quint64 mTimeEnd;

    BuildState mState;
};


// == INLINE FUNCTIONS ==
inline const QString & BuildData::GetProject() const { return mProject; }
inline quint64 BuildData::GetTimeStart() const { return mTimeStart; }
inline quint64 BuildData::GetTimeEnd() const { return mTimeEnd; }
inline BuildState BuildData::GetState() const { return mState; }

} // namespace Sigbuild
