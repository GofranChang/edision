#ifndef __EDISION_DEVICE_IDEVICE_H__
#define __EDISION_DEVICE_IDEVICE_H__

#include <memory>

#include "IAVDataSink.h"
#include "IAVDataSource.h"
#include "IAVFormat.h"
#include "AVError.h"

namespace edision {

enum DeviceType {
  DEVICE_MICROPHONE,
  DEVICE_CAMERA,
};

class IInputDevice : public IAVDataSource {
protected:
  IInputDevice();
  virtual ~IInputDevice();

public:
  static std::shared_ptr<IInputDevice> createNew(DeviceType type);

  virtual AV_RET init(std::string inputName, std::string formatName);

  virtual AV_RET setFormat(std::shared_ptr<IAVFormat> fmt);

  virtual void uninit();

  virtual AV_RET readData() = 0;

protected:
  AVPacket*                     _mOutputPkt;
  AVFormatContext*              _mInputFmtCtx;
  std::shared_ptr<IAVFormat>    _mFmtBase;
};

} // namespace edision 

#endif // __EDISION_DEVICE_IDEVICE_H__
