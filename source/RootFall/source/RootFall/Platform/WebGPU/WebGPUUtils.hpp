#pragma once

namespace hub33k {

  struct WebGPUContext {
    wgpu::Instance Instance;
    wgpu::Adapter Adapter;
    wgpu::Device Device;
    wgpu::Queue Queue;
    wgpu::Surface Surface;
    wgpu::SurfaceConfiguration SurfaceConfiguration;
    wgpu::TextureFormat PreferredSurfaceTextureFormat;
  };

  void InitWebGPU(SDL_Window *window, WebGPUContext &context);
  void DeinitWebGPU(WebGPUContext &context);

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    const int width, const int height, const bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
    const wgpu::Device &device, wgpu::TextureFormat &preferredSurfaceTextureFormat
  );

} // namespace hub33k
