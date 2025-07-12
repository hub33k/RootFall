#pragma once

namespace hub33k {

  class Renderer final {
  public:
    explicit Renderer(SDL_Window &window);
    ~Renderer();

    void BeginFrame();
    void EndFrame();

    /// Swap buffers.
    void Display();

    void ConfigureSurface(int width, int height, bool vsync = true);

  private:
    SDL_Window &m_Window;
    wgpu::Instance m_Instance;
    wgpu::Adapter m_Adapter;
    wgpu::Surface m_Surface;
    wgpu::Device m_Device;
    wgpu::Queue m_Queue;
    wgpu::TextureFormat m_PreferredSurfaceTextureFormat = wgpu::TextureFormat::Undefined;
    wgpu::SurfaceConfiguration m_SurfaceConfiguration;

    wgpu::RenderPassEncoder m_Pass;
    wgpu::CommandEncoder m_CommandEncoder;
    wgpu::RenderPipeline m_Pipeline;

  private:
    void Init();
    void Shutdown();

    void InitImGui();
    void ShutdownImGui();

    void CreatePipeline();
  };

} // namespace hub33k
