#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
// #include <dawn/webgpu_cpp_print.h>
#include <webgpu/webgpu_cpp.h>
#if defined(__EMSCRIPTEN__)
  #include <emscripten.h>
  #include <emscripten/emscripten.h>
#else
#endif

int main(const int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  // wgpu::InstanceDescriptor instanceDescriptor = {};
  // instanceDescriptor.nextInChain = nullptr;
  // static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
  // instanceDescriptor.requiredFeatureCount = 1;
  // instanceDescriptor.requiredFeatures = &kTimedWaitAny;
  // wgpu::Instance instance = wgpu::CreateInstance(&instanceDescriptor);

  // wgpu::InstanceDescriptor instanceDesc{};
  // wgpu::Instance instance = wgpu::CreateInstance(&instanceDesc);
  // std::cout << "Instance: " << instance.Get() << std::endl;

  std::cout << "RootFall" << std::endl;

  std::println("Version 0.0.1");

  spdlog::info("RootFall");
  spdlog::info("Version 0.0.1");

  glm::vec2 a(1.0f, 2.0f);
  glm::vec2 b(3.0f, 4.0f);
  glm::vec2 c = a + b;

  std::cout << "c.x: " << c.x << std::endl;
  std::cout << "c.y: " << c.y << std::endl;

#if defined(__EMSCRIPTEN__)
  std::cout << "EMSCRIPTEN" << std::endl;
#else
  std::cout << "NOT EMSCRIPTEN" << std::endl;
#endif

  return 0;
}
