#ifndef __EDISION_DEVICE_INPUTDEVICEBASE_H__
#define __EDISION_DEVICE_INPUTDEVICEBASE_H__

#include <memory>

#include "base/AVDataSinkBase.h"
#include "base/AVDataSourceBase.h"
#include "AVFormatBase.h"
#include "AVError.h"

namespace edision {

enum DeviceType {
  DEVICE_MICROPHONE,
  DEVICE_CAMERA,
};

class InputDeviceBase : public AVDataSourceBase {
protected:
  InputDeviceBase();
  virtual ~InputDeviceBase();

public:
  static std::shared_ptr<InputDeviceBase> createNew(DeviceType type);

  virtual AV_RET init(std::string inputName, std::string formatName);

  virtual AV_RET setFormat(std::shared_ptr<AVFormatBase> fmt);

  virtual void uninit();

  virtual AV_RET readData() = 0;

protected:
  AVPacket*                     _mOutputPkt;
  AVFormatContext*              _mInputFmtCtx;
  std::shared_ptr<AVFormatBase> _mFmtBase;
};

} // namespace edision 

#endif