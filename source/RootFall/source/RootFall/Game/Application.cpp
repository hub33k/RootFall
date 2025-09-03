#include "Application.hpp"

namespace hub33k {

  Application::Application() {
    HK_LOG_INFO("Application created");
  }

  Application::~Application() {
    HK_LOG_INFO("Application destroyed");
  }

  void Application::Run() {
    HK_LOG_INFO("Application running");
  }

} // namespace hub33k
