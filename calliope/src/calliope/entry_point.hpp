#pragma once
#include "application.hpp"
#include "logger.hpp"

#ifdef CLP_PLATFORM_WINDOWS

namespace Calliope {
  extern AppPtr CreateApplication();
}

int main(int /*argc*/, char** /*argv*/) {
  Calliope::CoreLogger->Warn("Initialized Log!");
  int a = 5;
  Calliope::ClientLogger->Info("Hello! var = {0}", a);
  auto app = Calliope::CreateApplication();

  app->Run();
}

#endif // CLP_PLATFORM_WINDOWS
