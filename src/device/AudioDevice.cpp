#include "AudioDevice.h"

namespace edision {

AudioRecorder::AudioRecorder() : _mAVPkt(nullptr)
                               , _mFmtCtx(nullptr) {
}

AudioRecorder::~AudioRecorder() {
  uninit();
}

int AudioRecorder::init(std::string& devName, std::string& inpName) {
  avdevice_register_all();

  int ret = avformat_open_input(&_mFmtCtx, devName.c_str(), av_find_input_format(inpName.c_str()), NULL);
  if (ret < 0) {
    char errors[1024];
    av_strerror(ret, errors, 1024);
    LOGE("A Recorder", "Open microphone error, error message \"{}\"", errors);
    return -1;
  }
  
  LOGI("A Recorder", "Open microphone {} success", devName);
  LOGI("A Recorder", "");

  _mAVPkt = av_packet_alloc();
  if (NULL == _mAVPkt) {
    LOGE("A Recorder", "Alloc AVPacket error");
    return -2;
  }

  return 0;
}

void AudioRecorder::uninit() {
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

int AudioRecorder::record() {
  int ret = 0;
  ret = av_read_frame(_mFmtCtx, _mAVPkt);
  if (_mDataSink.get())  {
    _mDataSink->onData((uint8_t*)(_mAVPkt->data), _mAVPkt->size);
  }
  
  av_packet_unref(_mAVPkt);
  
  return ret;
}

AudioResample::AudioResample() : _mSwrCtx(swr_free) {
}

int AudioResample::init(AudioConfig& inCfg, AudioConfig& outCfg) {
  _mSrcCfg = inCfg;
  _mSinkCfg = outCfg;

  SwrContext** ctx = _mSwrCtx.get();
  *ctx = swr_alloc_set_opts(NULL, _mSinkCfg._mChannelLayout, _mSinkCfg._mSampleFmt, _mSinkCfg._mSampleRate, AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_FLT, 48000, 0, NULL);
  
  if (NULL == *ctx) {
    LOGE("A Resample", "Alloc swrcontext error");
    return -1;
  }
  
  if (swr_init(*ctx)) {
    LOGE("A Resample", "Init swr context error");
    return -2;
  }
  
  return 0;
}

void AudioResample::onData(uint8_t* data, size_t size) {
  memcpy(NULL, data, size);
}

} //namespace edision
