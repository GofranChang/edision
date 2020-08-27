/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: EncoderBase.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_CODEC_IENCODER_H__
#define __EDISION_CODEC_IENCODER_H__

#include <memory>
#include <string>
//#include <thread>
//#include <time.h>

#include "IAVDataSink.h"
#include "IAVDataSource.h"
#include "IAVFormat.h"
#include "AVError.h"

namespace edision {

enum CodecType {
  AUDIO_ENCODER = 1000,
  IMAGE_ENCODER,
  VIDEO_ENCODER,
  AUDIO_DECODER,
  IMAGE_DECODER,
  VIDEO_DECODER,
};

class IEncoder : public IAVDataSource {
protected:
  IEncoder(std::string& codecName);
  virtual ~IEncoder() = default;

public:
  static std::shared_ptr<IEncoder> createNew(CodecType type, std::string& codecName);

  virtual AV_RET init();
  virtual AV_RET setConfig(std::shared_ptr<IAVFormat> srcFmt, std::shared_ptr<IAVFormat> dstFmt) = 0;
  virtual void uninit();

  virtual AV_RET encode(const uint8_t* data, size_t size) = 0;

protected:
  MediaType _mType;
  std::string _mCodecName;
  
  std::shared_ptr<IAVFormat> _mFormat;

  AVCodecContext* _mCodecCtx;
  AVCodec *_mCodec;

  AVFrame* _mFrame;
  AVPacket* _mPacket;
};

}

#endif //__EDISION_CODEC_IENCODER_H__
