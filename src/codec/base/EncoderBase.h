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

#ifndef __EDISION_CODEC_ENCODERBASE_H__
#define __EDISION_CODEC_ENCODERBASE_H__

#include <memory>
#include <string>
#include <thread>

#include "base/AVDataSinkBase.h"
#include "base/AVDataSourceBase.h"
#include "AVFormatBase.h"
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

class EncoderBase : public AVDataSourceBase {
protected:
  EncoderBase(std::string& codecName);
  virtual ~EncoderBase() = default;

public:
  static std::shared_ptr<EncoderBase> createNew(CodecType type, std::string& codecName);

  virtual AV_RET init();
  virtual AV_RET setConfig(std::shared_ptr<AVFormatBase> srcFmt, std::shared_ptr<AVFormatBase> dstFmt) = 0;
  virtual void uninit();

  virtual AV_RET encode(const uint8_t* data, size_t size) = 0;

protected:
  MediaType _mType;
  std::string _mCodecName;
  
  std::shared_ptr<AVFormatBase> _mFormat;

  AVCodecContext* _mCodecCtx;
  AVCodec *_mCodec;

  AVFrame* _mFrame;
  AVPacket* _mPacket;
};

}

#endif //__EDISION_CODEC_ENCODERBASE_H__
