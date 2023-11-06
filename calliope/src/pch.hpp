#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <format>
#include <chrono>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "calliope/logger.hpp"

#ifdef CLP_PLATFORM_WINDOWS
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <Windowsx.h>
#endif

using i8  = signed char;
using i16 = short;
using i23 = int;
using i64 = long long;
using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;