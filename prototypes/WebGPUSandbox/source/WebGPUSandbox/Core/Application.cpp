#include "Application.hpp"

#include "WebGPUSandbox/Platform/WebGPU/WebGPUUtils.hpp"

namespace hub33k {

  Application::Application() {
    Init();
  }

  Application::~Application() {
    Shutdown();
  }

  void Application::Run() {
    SDL_ShowWindow(m_Window);
    while (m_IsRunning) {
      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
          m_IsRunning = false;
        }
        if (event.type == SDL_EVENT_KEY_DOWN) {
          if (event.key.key == SDLK_ESCAPE) {
            m_IsRunning = false;
          }
        }
      }

      Frame();
    }
  }

  // ================================================================

  void Application::Frame() {}

  void Application::Init() {
    // true on success or false on failure
    if (!SDL_Init(SDL_INIT_VIDEO)) {
      std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
      HK_ASSERT(false, "Failed to initialize SDL")
    }

    m_WindowProps.Flags = SDL_WINDOW_HIDDEN;

    m_Window =
      SDL_CreateWindow(m_WindowProps.Title.c_str(), m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Flags);
    if (!m_Window) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
      SDL_Quit();
      HK_ASSERT(m_Window, "Failed to create SDL window")
    }
    HK_LOG_CORE_INFO(
      "Creating window: \"{0}\" ({1}x{2})", m_WindowProps.Title, m_WindowProps.Width, m_WindowProps.Height
    );

    // Center window
    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    InitWebGPU();
  }

  void Application::Shutdown() {
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

  void Application::InitWebGPU() {
    // m_Instance = CreateInstance();
    // std::cout << "Instance: " << m_Instance.Get() << std::endl;

    // m_Surface = CreateSurface(m_Instance, m_Window);
    // std::cout << "Surface: " << m_Surface.Get() << std::endl;

    // m_Adapter = CreateAdapter(m_Instance, m_Surface);
    // std::cout << "Adapter: " << m_Adapter.Get() << std::endl;

    // m_Device = CreateDevice(m_Instance, m_Adapter);
    // std::cout << "Device: " << m_Device.Get() << std::endl;

    // m_Queue = m_Device.GetQueue();
    // std::cout << "Queue: " << m_Queue.Get() << std::endl;

    // ConfigureSurface(m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.IsVsync);

    // std::cout << static_cast<std::underlying_type_t<wgpu::TextureFormat>>(m_PreferredSurfaceTextureFormat) << '\n';
    // std::cout << "PreferredSurfaceTextureFormat: " << TextureFormatToString(m_PreferredSurfaceTextureFormat) << '\n';

    // WebGPU::Info::DumpAdapter(m_Adapter);
    // WebGPU::Info::DumpDevice(m_Device);
  }

  void Application::ConfigureSurface(const int width, const int height, const bool vsync) {
    m_SurfaceConfiguration =
      CreateSurfaceConfiguration(width, height, vsync, m_Surface, m_Adapter, m_Device, m_PreferredSurfaceTextureFormat);
  }

} // namespace hub33k
