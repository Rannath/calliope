#pragma once

#include "core/window.hpp"
#include "managers/log_manager.hpp"

namespace clp {
  class Engine {
  public:
    static Engine& Instance();
    ~Engine() {}

    void Run();
    inline void Quit() { isRunning_ = false; }
  private:
    core::Window window_;
    bool isRunning_ = false;
    bool isInitialized_ = false;

    managers::LogManager logManager_;

    Engine() {}

    void GetInfo();
    [[nodiscard]] bool Initialize();
    void Shutdown();
  };
}