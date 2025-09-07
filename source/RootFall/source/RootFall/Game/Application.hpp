#pragma once

#include "RootFall/Renderer/Renderer.hpp"
#include "RootFallCore/Core/Timestep.hpp"

namespace hub33k {

  struct WindowProps {
    std::string Title = "RootFall";
    int Width = 800;
    int Height = 600;
    bool Fullscreen = false;
    bool VSync = true;
    SDL_WindowFlags Flags = 0;
  };

  class Application {
  public:
    static Application &Get() { return *s_Instance; }

  public:
    Application();
    ~Application();

    void Run();

    void ProcessEvents();
    void Update(const Timestep ts);
    void Render(const Timestep ts);

    void Close();

    // Getters & setters
    // ================================================================

    bool IsRunning() const { return m_IsRunning; }
    SDL_Window *GetWindow() const { return m_Window; }
    Ref<Renderer> GetRenderer() const { return m_Renderer; }

    // Window props
    const WindowProps &GetWindowProps() const { return m_WindowProps; }
    const std::string &GetTitle() const { return m_WindowProps.Title; }
    int GetWidth() const { return m_WindowProps.Width; }
    int GetHeight() const { return m_WindowProps.Height; }

  private:
    static Application *s_Instance;

  private:
    bool m_IsRunning = true;
    SDL_Window *m_Window = nullptr;
    WindowProps m_WindowProps;
    Ref<Renderer> m_Renderer;
  };

} // namespace hub33k
