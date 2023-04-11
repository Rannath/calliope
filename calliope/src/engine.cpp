#include <engine.hpp>
#include <iostream>

namespace clp {
    int add(int a, int b) { return a + b; }

    void GetInfo() {
#ifdef CLP_CONFIG_DEBUG
        std::cout << "Configuration: DEBUG" << std::endl;
#endif
#ifdef CLP_CONFIG_RELEASE
        std::cout << "Configuration: RELEASE" << std::endl;
#endif
#ifdef CLP_PLATFORM_WINDOWS
        std::cout << "Platform: WINDOWS" << std::endl;
#endif
#ifdef CLP_PLATFORM_MAC
        std::cout << "Platform: MAC" << std::endl;
#endif
#ifdef CLP_PLATFORM_LINUX
        std::cout << "Platform: LINUX" << std::endl;
#endif
    }
}