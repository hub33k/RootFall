#include "Window.hpp"

#if HK_PLATFORM_IS(WINDOWS) || HK_PLATFORM_IS(MACOS)
  #include "RootFall/Platform/SDL/SDLWindow.hpp"
#endif

namespace hub33k {

  Scope<Window> Window::Create(const WindowProps &props) {
#if HK_PLATFORM_IS(WINDOWS) || HK_PLATFORM_IS(MACOS)
    return CreateScope<SDLWindow>(props);
#else
    HK_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
  }

} // namespace hub33k
