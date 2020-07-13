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
                               , _mFmtCtx(nullptr) {
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
AV_RET VideoRecorder::init(std::string& devName, std::string& inpName) {
  avdevice_register_all();

  //TODO: Set optionals

  AVDictionary* optional = NULL;
  av_dict_set(&optional, "video_size", "640x480", 0);
  av_dict_set(&optional, "framerate", "30", 0);

  int ret = avformat_open_input(&_mFmtCtx, devName.c_str(), av_find_input_format(inpName.c_str()), &optional);
  if (ret < 0) {
    char errors[1024];
    av_strerror(ret, errors, 1024);
    LOGE("V Recorder", "Open camera error, error message \"{}\"", errors);
    return AV_OPEN_INPUT_ERR;
  }
  
  LOGI("V Recorder", "Open camera {} success", devName);

  _mAVPkt = av_packet_alloc();
  if (NULL == _mAVPkt) {
    LOGE("A Recorder", "Alloc AVPacket error");
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
    _mDataSink->onData((uint8_t*)(_mAVPkt->data), _mAVPkt->size);
  }
  
  av_packet_unref(_mAVPkt);
  
  return AV_SUCCESS;
}

} //namespace edision
