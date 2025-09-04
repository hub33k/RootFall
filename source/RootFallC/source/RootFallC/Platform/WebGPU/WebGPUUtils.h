#pragma once

typedef struct WebGPUContext WebGPUContext;

struct WebGPUContext {
  bool initialized;
  WGPUInstance instance;
  WGPUAdapter adapter;
  WGPUDevice device;
  WGPUQueue queue;
  WGPUSurface surface;
};

void webgpu_init(SDL_Window *window, WebGPUContext *context);
void webgpu_destroy(WebGPUContext *context);
