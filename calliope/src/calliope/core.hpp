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

namespace Calliope {

}