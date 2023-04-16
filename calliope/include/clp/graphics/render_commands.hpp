#pragma once

#include <memory>

namespace clp::graphics{

  class Mesh;
  class Shader;

  namespace rendercommands{
    class RenderCommand{
    public:
      virtual void Execute() = 0;
      virtual ~RenderCommand(){}
    };

    class RenderMesh: public RenderCommand{
    public:
      RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<Shader> shader)
      : mesh_(mesh), shader_(shader) {}
      
      // Inherited via RenderCommand
      virtual void Execute() override;

    private:
      std::weak_ptr<Mesh> mesh_;
      std::weak_ptr<Shader> shader_;
    };
  }
}