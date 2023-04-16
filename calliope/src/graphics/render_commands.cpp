#include <clp/graphics/render_commands.hpp>
#include <clp/log.hpp>

#include <clp/graphics/helper.hpp>
#include <clp/graphics/mesh.hpp>
#include <clp/graphics/shader.hpp>

#include <glad/glad.h>

namespace clp::graphics::rendercommands {
  void RenderMesh::Execute() {
    auto mesh = mesh_.lock();
    auto shader = shader_.lock();

    if (mesh && shader){
      mesh->Bind();
      shader->Bind();

      if (mesh->GetElementCount() > 0)
        glDrawElements(GL_TRIANGLES, mesh->GetElementCount(), GL_UNSIGNED_INT, 0);
      else
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount()); CheckGlError();

      shader->UnBind();
      mesh->UnBind();
    }
    else{
      Warn("Attempting to execute RenderMesh with invalid data");
    }
  }
}