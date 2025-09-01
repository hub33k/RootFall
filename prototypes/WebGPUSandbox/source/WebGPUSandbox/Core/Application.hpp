#pragma once

namespace hub33k {

  struct WindowProps {
    int Width = 800;
    int Height = 600;
    std::string Title = "WebGPU Sandbox";
    SDL_WindowFlags Flags = 0;
    bool IsVsync = true;
  };

  class Application {
  public:
    Application();
    virtual ~Application();

    void Run();

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
    void Frame();
    void Init();
    void Shutdown();

    void InitWebGPU();

    void ConfigureSurface(int width, int height, bool vsync);
  };

} // namespace hub33k
