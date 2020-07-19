/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: VideoEncoder.cpp
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#include "VideoEncoder.h"
#include "AVError.h"

#include "MyLogger.h"

namespace edision {

VideoEncoder::VideoEncoder(std::string& codecName) : EncoderBase(codecName)
                                                   , _mPts(0) {
}

VideoEncoder::~VideoEncoder() {
  uninit();
}

AV_RET VideoEncoder::setConfig(std::shared_ptr<MediaConfig> config) {
  _mConfig = config;
  H264Config* vConfig = static_cast<H264Config*>(_mConfig.get());
  
  // Resolution
  _mCodecCtx->width = vConfig->_mWidth;
  _mCodecCtx->height = vConfig->_mHeight;
  
  // Profile & level
  _mCodecCtx->profile = vConfig->_mProfile;
  _mCodecCtx->level = vConfig->_mLevel;
  
  // Source format
  _mCodecCtx->pix_fmt = vConfig->_mFmt;
  
  // Gop
  _mCodecCtx->gop_size = vConfig->_mGopSize;
  
  // Bitrate
  _mCodecCtx->bit_rate = vConfig->_mBitRate;
  
  // FrameRate
  _mCodecCtx->time_base = (AVRational){1, vConfig->_mFrameRate};
  _mCodecCtx->framerate = (AVRational){vConfig->_mFrameRate, 1};

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
  _mFrame->format = AV_PIX_FMT_NV12;
  _mFrame->width = vConfig->_mWidth;
  _mFrame->height = vConfig->_mHeight;
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

AV_RET VideoEncoder::encode(const uint8_t* data, size_t size) {
  memcpy((void*)_mFrame->data[0], data, size);

  int ret = 0;
  _mFrame->pts = _mPts++;
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
