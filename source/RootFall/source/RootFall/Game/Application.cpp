#include "Application.hpp"

wgpu::RenderPipeline pipeline;
const char shaderCode[] = R"(
  @vertex fn vertexMain(@builtin(vertex_index) i : u32) -> @builtin(position) vec4f {
    const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
    return vec4f(pos[i], 0, 1);
  }
  @fragment fn fragmentMain() -> @location(0) vec4f {
    return vec4f(1, 0, 1, 1);
  }
)";

void CreateRenderPipeline(const wgpu::Device &device, const wgpu::TextureFormat &format) {
  wgpu::ShaderSourceWGSL wgsl{{
    .nextInChain = nullptr,
    .code = shaderCode,
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
    .vertex = {.module = shaderModule},
    .fragment = &fragmentState,
  };
  pipeline = device.CreateRenderPipeline(&descriptor);
}

namespace hub33k {

  Application *Application::s_Instance = nullptr;

  Application::Application() {
    HK_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      HK_LOG_ERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
      HK_ASSERT(false, "Failed to initialize SDL")
    }

    m_WindowProps.Flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    m_Window =
      SDL_CreateWindow(m_WindowProps.Title.c_str(), m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.Flags);
    if (!m_Window) {
      SDL_Quit();
      HK_LOG_ERROR("Window could not be created! SDL_Error: {}", SDL_GetError());
      HK_ASSERT(m_Window, "Failed to create SDL window")
    }
    HK_LOG_INFO("Creating window: \"{0}\" ({1}x{2})", m_WindowProps.Title, m_WindowProps.Width, m_WindowProps.Height);

    // Center window
    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    InitWebGPU(m_Window, m_WebGPUContext);

    // std::cout << "Instance: " << m_WebGPUContext.Instance.Get() << '\n';
    // std::cout << "Surface: " << m_WebGPUContext.Surface.Get() << '\n';
    // std::cout << "Adapter: " << m_WebGPUContext.Adapter.Get() << '\n';
    // std::cout << "Device: " << m_WebGPUContext.Device.Get() << '\n';
    // std::cout << "Queue:: " << m_WebGPUContext.Queue.Get() << '\n';

    m_WebGPUContext.SurfaceConfiguration = CreateSurfaceConfiguration(
      m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.VSync, m_WebGPUContext.Surface, m_WebGPUContext.Adapter,
      m_WebGPUContext.Device, m_WebGPUContext.PreferredSurfaceTextureFormat
    );

    CreateRenderPipeline(m_WebGPUContext.Device, m_WebGPUContext.PreferredSurfaceTextureFormat);
  }

  Application::~Application() {
    // std::println("Shutting down...");
    DeinitWebGPU(m_WebGPUContext);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

  // ================================================================

  void Application::Run() {
    SDL_ShowWindow(m_Window);

    // https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75

#if HK_PLATFORM_IS(EMSCRIPTEN)
    emscripten_set_main_loop_arg(
      [](void *userData) {
        Application &app = *reinterpret_cast<Application *>(userData);

        Timestep ts = Timestep(1.0f / 60.0f);
        app.ProcessEvents();
        app.Update(Timestep(ts));
        app.Render(Timestep(ts));
        // emscripten_sleep(100);
      },
      (void *)this, 0, true
    );
#else
    Timestep ts = Timestep(1.0f / 60.0f);

    while (m_IsRunning) {
      ProcessEvents();
      Update(Timestep(ts));
      Render(Timestep(ts));

      m_WebGPUContext.Surface.Present();
      m_WebGPUContext.Instance.ProcessEvents();
    }
#endif
  }

  void Application::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        Close();
      }

      if (event.type == SDL_EVENT_WINDOW_RESIZED) {
        m_WindowProps.Width = event.window.data1;
        m_WindowProps.Height = event.window.data2;

        m_WebGPUContext.SurfaceConfiguration = CreateSurfaceConfiguration(
          m_WindowProps.Width, m_WindowProps.Height, m_WindowProps.VSync, m_WebGPUContext.Surface,
          m_WebGPUContext.Adapter, m_WebGPUContext.Device, m_WebGPUContext.PreferredSurfaceTextureFormat
        );
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          Close();
        }

        if (event.key.key == SDLK_RETURN) {
          HK_LOG_INFO("Enter");
        }
      }
    }
  }

  void Application::Update(const Timestep ts) {
    (void)ts;
  }

  void Application::Render(const Timestep ts) {
    (void)ts;

    wgpu::SurfaceTexture surfaceTexture;
    m_WebGPUContext.Surface.GetCurrentTexture(&surfaceTexture);

    wgpu::RenderPassColorAttachment attachment{
      .view = surfaceTexture.texture.CreateView(),
      .loadOp = wgpu::LoadOp::Clear,
      .storeOp = wgpu::StoreOp::Store,
      // .clearValue = {1.0f, 1.0f, 1.0f, 1.0f},
      .clearValue = {0.0f, 0.0f, 0.0f, 1.0f},
    };

    wgpu::RenderPassDescriptor renderpass{
      .colorAttachmentCount = 1,
      .colorAttachments = &attachment,
    };

    wgpu::CommandEncoder encoder = m_WebGPUContext.Device.CreateCommandEncoder();
    wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
    pass.SetPipeline(pipeline);
    pass.Draw(3);
    pass.End();
    wgpu::CommandBuffer commands = encoder.Finish();
    m_WebGPUContext.Queue.Submit(1, &commands);
  }

  // ================================================================

  void Application::Close() {
    m_IsRunning = false;

#if HK_PLATFORM_IS(EMSCRIPTEN)
    // emscripten_cancel_main_loop();
#endif
  }

} // namespace hub33k
