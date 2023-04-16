#include <clp/graphics/mesh.hpp>

#include <clp/graphics/helper.hpp>
#include <glad/glad.h>

namespace clp::graphics {
  Mesh::Mesh(float* vertexArray, uint32_t vertexCount, uint32_t dimensions)
    : vertexCount_(vertexCount), elementCount_(0), ebo_(0)
  {
    glGenVertexArrays(1, &vao_); CheckGlError();
    glBindVertexArray(vao_); CheckGlError();

    glGenBuffers(1, &positionBO_); CheckGlError();
    glBindBuffer(GL_ARRAY_BUFFER, positionBO_); CheckGlError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * dimensions, vertexArray, GL_STATIC_DRAW); CheckGlError();

    glEnableVertexAttribArray(0); CheckGlError();
    glVertexAttribPointer(0, dimensions, GL_FLOAT, GL_FALSE, 0, 0); CheckGlError();
    glDisableVertexAttribArray(0); CheckGlError();
    glBindBuffer(GL_ARRAY_BUFFER, 0); CheckGlError();

    glBindVertexArray(0); CheckGlError();
  }

  Mesh::Mesh(float* vertexArray, uint32_t vertexCount, uint32_t dimensions, uint32_t* elementArray, uint32_t elementCount)
  : Mesh(vertexArray, vertexCount, dimensions)
  {
    elementCount_ = elementCount;
    glBindVertexArray(vao_); CheckGlError();

    glGenBuffers(1, &ebo_); CheckGlError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_); CheckGlError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * elementCount, elementArray, GL_STATIC_DRAW); CheckGlError();

    glBindVertexArray(0); CheckGlError();
  }

  Mesh::~Mesh() {
    glDeleteBuffers(1, &positionBO_); CheckGlError();
    if(ebo_ != 0)
      glDeleteBuffers(1, &ebo_); CheckGlError();
    glDeleteVertexArrays(1, &vao_); CheckGlError();
  }

  void Mesh::Bind() {
    glBindVertexArray(vao_); CheckGlError();
    glEnableVertexAttribArray(0); CheckGlError();
  }

  void Mesh::UnBind() {
    glDisableVertexAttribArray(0); CheckGlError();
    glBindVertexArray(0); CheckGlError();
  }
}