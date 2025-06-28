#include "Renderer.hpp"

// TODO (hub33k): for now using SDL as window - check CreateSurface below (in Init)

#include "RootFall/Platform/WebGPU/WebGPUUtils.hpp"
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_wgpu.h>
#include <imgui.h>

namespace hub33k {

  Renderer::Renderer(Window &window) : m_Window(window) {
    Init();
  }

  Renderer::~Renderer() {
    Shutdown();
  }

  void Renderer::BeginFrame() {
    wgpu::SurfaceTexture surfaceTexture;
    m_Surface.GetCurrentTexture(&surfaceTexture);

    // Create a view for this surface texture
    const wgpu::TextureViewDescriptor viewDescriptor{
      .nextInChain = nullptr,
      .label = "Surface texture view",
      .format = m_PreferredSurfaceTextureFormat, // surfaceTexture.texture.GetFormat()
      .dimension = wgpu::TextureViewDimension::e2D,
      .baseMipLevel = 0,
      .mipLevelCount = 1,
      .baseArrayLayer = 0,
      .arrayLayerCount = 1,
      .aspect = wgpu::TextureAspect::All,
    };

    const wgpu::TextureView targetView = surfaceTexture.texture.CreateView(&viewDescriptor);

    // Create a command encoder for the draw call
    constexpr wgpu::CommandEncoderDescriptor encoderDesc = {
      .nextInChain = nullptr,
      .label = "My command encoder",
    };
    m_CommandEncoder = m_Device.CreateCommandEncoder(&encoderDesc);

    // The attachment part of the render pass descriptor describes the target texture of the pass
    const wgpu::RenderPassColorAttachment renderPassColorAttachment = {
      .view = targetView,
      .resolveTarget = nullptr,
      .loadOp = wgpu::LoadOp::Clear,
      .storeOp = wgpu::StoreOp::Store,
      .clearValue = wgpu::Color{0.1, 0.1, 0.1, 1.0},
    };

    // Create the render pass that clears the screen with our color
    const wgpu::RenderPassDescriptor renderPassDesc = {
      .nextInChain = nullptr,
      .colorAttachmentCount = 1,
      .colorAttachments = &renderPassColorAttachment,
      .depthStencilAttachment = nullptr,
      .timestampWrites = nullptr,
    };

    // Create the render pass and end it immediately (we only clear the screen but do not draw anything)
    m_Pass = m_CommandEncoder.BeginRenderPass(&renderPassDesc);

    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
  }

  void Renderer::EndFrame() {
    ImGui::Render();
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), m_Pass.Get());
    m_Pass.End();

    // Finally encode and submit the render pass
    constexpr wgpu::CommandBufferDescriptor cmdBufferDescriptor = {
      .nextInChain = nullptr,
      .label = "Command buffer",
    };
    const wgpu::CommandBuffer command = m_CommandEncoder.Finish(&cmdBufferDescriptor);

    m_Queue.Submit(1, &command);

    m_Device.Tick();
    m_Instance.ProcessEvents();
  }

  void Renderer::Display() {
    m_Surface.Present();
  }

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

    ConfigureSurface(static_cast<int>(m_Window.GetWidth()), static_cast<int>(m_Window.GetHeight()), m_Window.IsVSync());

    // WebGPU::Info::DumpAdapter(m_Adapter);
    // WebGPU::Info::DumpDevice(m_Device);

    CreatePipeline();
    InitImGui();
  }

  void Renderer::Shutdown() {
    ShutdownImGui();
  }

  void Renderer::InitImGui() {
    // https://github.com/bottosson/webgpu-sdl-imgui-cpp-starter/blob/main/src/main/main.cpp

    const auto window = static_cast<SDL_Window *>(m_Window.GetNativeHandle());

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

#if HK_PLATFORM_IS(WINDOWS)
    ImGui_ImplSDL3_InitForD3D(window);
#elif HK_PLATFORM_IS(MACOS)
    ImGui_ImplSDL3_InitForMetal(window);
#else
    ImGui_ImplSDL3_InitForVulkan(window);
#endif

    ImGui_ImplWGPU_InitInfo init_info = {};
    init_info.Device = m_Device.Get();
    init_info.RenderTargetFormat = static_cast<WGPUTextureFormat>(m_PreferredSurfaceTextureFormat);
    // init_info.DepthStencilFormat = WGPUTextureFormat_Depth24PlusStencil8;
    init_info.NumFramesInFlight = 3;
    ImGui_ImplWGPU_Init(&init_info);
  }

  void Renderer::ShutdownImGui() {
    ImGui_ImplWGPU_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
  }

  // ================================================================

  void Renderer::CreatePipeline() {}

} // namespace hub33k
