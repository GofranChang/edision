/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: VideoDevice.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_VIDEO_DEVICE_H__
#define __EDISION_VIDEO_DEVICE_H__

#include <iostream>
#include <memory>
#include <string>
#include "MyLogger.h"

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

#include "AVGuard.h"
#include "AVError.h"
#include "DeviceBase.h"

namespace edision {

class VideoRecorder : public InputDeviceBase {
public:
  VideoRecorder();
  ~VideoRecorder();

  virtual AV_RET init(std::string inputName, std::string formatName) override;
//  
  virtual void uninit() override;

  virtual AV_RET readData() override;

  virtual AV_RET setFormat(std::shared_ptr<AVFormatBase> fmt) override;

private:
  AVDictionary* _mVideoOptionals;

  int           _mFrameSize;
};

} // namespace edision

#endif // __EDISION_VIDEO_DEVICE_H__

