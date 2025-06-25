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
    wgpu::TextureViewDescriptor viewDescriptor;
    viewDescriptor.nextInChain = nullptr;
    viewDescriptor.label = "Surface texture view";
    viewDescriptor.format = m_PreferredSurfaceTextureFormat; // surfaceTexture.texture.GetFormat()
    viewDescriptor.dimension = wgpu::TextureViewDimension::e2D;
    viewDescriptor.baseMipLevel = 0;
    viewDescriptor.mipLevelCount = 1;
    viewDescriptor.baseArrayLayer = 0;
    viewDescriptor.arrayLayerCount = 1;
    viewDescriptor.aspect = wgpu::TextureAspect::All;
    wgpu::TextureView targetView = surfaceTexture.texture.CreateView(&viewDescriptor);

    // Create a command encoder for the draw call
    wgpu::CommandEncoderDescriptor encoderDesc = {};
    encoderDesc.nextInChain = nullptr;
    encoderDesc.label = "My command encoder";
    m_CommandEncoder = m_Device.CreateCommandEncoder(&encoderDesc);

    // Create the render pass that clears the screen with our color
    wgpu::RenderPassDescriptor renderPassDesc = {};
    renderPassDesc.nextInChain = nullptr;

    // The attachment part of the render pass descriptor describes the target texture of the pass
    wgpu::RenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = targetView;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = wgpu::LoadOp::Clear;
    renderPassColorAttachment.storeOp = wgpu::StoreOp::Store;
    renderPassColorAttachment.clearValue = wgpu::Color{0.1, 0.1, 0.1, 1.0};

    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = nullptr;
    renderPassDesc.timestampWrites = nullptr;

    // Create the render pass and end it immediately (we only clear the screen but do not draw anything)
    m_Pass = m_CommandEncoder.BeginRenderPass(&renderPassDesc);

    // render here

    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ImGui::ShowDemoWindow();

    ImGui::Begin("Test");
    ImGui::End();

    ImGui::Render();
  }

  void Renderer::EndFrame() {
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), m_Pass.Get());
    m_Pass.End();

    // Finally encode and submit the render pass
    wgpu::CommandBufferDescriptor cmdBufferDescriptor = {};
    cmdBufferDescriptor.nextInChain = nullptr;
    cmdBufferDescriptor.label = "Command buffer";
    const wgpu::CommandBuffer command = m_CommandEncoder.Finish(&cmdBufferDescriptor);

    m_Queue.Submit(1, &command);

    m_Surface.Present();
    m_Device.Tick();
    m_Instance.ProcessEvents();
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
