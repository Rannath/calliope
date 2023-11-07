#include "pch.hpp"
#include "application.hpp"

#include "event/application_events.hpp"

#include <glad/glad.h>

namespace Calliope {
  Application::Application() {
    window_ = Window::Create();
    window_->SetEventCallback([this](Event& e) { OnEvent(e); });
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher d{ e };
#define DISPATCH(x) d.Dispatch<x>([this](x& e) { return OnEvent(e); })
    DISPATCH(WindowCloseEvent);
#undef DISPATCH

    for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
      (*--it)->OnEvent(e);
      if (e.Handled) break;
    }
  }

  void Application::PushLayer(Layer* layer) {
    layer_stack_.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer) {
    layer_stack_.PushOverlay(layer);
    layer->OnAttach();
  }

  bool Application::OnEvent(WindowCloseEvent& e) {
    running_ = false;
    return true;
  }

  Application::~Application() { }

  void Application::Run() {
    glClearColor(1, 0, 1, 1);
    while (running_) {
      glClear(GL_COLOR_BUFFER_BIT);

      for (auto* layer : layer_stack_)
        layer->OnUpdate();

      window_->OnUpdate();
    }
  }
}