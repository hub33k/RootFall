#pragma once

#include "RootFall/Platform/Window.hpp"

namespace hub33k {

  class SDLWindow final : public Window {
  public:
    explicit SDLWindow(const WindowProps &props);
    ~SDLWindow() override;

    void OnUpdate() override;
    void Show() override;

    // Getters & Setters
    // ================================================================

    void *GetNativeHandle() override { return m_Window; }

    [[nodiscard]] uint32_t GetWidth() const override { return m_Data.Width; }
    [[nodiscard]] uint32_t GetHeight() const override { return m_Data.Height; }
    void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }
    void SetVSync(const bool enabled) override { m_Data.VSync = enabled; }
    [[nodiscard]] bool IsVSync() const override { return m_Data.VSync; }

  private:
    SDL_Window *m_Window = nullptr;
    WindowData m_Data;

  private:
    virtual void Init(const WindowProps &props);
    virtual void Shutdown();
  };

} // namespace hub33k
