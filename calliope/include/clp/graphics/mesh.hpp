#pragma once
#include <cstdint>

namespace clp::graphics {
  class Mesh {
  public:
    Mesh(float* vertexArray, uint32_t vertexCount, uint32_t dimensions);
    Mesh(float* vertexArray, uint32_t vertexCount, uint32_t dimensions, uint32_t* elementArray, uint32_t elementCount);
    ~Mesh();

    void Bind();
    void UnBind();

    inline uint32_t GetVertexCount() const { return vertexCount_; }
    inline uint32_t GetElementCount() const { return elementCount_; }

  private:
    uint32_t vertexCount_, elementCount_;
    uint32_t vao_, ebo_;
    uint32_t positionBO_;
  };
}