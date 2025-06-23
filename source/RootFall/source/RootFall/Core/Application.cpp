#include "Application.hpp"

namespace hub33k {

  Application *Application::s_Instance = nullptr;

  Application::Application(const ApplicationSpecification &specification) : m_Specification(specification) {
    HK_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = Window::Create(WindowProps(m_Specification.Name));
    m_Window->SetEventCallback(HK_BIND_EVENT_FN(Application::OnEvent));
  }

  Application::~Application() {}

  void Application::Run() {
    m_Window->Show();
    while (m_IsRunning) {
      ExecuteMainThreadQueue();

      if (!m_IsMinimized) {
      }

      m_Window->OnUpdate();
    }
  }

  void Application::OnEvent(Event &event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(HK_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(HK_BIND_EVENT_FN(Application::OnWindowResize));
  }

  void Application::SubmitToMainThread(const std::function<void()> &function) {
    std::scoped_lock lock(m_MainThreadQueueMutex);
    m_MainThreadQueue.emplace_back(function);
  }

  void Application::Close() {
    m_IsRunning = false;
  }

  // ================================================================

  void Application::ExecuteMainThreadQueue() {
    std::scoped_lock lock(m_MainThreadQueueMutex);

    for (auto &func : m_MainThreadQueue) {
      func();
    }

    m_MainThreadQueue.clear();
  }

  // Events
  // ================================================================

  bool Application::OnWindowClose(const WindowCloseEvent &event) {
    (void)event;

    Close();
    return true;
  }

  bool Application::OnWindowResize(const WindowResizeEvent &event) {
    (void)event;
    return false;
  }

} // namespace hub33k
