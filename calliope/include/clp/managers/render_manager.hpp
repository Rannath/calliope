#pragma once

#include <clp/graphics/render_commands.hpp>

#include <queue>

namespace clp::managers {
  class RenderManager {
    using CommandPtr = std::unique_ptr<graphics::rendercommands::RenderCommand>;
  public:
    RenderManager() {}
    ~RenderManager() {}

    void Initialize();
    void Shutdown();

    void Clear();
    void SetClearColour(float r, float g, float b, float a = 1.f);
    void SetWirefameMode(bool enabled = true);

    void Submit(CommandPtr rc);
    
    //execute submitted rendercommands in order submitted.
    void Flush();

  private:
    std::queue<CommandPtr> renderCommands_;
  };
}