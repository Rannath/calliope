#include <managers/log_manager.hpp>

#include <log.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace clp::managers {
  void LogManager::Initialize() {
    auto consoleSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %v%$");

    std::vector<spdlog::sink_ptr> sinks{consoleSink};
    auto logger = std::make_shared<spdlog::logger>(loggerName, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::register_logger(logger);
  }

  void LogManager::Shutdown() {
    spdlog::shutdown();
  }
}