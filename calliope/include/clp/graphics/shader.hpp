#pragma once
#include <string_view>
#include <unordered_map>

namespace clp::graphics {
  class Shader {
  public:
    Shader(const std::string& vertex, const std::string& fragment);
    ~Shader();

    void Bind();
    void UnBind();

    void SetUniform(const std::string& name, int val);
    void SetUniform(const std::string& name, float val);
    void SetUniform(const std::string& name, float val1, float val2);
    void SetUniform(const std::string& name, float val1, float val2, float val3);
    void SetUniform(const std::string& name, float val1, float val2, float val3, float val4);

  private:
    uint32_t programId_;
    std::unordered_map<std::string, int> uniformLocations_;

    int GetUniformLocation(const std::string& name);
  };
}