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
//////////////////////////////////////////////////////
// Audio Sink
/*
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
*/

#if 0
/*
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
 */
#endif

class MyFileWriterSink : public AVDataSinkBase {
public:
  MyFileWriterSink(std::string fileName);
  ~MyFileWriterSink();
  
  virtual void onData(uint8_t* data, size_t size) override;
  
private:
  FILE* _mOutputFile;
};

MyFileWriterSink::MyFileWriterSink(std::string fileName) {
  _mOutputFile = fopen(fileName.c_str(), "wb+");
}

MyFileWriterSink::~MyFileWriterSink() {
  if (nullptr != _mOutputFile)
    fclose(_mOutputFile);
}

void MyFileWriterSink::onData(uint8_t* data, size_t size) {
  if (nullptr != _mOutputFile) {
    LOGD("Demo", "Receive data, size ({})", size);
    fwrite(data, 1, size, _mOutputFile);
  }
}


int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
#ifdef ENCODE_H264_TEST
  /**
   * Recoder Audio test
   */
  std::shared_ptr<InputDeviceBase> audioRecorder(InputDeviceBase::createNew(DEVICE_MICROPHONE));
  audioRecorder->init(":0", "avfoundation");
  
  std::shared_ptr<MyFileWriterSink> fileSink(new MyFileWriterSink("/Users/gofran/Documents/workspace/gitproj/edision/resource/out.pcm"));
  audioRecorder->setDataSink(fileSink);
  
  for (int i = 0; i < 500; i++)
    audioRecorder->readData();
#endif
  
  /**
   * Recoder Video test
   */
  std::shared_ptr<InputDeviceBase> videoRecorder(InputDeviceBase::createNew(DEVICE_CAMERA));

  std::shared_ptr<MyFileWriterSink> fileSink(new MyFileWriterSink("/Users/gofran/Documents/workspace/gitproj/edision/resource/out.yuv"));
  videoRecorder->setDataSink(fileSink);
  
  std::shared_ptr<YUVFormat> recordYuvForat(new YUVFormat(AV_PIX_FMT_NV12, 1280, 720));
//  recordYuvForat->_mPixelFormat = AV_PIX_FMT_NV12;
//  recordYuvForat->_mWidth = 1280;
//  recordYuvForat->_mHeight = 720;
  recordYuvForat->_mFrameRate = 30;
  videoRecorder->setFormat(recordYuvForat);
  
  videoRecorder->init("0", "avfoundation");
  
  for (int i = 0; i < 500; i++)
    videoRecorder->readData();

#ifdef ENCODE_H264_TEST
  /**
   * Encoder yuv to h264 test
   */
  ////////////////////////////////////////////////////////
  std::string encoderName = "libx264";
  std::shared_ptr<EncoderBase> encoder(EncoderBase::createNew(VIDEO_ENCODER, encoderName));
  encoder->init();
  
  std::shared_ptr<YUVFormat> inputFmt(new YUVFormat(AV_PIX_FMT_YUV420P, 1280, 720));
  
  std::shared_ptr<H264Format> outputFmt(new H264Format(1280, 720));
  outputFmt->_mBitRate = 1000 * 1024;
  outputFmt->_mProfile = FF_PROFILE_H264_HIGH_444;
  outputFmt->_mFrameRate = 50;
  outputFmt->_mGopSize = 25;
  
  encoder->setConfig(inputFmt, outputFmt);
  
  std::shared_ptr<MyFileWriterSink> recDataSink(new MyFileWriterSink("/Users/gofran/Documents/workspace/gitproj/edision/build/newout.h264"));
  encoder->setDataSink(recDataSink);
  
  FILE* inputYuv = fopen("/Users/gofran/Documents/workspace/gitproj/edision/build/ConferenceMotion_1280_720_50.yuv", "rb+");
  if (nullptr == inputYuv) {
    LOGE("Main", "Open input yuv failed, {}", strerror(errno));
    return -1;
  }

  int ret = 0;
  uint8_t readBuf[1382400];
  memset(readBuf, 0, 1280 * 720 * 3 / 2);
  while (!feof(inputYuv)) {
    ret = fread(readBuf, 1, 1280 * 720 * 3 / 2, inputYuv);
    
    encoder->encode(readBuf, 1280 * 720 * 3 / 2);
  }
  
  fclose(inputYuv);
  ////////////////////////////////////////////////////////
#endif // ENCODE_H264_TEST
  
  return 0;
}
