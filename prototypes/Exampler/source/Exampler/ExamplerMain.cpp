#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_cpp_print.h>

#include <cstdlib>
#include <iostream>

int main(const int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  constexpr int compiled = SDL_VERSION;
  const int linked = SDL_GetVersion();
  SDL_Log(
    "We compiled against SDL version %d.%d.%d ...\n", SDL_VERSIONNUM_MAJOR(compiled), SDL_VERSIONNUM_MINOR(compiled),
    SDL_VERSIONNUM_MICRO(compiled)
  );
  SDL_Log(
    "But we are linking against SDL version %d.%d.%d.\n", SDL_VERSIONNUM_MAJOR(linked), SDL_VERSIONNUM_MINOR(linked),
    SDL_VERSIONNUM_MICRO(linked)
  );

  std::println("Exampler v0.0.1");

  // true on success or false on failure
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("An SDL3 window", 640, 480, SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  wgpu::InstanceDescriptor instanceDescriptor = {};
  // instanceDescriptor.nextInChain = togglesChain;
  static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
  instanceDescriptor.requiredFeatureCount = 1;
  instanceDescriptor.requiredFeatures = &kTimedWaitAny;
  wgpu::Instance instance = wgpu::CreateInstance(&instanceDescriptor);
  if (instance == nullptr) {
    std::cerr << "Instance creation failed!\n";
    return EXIT_FAILURE;
  }
  std::cout << "Instance: " << instance.Get() << std::endl;

  // Setup base adapter options with toggles.
  wgpu::RequestAdapterOptions adapterOptions = {};
  // adapterOptions.nextInChain = togglesChain;
  adapterOptions.backendType = wgpu::BackendType::D3D12;
  adapterOptions.forceFallbackAdapter = false;
  adapterOptions.powerPreference = wgpu::PowerPreference::HighPerformance;

  instance.WaitAny(
    instance.RequestAdapter(
      &adapterOptions, wgpu::CallbackMode::WaitAnyOnly,
      [](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, wgpu::StringView message) {
        if (status != wgpu::RequestAdapterStatus::Success) {
          // dawn::ErrorLog() << "Failed to get an adapter: " << message;
          return;
        }
        std::cout << "Adapter: " << adapter.Get() << std::endl;
        // sample->adapter = std::move(adapter);
      }
    ),
    UINT64_MAX
  );

  bool done = false;
  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          done = true;
        }
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
