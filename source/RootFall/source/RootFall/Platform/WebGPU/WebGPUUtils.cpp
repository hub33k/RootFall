#include "WebGPUUtils.hpp"

#include <sdl3webgpu.h>

#if !HK_PLATFORM_IS(EMSCRIPTEN)
  #include "dawn/native/DawnNative.h"
  #include "dawn/webgpu_cpp_print.h"
#endif

namespace hub33k {

#if !HK_PLATFORM_IS(EMSCRIPTEN)
  // DawnInfo.cpp
  namespace WebGPU::Info {

    // Wraps a string to about 75 characters and prints indented. Splits on whitespace instead of
    // between characters in a word.
    std::string WrapString(const std::string &in, const std::string &indent) {
      std::stringstream out;

      size_t last_space = 0;
      size_t start_pos = 0;
      for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == ' ') {
          last_space = i;
        } else if (in[i] == '\n') {
          last_space = i;
        }

        if ((i - start_pos) != 0 && ((i - start_pos) % 75) == 0) {
          out << indent << in.substr(start_pos, last_space - start_pos) << "\n";
          start_pos = last_space + 1;
          last_space = start_pos;
        }
      }
      out << indent << in.substr(start_pos, in.size() - start_pos);

      return out.str();
    }

    std::string AdapterTypeToString(wgpu::AdapterType type) {
      switch (type) {
        case wgpu::AdapterType::DiscreteGPU:
          return "discrete GPU";
        case wgpu::AdapterType::IntegratedGPU:
          return "integrated GPU";
        case wgpu::AdapterType::CPU:
          return "CPU";
        case wgpu::AdapterType::Unknown:
          break;
      }
      return "unknown";
    }

    std::string BackendTypeToString(wgpu::BackendType type) {
      switch (type) {
        case wgpu::BackendType::Null:
          return "Null";
        case wgpu::BackendType::WebGPU:
          return "WebGPU";
        case wgpu::BackendType::D3D11:
          return "D3D11";
        case wgpu::BackendType::D3D12:
          return "D3D12";
        case wgpu::BackendType::Metal:
          return "Metal";
        case wgpu::BackendType::Vulkan:
          return "Vulkan";
        case wgpu::BackendType::OpenGL:
          return "OpenGL";
        case wgpu::BackendType::OpenGLES:
          return "OpenGLES";
        case wgpu::BackendType::Undefined:
          return "Undefined";
      }
      return "unknown";
    }

    std::string PowerPreferenceToString(const wgpu::DawnAdapterPropertiesPowerPreference &prop) {
      switch (prop.powerPreference) {
        case wgpu::PowerPreference::LowPower:
          return "low power";
        case wgpu::PowerPreference::HighPerformance:
          return "high performance";
        case wgpu::PowerPreference::Undefined:
          return "<undefined>";
      }
      return "<unknown>";
    }

    std::string AsHex(uint32_t val) {
      std::stringstream hex;
      hex << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << val;
      return hex.str();
    }

    std::string AdapterInfoToString(const wgpu::AdapterInfo &info) {
      std::stringstream out;
      out << "VendorID: " << AsHex(info.vendorID) << "\n";
      out << "Vendor: " << info.vendor << "\n";
      out << "Architecture: " << info.architecture << "\n";
      out << "DeviceID: " << AsHex(info.deviceID) << "\n";
      out << "Name: " << info.device << "\n";
      out << "Driver description: " << info.description << "\n";
      out << "Adapter Type: " << AdapterTypeToString(info.adapterType) << "\n";
      out << "Backend Type: " << BackendTypeToString(info.backendType) << "\n";

      return out.str();
    }

    std::string FormatNumber(uint64_t num) {
      auto s = std::to_string(num);
      std::stringstream ret;

      auto remainder = s.length() % 3;
      ret << s.substr(0, remainder);
      for (size_t i = remainder; i < s.length(); i += 3) {
        if (i > 0) {
          ret << ",";
        }
        ret << s.substr(i, 3);
      }
      return ret.str();
    }

    std::string LimitsToString(const wgpu::Limits &limits, const std::string &indent) {
      std::stringstream out;

      out << indent << "maxTextureDimension1D: " << FormatNumber(limits.maxTextureDimension1D) << "\n";
      out << indent << "maxTextureDimension2D: " << FormatNumber(limits.maxTextureDimension2D) << "\n";
      out << indent << "maxTextureDimension3D: " << FormatNumber(limits.maxTextureDimension3D) << "\n";
      out << indent << "maxTextureArrayLayers: " << FormatNumber(limits.maxTextureArrayLayers) << "\n";
      out << indent << "maxBindGroups: " << FormatNumber(limits.maxBindGroups) << "\n";
      out << indent << "maxBindGroupsPlusVertexBuffers: " << FormatNumber(limits.maxBindGroupsPlusVertexBuffers)
          << "\n";
      out << indent << "maxBindingsPerBindGroup: " << FormatNumber(limits.maxBindingsPerBindGroup) << "\n";
      out << indent << "maxDynamicUniformBuffersPerPipelineLayout: "
          << FormatNumber(limits.maxDynamicUniformBuffersPerPipelineLayout) << "\n";
      out << indent << "maxDynamicStorageBuffersPerPipelineLayout: "
          << FormatNumber(limits.maxDynamicStorageBuffersPerPipelineLayout) << "\n";
      out << indent << "maxSampledTexturesPerShaderStage: " << FormatNumber(limits.maxSampledTexturesPerShaderStage)
          << "\n";
      out << indent << "maxSamplersPerShaderStage: " << FormatNumber(limits.maxSamplersPerShaderStage) << "\n";
      out << indent << "maxStorageBuffersPerShaderStage: " << FormatNumber(limits.maxStorageBuffersPerShaderStage)
          << "\n";
      out << indent << "maxStorageTexturesPerShaderStage: " << FormatNumber(limits.maxStorageTexturesPerShaderStage)
          << "\n";
      out << indent << "maxUniformBuffersPerShaderStage: " << FormatNumber(limits.maxUniformBuffersPerShaderStage)
          << "\n";
      out << indent << "maxUniformBufferBindingSize: " << FormatNumber(limits.maxUniformBufferBindingSize) << "\n";
      out << indent << "maxStorageBufferBindingSize: " << FormatNumber(limits.maxStorageBufferBindingSize) << "\n";
      out << indent << "minUniformBufferOffsetAlignment: " << FormatNumber(limits.minUniformBufferOffsetAlignment)
          << "\n";
      out << indent << "minStorageBufferOffsetAlignment: " << FormatNumber(limits.minStorageBufferOffsetAlignment)
          << "\n";
      out << indent << "maxVertexBuffers: " << FormatNumber(limits.maxVertexBuffers) << "\n";
      out << indent << "maxBufferSize: " << FormatNumber(limits.maxBufferSize) << "\n";
      out << indent << "maxVertexAttributes: " << FormatNumber(limits.maxVertexAttributes) << "\n";
      out << indent << "maxVertexBufferArrayStride: " << FormatNumber(limits.maxVertexBufferArrayStride) << "\n";
      out << indent << "maxInterStageShaderVariables: " << FormatNumber(limits.maxInterStageShaderVariables) << "\n";
      out << indent << "maxColorAttachments: " << FormatNumber(limits.maxColorAttachments) << "\n";
      out << indent << "maxColorAttachmentBytesPerSample: " << FormatNumber(limits.maxColorAttachmentBytesPerSample)
          << "\n";
      out << indent << "maxComputeWorkgroupStorageSize: " << FormatNumber(limits.maxComputeWorkgroupStorageSize)
          << "\n";
      out << indent << "maxComputeInvocationsPerWorkgroup: " << FormatNumber(limits.maxComputeInvocationsPerWorkgroup)
          << "\n";
      out << indent << "maxComputeWorkgroupSizeX: " << FormatNumber(limits.maxComputeWorkgroupSizeX) << "\n";
      out << indent << "maxComputeWorkgroupSizeY: " << FormatNumber(limits.maxComputeWorkgroupSizeY) << "\n";
      out << indent << "maxComputeWorkgroupSizeZ: " << FormatNumber(limits.maxComputeWorkgroupSizeZ) << "\n";
      out << indent << "maxComputeWorkgroupsPerDimension: " << FormatNumber(limits.maxComputeWorkgroupsPerDimension)
          << "\n";

      return out.str();
    }

    void DumpAdapterInfo(const wgpu::Adapter &adapter) {
      wgpu::DawnAdapterPropertiesPowerPreference power_props{};

      wgpu::AdapterInfo info{};
      info.nextInChain = &power_props;

      adapter.GetInfo(&info);
      std::cout << AdapterInfoToString(info);
      std::cout << "Subgroup min size: " << info.subgroupMinSize << "\n";
      std::cout << "Subgroup max size: " << info.subgroupMaxSize << "\n";
      std::cout << "Power: " << PowerPreferenceToString(power_props) << "\n";
      std::cout << "\n";
    }

    void DumpAdapterFeatures(const wgpu::Adapter &adapter) {
      wgpu::SupportedFeatures supportedFeatures;
      adapter.GetFeatures(&supportedFeatures);
      std::cout << "  Features\n";
      std::cout << "  ========\n";
      for (uint32_t i = 0; i < supportedFeatures.featureCount; ++i) {
        wgpu::FeatureName f = supportedFeatures.features[i];
        auto info = dawn::native::GetFeatureInfo(f);
        std::cout << "   * " << info->name << "\n";
        std::cout << WrapString(info->description, "      ") << "\n";
        std::cout << "      " << info->url << "\n";
      }
    }

    void DumpAdapterLimits(const wgpu::Adapter &adapter) {
      wgpu::Limits adapterLimits;
      if (adapter.GetLimits(&adapterLimits)) {
        std::cout << "\n";
        std::cout << "  Adapter Limits\n";
        std::cout << "  ==============\n";
        std::cout << LimitsToString(adapterLimits, "    ") << "\n";
      }
    }

    void DumpAdapter(const wgpu::Adapter &adapter) {
      std::cout << "Adapter\n";
      std::cout << "=======\n";

      DumpAdapterInfo(adapter);
      DumpAdapterFeatures(adapter);
      DumpAdapterLimits(adapter);
    }

  } // namespace WebGPU::Info
#endif

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

#if !HK_PLATFORM_IS(EMSCRIPTEN)
    // WebGPU::Info::DumpAdapterInfo(context.Adapter);
    // WebGPU::Info::DumpAdapter(context.Adapter);
#endif
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
