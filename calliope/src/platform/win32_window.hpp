#pragma once

#include "calliope/window.hpp"
#include "pch.hpp"

namespace Calliope {
  class Win32Window : public Window {
  public:
    Win32Window() {}
    explicit Win32Window(const WindowProps& props);
    ~Win32Window() override;

    Win32Window(const Win32Window&) = delete;
    Win32Window(Win32Window&&) noexcept;

    Win32Window& operator=(const Win32Window&) = delete;
    Win32Window& operator=(Win32Window&&) noexcept;

    // Inherited via Window
    void OnUpdate() override;

    u32 GetWidth() const override;
    u32 GetHeight() const override;
    
    //Window attributes
    void SetEventCallback(const EventCallbackFn& callback) override;
    void SetVSync(bool enabled) override;
    void IsVSync() override;

  private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();
  private:
    friend static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    template<typename TEvent, typename... TArgs>
    friend void Broadcast(HWND hwnd, TArgs&&... args);

    HWND hwnd_ = nullptr;
    HDC hdc_ = nullptr;

    struct WindowData {
      std::string title;
      u32 width, height;
      bool vsync;

      EventCallbackFn event_callback;
    } data_{};
  };
}