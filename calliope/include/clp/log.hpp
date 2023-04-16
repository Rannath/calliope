#pragma once
#include <clp/core/config.hpp>
#include <spdlog/spdlog.h>

constexpr const char* loggerName = "CalliopeLogger";

#if defined(CLP_PLATFORM_WINDOWS)
#define CLP_BREAK __debugbreak();
#elif defined(CLP_PLATFORM_MAC)
#define CLP_BREAK __builtin_debugtrap();
#else
#define CLP_BREAK __builtin_trap();
#endif

#ifndef CLP_CONFIG_RELEASE
#define CLP_ASSERT(x, msg) if ((x)) {} else { Fatal("ASSERT - {}\n\t{}\n\tin file: {}, on line: {}", #x, msg, __FILE__, __LINE__); CLP_BREAK }
#else
#define CLP_ASSERT(x, msg) (void)0
#endif

namespace clp {


  template<typename... TArgs>
  void Trace(TArgs&&... args) {
    if constexpr (debug) {
      auto logger = spdlog::get(loggerName);
      if (logger != nullptr) {
        logger->trace(std::forward<TArgs>(args)...);
      }
    }
  }

  template<typename... TArgs>
  void Debug(TArgs&&... args) {
    if constexpr (debug) {
      auto logger = spdlog::get(loggerName);
      if (logger != nullptr) {
        logger->debug(std::forward<TArgs>(args)...);
      }
    }
  }

  template<typename... TArgs>
  void Info(TArgs&&... args) {
    if constexpr (debug) {
      auto logger = spdlog::get(loggerName);
      if (logger != nullptr) {
        logger->info(std::forward<TArgs>(args)...);
      }
    }
  }

  template<typename... TArgs>
  void Warn(TArgs&&... args) {
    if constexpr (debug) {
      auto logger = spdlog::get(loggerName);
      if (logger != nullptr) {
        logger->warn(std::forward<TArgs>(args)...);
      }
    }
  }

  template<typename... TArgs>
  void Error(TArgs&&... args) {
    if constexpr (debug) {
      auto logger = spdlog::get(loggerName);
      if (logger != nullptr) {
        logger->error(std::forward<TArgs>(args)...);
      }
    }
  }

  template<typename... TArgs>
  void Fatal(TArgs&&... args) {
    if constexpr (debug) {
      auto logger = spdlog::get(loggerName);
      if (logger != nullptr) {
        logger->critical(std::forward<TArgs>(args)...);
      }
    }
  }
}