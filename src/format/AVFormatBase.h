#ifndef __EDISION_FORMAT_AVFORMAT_BASE_H__
#define __EDISION_FORMAT_AVFORMAT_BASE_H__

#ifdef __cplusplus 
extern "C" { 
#endif

#include "libavformat/avformat.h"

#ifdef __cplusplus 
} 
#endif

#include <map>
#include <string>

namespace edision {

enum MediaType {
  MEDIA_UNKNOWN = -1,
  AudioType = 0,
  VideoType,
};

enum VideoFormatType {
  VIDEO_UNKNOWN = -1,

  // YUV format type
  VIDEO_YUV = 0,
  
  // RGB format type
  VIDEO_RGB,

  // Codeced video type
  VIDEO_H264,
  VIDEO_H265,
};

/** 
 * AVFormatBase
 */
struct AVFormatBase {
  MediaType _mMediaType;

  AVFormatBase(MediaType type);
};

/** 
 * AudioFormatBase
 */
struct AudioFormatBase : public AVFormatBase {
  AVSampleFormat _mSampleFmt;
  int            _mSampleRate;
  uint64_t       _mChannelLayout;
  int            _mProfile;
  
  AudioFormatBase() : AVFormatBase(AudioType) {}
};

/** 
 * VideoFormatBase
 */
struct VideoFormatBase : public AVFormatBase {
  VideoFormatType _mVideoFormat;
  int             _mWidth;
  int             _mHeight;
  int             _mFrameRate;

  VideoFormatBase(VideoFormatType type, int width, int height);
  VideoFormatBase(const VideoFormatBase&) = default;
};

/** 
 * YUVFormat
 */
struct YUVFormat : public VideoFormatBase {
  YUVFormat(AVPixelFormat formatType, int wigth, int height);

  AVPixelFormat _mPixelFormat;
  int           _mFrameSize;

  // Y U V start location and size
  int           _mYOffset;
  int           _mYSize;
  int           _mUOffset;
  int           _mUSize;
  int           _mVOffset;
  int           _mVSize;

  static std::map<AVPixelFormat, std::string> _mFmtUpon;
  static std::map<AVPixelFormat, std::string> createFmtUpon();
};

/** 
 * H264Format
 */
struct H264Format : public VideoFormatBase {
  H264Format(int width, int height);

  // Required
  int _mProfile;
  int _mLevel;
  int _mGopSize;
  int _mBitRate;
  
  // Optional
  bool _mIsBFrame;
  int _mMaxBFrames;
  int _mRefs;
};

} // namespace edision

#endif
