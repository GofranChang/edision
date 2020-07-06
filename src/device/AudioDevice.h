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
  
  inline void setDataSink(AVDataSinkBase* dataSink) {
    _mDataSink = dataSink;
  }
  
protected:
  AVDataSinkBase* _mDataSink;
};

class AudioRecorder : public AVDataSourceBase {
public:
  AudioRecorder();
  ~AudioRecorder();
  
  int init(std::string& devName, std::string& inpName);

  void uninit();
  
  int record();
  
private:
  AVPacket* _mAVPkt;

  AVFormatContext* _mFmtCtx;
};

class AudioResampler : public AVDataSourceBase {
public:
  AudioResampler();
  ~AudioResampler();

  int init(AudioConfig& inCfg, AudioConfig& outCfg, int srcNbSample);
  
  void uninit();

  int resample(uint8_t* data, size_t size);

private:
  SwrContext* _mSwrCtx;

  int _mSrcNbSample;
  int _mDstNbSample;
  int _mMaxDstNbSample;

  uint8_t** _mSrcData;
  uint8_t** _mDstData;

  int _mSrcLinesize;
  int _mDstLinesize;

  AudioConfig _mSrcCfg;
  AudioConfig _mSinkCfg;
};

} // namespace edision

#endif // __FFMPEGDEMO_AUDIO_DEVICE_H__

