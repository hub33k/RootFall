#pragma once

#include "RootFallCore/Events/Event.hpp"

namespace hub33k {

  using EventCallbackFn = std::function<void(Event &)>;

  struct WindowProps {
    std::string Title;
    uint32_t Width;
    uint32_t Height;

    explicit WindowProps(
      const std::string &title = "RootFall", const uint32_t width = 1280, const uint32_t height = 720
    )
      : Title(title), Width(width), Height(height) {}
  };

  struct WindowData {
    std::string Title;
    uint32_t Width, Height;
    bool VSync;

    EventCallbackFn EventCallback;
  };

  // Interface representing a desktop system based Window
  class Window {
  public:
    static Scope<Window> Create(const WindowProps &props = WindowProps());

  public:
    virtual ~Window() = default;

    virtual void OnUpdate() = 0;
    virtual void Show() = 0;

    virtual void *GetNativeHandle() = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;
  };

} // namespace hub33k
