#include "Application.hpp"

namespace hub33k {

  Application *Application::s_Instance = nullptr;

  Application::Application() {
    HK_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      HK_LOG_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
      HK_ASSERT(false, "Failed to initialize SDL")
    }

    m_WindowProps.Flags = SDL_WINDOW_HIDDEN;
    m_Window =
      SDL_CreateWindow(m_WindowProps.Title.c_str(), m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Flags);
    if (!m_Window) {
      SDL_Quit();
      HK_LOG_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());
      HK_ASSERT(m_Window, "Failed to create SDL window")
    }
    HK_LOG_INFO("Creating window: \"{0}\" ({1}x{2})", m_WindowProps.Title, m_WindowProps.Width, m_WindowProps.Height);

    InitWebGPU(m_Window, m_WebGPUContext);

    std::cout << "Instance: " << m_WebGPUContext.Instance.Get() << '\n';
    std::cout << "Adapter: " << m_WebGPUContext.Adapter.Get() << '\n';
  }

  Application::~Application() {
    std::println("Shutting down...");
    DeinitWebGPU(m_WebGPUContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

  // ================================================================

  void Application::Run() {
    SDL_ShowWindow(m_Window);

    // https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75

#if HK_PLATFORM_IS(EMSCRIPTEN)
    emscripten_set_main_loop_arg(
      [](void *userData) {
        Application &app = *reinterpret_cast<Application *>(userData);

        Timestep ts = Timestep(1.0f / 60.0f);
        app.ProcessEvents();
        app.Update(Timestep(ts));
        app.Render(Timestep(ts));
      },
      (void *)this, 0, true
    );
#else
    Timestep ts = Timestep(1.0f / 60.0f);

    while (m_IsRunning) {
      ProcessEvents();
      Update(Timestep(ts));
      Render(Timestep(ts));
    }
#endif
  }

  void Application::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        Close();
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          Close();
        }

        if (event.key.key == SDLK_RETURN) {
          HK_LOG_INFO("Enter");
        }
      }
    }
  }

  void Application::Update(const Timestep ts) {
    (void)ts;
  }

  void Application::Render(const Timestep ts) {
    (void)ts;
  }

  // ================================================================

  void Application::Close() {
    m_IsRunning = false;

#if HK_PLATFORM_IS(EMSCRIPTEN)
    // emscripten_cancel_main_loop();
#endif
  }

} // namespace hub33k
