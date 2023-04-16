#pragma once
#include <clp/log.hpp>
#include <clp/core/config.hpp>

#include <glad/glad.h>
#include <string>

namespace clp::graphics {
  inline void CheckGlError() {
    if constexpr (debug) {
      GLenum error = glGetError();

      bool shouldAssert = error != GL_NO_ERROR;
      while ( error != GL_NO_ERROR ) {
        std::string errorStr;
        switch ( error ) {
        case GL_INVALID_OPERATION:              errorStr = "GL_INVALID_OPERATION"; break;
        case GL_INVALID_ENUM:                   errorStr = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                  errorStr = "GL_INVALID_VALUE"; break;
        case GL_OUT_OF_MEMORY:                  errorStr = "GL_OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:  errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        default:                                errorStr = std::to_string(error); break;
        }

        Error("OpenGl Error: {}", errorStr.c_str());
        error = glGetError();
      }

      CLP_ASSERT(!shouldAssert, "OpenGl Error!");
    }
  }
}