#ifndef __EDISION_CORE_IEDISIONCORE_H__
#define __EDISION_CORE_IEDISIONCORE_H__

#include <string>

namespace edision {

enum LOG_LEVEL {
  LOG_TRACE = 0,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_CRITICAL,
};

class IEdisionCore {
public:
  IEdisionCore() = default;
  ~IEdisionCore() = default;

public:
  void InitLogger(LOG_LEVEL level, bool isConsoleLog, std::string logPath = "", bool isSimpleLog = true);
};

}

#endif // __EDISION_CORE_IEDISIONCORE_H__
