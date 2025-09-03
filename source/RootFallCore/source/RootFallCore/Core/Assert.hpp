#pragma once

#include "RootFallCore/Core/Base.hpp"

#if HK_BUILD_DEBUG
  #define HK_ENABLE_ASSERTS 1
#else
  #define HK_ENABLE_ASSERTS 0
#endif

// Debug break
// ================================================================

#if HK_ENABLE_ASSERTS
  #if HK_PLATFORM_IS(WINDOWS)
    #define HK_DEBUGBREAK() __debugbreak()
  #elif HK_PLATFORM_IS(LINUX)
    #include <signal.h>
    // __builtin_debugtrap()
    #define HK_DEBUGBREAK() raise(SIGTRAP)
  #elif HK_PLATFORM_IS(MACOS)
    #define HK_DEBUGBREAK() __builtin_trap()
  #elif HK_PLATFORM_IS(EMSCRIPTEN)
    // __asm__("debugger") or emscripten_debugger()
    #define HK_DEBUGBREAK() emscripten_debugger()
  #else
    #error "Platform doesn't support debugbreak yet!"
  #endif
#else
  #define HK_DEBUGBREAK()
#endif

// Asserts
// ================================================================

#if HK_ENABLE_ASSERTS
  // Alternatively we could use the same "default" message for both "WITH_MSG"
  // and "NO_MSG" and provide support for custom formatting by concatenating the
  // formatting string instead of having the format inside the default message
  #define HK_INTERNAL_ASSERT_IMPL(type, check, msg, ...)                                                               \
    {                                                                                                                  \
      if (!(check)) {                                                                                                  \
        HK_LOG##type##ERROR(msg, __VA_ARGS__);                                                                         \
        HK_DEBUGBREAK();                                                                                               \
      }                                                                                                                \
    }
  #define HK_INTERNAL_ASSERT_WITH_MSG(type, check, ...)                                                                \
    HK_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
  #define HK_INTERNAL_ASSERT_NO_MSG(type, check)                                                                       \
    HK_INTERNAL_ASSERT_IMPL(                                                                                           \
      type, check, "Assertion '{0}' failed at {1}:{2}", HK_STRINGIFY_MACRO(check),                                     \
      std::filesystem::path(__FILE__).filename().string(), __LINE__                                                    \
    )

  #define HK_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
  #define HK_INTERNAL_ASSERT_GET_MACRO(...)                                                                            \
    HK_EXPAND_MACRO(                                                                                                   \
      HK_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HK_INTERNAL_ASSERT_WITH_MSG, HK_INTERNAL_ASSERT_NO_MSG)           \
    )

  // Currently accepts at least the condition and one additional parameter (the message) being optional
  #define HK_ASSERT(...) HK_EXPAND_MACRO(HK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
  #define HK_CORE_ASSERT(...) HK_EXPAND_MACRO(HK_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
  #define HK_ASSERT(...)
  #define HK_CORE_ASSERT(...)
#endif

// Unreachable hints
// ================================================================

#if defined(__clang__) || defined(__GNUC__)
  #define HK_BUILTIN_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
  #define HK_BUILTIN_UNREACHABLE() __assume(0)
#else
  // Fallback: no optimizer hint available
  #define HK_BUILTIN_UNREACHABLE() ((void)0)
#endif

// Debug-only "not reached". In release, provides optimization hints.
#if HK_ENABLE_ASSERTS
  #define HK_UNREACHABLE()                                                                                             \
    do {                                                                                                               \
      HK_ASSERT(false, "Unreachable code hit");                                                                        \
      HK_BUILTIN_UNREACHABLE();                                                                                        \
    } while (0)
#else
  #define HK_UNREACHABLE()                                                                                             \
    do {                                                                                                               \
      HK_BUILTIN_UNREACHABLE();                                                                                        \
    } while (0)
#endif
