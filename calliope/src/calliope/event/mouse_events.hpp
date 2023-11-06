#pragma once
#include "event.hpp"

#include "pch.hpp"

#include "event_macros_start.hpp"

namespace Calliope {
  class CALLIOPE_API MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(float x, float y)
      :x_{ x }, y_{ y } {}

    inline float GetX() const { return x_; }
    inline float GetY() const { return y_; }

    std::string ToString() const override {
      return std::format("MouseMovedEvent: ({}, {})", x_, y_);
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
  private:
    float x_, y_;
  };

  class CALLIOPE_API MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(float deltaX, float deltaY)
      :delta_x_{ deltaX }, delta_y_{deltaY} {}

    inline float GetDeltaX() const { return delta_x_; }
    inline float GetDeltaY() const { return delta_y_; }

    std::string ToString() const override {
      return std::format("MouseScrolledEvent: ({}, {})", delta_x_, delta_y_);
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
  private:
    float delta_x_, delta_y_;
  };

  class CALLIOPE_API MouseButtonEvent : public Event {
  public:
    inline int GetKeyCode() const { return button_; }

    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
  protected:
    MouseButtonEvent(int button) :button_{ button } {}

    int button_;
  };

  class CALLIOPE_API MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(int button)
      :MouseButtonEvent{ button } {}

    std::string ToString() const override {
      return std::format("MouseButtonPressedEvent: {} ", button_);
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
  };

  class CALLIOPE_API MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(int button)
      :MouseButtonEvent{ button } {}

    std::string ToString() const override {
      return std::format("MouseButtonReleasedEvent: {} ", button_);
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
  };

}

#include "event_macros_end.hpp"