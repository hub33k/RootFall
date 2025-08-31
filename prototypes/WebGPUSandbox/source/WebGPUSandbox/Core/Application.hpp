#pragma once

namespace hub33k {

  struct WindowProps {
    int Width = 800;
    int Height = 600;
    std::string Title = "WebGPUSandbox";
    SDL_WindowFlags Flags = 0;
  };

  class Application {
  public:
    Application();
    virtual ~Application();

  private:
    bool m_IsRunning = true;
    WindowProps m_WindowProps;
    SDL_Window *m_Window = nullptr;

    wgpu::Instance m_Instance = nullptr;
    wgpu::Adapter m_Adapter = nullptr;
    wgpu::Device m_Device = nullptr;
    wgpu::Queue m_Queue = nullptr;
    wgpu::Surface m_Surface = nullptr;
    wgpu::TextureFormat m_PreferredSurfaceTextureFormat = wgpu::TextureFormat::Undefined;
    wgpu::SurfaceConfiguration m_SurfaceConfiguration = {};

  private:
    void Init();
    void Shutdown();
    void Run();
    void Frame();
  };

} // namespace hub33k
