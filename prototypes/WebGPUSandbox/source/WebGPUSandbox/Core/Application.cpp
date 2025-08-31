#include "Application.hpp"

namespace hub33k {

  Application::Application() {
    Init();
  }

  Application::~Application() {
    Shutdown();
  }

  void Application::Init() {}

  void Application::Shutdown() {}

  void Application::Run() {}

  void Application::Frame() {}

  // ================================================================

} // namespace hub33k
