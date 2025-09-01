#pragma once

// IWYU pragma: begin_exports

// C Standard Library header files
// https://en.cppreference.com/w/c/header
// ================================================================

// C++ Standard Library headers
// https://en.cppreference.com/w/cpp/header
// ================================================================

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <print>
#include <string>

#include <future>
#include <mutex>
#include <numeric>
#include <vector>

// Vendor
// ================================================================

#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

// Internal
// ================================================================

#include "RootFallCore/Core/Base.hpp"
#include "RootFallCore/Debug/Instrumentor.hpp"

#include "WebGPUSandbox/Core/Base.hpp"

// Platform specific
// ================================================================

#if HK_PLATFORM_IS(EMSCRIPTEN)
  #include <emscripten.h>
  #include <emscripten/emscripten.h>
#else
  #include <webgpu/webgpu_cpp.h>
#endif

// IWYU pragma: end_exports
