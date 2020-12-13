#ifndef __EDISION_DEVICE_FFMPEGVIDEORECORDER_H__
#define __EDISION_DEVICE_FFMPEGVIDEORECORDER_H__

#include "IDevices.h"

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

namespace edision {

class FfmpegVideoRecorder : public IInputDevice {
public:
    FfmpegVideoRecorder();
    ~FfmpegVideoRecorder();

    virtual AV_RET init(std::string inputName, std::string formatName) override;

    virtual void uninit() override;

    virtual AV_RET readData() override;

    virtual AV_RET setFormat(std::shared_ptr<IAVFormatBase> fmt) override;

private:
    AVDictionary* _mVideoOptionals;

    int           _mFrameSize;

    AVPacket*        _mOutputPkt;
    AVFormatContext* _mInputFmtCtx;
};

} // namespace edision

#endif // __EDISION_DEVICE_FFMPEGVIDEORECORDER_H__
