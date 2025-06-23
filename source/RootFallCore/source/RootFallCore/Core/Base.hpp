#pragma once

// IWYU pragma: begin_exports
#include "RootFallCore/Core/Compiler.hpp"
#include "RootFallCore/Core/Platform.hpp"
// #include "hub33k/Core/PlatformDetection.hpp"
// IWYU pragma: end_exports

#include <memory>

#define DATA_DIR PROJECT_DATA_DIR

// Build type
// ================================================================

// Debug define - BUILD_DEBUG, !defined(NDEBUG)
// Release define - BUILD_RELEASE, defined(NDEBUG)
#if defined(BUILD_DEBUG) || !defined(NDEBUG)
  #define HK_BUILD_DEBUG 1
  #define HK_BUILD_RELEASE 0
#elif defined(BUILD_RELEASE) || defined(NDEBUG)
  #define HK_BUILD_DEBUG 0
  #define HK_BUILD_RELEASE 1
#else
  #error "Unknown build type!"
#endif

#if HK_BUILD_DEBUG
  #define HK_ENABLE_ASSERTS 1
#else
  #define HK_ENABLE_ASSERTS 0
#endif

// ================================================================

#if HK_BUILD_DEBUG
  #define ASSETS_DIR PROJECT_ROOT_DIR + std::string("data/")
#else
  #define ASSETS_DIR "data/"
#endif

#define TEXTURES_DIR(fileName) (std::string(ASSETS_DIR) + "textures/" + (fileName))
#define SHADERS_DIR(fileName) (std::string(ASSETS_DIR) + "shaders/" + (fileName))
#define FONTS_DIR(fileName) (std::string(ASSETS_DIR) + "fonts/" + (fileName))
#define SOUNDS_DIR(fileName) (std::string(ASSETS_DIR) + "sounds/" + (fileName))

using f32 = float;
using f64 = double;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using usize = std::size_t;
using isize = std::ptrdiff_t;
// using isize = std::ssize(); // ssize_t

#define PI 3.14159265359f

// ================================================================

#define BIT(x) (1 << x)

#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

#define HK_EXPAND_MACRO(x) x
#define HK_STRINGIFY_MACRO(x) #x
#define HK_BIND_EVENT_FN(fn)                                                                                           \
  [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// ================================================================

namespace hub33k {

  template <typename T>
  using Scope = std::unique_ptr<T>;

  template <typename T, typename... Args>
  constexpr Scope<T> CreateScope(Args &&...args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using Ref = std::shared_ptr<T>;

  template <typename T, typename... Args>
  constexpr Ref<T> CreateRef(Args &&...args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

} // namespace hub33k

// IWYU pragma: begin_exports
#include "RootFallCore/Core/Assert.hpp"
#include "RootFallCore/Core/Defines.hpp"

#if HK_PLATFORM_IS(EMSCRIPTEN)
  #define HK_LOG(level, category, ...)                                                                                 \
    std::cout << std::format("[{}] [{}] {}\n", category, level, std::format(__VA_ARGS__))

  // Core log macros
  #define HK_LOG_CORE_TRACE(...) HK_LOG("TRACE", "HK_CORE", __VA_ARGS__)
  #define HK_LOG_CORE_INFO(...) HK_LOG("INFO", "HK_CORE", __VA_ARGS__)
  #define HK_LOG_CORE_WARN(...) HK_LOG("WARN", "HK_CORE", __VA_ARGS__)
  #define HK_LOG_CORE_ERROR(...) HK_LOG("ERROR", "HK_CORE", __VA_ARGS__)
  #define HK_LOG_CORE_CRITICAL(...) HK_LOG("CRITICAL", "HK_CORE", __VA_ARGS__)

  // Client log macros
  #define HK_LOG_TRACE(...) HK_LOG("TRACE", "HK", __VA_ARGS__)
  #define HK_LOG_INFO(...) HK_LOG("INFO", "HK", __VA_ARGS__)
  #define HK_LOG_WARN(...) HK_LOG("WARN", "HK", __VA_ARGS__)
  #define HK_LOG_ERROR(...) HK_LOG("ERROR", "HK", __VA_ARGS__)
  #define HK_LOG_CRITICAL(...) HK_LOG("CRITICAL", "HK", __VA_ARGS__)
#else
  #include "RootFallCore/Core/Log.hpp"
#endif
// IWYU pragma: end_exports
