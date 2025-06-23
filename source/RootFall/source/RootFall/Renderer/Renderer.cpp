#include "Renderer.hpp"

// TODO (hub33k): for now using SDL as window - check CreateSurface below (in Init)

#include "RootFall/Platform/WebGPU/WebGPUUtils.hpp"

namespace hub33k {

  Renderer::Renderer(Window &window) : m_Window(window) {
    Init();
  }

  Renderer::~Renderer() {
    Shutdown();
  }

  void Renderer::BeginFrame() {}

  void Renderer::EndFrame() {}

  void Renderer::ConfigureSurface(const int width, const int height, const bool vsync) {
    m_SurfaceConfiguration =
      CreateSurfaceConfiguration(width, height, vsync, m_Surface, m_Adapter, m_Device, m_PreferredSurfaceTextureFormat);
  }

  // ================================================================

  void Renderer::Init() {
    m_Instance = CreateInstance();
    m_Surface = CreateSurface(m_Instance, static_cast<SDL_Window *>(m_Window.GetNativeHandle()));
    m_Adapter = CreateAdapter(m_Instance, m_Surface);
    m_Device = CreateDevice(m_Instance, m_Adapter);
    m_Queue = m_Device.GetQueue();

    ConfigureSurface(m_Window.GetWidth(), m_Window.GetHeight(), m_Window.IsVSync());
  }

  void Renderer::Shutdown() {}

  void Renderer::CreatePipeline() {}

} // namespace hub33k
