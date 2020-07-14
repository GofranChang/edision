#include "MyLogger.h"

namespace LOGGER_NAMESPACE {

void KooLogger::initLogger(spdlog::level::level_enum logLevel, bool isConsoleLog /* = false*/, std::string path /* = ""*/, bool isSimpleLog /* = true*/) {
  if(isConsoleLog) {
    _mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }

  if ("" != path) {
    // TODO：检查路径是否有效
    _mSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(path));
  }
  _mLogger = std::make_shared<spdlog::logger>("Logger", begin(_mSinks), end(_mSinks));
  _mLogger->set_level(spdlog::level::err);
  if (isSimpleLog) {
    _mPattern = "%D %T:%e %5t %^%L%$: [%10n] %v ; (%s:%#)";
  } else {
    _mPattern = "%t [%D %T:%e][%^%7l%$][%10n] %v (%! %s:%#)";
  }
  _mLogger->set_pattern(_mPattern);
  _mLogger->flush_on(spdlog::level::err);
  spdlog::flush_every(std::chrono::seconds(5));

  _mLevel = logLevel;
}

void KooLogger::uninitLogger() {
  for (auto i : _mLoggerList) {
    i->flush();
  }

  for (auto i : _mLoggerList) {
    i.reset();
  }
}

void KooLogger::addLogger(std::string& loggerName) {
  //_mLogger->info("Start create logger \"{}\".", loggerName);
  SPDLOG_LOGGER_INFO(_mLogger, "Start create logger \"{}\".", loggerName);

  if(0 == _mSinks.size()) {
    // If not initialize logger sink, use default sink.
    _mSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  }
  auto retLogger = std::make_shared<spdlog::logger>(loggerName, std::begin(_mSinks), std::end(_mSinks));

  retLogger->set_level(_mLevel);
  retLogger->flush_on(spdlog::level::err);
  retLogger->set_pattern(_mPattern);

  std::lock_guard<std::mutex> lock(_m_LoggerMutex);
  _mLoggerList.emplace_back(retLogger);

  spdlog::register_logger(retLogger);
  //_mLogger->info("Create logger \"{}\" success.", loggerName);
  SPDLOG_LOGGER_INFO(_mLogger, "Create logger \"{}\" success.", loggerName);
}

std::shared_ptr<spdlog::logger> KooLogger::getLogger(std::string loggerName) {
  auto retLogger = spdlog::get(loggerName);

  if (!retLogger.get()) {
    SPDLOG_LOGGER_WARN(_mLogger, "Havn't found logger \"{}\", create it at first.", loggerName);
    addLogger(loggerName);
    retLogger = spdlog::get(loggerName);
  }

  return retLogger;
}

}