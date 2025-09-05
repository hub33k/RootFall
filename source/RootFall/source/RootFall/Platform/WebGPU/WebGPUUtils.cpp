#include "WebGPUUtils.hpp"

#include <sdl3webgpu.h>

namespace hub33k {

  // Create the toggles descriptor if not using emscripten.
  static wgpu::ChainedStruct *togglesChain = nullptr;
  static std::vector<std::string> enableToggles;
  static std::vector<std::string> disableToggles;

  wgpu::Instance CreateWGPUInstance() {
    wgpu::Instance instance = nullptr;

    // Create the instance with the toggles
    wgpu::InstanceDescriptor instanceDescriptor = {};
    instanceDescriptor.nextInChain = togglesChain;
    static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
    instanceDescriptor.requiredFeatureCount = 1;
    instanceDescriptor.requiredFeatures = &kTimedWaitAny;
    instance = wgpu::CreateInstance(&instanceDescriptor);

    HK_ASSERT(instance, "Failed to create wgpu::Instance")

    return instance;
  }

  wgpu::Surface CreateWGPUSurface(const wgpu::Instance &instance, SDL_Window *window) {
    HK_ASSERT(instance, "[CreateWGPUSurface] Instance is null");
    HK_ASSERT(window, "[CreateWGPUSurface] Failed to get SDL window")

    wgpu::Surface surface = wgpu::Surface::Acquire(SDL_GetWGPUSurface(instance.Get(), window));

    HK_ASSERT(surface, "[CreateWGPUSurface] Failed to create wgpu::Surface")

    return surface;
  }

