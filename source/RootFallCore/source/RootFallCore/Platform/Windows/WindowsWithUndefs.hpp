#pragma once

#include "RootFallCore/Core/Platform.hpp"

#if !HK_PLATFORM_IS(WINDOWS)
  #error "WindowsWithUndefs.hpp included on non-Windows"
#endif

// This header includes <windows.h> but removes all the extra defines that conflict with identifiers
// in internal code. It should never be included in something that is part of the public interface.
#include <windows.h>

// Macros defined for ANSI / Unicode support
#undef CreateWindow
#undef GetMessage

// Macros defined to produce compiler intrinsics
#undef MemoryBarrier

// Macro defined as an alias of GetTickCount
#undef GetCurrentTime
