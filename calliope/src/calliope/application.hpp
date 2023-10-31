#pragma once
#include "core.hpp"

#include <memory>

namespace Calliope {
  class CALLIOPE_API Application {
  public:
    Application();
    virtual ~Application();

    void Run();
  };

  //To be defined by client
  using AppPtr = std::unique_ptr<Application>;
  AppPtr CreateApplication();
}