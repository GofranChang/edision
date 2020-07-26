/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: VideoDevice.cpp
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#include "VideoDevice.h"

namespace edision {

/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
VideoRecorder::VideoRecorder() {
}

/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
VideoRecorder::~VideoRecorder() {
}

/**
 * Initialize audio recorder:
 *   Open input device, alloc AVPacket (for output)
 *
 * @param devName: Audio input device name
 * @param inpName: Input format name.
 *                eg. In macos is "avfoundation"
 *
 */
AV_RET VideoRecorder::setFormat(std::shared_ptr<AVFormatBase> fmt) {
  avdevice_register_all();

  //TODO: Set optionals
  InputDeviceBase::setFormat(fmt);

  if (fmt->_mMediaType != VideoType) {
    LOGE("V Recoder", "Set video recorder format failed, format type not video");
    return AV_BAD_PARAMETER;
  }

  VideoFormatBase* videoFmtBase = static_cast<VideoFormatBase*>(fmt.get());

  // Set resolution
  char resolution[32];
  memset(resolution, '\0', 32);
  sprintf(resolution, "%dx%d", videoFmtBase->_mWidth, videoFmtBase->_mHeight);
  av_dict_set(&_mVideoOptionals, "video_size", resolution, 0);

  // Set frame rate
  char frameRate[8];
  memset(frameRate, '\0', 8);
  sprintf(frameRate, "%d", videoFmtBase->_mFrameRate);
  av_dict_set(&_mVideoOptionals, "framerate", frameRate, 0);

  if (VIDEO_YUV == videoFmtBase->_mVideoFormat) {
    YUVFormat* yuvFmt = static_cast<YUVFormat*>(videoFmtBase);
    av_dict_set(&_mVideoOptionals, "pixel_format", YUVFormat::_mFmtUpon[yuvFmt->_mPixelFormat].c_str(), 0);

    switch (yuvFmt->_mPixelFormat) {
    case AV_PIX_FMT_YUV444P:
      _mFrameSize = videoFmtBase->_mWidth * videoFmtBase->_mHeight * 3;
      break;

    case AV_PIX_FMT_UYVY422:
    case AV_PIX_FMT_YUYV422:
    case AV_PIX_FMT_YUV422P:
      _mFrameSize = videoFmtBase->_mWidth * videoFmtBase->_mHeight * 2;
      break;

    case AV_PIX_FMT_NV12:
    case AV_PIX_FMT_NV21:
    case AV_PIX_FMT_YUV420P:
      _mFrameSize = videoFmtBase->_mWidth * videoFmtBase->_mHeight * 3 / 2;
      break;

    default:
      break;
    }
  }

  return AV_SUCCESS;
}

//void AudioRecorder::setDataSink(std::shared_ptr<DataSink> dataSink) {
//  _mDataSink = dataSink;
//}

/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
AV_RET VideoRecorder::readData() {
  int ret = 0;
  if (NULL == _mOutputPkt || NULL == _mInputFmtCtx) {
    LOGE("A Recoder", "Read frame error, format context or output packet null, maybe not initialize");
    return AV_UNINITIALIZE;
  }

  ret = av_read_frame(_mInputFmtCtx, _mOutputPkt);
  if (_mDataSink != nullptr)  {
    _mDataSink->onData((uint8_t*)(_mOutputPkt->data) + 32, _mFrameSize);
  }
  
  av_packet_unref(_mOutputPkt);
  
  return AV_SUCCESS;
}

} //namespace edision
