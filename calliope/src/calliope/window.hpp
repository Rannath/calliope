#pragma once

#include "pch.hpp"

#include "calliope/core.hpp"
#include "calliope/event/event.hpp"

namespace Calliope {
  struct WindowProps {
    std::string title;
    u32 width, height;

    WindowProps(const std::string& title = "Calliope Engine",
      u32 width = 1280, u32 height = 720)
      :title{ title }, width{ width }, height{ height } {}
  };

  class CALLIOPE_API Window {
  public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual u32 GetWidth() const = 0;
    virtual u32 GetHeight() const = 0;

    //Window Attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual void IsVSync() = 0;

    static std::unique_ptr<Window> Create(const WindowProps& props = {});
  };

}