#pragma once
#include "../logger.hpp"

#include <format>
#include <iostream>
#include <chrono>

namespace Calliope::Utility {
  class CALLIOPE_API AnsiConsoleLogger : public Logger {
  public:
    AnsiConsoleLogger(std::string_view name) : name_{ name } {}

    // Inherited via Logger
    void Trace(Message message) override {
      LogConsole(message, CYAN);
    }

    void Info(Message message) override {
      LogConsole(message, GREEN);
    }

    void Warn(Message message) override {
      LogConsole(message, YELLOW);
    }

    void Error(Message message) override {
      LogConsole(message, MAGENTA);
    }

    void Fatal(Message message) override {
      LogConsole(message, RED);
    }

  private:
    constexpr static int GREEN = 32;
    constexpr static int YELLOW = 33;
    constexpr static int MAGENTA = 35;
    constexpr static int RED = 31;
    constexpr static int CYAN = 36;

    std::string name_;

    void LogConsole(Message message, int colour) {
      auto start = floor<std::chrono::seconds>(std::chrono::system_clock::now());
      std::cout << std::format("\033[1;{}m[{:%H:%M:%S}] {}: {}\033[0m\n", colour, start, name_, message);
    }
  };

}