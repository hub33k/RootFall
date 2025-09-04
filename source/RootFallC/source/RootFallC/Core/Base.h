#pragma once

// https://github.com/travisvroman/kohi/blob/main/kohi.core/src/defines.h

// IWYU pragma: begin_exports
#include "RootFallC/Core/Compiler.h"
#include "RootFallC/Core/Endianness.h"
#include "RootFallC/Core/Platform.h"
// IWYU pragma: end_exports

// Types
// ================================================================

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef size_t usize;

// ================================================================

#define UNUSED(x) ((void)(x))
