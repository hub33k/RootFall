#include <SDL3/SDL.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_cpp_print.h>

#include <cstdlib>
#include <iostream>

struct position {
  float x;
  float y;
};

struct velocity {
  float dx;
  float dy;
};

void update(entt::registry &registry) {
  auto view = registry.view<const position, velocity>();

  // use a callback
  view.each([](const auto &pos, auto &vel) { /* ... */ });

  // use an extended callback
  view.each([](const auto entity, const auto &pos, auto &vel) { /* ... */ });

  // use a range-for
  for (auto [entity, pos, vel] : view.each()) {
    // ...
  }

  // use forward iterators and get only the components of interest
  for (auto entity : view) {
    auto &vel = view.get<velocity>(entity);
    // ...
  }
}

int main(const int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  entt::registry registry;

  for (auto i = 0u; i < 10u; ++i) {
    const auto entity = registry.create();
    registry.emplace<position>(entity, i * 1.f, i * 1.f);
    if (i % 2 == 0) {
      registry.emplace<velocity>(entity, i * .1f, i * .1f);
    }
  }

  update(registry);

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

  constexpr auto v = glm::vec3(1.0f, 2.0f, 3.0f);
  constexpr auto v2 = glm::vec3(4.0f, 5.0f, 6.0f);
  glm::vec3 v3 = v + v2;
  std::println("vector3: {:.2f}", v3.x);

  spdlog::info("Welcome to spdlog!");

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
