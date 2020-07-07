#ifndef __EDISION_COMMON_BASE_AV_SOURCE_BASE__
#define __EDISION_COMMON_BASE_AV_SOURCE_BASE__

#include <stdint.h>
#include <stddef.h>

#include "AVDataSinkBase.h"

namespace edision {

class AVDataSourceBase {
public:
  AVDataSourceBase() : _mDataSink(nullptr) {
  }
  ~AVDataSourceBase() = default;
  
  inline void setDataSink(AVDataSinkBase* dataSink) {
    _mDataSink = dataSink;
  }
  
protected:
  AVDataSinkBase* _mDataSink;
};

} // namespace edision

#endif // __EDISION_COMMON_BASE_AV_SOURCE_BASE__
