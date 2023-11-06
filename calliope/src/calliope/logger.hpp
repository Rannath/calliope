#pragma once
#include "core.hpp"
#include "pch.hpp"

namespace Calliope {
  class CALLIOPE_API Logger {
  public:
    using Message = std::string_view;

    virtual ~Logger() {}
    
    virtual void Trace(Message message) = 0;
    template<typename... TArgs>
    void Trace(const std::format_string<TArgs...> fmt, TArgs&&... args) {
      Trace(std::format(fmt, std::forward<TArgs>(args)...));
    }

    virtual void Info(Message message) = 0;
    template<typename... TArgs>
    void Info(const std::format_string<TArgs...> fmt, TArgs&&... args) {
      Info(std::format(fmt, std::forward<TArgs>(args)...));
    }

    virtual void Warn(Message message) = 0;
    template<typename... TArgs>
    void Warn(const std::format_string<TArgs...> fmt, TArgs&&... args) {
      Warn(std::format(fmt, std::forward<TArgs>(args)...));
    }

    virtual void Error(Message message) = 0;
    template<typename... TArgs>
    void Error(const std::format_string<TArgs...> fmt, TArgs&&... args) {
      Error(std::format(fmt, std::forward<TArgs>(args)...));
    }

    virtual void Fatal(Message message) = 0;
    template<typename... TArgs>
    void Fatal(const std::format_string<TArgs...> fmt, TArgs&&... args) {
      Fatal(std::format(fmt, std::forward<TArgs>(args)...));
    }
  };

  extern CALLIOPE_API std::unique_ptr<Logger> CoreLogger;
  extern CALLIOPE_API std::unique_ptr<Logger> ClientLogger;
}