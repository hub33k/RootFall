#pragma once

#include "Pong/Renderer/Renderer.hpp"
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

  using EventCallbackFn = std::function<void(Event &)>;

  struct WindowData {
    std::string Title;
    int Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  struct WindowProps {
    std::string Title;
    int Width;
    int Height;

    explicit WindowProps(const std::string &title = "Pong", const int width = 1280, const int height = 720)
      : Title(title), Width(width), Height(height) {}
  };

  class Application final {
  public:
    static Application &Get() { return *s_Instance; }

  public:
    explicit Application(const ApplicationSpecification &specification);
    ~Application();

    void Run();
    void OnEvent(Event &event);
    void Close();
    void SubmitToMainThread(const std::function<void()> &function);

    void SetEventCallback(const EventCallbackFn &callback) { m_WindowData.EventCallback = callback; }

    // ================================================================

    [[nodiscard]] WindowData GetWindowData() const { return m_WindowData; }
    [[nodiscard]] SDL_Window &GetWindow() const { return *m_Window; }
    [[nodiscard]] const ApplicationSpecification &GetSpecification() const { return m_Specification; }

    void SetVSync(const bool enabled) { m_WindowData.VSync = enabled; }
    [[nodiscard]] bool IsVSync() const { return m_WindowData.VSync; }

    int GetWindowWidth() const { return m_WindowData.Width; }
    int GetWindowHeight() const { return m_WindowData.Height; }

  private:
    static Application *s_Instance;

  private:
    // TODO (hub33k): add state struct?

    bool m_IsRunning = true;
    bool m_IsMinimized = false;
    // bool m_IsImGuiEnabled = true;
    // bool m_IsInitialized = false;
    float m_LastFrameTime = 0.0f;
    ApplicationSpecification m_Specification;

    std::vector<std::function<void()>> m_MainThreadQueue;
    std::mutex m_MainThreadQueueMutex;

    WindowData m_WindowData;
    SDL_Window *m_Window = nullptr;

    Scope<Renderer> m_Renderer;

    entt::registry m_Registry;

  private:
    void Init();
    void Shutdown();
    void ExecuteMainThreadQueue();

    // Events
    bool OnWindowClose(const WindowCloseEvent &event);
    bool OnWindowResize(const WindowResizeEvent &event);
  };

} // namespace hub33k
