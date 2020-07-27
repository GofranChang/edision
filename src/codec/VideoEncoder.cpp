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

AV_RET VideoEncoder::setConfig(std::shared_ptr<AVFormatBase> srcFmt, std::shared_ptr<AVFormatBase> dstFmt) {
  _mFormat = dstFmt;
  
  VideoFormatBase* dstVideoFmtBase = static_cast<VideoFormatBase*>(_mFormat.get());
  
  VideoFormatBase* srcVideoFmtBase = static_cast<VideoFormatBase*>(srcFmt.get());
  if (VIDEO_YUV == srcVideoFmtBase->_mVideoFormat) {
    LOGD("V Encoder", "Encode video, src format is yuv");
    YUVFormat* yuvFmt = static_cast<YUVFormat*>(srcFmt.get());

    // Source format
    _mCodecCtx->pix_fmt = yuvFmt->_mYUVPixelFormat;
    _mFrame->format = yuvFmt->_mYUVPixelFormat;
    
    _mOffsetSize.push_back(std::make_pair(0, yuvFmt->_mYSize));
    _mOffsetSize.push_back(std::make_pair(yuvFmt->_mYSize, yuvFmt->_mUSize));
    _mOffsetSize.push_back(std::make_pair(yuvFmt->_mYSize + yuvFmt->_mUSize, yuvFmt->_mVSize));
  } else {
    LOGE("V Encoder", "Encode video, unsupport source format");
    return AV_FMT_UNSUPPORT;
  }
  
  if (VIDEO_H264 == dstVideoFmtBase->_mVideoFormat) {
    LOGD("V Encoder", "Encode video, dest format is h264");
    H264Format* h264Fmt = static_cast<H264Format*>(dstVideoFmtBase);
    
    // Set profile & level
    _mCodecCtx->profile  = h264Fmt->_mProfile;
    _mCodecCtx->level    = h264Fmt->_mLevel;
    
    // Resolution
    _mCodecCtx->width    = h264Fmt->_mWidth;
    _mCodecCtx->height   = h264Fmt->_mHeight;
    
    // Gop
    _mCodecCtx->gop_size = h264Fmt->_mGopSize;
    
    // B Frame
    if (h264Fmt->_mIsBFrame) {
      _mCodecCtx->has_b_frames = 1;
      _mCodecCtx->max_b_frames = h264Fmt->_mMaxBFrames;
    }
    
    if (h264Fmt->_mRefs > 0)
      _mCodecCtx->refs = h264Fmt->_mRefs;
    
    // Bitrate
    _mCodecCtx->bit_rate = h264Fmt->_mBitRate;
    // FrameRate
    _mCodecCtx->time_base = (AVRational){1, h264Fmt->_mFrameRate};
    _mCodecCtx->framerate = (AVRational){h264Fmt->_mFrameRate, 1};
    
  } else if (VIDEO_H265 == dstVideoFmtBase->_mVideoFormat) {
    // Encode H265
    LOGD("V Encoder", "Encoder video, dest format is h265");
  } else {
    LOGE("V Encoder", "Encode video, unsupport dest format");
    return AV_FMT_UNSUPPORT;
  }

  int ret = avcodec_open2(_mCodecCtx, _mCodec, NULL);
  if (ret < 0) {
    LOGE("A Encoder", "Open encoder error, ret ({})", ret);
    return AV_OPEN_CODEC_ERR;
  }

  _mFrame->width = dstVideoFmtBase->_mWidth;
  _mFrame->height = dstVideoFmtBase->_mHeight;
  av_frame_get_buffer(_mFrame, 32);
  if (NULL == _mFrame || NULL == _mFrame->buf[0]) {
    LOGE("A Encoder", "Alloc AVFrame failed");
    return AV_ALLOC_FRAME_ERR;
  }

  return AV_SUCCESS;
}

AV_RET VideoEncoder::encode(const uint8_t* data, size_t size) {
  for (int i = 0; i < _mOffsetSize.size(); i++) {
    if (NULL != _mFrame->data[0])
      memcpy((void*)_mFrame->data[i], data + _mOffsetSize[i].first, _mOffsetSize[i].second);
    else {
      LOGW("V Encoder", "Copy data failed, maybe not initialize");
      return AV_UNINITIALIZE;
    }
  }

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
