#ifndef __EDISION_COMMON_BASE_AVSINKBASE__
#define __EDISION_COMMON_BASE_AVSINKBASE__

#include <stdint.h>
#include <stddef.h>

namespace edision {

class AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) = 0;
};

} // namespace edision

#endif // __EDISION_COMMON_BASE_AVSINKBASE__
