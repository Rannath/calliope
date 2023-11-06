#include "pch.hpp"
#include "ansi_console_logger.hpp"

namespace Calliope::Utility {
  constexpr static int GREEN = 32;
  constexpr static int YELLOW = 33;
  constexpr static int MAGENTA = 35;
  constexpr static int RED = 31;
  constexpr static int CYAN = 36;

  struct AnsiConsoleLogger::Impl {
    std::string name;

    void LogConsole(Message message, int colour) const {
      auto start = floor<std::chrono::seconds>(std::chrono::system_clock::now());
      std::cout << std::format("\033[1;{}m[{:%H:%M:%S}] {}: {}\033[0m\n", colour, start, name, message);
    }
  };

  AnsiConsoleLogger::AnsiConsoleLogger(std::string_view name) : impl_{ new Impl{name.data()}} {}
  AnsiConsoleLogger::~AnsiConsoleLogger() { delete impl_; }

  void AnsiConsoleLogger::Trace(Message message) {
    impl_->LogConsole(message, CYAN);
  }

  void AnsiConsoleLogger::Info(Message message) {
    impl_->LogConsole(message, GREEN);
  }

  void AnsiConsoleLogger::Warn(Message message) {
    impl_->LogConsole(message, YELLOW);
  }

  void AnsiConsoleLogger::Error(Message message) {
    impl_->LogConsole(message, MAGENTA);
  }

  void AnsiConsoleLogger::Fatal(Message message) {
    impl_->LogConsole(message, RED);
  }
}

namespace Calliope {
  std::unique_ptr<Logger> CoreLogger = std::make_unique<Utility::AnsiConsoleLogger>("Core");
  std::unique_ptr<Logger> ClientLogger = std::make_unique<Utility::AnsiConsoleLogger>("Client");
}