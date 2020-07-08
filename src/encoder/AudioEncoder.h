#ifndef __EDISION_ENCODER_AUDIOENCODER_H__
#define __EDISION_ENCODER_AUDIOENCODER_H__

#include <string>
#include <thread>

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

#include "base/AVDataSinkBase.h"
#include "base/AVDataSourceBase.h"
#include "AVConfig.h"
#include "AVError.h"

namespace edision {

class AudioDataSource;

class AudioEncoder : public AVDataSourceBase {
public:
  AudioEncoder();
  ~AudioEncoder();

  AV_RET init(std::string& codecName, AudioConfig& config);
  
  void uninit();

  void recvThreadFun();

  AV_RET encode(const uint8_t* data, size_t size);

private:
  AVCodecContext* _mCodecCtx;
  AVCodec *_mCodec;
  AudioConfig _mConfig;

  AVFrame* _mFrame;
  AVPacket* _mPacket;

private:
  std::thread _mRecvThread;
};

}

#endif //__EDISION_ENCODER_AUDIOENCODER_H__
