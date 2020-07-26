/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AudioEncoder.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_ENCODER_AUDIOENCODER_H__
#define __EDISION_ENCODER_AUDIOENCODER_H__

#include <string>
#include <thread>

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

#include "base/AVDataSinkBase.h"
#include "base/AVDataSourceBase.h"
#include "AVError.h"
#include "EncoderBase.h"

namespace edision {

// TODO: Modify this later
class AudioEncoder : public EncoderBase {
public:
  AudioEncoder(std::string& codecName);
  ~AudioEncoder();

public:
  AV_RET setConfig(std::shared_ptr<AVFormatBase> srcFmt, std::shared_ptr<AVFormatBase> dstFmt) override;
  
  AV_RET encode(const uint8_t* data, size_t size) override;
};

}

#endif //__EDISION_ENCODER_AUDIOENCODER_H__
