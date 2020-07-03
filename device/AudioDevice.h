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

namespace edision {

class AVFormatCtxDeleter {
public:
  void operator()(AVFormatContext** p) {
    if (p != nullptr) {
      avformat_close_input(p);
    }
  }
};

class AVFmtCtxGurand {
public:
  AVFmtCtxGurand() : _mData(NULL) {}
  ~AVFmtCtxGurand() {
    if (_mData != NULL)
      avformat_close_input(&_mData);
  }
  
  inline AVFormatContext** get() {
    return &_mData;
  }
  
private:
  AVFormatContext* _mData;
};

class DataSink {
public:
  virtual void onData(uint8_t* data, size_t size) = 0;
};

class AudioRecorder {
public:
  AudioRecorder();
  ~AudioRecorder() = default;
  
  int init(std::string& devName, std::string& inpName);
  
  int record();
  
  inline void setDataSink(std::shared_ptr<DataSink> dataSink) {
    _mDataSink = dataSink;
  }
  
private:
  std::shared_ptr<DataSink> _mDataSink;
  
  std::unique_ptr<AVPacket> _mAVPkt;
  
//  std::unique_ptr<AVFormatContext*, AVFormatCtxDeleter> _mFmtCtx;
  AVFmtCtxGurand _mFmtCtx;
};

} // namespace edision

#endif // __FFMPEGDEMO_AUDIO_DEVICE_H__

