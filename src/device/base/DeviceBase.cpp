#include "DeviceBase.h"
#include "MyLogger.h"
#include "AudioDevice.h"
#include "VideoDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavutil/avutil.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"

#ifdef __cplusplus
}
#endif

namespace edision {

InputDeviceBase::InputDeviceBase() : _mOutputPkt(NULL)
                                   , _mInputFmtCtx(NULL) {
}

InputDeviceBase::~InputDeviceBase() {
  uninit();
}

std::shared_ptr<InputDeviceBase> InputDeviceBase::createNew(DeviceType type) {
  std::shared_ptr<InputDeviceBase> res(nullptr);

  switch (type) {
  case DEVICE_CAMERA:
      res.reset(new VideoRecorder());
    break;
  case DEVICE_MICROPHONE:
      res.reset(new AudioRecorder());
    break;
  default:
    LOGE("Encoder", "Create failed : unknown encoder type");
    break;
  }

  return res;
}

AV_RET InputDeviceBase::init(std::string inputName, std::string formatName) {
  avdevice_register_all();

  int ret = avformat_open_input(&_mInputFmtCtx, inputName.c_str(), av_find_input_format(formatName.c_str()), NULL);
  if (ret < 0) {
    char errors[1024];
    av_strerror(ret, errors, 1024);
    LOGE("A Recorder", "Open microphone error, error message \"{}\"", errors);
    return AV_OPEN_INPUT_ERR;
  }
  
  LOGI("A Recorder", "Open microphone {} success", inputName);

  _mOutputPkt = av_packet_alloc();
  if (NULL == _mOutputPkt) {
    LOGE("A Recorder", "Alloc AVPacket error");
    return AV_ALLOC_PACKET_ERR;
  }

  return AV_SUCCESS;
}

AV_RET InputDeviceBase::setFormat(std::shared_ptr<AVFormatBase> fmt) {
  _mFmtBase = fmt;
  return AV_SUCCESS;
}

void InputDeviceBase::uninit() {
  if (NULL != _mInputFmtCtx) {
    avformat_close_input(&_mInputFmtCtx);
  }
  
  if (NULL != _mOutputPkt) {
    av_packet_free(&_mOutputPkt);
  }
}

} // namespace edision
