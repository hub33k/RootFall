#pragma once

#include <cstddef>
#include <cstdint>

// typedefs, defines, macros, HAL (Hardware Abstraction Layer) etc.

// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Core/Base.h
// https://github.com/PolyEngineTeam/PolyEngine/blob/master/PolyEngine/Core/Src/Defines.hpp
// https://github.com/travisvroman/kohi/blob/main/kohi.core/src/defines.h
// https://github.com/godotengine/godot/blob/master/core/typedefs.h
// check ElectronicsArt, UnrealEngine, dawn etc.

/**
 * Basic definitions and simple functions to be used everywhere.
 */

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

namespace hk {

  using u8 = uint8_t;
  using u16 = uint16_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  using i8 = int8_t;
  using i16 = int16_t;
  using i32 = int32_t;
  using i64 = int64_t;

  using size = size_t;

  using f32 = float;
  using f64 = double;

} // namespace hk

// ================================================================

#define BIT(x) (1 << x)

#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

#define HK_EXPAND_MACRO(x) x
#define HK_STRINGIFY_MACRO(x) #x
#define HK_QUOTE(name) #name
#define HK_STR(macro) HK_QUOTE(macro)
#define HK_BIND_EVENT_FN(fn)                                                                                           \
  [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// ================================================================

// Convenient way to purposefully mark a variable as unused to avoid warning
#define HK_UNUSED(x) ((void)x)

// Forward declare a class or struct, and define a lot of typedefs for different pointer types all at once.
#define HK_CLASS(ClassName)                                                                                            \
  class ClassName;                                                                                                     \
  typedef std::shared_ptr<ClassName> ClassName##Ptr;                                                                   \
  typedef std::shared_ptr<const ClassName> ClassName##ConstPtr;                                                        \
  typedef std::weak_ptr<ClassName> ClassName##WeakPtr;                                                                 \
  typedef std::weak_ptr<const ClassName> ClassName##ConstWeakPtr;                                                      \
  typedef std::unique_ptr<ClassName> ClassName##UPtr;                                                                  \
  typedef std::unique_ptr<const ClassName> ClassName##ConstUPtr

#define HK_STRUCT(StructName)                                                                                          \
  struct StructName;                                                                                                   \
  typedef std::shared_ptr<StructName> StructName##Ptr;                                                                 \
  typedef std::shared_ptr<const StructName> StructName##ConstPtr;                                                      \
  typedef std::weak_ptr<StructName> StructName##WeakPtr;                                                               \
  typedef std::weak_ptr<const StructName> StructName##ConstWeakPtr;                                                    \
  typedef std::unique_ptr<StructName> StructName##UPtr;                                                                \
  typedef std::unique_ptr<const StructName> StructName##ConstUPtr
