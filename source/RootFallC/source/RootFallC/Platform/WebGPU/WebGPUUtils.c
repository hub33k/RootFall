#include "WebGPUUtils.h"

static WebGPUContext webgpuContext;

void webgpu_init(SDL_Window *window, WebGPUContext *wgpu_context) {
  wgpu_context->initialized = true;
  WGPUInstanceFeatureName requiredFeatures[1] = {
      WGPUInstanceFeatureName_TimedWaitAny};
  WGPUInstance instance = wgpuCreateInstance(&(WGPUInstanceDescriptor){
      .requiredFeatureCount = 1,
      .requiredFeatures = requiredFeatures,
  });
  // assert(wgpu_context->instance);
  // request_adapter(wgpu_context);
}

void webgpu_destroy(WebGPUContext *wgpu_context) {}

// ================================================================

static void request_adapter_cb(WGPURequestAdapterStatus status,
                               WGPUAdapter adapter, WGPUStringView message,
                               void *userdata1, void *userdata2) {
  UNUSED(message);
  UNUSED(userdata2);

  WebGPUContext *wgpu_context = (WebGPUContext *)userdata1;
  if (status != WGPURequestAdapterStatus_Success) {
    printf("wgpuInstanceRequestAdapter failed!\n");
    exit(10);
  }
  wgpu_context->adapter = adapter;
}

static void request_adapter(WebGPUContext *wgpu_context) {
  WGPUFuture future =
      wgpuInstanceRequestAdapter(wgpu_context->instance, 0,
                                 (WGPURequestAdapterCallbackInfo){
                                     .mode = WGPUCallbackMode_WaitAnyOnly,
                                     .callback = request_adapter_cb,
                                     .userdata1 = wgpu_context,
                                 });
  WGPUFutureWaitInfo future_info = {.future = future};
  WGPUWaitStatus res =
      wgpuInstanceWaitAny(wgpu_context->instance, 1, &future_info, UINT64_MAX);
  assert(res == WGPUWaitStatus_Success);
}
