#pragma once

#ifdef CLP_PLATFORM_WINDOWS
  #ifdef CLP_BUILD_DLL
    #define CALLIOPE_API __declspec(dllexport)
  #else
    #define CALLIOPE_API __declspec(dllimport)
  #endif // CLP_BUILD_DLL
#else
  #error Calliope only supports Windows.
#endif //CLP PLATFORM_WINDOWS

#ifdef CLP_ENABLE_ASSERTS
#define CLP_ASSERT(x, ...) { if(!(x)){ ::Calliope::ClientLogger->Error("Assertion Failed {}", __VA_ARGS__); __debugbreak(); } }
#define CLP_CORE_ASSERT(x, ...) { if(!(x)){ ::Calliope::CoreLogger->Error("Assertion Failed {}", __VA_ARGS__); __debugbreak(); } }
#else
#define CLP_ASSERT(x, ...)
#define CLP_CORE_ASSERT(x, ...)
#endif