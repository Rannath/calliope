#include <clp/managers/render_manager.hpp>
#include <clp/log.hpp>

#include <clp/graphics/helper.hpp>
#include <glad/glad.h>

namespace clp::managers {
  void RenderManager::Initialize() {
    Info("OpenGL Info:\n  Vendor:\t{}\n  Renderer:\t{}\n  Version:\t{}",
      (const char*)glGetString(GL_VENDOR),
      (const char*)glGetString(GL_RENDERER),
      (const char*)glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); graphics::CheckGlError();
    glDepthFunc(GL_LEQUAL); graphics::CheckGlError();

    glEnable(GL_BLEND); graphics::CheckGlError();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); graphics::CheckGlError();

    //defaults
    SetClearColour(
      static_cast<float>(0x64) / static_cast<float>(0xFF),
      static_cast<float>(0x95) / static_cast<float>(0xFF),
      static_cast<float>(0xED) / static_cast<float>(0xFF),
      1); //cornflower blue 6495ED
  }

  void RenderManager::Shutdown() {
    while ( renderCommands_.size() > 0 ) renderCommands_.pop();
  }

  void RenderManager::Clear() {
    CLP_ASSERT(renderCommands_.size() == 0, "Unflushed render commands in queue!");
    while ( renderCommands_.size() > 0 ) renderCommands_.pop();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); graphics::CheckGlError();
  }

  void RenderManager::SetClearColour(float r, float g, float b, float a) {
    glClearColor(r, g, b, a); graphics::CheckGlError();
  }

  void RenderManager::SetWirefameMode(bool enabled) {
    glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL); graphics::CheckGlError();
  }

  void RenderManager::Submit(CommandPtr rc) {
    renderCommands_.push(std::move(rc));
  }

  void RenderManager::Flush() {
    while ( renderCommands_.size() > 0 ) {
      auto rc = std::move(renderCommands_.front());
      renderCommands_.pop();

      rc->Execute();
    }
  }
}