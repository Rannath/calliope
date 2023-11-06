#include "pch.hpp"
#include "win32_window.hpp"

#include "glad/glad.h"

#include "calliope/event/key_events.hpp"
#include "calliope/event/mouse_events.hpp"
#include "calliope/event/application_events.hpp"

using wglCreateContextAttribsARB_type = HGLRC WINAPI(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;
using wglChoosePixelFormatARB_type = BOOL WINAPI (HDC hdc, const int* piAttribIList,
  const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;
using wglSwapIntervalEXT_type = BOOL WINAPI(int interval);
wglSwapIntervalEXT_type* wglSwapIntervalEXT;

namespace Calliope {
  constexpr char CLASS_NAME[] = "CLP_WINDOW";
  constexpr char PROP_NAME[] = "CLP_PROP";

  bool Register();
  void CreateImpl(const std::string& caption, int width, int height, LPVOID user_data);

  void InitGlExtensions();
  HGLRC MakeGlContext(HDC dc);

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

    SwapBuffers(hdc_);
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
    wglSwapIntervalEXT(enabled ? 1 : 0);

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

    InitGlExtensions();

    CreateImpl(props.title, props.width, props.height, this);
    CLP_CORE_ASSERT(hwnd_ != nullptr, "Could not Create Window");

    hdc_ = GetDC(hwnd_);
    auto rc = MakeGlContext(hdc_);

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
   /*   WS_THICKFRAME | \*/
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

  void InitGlExtensions() {
    constexpr char DUMMY_CLASS_NAME[] = "DUMMY_CLP_WINDOW";
    WNDCLASSA window_class = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        .lpfnWndProc = DefWindowProcA,
        .hInstance = GetModuleHandle(0),
        .lpszClassName = DUMMY_CLASS_NAME,
    };

    auto success = RegisterClassA(&window_class) != 0;
    CLP_CORE_ASSERT(success, "Failed to register dummy OpenGL window.");

    HWND dummy_window = CreateWindowExA(
      0,
      DUMMY_CLASS_NAME,
      "Dummy OpenGL Window",
      0,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      0,
      0,
      window_class.hInstance,
      0);
    CLP_CORE_ASSERT(dummy_window != nullptr, "Failed to create dummy OpenGL window.");

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(pfd),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cAlphaBits = 8,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE,
    };

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    CLP_CORE_ASSERT(pixel_format != 0, "Failed to find a suitable pixel format.");

    success = SetPixelFormat(dummy_dc, pixel_format, &pfd);
    CLP_CORE_ASSERT(success, "Failed to set the pixel format.");

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    CLP_CORE_ASSERT(dummy_context != nullptr, "Failed to create a dummy OpenGL rendering context.");

    success = wglMakeCurrent(dummy_dc, dummy_context);
    CLP_CORE_ASSERT(success, "Failed to activate dummy OpenGL rendering context.");

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
      "wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
      "wglChoosePixelFormatARB");

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);
  }

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

  HGLRC MakeGlContext(HDC dc) {
    // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    CLP_CORE_ASSERT(num_formats != 0, "Failed to set the OpenGL 3.3+ pixel format.");

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(dc, pixel_format, sizeof(pfd), &pfd);
    auto success = SetPixelFormat(dc, pixel_format, &pfd) == TRUE;
    CLP_CORE_ASSERT(success, "Failed to set the OpenGL 3.3+ pixel format.");

    // Specify that we want to create an OpenGL 3.3 core profile context
    int gl33_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    HGLRC gl46_context = wglCreateContextAttribsARB(dc, 0, gl33_attribs);
    CLP_CORE_ASSERT(gl46_context != nullptr, "Failed to create OpenGL 3.3+ context.");

    success = wglMakeCurrent(dc, gl46_context);
    CLP_CORE_ASSERT(success, "Failed to activate OpenGL 3.3+ rendering context.");

    auto status = gladLoadGL();
    CLP_CORE_ASSERT(status, "Failed to initialize GLAD!");

    wglSwapIntervalEXT = (wglSwapIntervalEXT_type*)wglGetProcAddress("wglSwapIntervalEXT");

    return gl46_context;
  }
}