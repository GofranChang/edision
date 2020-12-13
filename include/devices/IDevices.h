#ifndef __EDISION_DEVICE_IDEVICE_H__
#define __EDISION_DEVICE_IDEVICE_H__

#include <memory>

#include "IAVError.h"
#include "IAVFormat.h"
#include "IAVDataSource.h"

namespace edision {

enum DeviceType {
    DEVICE_FFMPEG_MICROPHONE,
    DEVICE_FFMPEG_CAMERA,
    DEVICE_ANDROID_MICROPHONE,
    DEVICE_ANDROID_CAMERA,
};

class IInputDevice : public IAVDataSource {
protected:
    IInputDevice();
    virtual ~IInputDevice();

public:
    static std::shared_ptr<IInputDevice> createNew(DeviceType type);

    virtual AV_RET init(std::string inputName, std::string formatName) = 0;

    virtual AV_RET setFormat(std::shared_ptr<IAVFormatBase> fmt) = 0;

    virtual void uninit() = 0;

    virtual AV_RET readData() = 0;

// protected:
//     AVPacket*                     _mOutputPkt;
//     AVFormatContext*              _mInputFmtCtx;
//     std::shared_ptr<IAVFormat>    _mFmtBase;
};

}; // namespace edision

#endif // __EDISION_DEVICE_IDEVICE_H__
