#include "AudioDevice.h"

namespace edision {

AudioRecorder::AudioRecorder() : _mDataSink(nullptr)
                               , _mAVPkt(nullptr) {
}

int AudioRecorder::init(std::string& devName, std::string& inpName) {
  avdevice_register_all();
  
  AVFormatContext* fmtCtx = NULL;

  int ret = avformat_open_input(_mFmtCtx.get(), devName.c_str(), av_find_input_format(inpName.c_str()), NULL);
  if (ret < 0) {
    char errors[1024];
    av_strerror(ret, errors, 1024);
    LOGE("A Recorder", "Open microphone error, error message \"{}\"", errors);
    return -1;
  }
  
  LOGI("A Recorder", "Open microphone {} success", devName);
  _mAVPkt.reset(av_packet_alloc());

  return 0;
}

//void AudioRecorder::setDataSink(std::shared_ptr<DataSink> dataSink) {
//  _mDataSink = dataSink;
//}

int AudioRecorder::record() {
  int ret = 0;
  ret = av_read_frame(*(_mFmtCtx.get()), _mAVPkt.get());
  if (_mDataSink.get())  {
    _mDataSink->onData((uint8_t*)(_mAVPkt->data), _mAVPkt->size);
  }
  
  av_packet_unref(_mAVPkt.get());
  
  return ret;
}

} //namespace edision
