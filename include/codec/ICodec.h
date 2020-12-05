#ifndef __EDISION_CODEC_ICODEC_H__
#define __EDISION_CODEC_ICODEC_H__

#include <memory>

#include "common/IAVError.h"
#include "common/IAVFormat.h"
#include "common/IAVDataSource.h"

namespace edision {

enum CodecType {
    FFMPEG_VIDEO_ENCODER,
    FFMPEG_AUDIO_ENCODER,
    FFMPEG_VIDEO_DECODER,
    FFMPEG_AUDIO_DECODER,
    ANDROID_MEDIA_CODEC,
    // DEVICE_ANDROID_MICROPHONE,
    // DEVICE_ANDROID_CAMERA,
};

class IEncoder : public IAVDataSource
               , public IAVDataSink {
protected:
    IEncoder(std::string codecName);
    virtual ~IEncoder();

public:
    static std::shared_ptr<IEncoder> createNew(CodecType type, std::string& codecName);

    virtual AV_RET init() = 0;

    virtual void uninit() = 0;

    virtual AV_RET setFormat(std::shared_ptr<IAVFormatBase> srcFmt, std::shared_ptr<IAVFormatBase> dstFmt) = 0;

    virtual AV_RET encode(const uint8_t* data, size_t size) = 0;

protected:
    std::string _mCodecName;

// protected:
//     AVPacket*                     _mOutputPkt;
//     AVFormatContext*              _mInputFmtCtx;
//     std::shared_ptr<IAVFormat>    _mFmtBase;
};

}; // namespace edision

#endif // __EDISION_CODEC_ICODEC_H__
