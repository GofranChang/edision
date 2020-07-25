/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AudioEncoder.cpp
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#include "AudioEncoder.h"
#include "AVError.h"

#include "MyLogger.h"

namespace edision {

#if 0
AudioEncoder::AudioEncoder(std::string& codecName) : EncoderBase(codecName) {
}

AudioEncoder::~AudioEncoder() {
  uninit();
}

AV_RET AudioEncoder::setConfig(std::shared_ptr<MediaConfig> config) {
  _mConfig = config;
  AudioConfig* aConfig = static_cast<AudioConfig*>(_mConfig.get());

  _mCodecCtx->sample_fmt = aConfig->_mSampleFmt;
  _mCodecCtx->sample_rate = aConfig->_mSampleRate;
  _mCodecCtx->channel_layout = aConfig->_mChannelLayout;
  _mCodecCtx->profile = FF_PROFILE_AAC_HE_V2;

  int ret = avcodec_open2(_mCodecCtx, _mCodec, NULL);
  if (ret < 0) {
    LOGE("A Encoder", "Open encoder error, ret ({})", ret);
    return AV_OPEN_CODEC_ERR;
  }

  _mFrame = av_frame_alloc();
  if (NULL == _mFrame) {
    LOGE("A Encoder", "Alloc AVFrame failed");
    return AV_ALLOC_FRAME_ERR;
  }

  //TODO: Modify this value later
  _mFrame->nb_samples = 512;
  _mFrame->format = aConfig->_mSampleFmt;
  _mFrame->channel_layout = aConfig->_mChannelLayout;
  av_frame_get_buffer(_mFrame, 0);
  if (NULL == _mFrame || NULL == _mFrame->buf[0]) {
    LOGE("A Encoder", "Alloc AVFrame failed");
    return AV_ALLOC_FRAME_ERR;
  }

  _mPacket = av_packet_alloc();
  if (NULL == _mPacket) {
    LOGE("A Encoder", "Alloc AVPacket failed");
    return AV_ALLOC_PACKET_ERR;
  }

  return AV_SUCCESS;
}

AV_RET AudioEncoder::encode(const uint8_t* data, size_t size) {
  memcpy((void*)_mFrame->data[0], data, size);

  int ret = 0;
  ret = avcodec_send_frame(_mCodecCtx, _mFrame);
  if (ret < 0) {
    LOGE("A Encoder", "Send audio encoder data error, ret ({})", ret);
    return AV_ENCODE_SEND_ERR;
  }

  ret = avcodec_receive_packet(_mCodecCtx, _mPacket);
  if (ret < 0) {
    if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
      LOGE("A Encoder", "Encoding error");
      return AV_ENCODE_RECV_ERR;
    }
  }

  if (_mDataSink != nullptr)
    _mDataSink->onData(_mPacket->data, _mPacket->size);

  return AV_SUCCESS;
}
#endif

} // namespace edision
