#pragma once

//#include "common.h"

// #include <thread>
// #include <mutex>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

#define LOGGER_NAMESPACE my_media

#define GET_LOGGER LOGGER_NAMESPACE::KooLogger::Instance()

#define LOGT(logger, ...) do { \
                if (GET_LOGGER->isInit()) { \
                  auto retLogger = LOGGER_NAMESPACE::KooLogger::Instance()->getLogger(logger); \
                  SPDLOG_LOGGER_TRACE(retLogger, __VA_ARGS__); \
                } \
              } while(0);

#define LOGD(logger, ...) do { \
                if (GET_LOGGER->isInit()) { \
                  auto retLogger = LOGGER_NAMESPACE::KooLogger::Instance()->getLogger(logger); \
                  SPDLOG_LOGGER_DEBUG(retLogger, __VA_ARGS__); \
                } \
              } while(0);

#define LOGI(logger, ...) do { \
                if (GET_LOGGER->isInit()) { \
                  auto retLogger = LOGGER_NAMESPACE::KooLogger::Instance()->getLogger(logger); \
                  SPDLOG_LOGGER_INFO(retLogger, __VA_ARGS__); \
                } \
              } while(0);

#define LOGW(logger, ...) do { \
                if (GET_LOGGER->isInit()) { \
                  auto retLogger = LOGGER_NAMESPACE::KooLogger::Instance()->getLogger(logger); \
                  SPDLOG_LOGGER_WARN(retLogger, __VA_ARGS__); \
                } \
              } while(0);

#define LOGE(logger, ...) do { \
                if (GET_LOGGER->isInit()) { \
                  auto retLogger = LOGGER_NAMESPACE::KooLogger::Instance()->getLogger(logger); \
                  SPDLOG_LOGGER_ERROR(retLogger, __VA_ARGS__); \
                } \
              } while(0);

#define LOGC(logger, ...) do { \
                if (GET_LOGGER->isInit()) { \
                  auto retLogger = LOGGER_NAMESPACE::KooLogger::Instance()->getLogger(logger); \
                  SPDLOG_LOGGER_CRITICAL(retLogger, __VA_ARGS__); \
                } \
              } while(0);

#define SINGLETON_DECLARE_LOG(className) \
    public: \
      static className* Instance() \
      { \
        static className m_instance;\
        return &m_instance;\
      } \
    private: \
      className(const className&) = delete; \
      className& operator=(const className&) = delete;

namespace LOGGER_NAMESPACE {

class KooLogger {
  SINGLETON_DECLARE_LOG(KooLogger)

private:
  std::shared_ptr<spdlog::logger> _mLogger;
  std::vector<std::shared_ptr<spdlog::logger>> _mLoggerList;

  std::vector<spdlog::sink_ptr> _mSinks;
  spdlog::level::level_enum _mLevel;
  std::string _mPattern;

  std::mutex _m_LoggerMutex;

  bool _mIsInit;

private:
  KooLogger() : _mLevel(spdlog::level::info)
              , _mIsInit(false) {
  }

public:
  void initLogger(spdlog::level::level_enum logLevel, bool isConsoleLog = false, std::string path = "", bool isSimpleLog = true);

  inline bool isInit() { return _mIsInit; }

  void uninitLogger();

  void addLogger(std::string& loggerName);

  std::shared_ptr<spdlog::logger> getLogger(std::string loggerName);

};
}
