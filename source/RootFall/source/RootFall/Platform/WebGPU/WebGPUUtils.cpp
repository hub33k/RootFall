#include "WebGPUUtils.hpp"

#include <SDL3/SDL.h>

#if !HK_PLATFORM_IS(EMSCRIPTEN)
  #include "RootFallCore/Utils/FileIO.hpp"
  #include <sdl3webgpu.h>
#endif

/*
Dawn helper files:

SampleUtils.cpp
HelloTriangle.cpp
ComboRenderPipelineDescriptor.h
WGPUHelpers.h
*/

// https://github.com/emscripten-core/emscripten/blob/main/system/include/webgpu/webgpu_cpp.h
// https://github.com/emscripten-core/emscripten/blob/main/system/lib/webgpu/webgpu.cpp
// https://github.com/emscripten-core/emscripten/blob/main/system/lib/webgpu/webgpu_cpp.cpp

namespace hub33k {

  wgpu::Instance CreateInstance() {
    constexpr wgpu::InstanceDescriptor instanceDescriptor = {
      .nextInChain = nullptr,
      .capabilities =
        {
          .timedWaitAnyEnable = true,
        },
    };

    wgpu::Instance instance = wgpu::CreateInstance(&instanceDescriptor);
    HK_CORE_ASSERT(instance, "Failed to create wgpu::Instance")

    return instance;
  }

  wgpu::Adapter CreateAdapter(const wgpu::Instance &instance, const wgpu::Surface &surface) {
    HK_CORE_ASSERT(instance, "[CreateAdapter] Instance is null");

    wgpu::RequestAdapterOptions adapterOptions = {
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
            HK_LOG_CORE_ERROR("[CreateAdapter] Failed to get an adapter: {0}", message.data);
            return;
          }
          *userdata = std::move(adapter);
        },
        &outAdapter
      ),
      UINT64_MAX
    );
    HK_CORE_ASSERT(outAdapter, "[CreateAdapter] Failed to create adapter");

    return outAdapter;
  }

  wgpu::Device CreateDevice(const wgpu::Instance &instance, const wgpu::Adapter &adapter) {
    HK_CORE_ASSERT(adapter, "[CreateDevice] Adapter is null");
    HK_CORE_ASSERT(instance, "[CreateDevice] Instance is null");

    wgpu::DeviceDescriptor deviceDescriptor = {};
    deviceDescriptor.nextInChain = nullptr;
    deviceDescriptor.label = "Default Device";

    // deviceDescriptor.requiredFeatures = {};
    // deviceDescriptor.requiredLimits->maxBindGroups;

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
          case wgpu::DeviceLostReason::InstanceDropped: {
            reasonName = "InstanceDropped";
            break;
          }
          case wgpu::DeviceLostReason::FailedCreation: {
            reasonName = "FailedCreation";
            break;
          }
          default: {
            // DAWN_UNREACHABLE();
            HK_CORE_ASSERT(false, "Unknown DeviceLostReason")
          }
        }

        // TODO (hub33k): is it OK?
        // The device is always “lost” when it is destroyed by the ultimate call
        // to wgpuDeviceRelease. It may also be lost for other reasons, mostly
        // meaning that the backend implementation panicked and crashed.
        if (strcmp(reasonName, "Destroyed") == 0) {
          HK_LOG_CORE_INFO("Device lost because of {}: {}", reasonName, message.data);
          return;
        }

        HK_LOG_CORE_WARN("Device lost because of {}: {}", reasonName, message.data);
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
        HK_LOG_CORE_ERROR("Device error: {} - {}", errorTypeName, message.data);
      }
    );

    wgpu::Device outDevice = nullptr;
    instance.WaitAny(
      adapter.RequestDevice(
        &deviceDescriptor, wgpu::CallbackMode::WaitAnyOnly,
        [](wgpu::RequestDeviceStatus status, wgpu::Device device, wgpu::StringView message, wgpu::Device *userdata) {
          if (status != wgpu::RequestDeviceStatus::Success) {
            HK_LOG_CORE_ERROR("Failed to get an device: {0}", message.data);
            return;
          }
          *userdata = std::move(device);
        },
        &outDevice
      ),
      UINT64_MAX
    );

    HK_CORE_ASSERT(outDevice, "[CreateDevice] Failed to create device");
    return outDevice;
  }

  wgpu::Surface CreateSurface(const wgpu::Instance &instance, SDL_Window *window) {
    HK_CORE_ASSERT(instance, "[CreateSurface] Instance is null");
    HK_CORE_ASSERT(window, "[CreateSurface] Failed to get SDL window")

    wgpu::Surface surface = wgpu::Surface::Acquire(SDL_GetWGPUSurface(instance.Get(), window));

    HK_CORE_ASSERT(surface, "[CreateSurface] Failed to create wgpu::Surface")
    return surface;
  }

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    int width, int height, bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
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
    config.presentMode = vsync ? wgpu::PresentMode::Fifo : wgpu::PresentMode::Immediate;

    surface.Configure(&config);

    // HK_LOG_CORE_INFO("[SurfaceConfiguration] Resized to {0}x{1}", width, height);

    return config;
  }

  // ================================================================

  wgpu::ShaderModule LoadShaderModule(const std::string &path, const wgpu::Device &device) {
    const std::string shaderCode = FileIO::ReadFile(SHADERS_DIR(path));

    // std::cout << "[LoadShaderModule] " << path << std::endl;
    // std::cout << shaderCode << std::endl;

    wgpu::ShaderSourceWGSL wgslDesc;
    wgslDesc.code = shaderCode.c_str();

    wgpu::ShaderModuleDescriptor descriptor;
    descriptor.nextInChain = &wgslDesc;

    wgpu::ShaderModule shaderModule = device.CreateShaderModule(&descriptor);
    HK_CORE_ASSERT(shaderModule, "[LoadShaderModule] Failed to create shader module");

    return shaderModule;
  }

} // namespace hub33k
