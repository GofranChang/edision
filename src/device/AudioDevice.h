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
#include "AVConfig.h"

namespace edision {

class AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) = 0;
};

class AVDataSourceBase {
public:
  AVDataSourceBase() : _mDataSink(nullptr) {
  }
  ~AVDataSourceBase() = default;
  
  inline void setDataSink(std::shared_ptr<AVDataSinkBase> dataSink) {
    _mDataSink = dataSink;
  }
  
protected:
  std::shared_ptr<AVDataSinkBase> _mDataSink;
};

class AudioRecorder : public AVDataSourceBase {
public:
  AudioRecorder();
  ~AudioRecorder();
  
  int init(std::string& devName, std::string& inpName);

  void uninit();
  
  int record();
  
private:
//  std::unique_ptr<AVPacket> _mAVPkt;
  AVPacket* _mAVPkt;

  // AVGuard<AVFormatContext> _mFmtCtx;
  AVFormatContext* _mFmtCtx;
};

class AudioResample : public AVDataSourceBase
                    , public AVDataSinkBase {
public:
  AudioResample();
  ~AudioResample() = default;

  int init(AudioConfig& inCfg, AudioConfig& outCfg);

  // int record();

  int resample();

  virtual void onData(uint8_t* data, size_t size) override;

private:
  AVGuard<SwrContext> _mSwrCtx;

//  AVGuard<uint8_t> _mSrcData;
//  AVGuard<uint8_t> _mDstData;

  AudioConfig _mSrcCfg;
  AudioConfig _mSinkCfg;
};

} // namespace edision

#endif // __FFMPEGDEMO_AUDIO_DEVICE_H__

