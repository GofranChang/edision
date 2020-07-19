/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: main.cpp
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#include "AudioDevice.h"
#include "VideoDevice.h"
#include "MyLogger.h"
#include "AudioEncoder.h"

using namespace edision;

static FILE* gFout = nullptr;

class MyRecDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
  inline void setResampler(std::shared_ptr<AudioResampler> rsp) {
    _mResampler = rsp;
  }
  
private:
  std::shared_ptr<AudioResampler> _mResampler;
};


class MyResampleDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
  inline void setEncoder(std::shared_ptr<EncoderBase> ecdr) {
    _mEncoder = ecdr;
  }

private:
  std::shared_ptr<EncoderBase> _mEncoder;
};

class MyEncodedDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
};


void MyRecDataSink::onData(uint8_t *data, size_t size) {
  _mResampler->resample(data, size);
//  LOGD("Main", "onData, size ({})", size);
//  if (gFout)
//    fwrite(data, 1, size, gFout);
}


void MyResampleDataSink::onData(uint8_t *data, size_t size) {
  // LOGD("Main", "onData, size ({})", size);
  // if (gFout)
  //   fwrite(data, 1, size, gFout);
  _mEncoder->encode(data, size);
}


void MyEncodedDataSink::onData(uint8_t* data, size_t size) {
  LOGD("Main", "onData, size ({})", size);
  if (gFout)
    fwrite(data, 1, size, gFout);
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
  // Audio Encoder test
#if 1
  AudioRecorder recorder;
  std::string devName = ":0";
  std::string inpName = "avfoundation";
  recorder.init(devName, inpName);
  
  std::shared_ptr<MyRecDataSink> recDataSink(new MyRecDataSink);
  recorder.setDataSink(recDataSink);
  
  std::shared_ptr<AudioResampler> resampler(new AudioResampler);
  AudioConfig inCfg;
  AudioConfig outCfg;

  inCfg._mChannelLayout = AV_CH_LAYOUT_MONO;
  inCfg._mChannelNums = 1;
  inCfg._mSampleFmt = AV_SAMPLE_FMT_FLT;
  inCfg._mSampleRate = 48000;

  outCfg._mChannelLayout = AV_CH_LAYOUT_STEREO;
  outCfg._mChannelNums = 2;
  outCfg._mSampleFmt = AV_SAMPLE_FMT_S16;
  outCfg._mSampleRate = 48000;
  
  std::shared_ptr<AudioConfig> AEncoderCfg(new AudioConfig);
  AEncoderCfg->_mChannelLayout = AV_CH_LAYOUT_STEREO;
  AEncoderCfg->_mChannelNums = 2;
  AEncoderCfg->_mSampleFmt = AV_SAMPLE_FMT_S16;
  AEncoderCfg->_mSampleRate = 48000;

  
  resampler->init(inCfg, outCfg, 512);
  recDataSink->setResampler(resampler);
  
  std::shared_ptr<MyResampleDataSink> resampleSink(new MyResampleDataSink);
  resampler->setDataSink(resampleSink);

  std::string encoderName = "libfdk_aac";
  std::shared_ptr<EncoderBase> encoder(EncoderBase::createNew(AUDIO, encoderName));
  encoder->init();
//  std::shared_ptr<AudioConfig> AEncoderCfg(&outCfg);
  encoder->setConfig(AEncoderCfg);
  
  resampleSink->setEncoder(encoder);
  
  std::shared_ptr<MyEncodedDataSink> encoderSink(new MyEncodedDataSink);
  encoder->setDataSink(encoderSink);
  
  gFout = fopen("/Users/gofran/Documents/workspace/gitproj/edision/resource/newout.aac", "wb+");
  for (int i = 0; i < 500; i++) {
//  while (1) {
    recorder.record();
  }
  
  fflush(gFout);
  fclose(gFout);
#endif

#if 0
  // Video Recoder test
  VideoRecorder vRec;
  std::string devName = "0";
  std::string inpName = "avfoundation";
  VideoConfig vCfg;
  vCfg._mFmt = AV_PIX_FMT_NV12;
  vCfg._mWidth = 640;
  vCfg._mHeight = 480;
  vCfg._mFrameRate = 30;
  vRec.init(devName, inpName, vCfg);

  std::shared_ptr<MyEncodedDataSink> recDataSink(new MyEncodedDataSink);
  vRec.setDataSink(recDataSink);

  gFout = fopen("/Users/gofran/Documents/workspace/gitproj/edision/resource/newout.yuv", "wb+");
  for (int i = 0; i < 500; i++) {
//  while (1) {
    vRec.record();
  }
#endif
  
  return 0;
}
