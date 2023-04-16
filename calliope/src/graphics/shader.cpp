#include <clp/graphics/shader.hpp>
#include <clp/log.hpp>

#include <clp/graphics/helper.hpp>
#include <glad/glad.h>

namespace clp::graphics {
  auto GetInfo(void (*get)(GLuint, GLenum, GLint*), void (*info)(GLuint, GLsizei, GLsizei*, GLchar*), GLuint id, GLenum name) {
    struct _{ bool success; std::string error; };
    GLint status;
    get(id, name, &status); CheckGlError(); 
    if (status != GL_TRUE) {
      GLint len;
      get(id, GL_INFO_LOG_LENGTH, &len); CheckGlError();
      std::string error(len, ' ');
      info(id, len, nullptr, error.data()); CheckGlError();
      return _{ false, error };
    }
    return _{ true, "" };
  }

  Shader::Shader(const std::string& vertex, const std::string& fragment) {
    programId_ = glCreateProgram();

    bool success = false;

    auto vertexShaderId = glCreateShader(GL_VERTEX_SHADER); CheckGlError();
    {
      const GLchar* glSource = vertex.c_str();
      glShaderSource(vertexShaderId, 1, &glSource, nullptr); CheckGlError();
      glCompileShader(vertexShaderId); CheckGlError();
      auto [success1, error] = GetInfo(glGetShaderiv, glGetShaderInfoLog, vertexShaderId, GL_COMPILE_STATUS);
      success = success1;
      if (!success)
        Error("Vertex shader compilation error: {}", error);
      else {
        glAttachShader(programId_, vertexShaderId); CheckGlError();
      }
    }
    auto fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); CheckGlError();
    if(success) {
      const GLchar* glSource = fragment.c_str();
      glShaderSource(fragmentShaderId, 1, &glSource, nullptr); CheckGlError();
      glCompileShader(fragmentShaderId); CheckGlError();
      auto [success1, error] = GetInfo(glGetShaderiv, glGetShaderInfoLog, fragmentShaderId, GL_COMPILE_STATUS);
      success = success1;
      if (!success)
        Error("Fragment shader compilation error: {}", error);
      else {
        glAttachShader(programId_, fragmentShaderId); CheckGlError();
      }
    }

    CLP_ASSERT(success, "Error compiling shader");

    glLinkProgram(programId_); CheckGlError();
    glValidateProgram(programId_); CheckGlError();
    auto [success1, error] = GetInfo(glGetProgramiv, glGetProgramInfoLog, programId_, GL_LINK_STATUS);
    if (!success1){
      Error("Shader link error: {}", error);
      glDeleteProgram(programId_); CheckGlError();
      programId_ = -1;
    }

    glDeleteShader(vertexShaderId); CheckGlError();
    glDeleteShader(fragmentShaderId); CheckGlError();
  }

  Shader::~Shader() {
    glUseProgram(0); CheckGlError();
    glDeleteProgram(programId_); CheckGlError();
  }

  void Shader::Bind() {
    glUseProgram(programId_); CheckGlError();
  }

  void Shader::UnBind() {
    glUseProgram(0); CheckGlError();
  }

  void Shader::SetUniform(const std::string& name, int val) {
    glUseProgram(programId_); CheckGlError();
    glUniform1i(GetUniformLocation(name), val); CheckGlError();
  }

  void Shader::SetUniform(const std::string& name, float val) {
    glUseProgram(programId_); CheckGlError();
    glUniform1f(GetUniformLocation(name), val); CheckGlError();
  }

  void Shader::SetUniform(const std::string& name, float val1, float val2){
    glUseProgram(programId_); CheckGlError();
    glUniform2f(GetUniformLocation(name), val1, val2); CheckGlError();
  }

  void Shader::SetUniform(const std::string& name, float val1, float val2, float val3){
    glUseProgram(programId_); CheckGlError();
    glUniform3f(GetUniformLocation(name), val1, val2, val3); CheckGlError();
  }

  void Shader::SetUniform(const std::string& name, float val1, float val2, float val3, float val4){
    glUseProgram(programId_); CheckGlError();
    glUniform4f(GetUniformLocation(name), val1, val2, val3, val4); CheckGlError();
  }

  //private
  int Shader::GetUniformLocation(const std::string& name){
    auto it = uniformLocations_.find(name);
    if (it == uniformLocations_.end()){
      uniformLocations_[name] = glGetUniformLocation(programId_, name.c_str()); CheckGlError();
    }

    return uniformLocations_[name];
  }
}