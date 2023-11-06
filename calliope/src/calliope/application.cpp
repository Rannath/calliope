#include "pch.hpp"
#include "application.hpp"

#include "event/application_events.hpp"

#define LAMBDA(x, y) [this](x& e) { return y(e); }

namespace Calliope {
  Application::Application() {
    window_ = Window::Create();
    window_->SetEventCallback([this](Event& e) { OnEvent(e); });
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher d{ e };
    d.Dispatch<WindowCloseEvent>(LAMBDA(WindowCloseEvent, OnWindowClose));

    for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
      (*--it)->OnEvent(e);
      if (e.Handled) break;
    }
  }

  void Application::PushLayer(Layer* layer) {
    layer_stack_.PushLayer(layer);
  }

  void Application::PushOverlay(Layer* layer) {
    layer_stack_.PushOverlay(layer);
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    running_ = false;
    return true;
  }

  Application::~Application() {
  }

  void Application::Run() {
    while (running_) {

      for (auto* layer : layer_stack_)
        layer->OnUpdate();

      window_->OnUpdate();
    }
  }
}

#undef LAMBDA