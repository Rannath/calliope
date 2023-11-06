#include "pch.hpp"
#include "win32_window.hpp"

#include "calliope/event/key_events.hpp"
#include "calliope/event/mouse_events.hpp"
#include "calliope/event/application_events.hpp"

namespace Calliope {
  constexpr char CLASS_NAME[] = "CLP_WINDOW";
  constexpr char PROP_NAME[] = "CLP_PROP";

  bool Register();
  void CreateImpl(const std::string& caption, int width, int height, LPVOID user_data);

  std::unique_ptr<Window> Window::Create(const WindowProps& props) {
    return std::make_unique<Win32Window>(props);
  }

  Win32Window::Win32Window(const WindowProps& props) {
    Init(props);
  }

  Win32Window::~Win32Window() {
  }

  Win32Window::Win32Window(Win32Window&& other) noexcept {
    hwnd_ = other.hwnd_;
    other.hwnd_ = nullptr;
    data_ = std::move(other.data_);
  }

  Win32Window& Win32Window::operator=(Win32Window&& other) noexcept {
    hwnd_ = other.hwnd_;
    other.hwnd_ = nullptr;
    data_ = std::move(other.data_);

    return *this;
  }

  void Win32Window::OnUpdate() {
    MSG msg;
    while (PeekMessageA(&msg, hwnd_, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessageA(&msg);
    }
  }

  u32 Win32Window::GetWidth() const {
    return data_.width;
  }

  u32 Win32Window::GetHeight() const {
    return data_.height;
  }

  void Win32Window::SetEventCallback(const EventCallbackFn& callback) {
    data_.event_callback = callback;
  }

  void Win32Window::SetVSync(bool enabled) {

    data_.vsync = enabled;
  }

  void Win32Window::IsVSync() {
    data_.vsync;
  }

  void Win32Window::Init(const WindowProps& props) {
    data_.title = props.title;
    data_.width = props.width;
    data_.height = props.height;

    CoreLogger->Info("Creating window {} ({}, {})", props.title, props.width, props.height);

    auto success = Register();
    CLP_CORE_ASSERT(success, "Could not Register Window Class");
    CreateImpl(props.title, props.width, props.height, this);
    CLP_CORE_ASSERT(hwnd_ == nullptr, "Could not Create Window");

    //set user ptr = data_
    SetVSync(true);

    ShowWindow(hwnd_, SW_SHOW);
  }

  void Win32Window::Shutdown() {
    DestroyWindow(hwnd_);
  }

  static auto Self(HWND hwnd) -> Win32Window* {
    return reinterpret_cast<Win32Window*>(GetPropA(hwnd, PROP_NAME));
  }

  template<typename TEvent, typename... TArgs>
  void Broadcast(HWND hwnd, TArgs&&... args) {
    TEvent e{std::forward<TArgs>(args)...};
    Self(hwnd)->data_.event_callback(e);
  }

  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_NCCREATE:
    {
      auto cs = reinterpret_cast<CREATESTRUCTA*>(lParam);
      auto self = reinterpret_cast<Win32Window*>(cs->lpCreateParams);
      self->hwnd_ = hwnd;
      SetPropA(hwnd, PROP_NAME, self);
    }
    break;
    case WM_CLOSE:
      Broadcast<WindowCloseEvent>(hwnd);
      return 0;
    case WM_KEYDOWN:
      Broadcast<KeyPressedEvent>(hwnd, (int)wParam, (int)(lParam & 0xFF));
      return 0;
    case WM_KEYUP:
      Broadcast<KeyReleasedEvent>(hwnd, (int)wParam);
      return 0;
    case WM_LBUTTONDOWN:
      Broadcast<MouseButtonPressedEvent>(hwnd, 0);
      return 0;
    case WM_RBUTTONDOWN:
      Broadcast<MouseButtonPressedEvent>(hwnd, 1);
      return 0;
    case WM_MBUTTONDOWN:
      Broadcast<MouseButtonPressedEvent>(hwnd, 2);
      return 0;
    case WM_XBUTTONDOWN:
      Broadcast<MouseButtonPressedEvent>(hwnd, GET_XBUTTON_WPARAM(wParam) + 2);
      return 0;
    case WM_LBUTTONUP:
      Broadcast<MouseButtonReleasedEvent>(hwnd, 0);
      return 0;
    case WM_RBUTTONUP:
      Broadcast<MouseButtonReleasedEvent>(hwnd, 1);
      return 0;
    case WM_MBUTTONUP:
      Broadcast<MouseButtonReleasedEvent>(hwnd, 2);
      return 0;
    case WM_XBUTTONUP:
      Broadcast<MouseButtonReleasedEvent>(hwnd, GET_XBUTTON_WPARAM(wParam) + 2);
      return 0;
    case WM_MOUSEWHEEL:
      Broadcast<MouseScrolledEvent>(hwnd, .0f, GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
      return 0;
    case WM_MOUSEHWHEEL:
      Broadcast<MouseScrolledEvent>(hwnd, GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, .0f);
      return 0;
    case WM_MOUSEMOVE:
      Broadcast<MouseMovedEvent>(hwnd, (float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
      return 0;
    case WM_SIZE:
    {
      auto& data = Self(hwnd)->data_;

      data.width = LOWORD(lParam);
      data.height = HIWORD(lParam);
      if (!data.event_callback) break;

      Broadcast<WindowResizeEvent>(hwnd, data.width, data.height);
      return 0;
    }
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
  }

  //private
  bool Register() {
    static bool registered = false;
    if (registered) return true;
    registered = true;

    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = CLASS_NAME;
    return RegisterClassExA(&wc) != 0;
  }

  void Adjust(int& width, int& height, DWORD style, DWORD exStyle) {
    RECT rect{ .left = 0, .top = 0, .right = width, .bottom = height };
    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
  }

  std::string GetLastErrorString() {
    return std::system_category().message(::GetLastError());
  }

  void CreateImpl(const std::string& caption, int width, int height, LPVOID user_data) {
    auto style = WS_OVERLAPPED | \
      WS_CAPTION | \
      WS_SYSMENU | \
      WS_THICKFRAME | \
      WS_MINIMIZEBOX | \
      WS_MAXIMIZEBOX;
    Adjust(width, height, style, WS_EX_APPWINDOW);

    CreateWindowExA(
      WS_EX_APPWINDOW,
      CLASS_NAME,
      caption.data(),
      style,
      CW_USEDEFAULT, CW_USEDEFAULT, width, height,
      nullptr, nullptr, //parent, menu
      GetModuleHandleA(nullptr),
      user_data
    );
  }
}