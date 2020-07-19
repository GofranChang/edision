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
AudioRecorder::AudioRecorder() : _mAVPkt(nullptr)
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
AudioRecorder::~AudioRecorder() {
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
AV_RET AudioRecorder::init(std::string& devName, std::string& inpName) {
  avdevice_register_all();

  int ret = avformat_open_input(&_mFmtCtx, devName.c_str(), av_find_input_format(inpName.c_str()), NULL);
  if (ret < 0) {
    char errors[1024];
    av_strerror(ret, errors, 1024);
    LOGE("A Recorder", "Open microphone error, error message \"{}\"", errors);
    return AV_OPEN_INPUT_ERR;
  }
  
  LOGI("A Recorder", "Open microphone {} success", devName);

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

/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
AV_RET AudioRecorder::record() {
  int ret = 0;
  ret = av_read_frame(_mFmtCtx, _mAVPkt);
  if (_mDataSink != nullptr)  {
    _mDataSink->onData((uint8_t*)(_mAVPkt->data), _mAVPkt->size);
  }
  
  av_packet_unref(_mAVPkt);
  
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

} //namespace edision
