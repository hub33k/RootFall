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
DawnInfo.cpp
*/

// https://github.com/emscripten-core/emscripten/blob/main/system/include/webgpu/webgpu_cpp.h
// https://github.com/emscripten-core/emscripten/blob/main/system/lib/webgpu/webgpu.cpp
// https://github.com/emscripten-core/emscripten/blob/main/system/lib/webgpu/webgpu_cpp.cpp

namespace hub33k {

  wgpu::Instance CreateInstance() {
    return nullptr;
  }

  wgpu::Adapter CreateAdapter(const wgpu::Instance &instance, const wgpu::Surface &surface) {
    return nullptr;
  }

  wgpu::Device CreateDevice(const wgpu::Instance &instance, const wgpu::Adapter &adapter) {
    return nullptr;
  }

  wgpu::Surface CreateSurface(const wgpu::Instance &instance, SDL_Window *window) {
    return nullptr;
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
    config.presentMode = vsync ? wgpu::PresentMode::Fifo : wgpu::PresentMode::Immediate;

    surface.Configure(&config);

    // HK_LOG_CORE_INFO("[SurfaceConfiguration] Resized to {0}x{1}", width, height);

    return config;
  }

} // namespace hub33k
