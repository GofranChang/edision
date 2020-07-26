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

AudioEncoder::AudioEncoder(std::string& codecName) : EncoderBase(codecName) {
}

AudioEncoder::~AudioEncoder() {
  uninit();
}

AV_RET AudioEncoder::setConfig(std::shared_ptr<AVFormatBase> srcFmt, std::shared_ptr<AVFormatBase> dstFmt) {
  _mFormat = dstFmt;
  AudioFormatBase* dstAudioFmtBase = static_cast<AudioFormatBase*>(_mFormat.get());

  _mCodecCtx->sample_fmt = dstAudioFmtBase->_mSampleFmt;
  _mCodecCtx->sample_rate = dstAudioFmtBase->_mSampleRate;
  _mCodecCtx->channel_layout = dstAudioFmtBase->_mChannelLayout;
  _mCodecCtx->profile = dstAudioFmtBase->_mProfile;

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
  _mFrame->format = dstAudioFmtBase->_mSampleFmt;
  _mFrame->channel_layout = dstAudioFmtBase->_mChannelLayout;
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

} // namespace edision
