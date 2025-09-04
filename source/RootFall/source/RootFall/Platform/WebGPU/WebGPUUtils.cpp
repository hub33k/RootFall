#include "WebGPUUtils.hpp"

namespace hub33k {

  // Create the toggles descriptor if not using emscripten.
  static wgpu::ChainedStruct *togglesChain = nullptr;
  static std::vector<std::string> enableToggles;
  static std::vector<std::string> disableToggles;

  wgpu::Instance CreateWGPUInstance() {
    wgpu::Instance instance = nullptr;

#if HK_PLATFORM_IS(EMSCRIPTEN)
    instance = wgpu::CreateInstance(nullptr);
#else
    // Create the instance with the toggles
    wgpu::InstanceDescriptor instanceDescriptor = {};
    instanceDescriptor.nextInChain = togglesChain;
    static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    instanceDescriptor.requiredFeatureCount = 1;
    instanceDescriptor.requiredFeatures = &kTimedWaitAny;
    instance = wgpu::CreateInstance(&instanceDescriptor);
#endif

    HK_ASSERT(instance, "Failed to create wgpu::Instance")

    return instance;
  }

  wgpu::Adapter CreateWGPUAdapter(const wgpu::Instance instance, const wgpu::Surface surface) {
    // Setup base adapter options with toggles.
    wgpu::RequestAdapterOptions adapterOptions = {
      .nextInChain = togglesChain,
      .featureLevel = wgpu::FeatureLevel::Core,
      .powerPreference = wgpu::PowerPreference::HighPerformance,
      .forceFallbackAdapter = false,
    };

    if (surface) {
      adapterOptions.compatibleSurface = surface;
    }

#if HK_PLATFORM_IS(WINDOWS)
    adapterOptions.backendType = wgpu::BackendType::D3D12;
#elif HK_PLATFORM_IS(MACOS)
    adapterOptions.backendType = wgpu::BackendType::Metal;
#else
    adapterOptions.backendType = wgpu::BackendType::Vulkan;
#endif

    wgpu::Adapter outAdapter = nullptr;

    // Synchronously create the adapter
    // instance.WaitAny(
    //   instance.RequestAdapter(
    //     &adapterOptions, wgpu::CallbackMode::WaitAnyOnly,
    //     [](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message) {
    //       if (status != wgpu::RequestAdapterStatus::Success) {
    //         // dawn::ErrorLog() << "Failed to get an adapter: " << message;
    //         HK_LOG_ERROR("Failed to get an adapter: {0}", message.data);
    //         return;
    //       }
    //       adapter = std::move(adapter);
    //     }
    //   ),
    //   UINT64_MAX
    // );

    instance.WaitAny(
      instance.RequestAdapter(
        &adapterOptions, wgpu::CallbackMode::WaitAnyOnly,
        [](
          wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message, wgpu::Adapter *userdata
        ) {
          if (status != wgpu::RequestAdapterStatus::Success) {
            HK_LOG_CORE_ERROR("[CreateAdapter] Failed to get an adapter: {0}", message.data);
            return;
          }
          *userdata = std::move(adapter);
        },
        &outAdapter
      ),
      UINT64_MAX
    );

    HK_ASSERT(outAdapter, "Failed to create wgpu::Adapter")

    return outAdapter;
  }

  // ================================================================

  void InitWebGPU(SDL_Window *window, WebGPUContext &context) {
    HK_UNUSED(window);

#if !HK_PLATFORM_IS(EMSCRIPTEN)
    // Create the toggles descriptor if not using emscripten.
    std::vector<const char *> enableToggleNames;
    enableToggleNames.push_back("enable_immediate_error_handling");
    std::vector<const char *> disabledToggleNames;
    for (const std::string &toggle : enableToggles) {
      enableToggleNames.push_back(toggle.c_str());
    }
    for (const std::string &toggle : disableToggles) {
      disabledToggleNames.push_back(toggle.c_str());
    }

    wgpu::DawnTogglesDescriptor toggles = {};
    toggles.enabledToggles = enableToggleNames.data();
    toggles.enabledToggleCount = enableToggleNames.size();
    toggles.disabledToggles = disabledToggleNames.data();
    toggles.disabledToggleCount = disabledToggleNames.size();

    togglesChain = &toggles;
#endif

    context.Instance = CreateWGPUInstance();
    context.Adapter = CreateWGPUAdapter(context.Instance, nullptr);
  }

  void DeinitWebGPU(WebGPUContext &context) {
    HK_UNUSED(context);
  }

} // namespace hub33k
