#include "AudioDevice.h"

namespace edision {

AudioRecorder::AudioRecorder() : _mAVPkt(nullptr)
                               , _mFmtCtx(avformat_close_input) {
}

int AudioRecorder::init(std::string& devName, std::string& inpName) {
  avdevice_register_all();

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

int AudioRecorder::record() {
  int ret = 0;
  ret = av_read_frame(*(_mFmtCtx.get()), _mAVPkt.get());
  if (_mDataSink.get())  {
    _mDataSink->onData((uint8_t*)(_mAVPkt->data), _mAVPkt->size);
  }
  
  av_packet_unref(_mAVPkt.get());
  
  return ret;
}

AudioResample::AudioResample() : _mSwrCtx(swr_free)
                                 {
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
