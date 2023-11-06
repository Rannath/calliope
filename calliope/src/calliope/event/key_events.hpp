#pragma once
#include "event.hpp"

#include "pch.hpp"

#include "event_macros_start.hpp"

namespace Calliope {
  class CALLIOPE_API KeyEvent : public Event {
  public:
    inline int GetKeyCode() const { return key_code_; }

    EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
  protected:
    KeyEvent(int key_code) :key_code_ { key_code } {}

    int key_code_;
  };

  class CALLIOPE_API KeyPressedEvent : public KeyEvent {
  public:
    KeyPressedEvent(int key_code, int repeat_count)
      :KeyEvent{ key_code }, repeat_count_{ repeat_count } {}

    inline int GetRepeatCount() const { return repeat_count_; }

    std::string ToString() const override {
      return std::format("KeyPressedEvent: {} ({} repeats)", key_code_, repeat_count_);
    }

    EVENT_CLASS_TYPE(KeyPressed)
  private:
    int repeat_count_;
  };

  class CALLIOPE_API KeyReleasedEvent : public KeyEvent {
  public:
    KeyReleasedEvent(int key_code)
      :KeyEvent{ key_code } {}

    std::string ToString() const override {
      return std::format("KeyReleasedEvent: {}", key_code_);
    }

    EVENT_CLASS_TYPE(KeyReleased)
  };
}

#include "event_macros_end.hpp"