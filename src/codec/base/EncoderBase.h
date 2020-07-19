/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: EncoderBase.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_CODEC_ENCODERBASE_H__
#define __EDISION_CODEC_ENCODERBASE_H__

#include <string>
#include <thread>

#include "base/AVDataSinkBase.h"
#include "base/AVDataSourceBase.h"
#include "AVConfig.h"
#include "AVError.h"

namespace edision {

enum MediaType {
  AUDIO = 0,
  VIDEO,
};

class EncoderBase : public AVDataSourceBase {
protected:
  EncoderBase(std::string& codecName);
  virtual ~EncoderBase() = default;

public:
  static std::shared_ptr<EncoderBase> createNew(MediaType type, std::string& codecName);

  virtual AV_RET init();
  virtual AV_RET setConfig(std::shared_ptr<MediaConfig> config) = 0;
  virtual void uninit();

  virtual AV_RET encode(const uint8_t* data, size_t size) = 0;

protected:
  MediaType _mType;
  std::string _mCodecName;
  
  std::shared_ptr<MediaConfig> _mConfig;

  AVCodecContext* _mCodecCtx;
  AVCodec *_mCodec;
//  MediaConfig _mConfig;

  AVFrame* _mFrame;
  AVPacket* _mPacket;
};

/*
class AudioDataSource;

class AudioEncoder : public AVDataSourceBase {
public:
  AudioEncoder();
  ~AudioEncoder();

  AV_RET init(std::string& codecName, AudioConfig& config);
  
  void uninit();

  void recvThreadFun();

  AV_RET encode(const uint8_t* data, size_t size);

private:
  AVCodecContext* _mCodecCtx;
  AVCodec *_mCodec;
  AudioConfig _mConfig;

  AVFrame* _mFrame;
  AVPacket* _mPacket;

private:
  std::thread _mRecvThread;
};
*/

}

#endif //__EDISION_CODEC_ENCODERBASE_H__
