#include "Renderer.hpp"

#include "RootFall/Game/Application.hpp"
#include "RootFall/Utils/Utils.hpp"

namespace hub33k {

  struct Renderer2DData {
    static const uint32_t MaxQuads = 20000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
  };

  static Renderer2DData s_Data;

  WebGPUContext Renderer::Context = {};

  wgpu::RenderPassEncoder pass;
  wgpu::CommandEncoder encoder;
  wgpu::RenderPipeline pipeline;

  void CreateRenderPipeline(const wgpu::Device &device, const wgpu::TextureFormat &format) {
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
    pipeline = device.CreateRenderPipeline(&descriptor);
  }

  void Renderer::Init() {
    Application &app = Application::Get();

    InitWebGPU(app.GetWindow(), Context);

    // std::cout << "Instance: " << m_WebGPUContext.Instance.Get() << '\n';
    // std::cout << "Surface: " << m_WebGPUContext.Surface.Get() << '\n';
    // std::cout << "Adapter: " << m_WebGPUContext.Adapter.Get() << '\n';
    // std::cout << "Device: " << m_WebGPUContext.Device.Get() << '\n';
    // std::cout << "Queue:: " << m_WebGPUContext.Queue.Get() << '\n';

    // int w, h;
    // SDL_GetWindowSize(app.GetWindow(), &w, &h);
    ConfigureSurface(app.GetWidth(), app.GetHeight());

    CreateRenderPipeline(Context.Device, Context.PreferredSurfaceTextureFormat);
  }

  void Renderer::Shutdown() {
    DeinitWebGPU(Context);
  }

  void Renderer::Display() {
#if !HK_PLATFORM_IS(EMSCRIPTEN)
    Context.Device.Tick();
#endif
    Context.Surface.Present();
    Context.Instance.ProcessEvents();
  }

  void Renderer::ConfigureSurface(const int width, const int height, const bool vsync) {
    Context.SurfaceConfiguration = CreateSurfaceConfiguration(
      width, height, vsync, Context.Surface, Context.Adapter, Context.Device, Context.PreferredSurfaceTextureFormat
    );
  }

  void Renderer::BeginScene(const wgpu::Color &clearColor) {
    wgpu::SurfaceTexture surfaceTexture;
    Context.Surface.GetCurrentTexture(&surfaceTexture);

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

    encoder = Context.Device.CreateCommandEncoder();
    pass = encoder.BeginRenderPass(&renderpass);

    pass.SetPipeline(pipeline);
    pass.Draw(3);
  }

  void Renderer::EndScene() {
    pass.End();
    wgpu::CommandBuffer commands = encoder.Finish();
    Context.Queue.Submit(1, &commands);
  }

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

} // namespace hub33k
