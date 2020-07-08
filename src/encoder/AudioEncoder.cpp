#include "AudioEncoder.h"

#include "MyLogger.h"

namespace edision {

AudioEncoder::AudioEncoder() : _mCodecCtx(NULL)
                             , _mCodec(NULL)
                             , _mFrame(NULL)
                             , _mPacket(NULL) {
}

AudioEncoder::~AudioEncoder() {
  uninit();
}

int AudioEncoder::init(std::string& codecName, AudioConfig& config) {
  avcodec_register_all();
  
  //TODO: Judge if alloc success
  _mCodec = avcodec_find_encoder_by_name(codecName.c_str());
  _mCodecCtx = avcodec_alloc_context3(_mCodec);

  _mConfig = config;
  _mCodecCtx->sample_fmt = _mConfig._mSampleFmt;
  _mCodecCtx->sample_rate = _mConfig._mSampleRate;
  _mCodecCtx->channel_layout = _mConfig._mChannelLayout;
  _mCodecCtx->profile = FF_PROFILE_AAC_HE_V2;
//  _mCodecCtx->profile = _mConfig._mProfile;

  int ret = avcodec_open2(_mCodecCtx, _mCodec, NULL);
  if (ret < 0) {
    LOGE("A Encoder", "Open encoder error, ret ({})", ret);
    return ret;
  }

  _mFrame = av_frame_alloc();
  if (NULL == _mFrame) {
    LOGE("A Encoder", "Alloc AVFrame failed");
    return -1;
  }

  //TODO: Modify this value later
  _mFrame->nb_samples = 512;
  _mFrame->format = _mConfig._mSampleFmt;
  _mFrame->channel_layout = _mConfig._mChannelLayout;
  av_frame_get_buffer(_mFrame, 0);
  if (NULL == _mFrame || NULL == _mFrame->buf[0]) {
    LOGE("A Encoder", "Alloc AVFrame failed");
    return -1;
  }

  _mPacket = av_packet_alloc();
  if (NULL == _mPacket) {
    LOGE("A Encoder", "Alloc AVPacket failed");
    return -2;
  }

  return 0;
}

void AudioEncoder::uninit() {
  //TODO: free codeccontext
  // if (_mCodecCtx != NULL)
  
  if (_mFrame != NULL)
    av_frame_free(&_mFrame);

  if (_mPacket != NULL)
    av_packet_free(&_mPacket);
}

void AudioEncoder::recvThreadFun() {
  // if (NULL == _mPacket) {
  //   LOGE()
  // }
}

int AudioEncoder::encode(const uint8_t* data, size_t size) {
  memcpy((void*)_mFrame->data[0], data, size);

  int ret = 0;
  ret = avcodec_send_frame(_mCodecCtx, _mFrame);
  if (ret < 0) {
    LOGE("A Encoder", "Send audio encoder data error, ret ({})", ret);
    return -1;
  }

  ret = avcodec_receive_packet(_mCodecCtx, _mPacket);
  if (ret < 0) {
    if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
      LOGE("A Encoder", "Encoding error");
      return -3;
    }
  }

  if (_mDataSink != nullptr)
    _mDataSink->onData(_mPacket->data, _mPacket->size);

  return 0;
}

} // namespace edision
