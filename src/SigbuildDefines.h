#pragma once

#include <QtGlobal>

namespace Sigbuild
{

enum class BuildState
{
	BUILDING = 0,
	FAILED,
	OK,

	NUM
};

static const int NUM_BUILD_STATES = static_cast<int>(BuildState::NUM);

} // namespace Sigbuild
