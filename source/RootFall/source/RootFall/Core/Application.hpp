#pragma once

#include "RootFall/Platform/Window.hpp"
#include "RootFall/Renderer/Renderer.hpp"
#include "RootFallCore/Events/ApplicationEvent.hpp"
#include "RootFallCore/Events/Event.hpp"

namespace hub33k {

  struct ApplicationCommandLineArgs {
    int Count = 0;
    char **Args = nullptr;

    const char *operator[](const int index) const {
      HK_CORE_ASSERT(index < Count);
      return Args[index];
    }
  };

  struct ApplicationSpecification {
    std::string Name = "RootFall";
    std::string WorkingDirectory;
    ApplicationCommandLineArgs CommandLineArgs;
  };

  class Application final {
  public:
    static Application &Get() { return *s_Instance; }

  public:
    Application(const ApplicationSpecification &specification);
    ~Application();

    void Run();
    void OnEvent(Event &event);
    void Close();
    void SubmitToMainThread(const std::function<void()> &function);

    // ================================================================

    Window &GetWindow() { return *m_Window; }
    const ApplicationSpecification &GetSpecification() const { return m_Specification; }

  private:
    static Application *s_Instance;

  private:
    bool m_IsRunning = true;
    bool m_IsMinimized = false;
    float m_LastFrameTime = 0.0f;
    ApplicationSpecification m_Specification;

    std::vector<std::function<void()>> m_MainThreadQueue;
    std::mutex m_MainThreadQueueMutex;

    Scope<Window> m_Window;
    Scope<Renderer> m_Renderer;
    entt::registry m_Registry;

  private:
    void ExecuteMainThreadQueue();

    // Events
    bool OnWindowClose(const WindowCloseEvent &event);
    bool OnWindowResize(const WindowResizeEvent &event);
  };

} // namespace hub33k
