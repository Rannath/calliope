#pragma once
#include "pch.hpp"

namespace Calliope::Utility {
  class CALLIOPE_API AnsiConsoleLogger : public Logger {
  public:
    AnsiConsoleLogger(std::string_view name);
    ~AnsiConsoleLogger();

    // Inherited via Logger
    void Trace(Message message) override;
    void Info(Message message) override;
    void Warn(Message message) override;
    void Error(Message message) override;
    void Fatal(Message message) override;

  private:
    struct Impl;
    const Impl* impl_;
  };
}