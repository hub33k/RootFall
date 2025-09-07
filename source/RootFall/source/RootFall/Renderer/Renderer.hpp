#pragma once

namespace hub33k {

  class Renderer {
  public:
    void Init(SDL_Window *window);
    void Shutdown();

    void Display();
    void ConfigureSurface(const int width, const int height, const bool vsync = true);

    void BeginScene(const wgpu::Color &clearColor = {0.45f, 0.55f, 0.60f, 1.00f});
    void EndScene();

    void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);

  private:
    // WebGPU
    wgpu::Instance m_Instance;
    wgpu::Adapter m_Adapter;
    wgpu::Device m_Device;
    wgpu::Queue m_Queue;
    wgpu::Surface m_Surface;
    wgpu::SurfaceConfiguration m_SurfaceConfiguration;
    wgpu::TextureFormat m_PreferredSurfaceTextureFormat = wgpu::TextureFormat::Undefined;

    // Per-Frame
    wgpu::RenderPassEncoder m_Pass;
    wgpu::CommandEncoder m_CommandEncoder;
    wgpu::RenderPipeline m_Pipeline;

  private:
    void InitWebGPU(SDL_Window *window);
    void ShutdownWebGPU();
  };

} // namespace hub33k
