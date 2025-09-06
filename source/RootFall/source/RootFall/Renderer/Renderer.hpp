#pragma once

#include "RootFall/Platform/WebGPU/WebGPUUtils.hpp"

namespace hub33k {

  class Renderer {
  public:
    static WebGPUContext Context;

  public:
    static void Init();
    static void Shutdown();

    static void Display();
    static void ConfigureSurface(const int width, const int height, const bool vsync = true);

    static void BeginScene(const wgpu::Color &clearColor = {0.45f, 0.55f, 0.60f, 1.00f});
    static void EndScene();

    static void DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color);
  };

} // namespace hub33k
