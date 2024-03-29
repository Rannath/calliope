#pragma once

#include "core.hpp"
#include "window.hpp"
#include "calliope/layer_stack.hpp"
#include "event/event.hpp"
#include "event/application_events.hpp"

namespace Calliope {
  class CALLIOPE_API Application {
  public:
    Application();
    virtual ~Application();

    void Run();

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

  protected:
    inline Window& window() { return *window_; }

  private:
    bool OnEvent(WindowCloseEvent& e);

    std::unique_ptr<Window> window_;
    bool running_ = true;
    LayerStack layer_stack_;
  };

  //To be defined by client
  using AppPtr = std::unique_ptr<Application>;
  AppPtr CreateApplication();
}