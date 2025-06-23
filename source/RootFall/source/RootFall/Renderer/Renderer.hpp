#pragma once

#include "RootFall/Platform/Window.hpp"

namespace hub33k {

  class Renderer final {
  public:
    explicit Renderer(Window &window);
    ~Renderer();

    void BeginFrame();
    void EndFrame();

    void ConfigureSurface(int width, int height, bool vsync = true);

  private:
    Window &m_Window;
    wgpu::Instance m_Instance;
    wgpu::Adapter m_Adapter;
    wgpu::Surface m_Surface;
    wgpu::Device m_Device;
    wgpu::Queue m_Queue;
    wgpu::TextureFormat m_PreferredSurfaceTextureFormat = wgpu::TextureFormat::Undefined;
    wgpu::SurfaceConfiguration m_SurfaceConfiguration;

    wgpu::RenderPipeline m_Pipeline;

  private:
    void Init();
    void Shutdown();

    void CreatePipeline();
  };

} // namespace hub33k
