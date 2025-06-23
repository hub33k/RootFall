#pragma once

namespace hub33k {

  wgpu::Instance CreateInstance();

  wgpu::Adapter CreateAdapter(const wgpu::Instance &instance, const wgpu::Surface &surface);

  wgpu::Device CreateDevice(const wgpu::Instance &instance, const wgpu::Adapter &adapter);

  wgpu::Surface CreateSurface(const wgpu::Instance &instance, SDL_Window *window);

  wgpu::SurfaceConfiguration CreateSurfaceConfiguration(
    int width, int height, bool vsync, const wgpu::Surface &surface, const wgpu::Adapter &adapter,
    const wgpu::Device &device, wgpu::TextureFormat &preferredSurfaceTextureFormat
  );

  // ================================================================

  wgpu::ShaderModule LoadShaderModule(const std::string &path, const wgpu::Device &device);

} // namespace hub33k
