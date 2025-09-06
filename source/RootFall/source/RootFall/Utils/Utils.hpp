#pragma once

namespace hub33k {

  template <typename T>
  T Clamp(T value, T min, T max) {
    return value < min ? min : value > max ? max : value;
  }

  // File IO
  // ================================================================

  std::string ReadFile(const std::string &path);

} // namespace hub33k
