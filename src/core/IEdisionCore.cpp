#include "IEdisionCore.h"

#include "MyLogger.h"

namespace edision {

void IEdisionCore::InitLogger(LOG_LEVEL level, bool isConsoleLog, std::string logPath /*= ""*/, bool isSimpleLog /*= true*/) {
  spdlog::level::level_enum spdlevel;

  switch (level) {
  case LOG_TRACE:
    spdlevel = spdlog::level::trace;
    break;
  case LOG_DEBUG:
    spdlevel = spdlog::level::debug;
    break;
  case LOG_INFO:
    spdlevel = spdlog::level::info;
    break;
  case LOG_WARN:
    spdlevel = spdlog::level::warn;
    break;
  case LOG_ERROR:
    spdlevel = spdlog::level::err;
    break;
  case LOG_CRITICAL:
    spdlevel = spdlog::level::critical;
    break;
  default:
    break;
  }

  GET_LOGGER->initLogger(spdlevel, isConsoleLog, logPath, isSimpleLog);
}

}