#include "SDLWindow.hpp"

#include "RootFall/Core/Application.hpp"
#include "RootFallCore/Events/ApplicationEvent.hpp"
#include "RootFallCore/Events/KeyEvent.hpp"
#include <backends/imgui_impl_sdl3.h>

namespace hub33k {

  // Returns true on success or false on failure
  // https://wiki.libsdl.org/SDL3/SDL_AddEventWatch
  static bool HandleSDLEvents(void *user_data, SDL_Event *event) {
    WindowData &data = *static_cast<WindowData *>(user_data);

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
      data.Width = event->window.data1;
      data.Height = event->window.data2;

      WindowResizeEvent resizeEvent(data.Width, data.Height);
      data.EventCallback(resizeEvent);

      // std::println("Window resized to {}x{}", data.Width, data.Height);
    }

    if (event->type == SDL_EVENT_QUIT) {
      WindowCloseEvent closeEvent;
      data.EventCallback(closeEvent);
    }

    // TODO (hub33k): handle key repeat
    // KeyPressedEvent event(key, true);
    // data.EventCallback(event);

    if (event->type == SDL_EVENT_KEY_DOWN) {
      KeyPressedEvent keyPressedEvent(event->key.key, false);
      data.EventCallback(keyPressedEvent);
    }

    if (event->type == SDL_EVENT_KEY_UP) {
      KeyReleasedEvent keyReleasedEvent(event->key.key);
      data.EventCallback(keyReleasedEvent);
    }

    // TODO (hub33k): handle more events

    return true;
  }

  SDLWindow::SDLWindow(const WindowProps &props) {
    Init(props);
  }

  SDLWindow::~SDLWindow() {
    Shutdown();
  }

  void SDLWindow::OnUpdate() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);

      // tmp
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          Application::Get().Close();
        }
      }
    }
  }

  void SDLWindow::Show() {
    SDL_ShowWindow(m_Window);
  }

  // ================================================================

  void SDLWindow::Init(const WindowProps &props) {
    m_Data.Title = props.Title;
    m_Data.Width = props.Width;
    m_Data.Height = props.Height;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      HK_CORE_ASSERT(false, "SDL_Init failed")
    }

    constexpr SDL_WindowFlags windowFlags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;
    // TODO (hub33k): fix for mac - SDL_WINDOW_HIGH_PIXEL_DENSITY;
    m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, windowFlags);
    HK_CORE_ASSERT(m_Window, "SDL_CreateWindow failed");
    HK_LOG_CORE_INFO("Creating window: \"{0}\" ({1}x{2})", m_Data.Title, m_Data.Width, m_Data.Height);

    // Center window
    SDL_SetWindowPosition(m_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_AddEventWatch(HandleSDLEvents, &m_Data);
  }

  void SDLWindow::Shutdown() {
    SDL_RemoveEventWatch(HandleSDLEvents, &m_Data);

    SDL_DestroyWindow(m_Window);
    SDL_Quit();
  }

} // namespace hub33k
