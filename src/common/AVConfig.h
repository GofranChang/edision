#ifndef __EDISION_COMMON_AVCONFIG__
#define __EDISION_COMMON_AVCONFIG__

#include <stdint.h>

#ifdef __cplusplus 
extern "C" { 
#endif

#include "libavformat/avformat.h"

#ifdef __cplusplus 
} 
#endif

namespace edision {

struct AudioConfig {
  int64_t _mChannelLayout;
  int _mChannelNums;
  AVSampleFormat _mSampleFmt;
  int _mSampleRate;
  int _mProfile;
};

} // namespace edision

#endif // __EDISION_COMMON_AVCONFIG__
