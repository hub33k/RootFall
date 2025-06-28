#include "Application.hpp"

#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>

namespace hub33k {

  Application *Application::s_Instance = nullptr;

  Application::Application(const ApplicationSpecification &specification) : m_Specification(specification) {
    HK_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = Window::Create(WindowProps(m_Specification.Name));
    m_Window->SetEventCallback(HK_BIND_EVENT_FN(Application::OnEvent));

    m_Renderer = CreateScope<Renderer>(*m_Window);
  }

  Application::~Application() {}

  void Application::Run() {
    // TODO (hub33k): implement the game loop
    //   https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75

    m_Window->Show();
    while (m_IsRunning) {
      ExecuteMainThreadQueue();

      if (!m_IsMinimized) {
        m_Renderer->BeginFrame();

        // ImGui::ShowDemoWindow();

        ImGui::Begin("App");
        ImGui::Text(
          "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate
        );
        ImGui::End();

        m_Renderer->EndFrame();
      }

      m_Window->OnUpdate();
      m_Renderer->Display();
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
    m_Renderer->ConfigureSurface(
      static_cast<int>(event.GetWidth()), static_cast<int>(event.GetHeight()), m_Window->IsVSync()
    );

    return false;
  }

} // namespace hub33k
