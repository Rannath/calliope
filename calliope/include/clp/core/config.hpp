#pragma once

#ifdef HIPPO_CONFIG_RELEASE
constexpr bool debug = false;
#else
constexpr bool debug = true;
#endif

enum class Platform { Windows, Linux, Mac };

#ifdef CLP_PLATFORM_WINDOWS
constexpr Platform platform = Platform::Windows;
#elif CLP_PLATFORM_LINUX
constexpr Platform platform = Platform::Linux;
#elif CLP_PLATFORM_MAC
constexpr Platform platform = Platform::Mac;
#endif
