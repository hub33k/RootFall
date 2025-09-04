// #include "RootFallC/Platform/WebGPU/WebGPUUtils.h"

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Window *window =
      SDL_CreateWindow("RootFall", 800, 600, SDL_WINDOW_HIDDEN);
  if (!window) {
    printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
    exit(1);
  }

  // webgpu_init(window, &webgpu_context);

  bool isRunning = true;
  SDL_ShowWindow(window);
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        isRunning = false;
      }
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
