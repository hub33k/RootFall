#pragma once

#include <memory>

// IWYU pragma: begin_exports
#include "RootFallCore/Core/Compiler.hpp"
#include "RootFallCore/Core/Platform.hpp"
// IWYU pragma: end_exports

// #define DATA_DIR PROJECT_DATA_DIR

// Build type
// ================================================================

// Debug define - BUILD_DEBUG, !defined(NDEBUG)
// Release define - BUILD_RELEASE, defined(NDEBUG)
#if defined(BUILD_DEBUG) || !defined(NDEBUG)
  #define HK_BUILD_DEBUG 1
  #define HK_BUILD_RELEASE 0
constexpr bool DebugEnabled = true;
#elif defined(BUILD_RELEASE) || defined(NDEBUG)
  #define HK_BUILD_DEBUG 0
  #define HK_BUILD_RELEASE 1
constexpr bool DebugEnabled = false;
#else
  #error "Unknown build type!"
#endif

// ================================================================

#if HK_BUILD_DEBUG
  #define ASSETS_DIR PROJECT_ROOT_DIR + std::string("data/")
#else
  #define ASSETS_DIR "data/"
#endif

#define DATA_DIR(fileName) (std::string(ASSETS_DIR) + (fileName))
#define TEXTURES_DIR(fileName) (std::string(ASSETS_DIR) + "textures/" + (fileName))
#define SHADERS_DIR(fileName) (std::string(ASSETS_DIR) + "shaders/" + (fileName))
#define FONTS_DIR(fileName) (std::string(ASSETS_DIR) + "fonts/" + (fileName))
#define SOUNDS_DIR(fileName) (std::string(ASSETS_DIR) + "sounds/" + (fileName))

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

// ================================================================

// IWYU pragma: begin_exports
#include "RootFallCore/Core/Assert.hpp"
#include "RootFallCore/Core/Endianness.hpp"
#include "RootFallCore/Core/Log.hpp"

#include "RootFallCore/Core/Defines.hpp"
// IWYU pragma: end_exports
