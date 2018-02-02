#pragma once

#include <QtGlobal>

#if defined(SIGBUILD_LIBRARY)
#  define SIGBUILD_EXPORT Q_DECL_EXPORT
#else
#  define SIGBUILD_EXPORT Q_DECL_IMPORT
#endif
