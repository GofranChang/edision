/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AudioDevice.cpp
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#include "AudioDevice.h"

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
AudioRecorder::AudioRecorder() {
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
AV_RET AudioRecorder::readData() {
  int ret = 0;
  if (NULL == _mOutputPkt || NULL == _mInputFmtCtx) {
    LOGE("A Recoder", "Read frame error, format context or output packet null, maybe not initialize");
    return AV_UNINITIALIZE;
  }

  ret = av_read_frame(_mInputFmtCtx, _mOutputPkt);
  if (_mDataSink != nullptr)  {
    LOGT("A Recoder", "Read audio data, size {}", _mOutputPkt->size)
    _mDataSink->onData((uint8_t*)(_mOutputPkt->data), _mOutputPkt->size);
  } else {
    LOGW("A Recoder", "Read audio data, but not set sink");
  }
  
  av_packet_unref(_mOutputPkt);
  
  return AV_SUCCESS;
}

#if 0
/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
AudioResampler::AudioResampler() : _mSwrCtx(NULL)
                               , _mSrcNbSample(1024)
                               , _mSrcData(NULL)
                               , _mDstData(NULL) {
}

AudioResampler::~AudioResampler() {
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
AV_RET AudioResampler::init(AudioConfig& inCfg, AudioConfig& outCfg, int srcNbSample) {
  _mSrcCfg = inCfg;
  _mSinkCfg = outCfg;

//  SwrContext** ctx = _mSwrCtx.get();
  _mSwrCtx = swr_alloc_set_opts(NULL, _mSinkCfg._mChannelLayout, _mSinkCfg._mSampleFmt, _mSinkCfg._mSampleRate, _mSrcCfg._mChannelLayout, _mSrcCfg._mSampleFmt, _mSrcCfg._mSampleRate, 0, NULL);
  
  if (NULL == _mSwrCtx) {
    LOGE("A Resample", "Alloc swrcontext error");
    return AV_ALLOC_SWR_CTX_ERR;
  }
  
  if (swr_init(_mSwrCtx)) {
    LOGE("A Resample", "Init swr context error");
    return AV_ALLOC_SWR_CTX_ERR;
  }
  
  // allocate src & dst buffer
  _mSrcNbSample = srcNbSample;
  int src_buf_size = av_samples_alloc_array_and_samples(&_mSrcData, &_mSrcLinesize, _mSrcCfg._mChannelNums, _mSrcNbSample, _mSrcCfg._mSampleFmt, 0);
  
  _mMaxDstNbSample = _mDstNbSample = (int)av_rescale_rnd(_mSrcNbSample, _mSinkCfg._mSampleRate, _mSrcCfg._mSampleRate, AV_ROUND_UP);
  
  int dst_buf_size = av_samples_alloc_array_and_samples(&_mDstData, &_mDstLinesize, _mSinkCfg._mChannelNums, _mDstNbSample, _mSinkCfg._mSampleFmt, 0);
  
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
AV_RET AudioResampler::resample(const uint8_t *data, size_t size) {
  if (NULL != _mSrcData) {
    memcpy((void *)_mSrcData[0], data, size);
    int ret = swr_convert(_mSwrCtx, _mDstData, _mDstNbSample, (const uint8_t **)_mSrcData, _mSrcNbSample);
  
    if (_mDataSink != nullptr)
      _mDataSink->onData(_mDstData[0], _mDstLinesize);
  
    return AV_SUCCESS;
  }
  
  return AV_MEM_EMPTY;
}

void AudioResampler::uninit() {
  if (_mSrcData) av_freep(&_mSrcData[0]);
  av_freep(_mSrcData);
  
  if (_mDstData) av_freep(&_mDstData[0]);
  av_freep(_mDstData);
  
  swr_free(&_mSwrCtx);
}
#endif

} //namespace edision
