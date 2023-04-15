#pragma once

struct SDL_Window;
using SDL_GLContext = void*;
namespace clp::core {
  class Window {
  public:
    Window();
    ~Window();

    bool Create();
    void Destroy();

    void EventPump();

    void BeginRender();
    void EndRender();

  private:
    SDL_Window* window_ = nullptr;
    SDL_GLContext glContext_ = nullptr;
  };
}