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

//////////////////////////////////////////////////////
// Audio Sink
class MyRecDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
  inline void setResampler(std::shared_ptr<AudioResampler> rsp) {
    _mResampler = rsp;
  }
  
private:
  std::shared_ptr<AudioResampler> _mResampler;
};

void MyRecDataSink::onData(uint8_t *data, size_t size) {
  _mResampler->resample(data, size);
//  LOGD("Main", "onData, size ({})", size);
//  if (gFout)
//    fwrite(data, 1, size, gFout);
}

class MyResampleDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
  inline void setEncoder(std::shared_ptr<EncoderBase> ecdr) {
    _mEncoder = ecdr;
  }

private:
  std::shared_ptr<EncoderBase> _mEncoder;
};

void MyResampleDataSink::onData(uint8_t *data, size_t size) {
  // LOGD("Main", "onData, size ({})", size);
  // if (gFout)
  //   fwrite(data, 1, size, gFout);
  _mEncoder->encode(data, size);
}


//////////////////////////////////////////////////////
// Video Sink
class MyCaptureDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
  inline void setEncoder(std::shared_ptr<EncoderBase> ecdr) {
    _mEncoder = ecdr;
  }

private:
  std::shared_ptr<EncoderBase> _mEncoder;
};
  
void MyCaptureDataSink::onData(uint8_t *data, size_t size) {
  uint8_t* revert = new uint8_t[size];
  
#if 0
  memcpy(revert, data, 307200); //copy Y data
  //307200之后，是UV
  for(int i=0; i < 307200/4; i++){
//      frame->data[1][i] = pkt.data[307200+i*2];
//      frame->data[2][i] = pkt.data[307201+i*2];
    revert[307200 + i] = data[307200+i*2];
    revert[307201 + i] = data[307201+i*2];
  }
#endif
  
  
  memcpy(revert, data, 307200);


  //u
  int i =0;
  for (int j =0; j < 307200 /2; j +=2) {
    revert[307200 + i] = data[307200 + j];
    i++;
  }

  //v
  i =0;
  for (int j =1; j < 307200 /2; j+=2) {
    revert[307200 *5 /4 + i] = data[307200 + j];
    i++;
  }
  
  
  
  
  
  
  if (_mEncoder.get())
    _mEncoder->encode(revert, size);
  
  delete[] revert;
}

class MyEncodedDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
};

void MyEncodedDataSink::onData(uint8_t* data, size_t size) {
  LOGD("Main", "onData, size ({})", size);
  if (gFout)
    fwrite(data, 1, size, gFout);
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
  // Audio Encoder test
#if 0
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

  std::shared_ptr<MyCaptureDataSink> capDataSink(new MyCaptureDataSink);
  std::shared_ptr<MyEncodedDataSink> recDataSink(new MyEncodedDataSink);
  
  std::shared_ptr<H264Config> VEncoderCfg(new H264Config);
  VEncoderCfg->_mFmt = AV_PIX_FMT_YUV420P;
  VEncoderCfg->_mWidth = 640;
  VEncoderCfg->_mHeight = 480;
  VEncoderCfg->_mFrameRate = 30;
  VEncoderCfg->_mProfile = FF_PROFILE_H264_HIGH;
  VEncoderCfg->_mLevel = 50;
  VEncoderCfg->_mGopSize = 25;
  VEncoderCfg->_mBitRate = 1000 * 1024;
  
  std::string encoderName = "libx264";
  std::shared_ptr<EncoderBase> encoder(EncoderBase::createNew(VIDEO, encoderName));
  encoder->init();
  //  std::shared_ptr<AudioConfig> AEncoderCfg(&outCfg);
  encoder->setConfig(VEncoderCfg);
  encoder->setDataSink(recDataSink);
  
  capDataSink->setEncoder(encoder);
//  vRec.setDataSink(recDataSink);
  vRec.setDataSink(capDataSink);

  gFout = fopen("/Users/gofran/Documents/workspace/gitproj/edision/resource/newout.h264", "wb+");
  for (int i = 0; i < 500; i++) {
//  while (1) {
    vRec.record();
  }
#endif
  
  
  /* Initialize encoder */
  std::string encoderName = "libx264";
  std::shared_ptr<EncoderBase> encoder(EncoderBase::createNew(VIDEO, encoderName));
  encoder->init();
  
  std::shared_ptr<H264Config> VEncoderCfg(new H264Config);
  VEncoderCfg->_mFmt = AV_PIX_FMT_YUV420P;
  VEncoderCfg->_mWidth = 640;
  VEncoderCfg->_mHeight = 480;
  VEncoderCfg->_mFrameRate = 30;
  VEncoderCfg->_mProfile = FF_PROFILE_H264_HIGH_444;
  VEncoderCfg->_mLevel = 50;
  VEncoderCfg->_mGopSize = 25;
  VEncoderCfg->_mBitRate = 1000 * 1024;
  
  std::shared_ptr<MyEncodedDataSink> recDataSink(new MyEncodedDataSink);
  
  encoder->setConfig(VEncoderCfg);
  encoder->setDataSink(recDataSink);
  
  FILE* inputYuv = fopen("/Users/gofran/Documents/workspace/gitproj/edision/build/video.yuv", "rb+");
  if (nullptr == inputYuv) {
    LOGE("Main", "Open input yuv failed, {}", strerror(errno));
    return -1;
  }
  
  gFout = fopen("/Users/gofran/Documents/workspace/gitproj/edision/build/newout.h264", "wb+");
  
  int ret = 0;
  uint8_t readBuf[1382400];
  memset(readBuf, 0, 640 * 480 * 3 / 2);
  while (!feof(inputYuv)) {
    ret = fread(readBuf, 1, 640 * 480 * 3 / 2, inputYuv);
    
    encoder->encode(readBuf, 640 * 480 * 3 / 2);
  }
  
  fclose(inputYuv);
  fclose(gFout);
  
  return 0;
}
