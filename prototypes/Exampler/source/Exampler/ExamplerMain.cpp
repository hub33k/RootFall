#include <SDL3/SDL.h>
#include <glm/glm.hpp>

int main(const int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  constexpr auto v = glm::vec3(1.0f, 2.0f, 3.0f);
  constexpr auto v2 = glm::vec3(4.0f, 5.0f, 6.0f);
  glm::vec3 v3 = v + v2;
  std::println("vector3: {:.2f}", v3.x);

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
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
