#include "AVConfig.h"

namespace edision {

std::map<VideoFmt, std::string>  VideoConfig::_mFmtUpon = VideoConfig::createFmtUpon();

VideoConfig::VideoConfig() {
  
}

std::map<VideoFmt, std::string> VideoConfig::createFmtUpon() {
  std::map<VideoFmt, std::string> res;
  res[VIDEO_YUV444] = "yuv444";
  res[VIDEO_YUV422] = "yuv422";
  res[VIDEO_UYVY422] = "uyuv422";
  res[VIDEO_NV12] = "nv12";
  res[VIDEO_NV21] = "nv21";

  return res;
}

} //namespace edision
