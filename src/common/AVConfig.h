/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AVConfig.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_COMMON_AVCONFIG__
#define __EDISION_COMMON_AVCONFIG__

#include <stdint.h>
#include <map>
#include <string>

#ifdef __cplusplus 
extern "C" { 
#endif

#include "libavformat/avformat.h"

#ifdef __cplusplus 
} 
#endif

namespace edision {

struct AudioConfig {
  int64_t        _mChannelLayout;
  int            _mChannelNums;
  AVSampleFormat _mSampleFmt;
  int            _mSampleRate;
  int            _mProfile;
};

enum VideoFmt {
  VIDEO_UNKnown = 0,
  // YUV444
  VIDEO_YUV444,
  // YUV422
  VIDEO_YUV422,
  VIDEO_UYVY422,
  // YUV420
  VIDEO_NV21,
  VIDEO_NV12,
};

struct VideoConfig {
  VideoFmt _mFmt;
  int      _mWidth;
  int      _mHigh;
  int      _mFrameRate;
  
  VideoConfig();
  static std::map<VideoFmt, std::string> _mFmtUpon;
  static std::map<VideoFmt, std::string> createFmtUpon();
};

} // namespace edision

#endif // __EDISION_COMMON_AVCONFIG__
