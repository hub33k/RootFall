#pragma once

namespace hub33k {

  struct WebGPUContext {
    wgpu::Instance Instance;
    wgpu::Adapter Adapter;
    wgpu::Device Device;
    wgpu::Queue Queue;
    wgpu::Surface Surface;
  };

  void InitWebGPU(SDL_Window *window, WebGPUContext &context);
  void DeinitWebGPU(WebGPUContext &context);

} // namespace hub33k
