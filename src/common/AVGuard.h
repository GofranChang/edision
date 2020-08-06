/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AVGuard.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_COMMON_AVGUARD__
#define __EDISION_COMMON_AVGUARD__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __APPLE1__
#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#else
#include "avutil.h"
#include "avdevice.h"
#include "avformat.h"
#include "avcodec.h"
#include "swresample.h"

#endif

#ifdef __cplusplus
}
#endif

namespace edision {

//typedef void(*ACRELEASE)()

template<class T>
class AVGuard {
public:
  typedef void(*deleter)(T**);
  AVGuard(deleter delFun) : _mData(NULL)
                          , _mDeleter(delFun) {
  }
  ~AVGuard() {
    if (_mData != NULL)
      (*_mDeleter)(&_mData);
  }

  inline T** get() {
    return &_mData;
  }

private:
  T* _mData;
  deleter _mDeleter;
};

} // namespace edision

#endif // __EDISION_COMMON_AVGUARD__
