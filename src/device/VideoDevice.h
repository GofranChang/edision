#ifndef __EDISION_VIDEO_DEVICE_H__
#define __EDISION_VIDEO_DEVICE_H__

#include <iostream>
#include <memory>
#include <string>
#include "MyLogger.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

#include "AVGuard.h"
#include "AVConfig.h"
#include "AVError.h"
#include "base/AVDataSinkBase.h"
#include "base/AVDataSourceBase.h"

namespace edision {

class VideoRecorder : public AVDataSourceBase {
public:
  VideoRecorder();
  ~VideoRecorder();

  AV_RET init(std::string& devName, std::string& inpName, VideoConfig& cfg);

  void uninit();

  AV_RET record();

private:
  AVPacket* _mAVPkt;

  AVFormatContext* _mFmtCtx;

  AVDictionary* _mVideoOptionals;

  VideoConfig _mVideoCfg;

  int _mFrameSize;
};

} // namespace edision

#endif // __EDISION_VIDEO_DEVICE_H__