  wgpu::Adapter CreateWGPUAdapter(const wgpu::Instance &instance, const wgpu::Surface &surface) {
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

    instance.WaitAny(
      instance.RequestAdapter(
        &adapterOptions, wgpu::CallbackMode::WaitAnyOnly,
        [](
          wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message, wgpu::Adapter *userdata
        ) {
          if (status != wgpu::RequestAdapterStatus::Success) {
            HK_LOG_ERROR("[CreateAdapter] Failed to get an adapter: {0}", message.data);
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

  wgpu::Device CreateWGPUDevice(const wgpu::Instance &instance, const wgpu::Adapter &adapter) {
    HK_ASSERT(adapter, "[CreateWGPUDevice] Adapter is null");
    HK_ASSERT(instance, "[CreateWGPUDevice] Instance is null");

    wgpu::DeviceDescriptor deviceDescriptor = {};
    deviceDescriptor.nextInChain = nullptr;
    deviceDescriptor.label = "Default Device";

    deviceDescriptor.requiredFeatures = nullptr;
    deviceDescriptor.requiredLimits = nullptr; // Default limits are minimal limits

    deviceDescriptor.SetDeviceLostCallback(
      wgpu::CallbackMode::AllowSpontaneous,
      [](const wgpu::Device &, wgpu::DeviceLostReason reason, wgpu::StringView message) {
        const char *reasonName = "";
        switch (reason) {
          case wgpu::DeviceLostReason::Unknown: {
            reasonName = "Unknown";
            break;
          }
          case wgpu::DeviceLostReason::Destroyed: {
            reasonName = "Destroyed";
            break;
          }
          case wgpu::DeviceLostReason::CallbackCancelled: {
            reasonName = "CallbackCancelled";
            break;
          }
          // case wgpu::DeviceLostReason::InstanceDropped: {
          //   reasonName = "InstanceDropped";
          //   break;
          // }
          case wgpu::DeviceLostReason::FailedCreation: {
            reasonName = "FailedCreation";
            break;
          }
          default: {
            // DAWN_UNREACHABLE();
            HK_ASSERT(false, "Unknown DeviceLostReason")
          }
        }

        // TODO (hub33k): is it OK?
        // The device is always “lost” when it is destroyed by the ultimate call
        // to wgpuDeviceRelease. It may also be lost for other reasons, mostly
        // meaning that the backend implementation panicked and crashed.
        if (strcmp(reasonName, "Destroyed") == 0) {
          HK_LOG_INFO("Device lost because of {}: {}", reasonName, message.data);
          return;
        }

        HK_LOG_WARN("Device lost because of {}: {}", reasonName, message.data);
      }
    );
    deviceDescriptor.SetUncapturedErrorCallback(
      [](const wgpu::Device &, wgpu::ErrorType type, wgpu::StringView message) {
        const char *errorTypeName = "";
        switch (type) {
          case wgpu::ErrorType::Validation: {
            errorTypeName = "Validation";
            break;
          }
          case wgpu::ErrorType::OutOfMemory: {
            errorTypeName = "Out of memory";
            break;
          }
          case wgpu::ErrorType::Unknown: {
            errorTypeName = "Unknown";
            break;
          }
          // case wgpu::ErrorType::DeviceLost: {
          //   errorTypeName = "Device lost";
          //   break;
          // }
          // case wgpu::ErrorType::Internal: {
          //   errorTypeName = "Internal";
          //   break;
          // }
          // case wgpu::ErrorType::NoError: {
          //   errorTypeName = "No error";
          //   break;
          // }
          default: {
            // DAWN_UNREACHABLE();
            HK_ASSERT(false, "Unknown ErrorType")
          }
        }
        HK_LOG_ERROR("Device error: {} - {}", errorTypeName, message.data);
      }
    );

    wgpu::Device outDevice = nullptr;
    instance.WaitAny(
      adapter.RequestDevice(
        &deviceDescriptor, wgpu::CallbackMode::WaitAnyOnly,
        [](wgpu::RequestDeviceStatus status, wgpu::Device device, wgpu::StringView message, wgpu::Device *userdata) {
          if (status != wgpu::RequestDeviceStatus::Success) {
            HK_LOG_ERROR("Failed to get an device: {0}", message.data);
            return;
          }
          *userdata = std::move(device);
        },
        &outDevice
      ),
      UINT64_MAX
    );

    HK_ASSERT(outDevice, "[CreateWGPUDevice] Failed to create device");

    return outDevice;
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

    context.Instance = CreateWGPUInstance();
    context.Surface = CreateWGPUSurface(context.Instance, window);
    context.Adapter = CreateWGPUAdapter(context.Instance, context.Surface);
    context.Device = CreateWGPUDevice(context.Instance, context.Adapter);
    context.Queue = context.Device.GetQueue();
  }

  void DeinitWebGPU(WebGPUContext &context) {
    HK_UNUSED(context);
  }

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    const int width, const int height, const bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
    const wgpu::Device &device, wgpu::TextureFormat &preferredSurfaceTextureFormat
  ) {
    wgpu::SurfaceCapabilities capabilities;
    surface.GetCapabilities(adapter, &capabilities);

    // wgpu::TextureFormat::RGBA8Unorm,
    preferredSurfaceTextureFormat = capabilities.formats[0];
    // preferredSurfaceTextureFormat = wgpu::TextureFormat::RGBA8Unorm; // NOTE (hub33k): override

    wgpu::SurfaceConfiguration config = {};
    config.device = device;
    config.format = preferredSurfaceTextureFormat;
    config.usage = wgpu::TextureUsage::RenderAttachment;
    // config.presentMode = wgpu::PresentMode::Fifo;

    config.width = static_cast<uint32_t>(width);
    config.height = static_cast<uint32_t>(height);

    // Extra
    config.nextInChain = nullptr;
    config.viewFormatCount = 0;
    config.viewFormats = nullptr;
    config.alphaMode = wgpu::CompositeAlphaMode::Auto;

#if HK_PLATFORM_IS(EMSCRIPTEN)
    HK_UNUSED(vsync);
    config.presentMode = wgpu::PresentMode::Fifo;
#else
    config.presentMode = vsync ? wgpu::PresentMode::Fifo : wgpu::PresentMode::Immediate;
#endif

    surface.Configure(&config);

    HK_LOG_INFO("[SurfaceConfiguration] Resized to {0}x{1}", width, height);

    return config;
  }

} // namespace hub33k
