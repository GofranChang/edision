#include "AVFormatBase.h"

namespace edision {

// AVFormatBase
AVFormatBase::AVFormatBase(MediaType type) : _mMediaType(type) {
}

// VideoFormat
std::map<AVPixelFormat, std::string>  YUVFormat::_mFmtUpon = YUVFormat::createFmtUpon();

VideoFormatBase::VideoFormatBase(VideoFormatType type, \
                                  int width, \
                                  int height) : _mVideoFormat(type)
                                              , _mWidth(width)
                                              , _mHeight(height)
                                              , AVFormatBase(VideoType) {
}

// YUVFormat
YUVFormat::YUVFormat(AVPixelFormat formatType, \
                      int width, \
                      int height) : VideoFormatBase(VIDEO_YUV, width, height) {
  _mYUVPixelFormat = formatType;
  switch (formatType) {
  case AV_PIX_FMT_NV12 :
  case AV_PIX_FMT_NV21 :
    _mFrameSize = _mWidth * _mHeight * 3 / 2;
    _mYOffset = 0;
    _mYSize = _mWidth * _mHeight;
    _mUOffset = -1;
    _mUSize = _mWidth * _mHeight / 4;
    _mVOffset = -1;
    _mVSize = _mWidth * _mHeight / 4;
    break;
  
  case AV_PIX_FMT_YUV420P :
    _mFrameSize = _mWidth * _mHeight * 3 / 2;
    _mYOffset = 0;
    _mYSize = _mWidth * _mHeight;
    _mUOffset = 0 + _mYSize;
    _mUSize = _mWidth * _mHeight / 4;
    _mVOffset = 0 + _mYSize + _mUSize;
    _mVSize = _mWidth * _mHeight / 4;
    break;
  
  default:
    break;
  }
}

std::map<AVPixelFormat, std::string> YUVFormat::createFmtUpon() {
  std::map<AVPixelFormat, std::string> res;

  res[AV_PIX_FMT_NV12] = "nv12";
  res[AV_PIX_FMT_NV21] = "nv21";
  res[AV_PIX_FMT_YUV420P] = "yuv420p";
  res[AV_PIX_FMT_UYVY422] = "uyvy422";
  res[AV_PIX_FMT_YUYV422] = "yuyv422";

  return res;
}

// H264Format
H264Format::H264Format(int width, int height) : VideoFormatBase(VIDEO_H264, width, height) {
}

}; // namespace edision
