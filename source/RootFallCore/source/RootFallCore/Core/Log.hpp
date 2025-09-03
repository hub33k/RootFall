#pragma once

#include "RootFallCore/Core/Base.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace hub33k {

  class Log {
  public:
    static void Init();

    static Ref<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
    static Ref<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

  private:
    static Ref<spdlog::logger> s_CoreLogger;
    static Ref<spdlog::logger> s_ClientLogger;
  };

} // namespace hub33k

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::vec<L, T, Q> &vector) {
  return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::mat<C, R, T, Q> &matrix) {
  return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, glm::qua<T, Q> quaternion) {
  return os << glm::to_string(quaternion);
}

// Core log macros
#define HK_LOG_CORE_TRACE(...) ::hub33k::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HK_LOG_CORE_INFO(...) ::hub33k::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HK_LOG_CORE_WARN(...) ::hub33k::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HK_LOG_CORE_ERROR(...) ::hub33k::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HK_LOG_CORE_CRITICAL(...) ::hub33k::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HK_LOG_TRACE(...) ::hub33k::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HK_LOG_INFO(...) ::hub33k::Log::GetClientLogger()->info(__VA_ARGS__)
#define HK_LOG_WARN(...) ::hub33k::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HK_LOG_ERROR(...) ::hub33k::Log::GetClientLogger()->error(__VA_ARGS__)
#define HK_LOG_CRITICAL(...) ::hub33k::Log::GetClientLogger()->critical(__VA_ARGS__)
