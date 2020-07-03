#ifndef __FFMPEGDEMO_AUDIO_DEVICE_H__
#define __FFMPEGDEMO_AUDIO_DEVICE_H__

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

namespace edision {

class DataSink {
public:
  virtual void onData(uint8_t* data, size_t size) = 0;
};

class AVDataSourceBase {
public:
  AVDataSourceBase() : _mDataSink(nullptr) {
  }
  ~AVDataSourceBase() = default;
  
  inline void setDataSink(std::shared_ptr<DataSink> dataSink) {
    _mDataSink = dataSink;
  }
  
protected:
  std::shared_ptr<DataSink> _mDataSink;
};

class AudioRecorder : public AVDataSourceBase {
public:
  AudioRecorder();
  ~AudioRecorder() = default;
  
  int init(std::string& devName, std::string& inpName);
  
  int record();
  
private:
  std::unique_ptr<AVPacket> _mAVPkt;

  AVGuard<AVFormatContext> _mFmtCtx;
};

class AudioResample : public AVDataSourceBase {
public:
  AudioResample();
  ~AudioResample() = default;
  
  int init();
  
  int record();
  
  int resample();
};

} // namespace edision

#endif // __FFMPEGDEMO_AUDIO_DEVICE_H__

