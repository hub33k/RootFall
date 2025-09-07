#pragma once

namespace hub33k {

  namespace WebGPU {

    wgpu::Instance CreateInstance(const wgpu::ChainedStruct *togglesChain = nullptr);
    wgpu::Surface CreateSurface(const wgpu::Instance &instance, SDL_Window *window);
    wgpu::Adapter CreateAdapter(
      const wgpu::Instance &instance, const wgpu::Surface &surface, const wgpu::ChainedStruct *togglesChain = nullptr
    );
    wgpu::Device CreateDevice(const wgpu::Instance &instance, const wgpu::Adapter &adapter);
    wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
      const int width, const int height, const bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
      const wgpu::Device &device, wgpu::TextureFormat &preferredSurfaceTextureFormat
    );

  } // namespace WebGPU

#if !HK_PLATFORM_IS(EMSCRIPTEN)
  namespace WebGPU::Info {

    void DumpAdapterInfo(const wgpu::Adapter &adapter);
    void DumpAdapterFeatures(const wgpu::Adapter &adapter);
    void DumpAdapterLimits(const wgpu::Adapter &adapter);
    void DumpAdapter(const wgpu::Adapter &adapter);
    // void DumpDevice(const wgpu::Device &device);
    void Test(const wgpu::Adapter &adapter, const wgpu::Device &device);

  } // namespace WebGPU::Info
#endif

} // namespace hub33k
