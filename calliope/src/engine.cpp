#include <core/config.hpp>
#include <engine.hpp>
#include <log.hpp>

#include <sdl2/SDL.h>

namespace clp {
  Engine& Engine::Instance() {
    static Engine instance{};
    return instance;
  }

  void Engine::Run() {
    if (Initialize()) {
      while (isRunning_) {
        window_.EventPump();

        window_.BeginRender();
        window_.EndRender();
      }
    }
    Shutdown();
  }

  //private
  void Engine::GetInfo() {
    Info("Calliope Engine v{}.{}", 0, 1);

    if constexpr (debug) Info("Configuration: DEBUG");
    else Info("Configuration: RELEASE");

    if constexpr (platform == Platform::Windows) Info("Platform: WINDOWS");
    else if (platform == Platform::Linux) Info("Platform: LINUX");
    else if (platform == Platform::Mac) Info("Platform: MAC");
  }

  bool Engine::Initialize() {
    bool ret = false;
    CLP_ASSERT(!isInitialized_, "Attempting to call Engine::Initialize() more than once!");
    if (isInitialized_) return ret;
    
    logManager_.Initialize();
    GetInfo();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      Error("Error initializing SDL2: {}", SDL_GetError());
    }
    else {
      SDL_version version{};
      SDL_VERSION(&version);
      Info("SDL {}.{}.{}", (int32_t)version.major, (int32_t)version.minor, (int32_t)version.patch);

      if (window_.Create()) {
        ret = true;
        isRunning_ = true;
        isInitialized_ = true;
      }
    }

    if (!ret) {
      Error("Engine initialization failed. Shutting down.");
      Shutdown();
    }

    return ret;
  }

  void Engine::Shutdown() {
    isRunning_ = false;
    isInitialized_ = false;
    logManager_.Shutdown();

    window_.Destroy();
    SDL_Quit();
  }
}