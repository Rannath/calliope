#include <clp/core/config.hpp>
#include <clp/engine.hpp>
#include <clp/log.hpp>

#include <clp/graphics/mesh.hpp>
#include <clp/graphics/shader.hpp>

#include <sdl2/SDL.h>

namespace clp {
  Engine& Engine::Instance() {
    static Engine instance{};
    return instance;
  }

  void Engine::Run() {
    if (Initialize()) {
      //Test stuff
      float vertices[] = {
         0.5f,   0.5f,  0.f,
         0.5f,  -0.5f,  0.f,
        -0.5f,  -0.5f,  0.f,
        -0.5f,   0.5f,  0.f
      };
      uint32_t elements[] = {
        0, 3, 1,
        1, 3, 2
      };
      auto mesh = std::make_shared<graphics::Mesh>(&vertices[0], 4, 3, &elements[0], 6);
      const char* vShader = R"(
        #version 330 core
        layout (location = 0) in vec3 position;
        
        out vec3 vpos;
        void main() {
          vpos = position + vec3(0.5, 0.5, 0);
          gl_Position = vec4(position, 1.0);
        })";
      const char* fShader = R"(
        #version 330 core
        out vec4 outColour;
        in vec3 vpos;
        
        uniform vec3 colour = vec3(0.0);
        void main() {
          outColour = vec4(vpos, 1.0);
        })";
      auto shader = std::make_shared<graphics::Shader>(vShader, fShader);
      shader->SetUniform("colour", 1, 0, 0);

      while (isRunning_) {
        window_.EventPump();

        window_.BeginRender();

        auto rc = std::make_unique<graphics::rendercommands::RenderMesh>(mesh, shader);
        renderManager_.Submit(std::move(rc));
        renderManager_.Flush();

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
        renderManager_.Initialize();

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

    renderManager_.Shutdown();
    logManager_.Shutdown();

    window_.Destroy();
    SDL_Quit();
  }
}