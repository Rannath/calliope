#include "ansi_console_logger.hpp"
#include <string>

namespace Calliope {
  std::unique_ptr<Logger> CoreLogger = std::make_unique<Utility::AnsiConsoleLogger>("Core");
  std::unique_ptr<Logger> ClientLogger = std::make_unique<Utility::AnsiConsoleLogger>("Client");
}