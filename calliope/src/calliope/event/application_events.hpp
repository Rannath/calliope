#pragma once
#include "event.hpp"

#include "pch.hpp"

#include "event_macros_start.hpp"

namespace Calliope {
  class CALLIOPE_API WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(u32 width, u32 height)
      : width_{ width }, height_{height} {}

    inline u32 GetWidth() const { return width_; }
    inline u32 GetHeight() const { return height_; }

    std::string ToString() const override {
      return std::format("WindowResizeEvent: ({}, {})", width_, height_);
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategory::Application)
  private:
    u32 width_, height_;
  };

  class CALLIOPE_API WindowCloseEvent : public Event {
  public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategory::Application)
  };

  class CALLIOPE_API AppTickEvent : public Event {
  public:
    AppTickEvent() {}

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategory::Application)
  };

  class CALLIOPE_API AppUpdateEvent : public Event {
  public:
    AppUpdateEvent() {}

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategory::Application)
  };

  class CALLIOPE_API ApprRenderEvent : public Event {
  public:
    ApprRenderEvent() {}

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategory::Application)
  };
}

#include "event_macros_end.hpp"