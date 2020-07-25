/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: EncoderBase.cpp
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#include "EncoderBase.h"
#include "MyLogger.h"
#include "AudioEncoder.h"
#include "VideoEncoder.h"

namespace edision {

std::shared_ptr<EncoderBase> EncoderBase::createNew(CodecType type, std::string& codecName) {
  std::shared_ptr<EncoderBase> res(nullptr);

  switch (type) {
  case AUDIO_ENCODER:
      // TODO: Open this later
//      res.reset(new AudioEncoder(codecName));
    break;
  case VIDEO_ENCODER:
      res.reset(new VideoEncoder(codecName));
    break;
  default:
    LOGE("Encoder", "Create failed : unknown encoder type");
    break;
  }

  return res;
}

EncoderBase::EncoderBase(std::string& codecName) : _mCodecName(codecName)
                                                 , _mCodecCtx(NULL)
                                                 , _mCodec(NULL)
                                                 , _mFrame(NULL)
                                                 , _mPacket(NULL) {
}

AV_RET EncoderBase::init() {
  avcodec_register_all();

  _mCodec = avcodec_find_encoder_by_name(_mCodecName.c_str());
  if (NULL == _mCodec) {
    LOGE("A Encoder", "Find encoder \"{}\" failed", _mCodecName);
    return AV_FIND_CODEC_ERR;
  }

  _mCodecCtx = avcodec_alloc_context3(_mCodec);
  if (NULL == _mCodecCtx) {
    LOGE("A Encoder", "Alloc codec context failed");
    return AV_ALLOC_CODEC_CTX_ERR;
  }

  return AV_SUCCESS;
}

void EncoderBase::uninit() {
  if (_mCodecCtx != NULL)
    avcodec_free_context(&_mCodecCtx);
    
  if (_mFrame != NULL)
    av_frame_free(&_mFrame);

  if (_mPacket != NULL)
    av_packet_free(&_mPacket);
}

} // namespace edision
