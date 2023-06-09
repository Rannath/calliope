#include <clp/core/window.hpp>
#include <clp/engine.hpp>
#include <clp/log.hpp>

#include <sdl2/SDL.h>
#include <glad/glad.h>

namespace clp::core {
  Window::Window() {}
  
  Window::~Window() {
    if (window_)
      Destroy();
  }

  bool Window::Create() {
    window_ = SDL_CreateWindow("CalliopeGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window_) {
      Error("Error Creating Window: {}", SDL_GetError());
      return false;
    }

    if constexpr (platform == Platform::Mac)
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_SetWindowMinimumSize(window_, 200, 200);

    glContext_ = SDL_GL_CreateContext(window_);
    if (glContext_ == nullptr) {
      Error("Error creating OpenGL context: {}", SDL_GetError());
      return false;
    }

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    return true;
  }

  void Window::Destroy() {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }
  
  void Window::EventPump() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        Engine::Instance().Quit();
        break;
      default:
        break;
      }
    }
  }
  
  void Window::BeginRender() {
    Engine::Instance().GetRenderManager().Clear();
  }
  
  void Window::EndRender() {
    SDL_GL_SwapWindow(window_);
  }
}