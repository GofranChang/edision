/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AudioDevice.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_AUDIO_DEVICE_H__
#define __EDISION_AUDIO_DEVICE_H__

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

class AudioRecorder : public AVDataSourceBase {
public:
  AudioRecorder();
  ~AudioRecorder();
  
  AV_RET init(std::string& devName, std::string& inpName);

  void uninit();
  
  AV_RET record();
  
private:
  AVPacket* _mAVPkt;

  AVFormatContext* _mFmtCtx;
};

class AudioResampler : public AVDataSourceBase {
public:
  AudioResampler();
  ~AudioResampler();

  AV_RET init(AudioConfig& inCfg, AudioConfig& outCfg, int srcNbSample);
  
  void uninit();

  AV_RET resample(const uint8_t* data, size_t size);

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

#endif // __EDISION_AUDIO_DEVICE_H__

