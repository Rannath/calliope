#pragma once

#include <clp/core/window.hpp>
#include <clp/managers/log_manager.hpp>
#include <clp/managers/render_manager.hpp>

namespace clp {
  class Engine {
  public:
    static Engine& Instance();
    ~Engine() {}

    void Run();
    inline void Quit() { isRunning_ = false; }

    inline managers::RenderManager& GetRenderManager() { return renderManager_; }

  private:
    core::Window window_;
    bool isRunning_ = false;
    bool isInitialized_ = false;

    managers::LogManager logManager_;
    managers::RenderManager renderManager_;

    Engine() {}

    void GetInfo();
    [[nodiscard]] bool Initialize();
    void Shutdown();
  };
}