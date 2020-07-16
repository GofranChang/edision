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
VideoRecorder::VideoRecorder() : _mAVPkt(nullptr)
                               , _mFmtCtx(nullptr)
                               , _mVideoOptionals(nullptr) {
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
  uninit();
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
AV_RET VideoRecorder::init(std::string& devName, std::string& inpName, VideoConfig& cfg) {
  avdevice_register_all();

  //TODO: Set optionals
  _mVideoCfg = cfg;

  // Set resolution
  char resolution[32];
  memset(resolution, '\0', 32);
  sprintf(resolution, "%dx%d", _mVideoCfg._mWidth, _mVideoCfg._mHigh);
  av_dict_set(&_mVideoOptionals, "video_size", resolution, 0);

  // Set frame rate
  char frameRate[8];
  memset(frameRate, '\0', 8);
  sprintf(frameRate, "%d", _mVideoCfg._mFrameRate);
  av_dict_set(&_mVideoOptionals, "framerate", frameRate, 0);
  
  av_dict_set(&_mVideoOptionals, "pixel_format", VideoConfig::_mFmtUpon[_mVideoCfg._mFmt].c_str(), 0);

  switch (_mVideoCfg._mFmt) {
  case VIDEO_YUV444:
    _mFrameSize = _mVideoCfg._mWidth * _mVideoCfg._mHigh * 3;
    break;

  case VIDEO_YUV422:
  case VIDEO_UYVY422:
    _mFrameSize = _mVideoCfg._mWidth * _mVideoCfg._mHigh * 2;
    break;

  case VIDEO_NV12:
  case VIDEO_NV21:
    _mFrameSize = _mVideoCfg._mWidth * _mVideoCfg._mHigh * 3 / 2;
    break;

  default:
    break;
  }

  int ret = avformat_open_input(&_mFmtCtx, devName.c_str(), av_find_input_format(inpName.c_str()), &_mVideoOptionals);
  if (ret < 0) {
    char errors[1024];
    av_strerror(ret, errors, 1024);
    LOGE("V Recorder", "Open camera error, error message \"{}\"", errors);
    return AV_OPEN_INPUT_ERR;
  }
  
  LOGI("V Recorder", "Open camera {} success", devName);

  _mAVPkt = av_packet_alloc();
  if (NULL == _mAVPkt) {
    LOGE("V Recorder", "Alloc AVPacket error");
    return AV_ALLOC_PACKET_ERR;
  }

  return AV_SUCCESS;
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
void VideoRecorder::uninit() {
  if (NULL != _mFmtCtx) {
    avformat_close_input(&_mFmtCtx);
  }
  
  if (NULL != _mAVPkt) {
    av_packet_free(&_mAVPkt);
  }

  if (NULL != _mVideoOptionals) {
    av_dict_free(&_mVideoOptionals);
  }
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
AV_RET VideoRecorder::record() {
  int ret = 0;
  ret = av_read_frame(_mFmtCtx, _mAVPkt);
  if (_mDataSink != nullptr)  {
    _mDataSink->onData((uint8_t*)(_mAVPkt->data), _mFrameSize);
  }
  
  av_packet_unref(_mAVPkt);
  
  return AV_SUCCESS;
}

} //namespace edision
