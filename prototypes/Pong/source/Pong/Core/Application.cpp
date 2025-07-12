#include "Application.hpp"

#include "RootFallCore/Events/KeyEvent.hpp"
#include "RootFallCore/Events/MouseEvent.hpp"
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>

namespace hub33k {

  // Returns true on success or false on failure
  // https://wiki.libsdl.org/SDL3/SDL_AddEventWatch
  static bool HandleSDLEvents(void *user_data, SDL_Event *event) {
    WindowData &data = *static_cast<WindowData *>(user_data);

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
      data.Width = event->window.data1;
      data.Height = event->window.data2;

      WindowResizeEvent resizeEvent(data.Width, data.Height);
      data.EventCallback(resizeEvent);

      // std::println("Window resized to {}x{}", data.Width, data.Height);
    }

    if (event->type == SDL_EVENT_QUIT) {
      WindowCloseEvent closeEvent;
      data.EventCallback(closeEvent);
    }

    // TODO (hub33k): handle key repeat
    // KeyPressedEvent event(key, true);
    // data.EventCallback(event);

    if (event->type == SDL_EVENT_KEY_DOWN) {
      KeyPressedEvent keyPressedEvent(event->key.key, false);
      data.EventCallback(keyPressedEvent);
    }

    if (event->type == SDL_EVENT_KEY_UP) {
      KeyReleasedEvent keyReleasedEvent(event->key.key);
      data.EventCallback(keyReleasedEvent);
    }

    // TODO (hub33k): handle more events

    return true;
  }

  Application *Application::s_Instance = nullptr;

  Application::Application(const ApplicationSpecification &specification) : m_Specification(specification) {
    HK_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    Init();
  }

  Application::~Application() {
    Shutdown();
  }

  void Application::Run() {
    // TODO (hub33k): implement the game loop
    //   https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75

    SDL_ShowWindow(m_Window);
    while (m_IsRunning) {
      ExecuteMainThreadQueue();

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);

        if (event.type == SDL_EVENT_WINDOW_MINIMIZED) {
          m_IsMinimized = true;
        }

        if (event.type == SDL_EVENT_WINDOW_RESTORED) {
          m_IsMinimized = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
          if (event.key.key == SDLK_ESCAPE) {
            Close();
          }
        }
      }

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

      // m_Window->OnUpdate();
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

  void Application::Init() {
    m_WindowData.Title = m_Specification.Name;
    m_WindowData.Width = 1280;
    m_WindowData.Height = 720;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      HK_CORE_ASSERT(false, "SDL_Init failed")
    }

    constexpr SDL_WindowFlags windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    // TODO (hub33k): fix for mac - SDL_WINDOW_HIGH_PIXEL_DENSITY;
    m_Window = SDL_CreateWindow(m_WindowData.Title.c_str(), m_WindowData.Width, m_WindowData.Height, windowFlags);
    HK_CORE_ASSERT(m_Window, "SDL_CreateWindow failed");
    HK_LOG_CORE_INFO("Creating window: \"{0}\" ({1}x{2})", m_WindowData.Title, m_WindowData.Width, m_WindowData.Height);

    // Center window
    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_AddEventWatch(HandleSDLEvents, &m_WindowData);
    SetEventCallback(HK_BIND_EVENT_FN(Application::OnEvent));

    m_Renderer = CreateScope<Renderer>(*m_Window);
  }

  void Application::Shutdown() {
    HK_LOG_CORE_INFO("Shutting down \"{0}\"...", m_WindowData.Title);

    SDL_RemoveEventWatch(HandleSDLEvents, &m_WindowData);

    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

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
    m_Renderer->ConfigureSurface(static_cast<int>(event.GetWidth()), static_cast<int>(event.GetHeight()), true);

    return false;
  }

} // namespace hub33k
