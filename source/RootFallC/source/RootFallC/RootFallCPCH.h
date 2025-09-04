#pragma once

// IWYU pragma: begin_exports

// C Standard Library header files
// https://en.cppreference.com/w/c/header
// ================================================================

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vendor
// ================================================================

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
// #include <glm/ext/matrix_transform.hpp>
// #include <glm/glm.hpp>
#include <webgpu/webgpu.h>

#include "RootFallC/Vendor/linmath.h/linmath.h"

// Internal
// ================================================================

#include "RootFallC/Core/Base.h"

// Platform specific
// ================================================================

// IWYU pragma: end_exports
