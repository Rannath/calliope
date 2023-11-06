#pragma once
#include "calliope/core.hpp"

#include "pch.hpp"

namespace Calliope {
  //Event are blocking
  enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
  };

  enum class EventCategory {
    None = 0,
    Application = 0b00001,
    Input       = 0b00010,
    Keyboard    = 0b00100,
    Mouse       = 0b01000,
    MouseButton = 0b10000
  };

  constexpr inline EventCategory operator|(EventCategory a, EventCategory b)
  {
    return static_cast<EventCategory>(static_cast<int>(a) | static_cast<int>(b));
  }

  constexpr inline bool operator&(EventCategory a, EventCategory b)
  {
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
  }

  class CALLIOPE_API Event {
  public:
    virtual ~Event() {}
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual EventCategory GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsIn(EventCategory category) {
      return GetCategoryFlags() & category;
    }

    bool Handled = false;
  protected:

  private:
    friend class EventDispatcher;
  };

  class EventDispatcher {
    template <typename T>
    using EventFn = std::function<bool(T&)>;
  public:
    EventDispatcher(Event& e)
      : event_{ e } {}

    template<typename T>
    bool Dispatch(EventFn<T> func) {
      if (event_.GetEventType() == T::GetStaticType())
      {
        event_.Handled = func(*static_cast<T*>(&event_));
        return true;
      }
      return false;
    }

  private:
    Event& event_;
  };
}