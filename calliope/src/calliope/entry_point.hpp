#pragma once
#include "application.hpp"
#include "logger.hpp"

#ifdef CLP_PLATFORM_WINDOWS

namespace Calliope {
  extern AppPtr CreateApplication();
}

int main(int /*argc*/, char** /*argv*/) {
  auto app = Calliope::CreateApplication();

  app->Run();
}

#endif // CLP_PLATFORM_WINDOWS
