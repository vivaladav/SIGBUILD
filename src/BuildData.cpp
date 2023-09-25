#include "BuildData.h"

#include "SigbuildDefines.h"

namespace Sigbuild
{

BuildData::BuildData(const QString & project, quint64 start, quint64 end, BuildState state)
    : mProject(project)
    , mTimeStart(start)
    , mTimeEnd(end)
    , mState(state)
{

}

} // namespace Sigbuild
