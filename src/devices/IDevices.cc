#include "devices/IDevices.h"

#include "MyLogger.h"
#include "ffmpeg/FfmpegVideoRecorder.h"

namespace edision {

IInputDevice::IInputDevice() {
}

IInputDevice::~IInputDevice() {
}

std::shared_ptr<IInputDevice> IInputDevice::createNew(DeviceType type) {
    std::shared_ptr<IInputDevice> res(nullptr);

    switch (type) {
    case DEVICE_FFMPEG_CAMERA:
        res.reset(new FfmpegVideoRecorder());
        break;
    case DEVICE_FFMPEG_MICROPHONE:
        //res.reset(new FfmpegAudioRecorder());
        break;
    default:
        LOGE("Encoder", "Create failed : unknown encoder type");
        break;
    }

    return res;
}

}; // namespace edision
