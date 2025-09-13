#include "Renderer.hpp"

#include "RootFall/Game/Application.hpp"
#include "RootFall/Platform/WebGPU/WebGPUUtils.hpp"
#include "RootFall/Utils/Utils.hpp"

namespace hub33k {

  // Create the toggles descriptor if not using emscripten.
  static wgpu::ChainedStruct *togglesChain = nullptr;
  static std::vector<std::string> enableToggles;
  static std::vector<std::string> disableToggles;

  wgpu::RenderPipeline CreateRenderPipeline(const wgpu::Device &device, const wgpu::TextureFormat &format) {
    std::string shaderSrc = ReadFile(SHADERS_DIR("shader.wgsl"));

    wgpu::ShaderSourceWGSL wgsl{{
      .nextInChain = nullptr,
      .code = shaderSrc.c_str(),
    }};

    wgpu::ShaderModuleDescriptor shaderModuleDescriptor{
      .nextInChain = &wgsl,
    };
    wgpu::ShaderModule shaderModule = device.CreateShaderModule(&shaderModuleDescriptor);

    wgpu::ColorTargetState colorTargetState{
      .format = format,
    };

    wgpu::FragmentState fragmentState{
      .module = shaderModule,
      .targetCount = 1,
      .targets = &colorTargetState,
    };

    wgpu::RenderPipelineDescriptor descriptor{
      .vertex =
        {
          .module = shaderModule,
        },
      .fragment = &fragmentState,
    };

    return device.CreateRenderPipeline(&descriptor);
  }

  void Renderer::Init(SDL_Window *window) {
    Application &app = Application::Get();

    InitWebGPU(window);
    ConfigureSurface(app.GetWidth(), app.GetHeight());

    m_Pipeline = CreateRenderPipeline(m_Device, m_PreferredSurfaceTextureFormat);
  }

  void Renderer::Shutdown() {
    ShutdownWebGPU();
  }

  void Renderer::Display() {
#if !HK_PLATFORM_IS(EMSCRIPTEN)
    m_Device.Tick();
#endif
    const wgpu::ConvertibleStatus isOk = m_Surface.Present();
    if (!isOk) {
      HK_LOG_ERROR("Failed to present surface");
    }

    m_Instance.ProcessEvents();
  }

  void Renderer::ConfigureSurface(const int width, const int height, const bool vsync) {
    m_SurfaceConfiguration = WebGPU::CreateSurfaceConfiguration(
      width, height, vsync, m_Surface, m_Adapter, m_Device, m_PreferredSurfaceTextureFormat
    );
  }

  void Renderer::BeginScene(const wgpu::Color &clearColor) {
    wgpu::SurfaceTexture surfaceTexture;
    m_Surface.GetCurrentTexture(&surfaceTexture);

    wgpu::RenderPassColorAttachment attachment{
      .view = surfaceTexture.texture.CreateView(),
      .loadOp = wgpu::LoadOp::Clear,
      .storeOp = wgpu::StoreOp::Store,
      .clearValue = clearColor,
    };

    wgpu::RenderPassDescriptor renderpass{
      .colorAttachmentCount = 1,
      .colorAttachments = &attachment,
    };

    m_CommandEncoder = m_Device.CreateCommandEncoder();
    m_Pass = m_CommandEncoder.BeginRenderPass(&renderpass);

    m_Pass.SetPipeline(m_Pipeline);
    m_Pass.Draw(3);
  }

  void Renderer::EndScene() {
    m_Pass.End();
    wgpu::CommandBuffer commands = m_CommandEncoder.Finish();
    m_Queue.Submit(1, &commands);
  }

  // ================================================================

  static bool isDrawing = false;

  void Renderer::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {
    HK_UNUSED(position);
    HK_UNUSED(size);
    HK_UNUSED(color);

    if (!isDrawing) {
      std::cout << "DrawQuad\n";
    }
    isDrawing = true;
  }

  // ================================================================

  void Renderer::InitWebGPU(SDL_Window *window) {
#if !HK_PLATFORM_IS(EMSCRIPTEN)
    // Create the toggles descriptor if not using emscripten.
    std::vector<const char *> enableToggleNames;
    enableToggleNames.push_back("enable_immediate_error_handling");
    std::vector<const char *> disabledToggleNames;
    for (const std::string &toggle : enableToggles) {
      enableToggleNames.push_back(toggle.c_str());
      std::cout << "Enabled toggle: " << toggle << '\n';
    }
    for (const std::string &toggle : disableToggles) {
      disabledToggleNames.push_back(toggle.c_str());
      std::cout << "Disabled toggle: " << toggle << '\n';
    }

    wgpu::DawnTogglesDescriptor toggles = {};
    toggles.enabledToggles = enableToggleNames.data();
    toggles.enabledToggleCount = enableToggleNames.size();
    toggles.disabledToggles = disabledToggleNames.data();
    toggles.disabledToggleCount = disabledToggleNames.size();

    togglesChain = &toggles;
#endif

    m_Instance = WebGPU::CreateInstance(togglesChain);
    m_Surface = WebGPU::CreateSurface(m_Instance, window);
    m_Adapter = WebGPU::CreateAdapter(m_Instance, m_Surface, togglesChain);
    m_Device = WebGPU::CreateDevice(m_Instance, m_Adapter);
    m_Queue = m_Device.GetQueue();

    // std::cout << "Instance: " << m_Instance.Get() << '\n';
    // std::cout << "Surface: " << m_Surface.Get() << '\n';
    // std::cout << "Adapter: " << m_Adapter.Get() << '\n';
    // std::cout << "Device: " << m_Device.Get() << '\n';
    // std::cout << "Queue:: " << m_Queue.Get() << '\n';

#if !HK_PLATFORM_IS(EMSCRIPTEN)
    // WebGPU::Info::DumpAdapterInfo(m_Adapter);
    // WebGPU::Info::DumpAdapter(m_Adapter);
    // WebGPU::Info::Test(m_Adapter, m_Device);
#endif
  }

  void Renderer::ShutdownWebGPU() {}

} // namespace hub33k
