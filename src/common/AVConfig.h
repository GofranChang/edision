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

struct MediaConfig {
};

struct AudioConfig : public MediaConfig {
  int64_t        _mChannelLayout;
  int            _mChannelNums;
  AVSampleFormat _mSampleFmt;
  int            _mSampleRate;
  int            _mProfile;
};

struct VideoConfig : public MediaConfig {
  AVPixelFormat _mFmt;
  int           _mWidth;
  int           _mHeight;
  int           _mFrameRate;
  
  VideoConfig();
  static std::map<AVPixelFormat, std::string> _mFmtUpon;
  static std::map<AVPixelFormat, std::string> createFmtUpon();
};

struct H264Config : public VideoConfig {
  int _mProfile;
  int _mLevel;
  int _mGopSize;
  int _mBitRate;
};

} // namespace edision

#endif // __EDISION_COMMON_AVCONFIG__
