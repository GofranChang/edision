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
#include "SDL.h"

using namespace edision;

class SDLRenderSink : public IAVDataSink {
public:
  SDLRenderSink() = default;
  ~SDLRenderSink() = default;
  
  int init(int width, int height);
  virtual void onData(uint8_t* data, size_t size) override;
  
private:
  SDL_Window*   _mSDLScreen;
  SDL_Renderer* _mSDLRenderer;
  SDL_Texture*  _mSDLTexture;
  SDL_Rect      _mSDLRect;
};

int SDLRenderSink::init(int width, int height) {
  if(SDL_Init(SDL_INIT_EVERYTHING)) {
    LOGE("main", "Could not initialize SDL - {}", SDL_GetError());
    return -1;
  }
  
  _mSDLScreen = SDL_CreateWindow("Simply Darwin player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
  if(!_mSDLScreen) {
    LOGE("SDL: could not create window - exiting:%s\n",SDL_GetError());
    return -1;
  }
  
  _mSDLRenderer = SDL_CreateRenderer(_mSDLScreen, -1, 0);
  _mSDLTexture = SDL_CreateTexture(_mSDLRenderer, SDL_PIXELFORMAT_NV12, SDL_TEXTUREACCESS_STREAMING, width, height);
  
  _mSDLRect.x = 0;
  _mSDLRect.y = 0;
  _mSDLRect.w = width;
  _mSDLRect.h = height;

  return 0;
}

void SDLRenderSink::onData(uint8_t *data, size_t size) {
  SDL_UpdateTexture(_mSDLTexture, NULL, data, 1280);
  
  SDL_RenderClear(_mSDLRenderer);
  SDL_RenderCopy(_mSDLRenderer, _mSDLTexture, NULL, NULL);
  SDL_RenderPresent(_mSDLRenderer);
}

//////////////////////////////////////////////////////
class MyFileWriterSink : public IAVDataSink {
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

#if 0
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
  
#if 1
  /**
   * Recoder Video test
   */
  std::shared_ptr<IInputDevice> videoRecorder(IInputDevice::createNew(DEVICE_CAMERA));

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
#endif

//#ifdef ENCODE_H264_TEST
#if 0
  /**
   * Encoder yuv to h264 test
   */
  ////////////////////////////////////////////////////////
  std::string encoderName = "libx264";
  std::shared_ptr<IEncoder> encoder(IEncoder::createNew(VIDEO_ENCODER, encoderName));
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
#endif

int main(int argc, const char* argv[]) {
  int ret = 0;
  
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);

  std::shared_ptr<IInputDevice> videoRecorder(IInputDevice::createNew(DEVICE_CAMERA));
  std::shared_ptr<YUVFormat> recordYuvForat(new YUVFormat(AV_PIX_FMT_NV12, 1280, 720));
  
  std::shared_ptr<SDLRenderSink> sdlSink(new SDLRenderSink);
  sdlSink->init(1280, 720);
//  videoRecorder->setDataSink(sdlSink);
  
  std::shared_ptr<MyFileWriterSink> fileSink(new MyFileWriterSink("/Users/gofran/Documents/workspace/gitproj/edision/resource/22222.yuv"));
  videoRecorder->setDataSink(fileSink);
  
  recordYuvForat->_mFrameRate = 30;
  videoRecorder->setFormat(recordYuvForat);
  
  videoRecorder->init("0", "avfoundation");
  for (int i = 0; i < 500; i++)
    videoRecorder->readData();
  
  return ret;
}
