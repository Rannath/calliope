#include <engine.hpp>
#include <iostream>

#include <sdl2/SDL.h>

namespace clp {
    void GetInfo() {
#ifdef CLP_CONFIG_DEBUG
      std::cout << "Configuration: DEBUG\n";
#endif
#ifdef CLP_CONFIG_RELEASE
      std::cout << "Configuration: RELEASE\n";
#endif
#ifdef CLP_PLATFORM_WINDOWS
      std::cout << "Platform: WINDOWS\n";
#endif
#ifdef CLP_PLATFORM_MAC
      std::cout << "Platform: MAC\n";
#endif
#ifdef CLP_PLATFORM_LINUX
      std::cout << "Platform: LINUX\n";
#endif
    }

    bool Initialize() {
      bool ret = true;

      if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL2: " << SDL_GetError() << '\n';
        ret = false;
      }
      else {
        SDL_version version{};
        SDL_VERSION(&version);
        std::cout << "SDL " << (int32_t)version.major << "." << (int32_t)version.minor << "." << (int32_t)version.patch << '\n';
      }

      return ret;
    }

    void Shutdown() {
      SDL_Quit();
    }
}