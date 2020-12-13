#include "ICodec.h"

#include "MyLogger.h"
#include "ffmpeg/FfmpegVideoEncoder.h"

namespace edision {

IEncoder::IEncoder(std::string codecName) : _mCodecName(codecName) {
}

IEncoder::~IEncoder() {
}

std::shared_ptr<IEncoder> IEncoder::createNew(CodecType type, std::string& codecName) {
    std::shared_ptr<IEncoder> res(nullptr);

    switch (type) {
    case FFMPEG_VIDEO_ENCODER:
        res.reset(new FfmpegVideoEncoder(codecName));
        break;
    case FFMPEG_AUDIO_ENCODER:
        //res.reset(new FfmpegAudioRecorder());
        break;
    default:
        LOGE("Encoder", "Create failed : unknown encoder type");
        break;
    }

    return res;
}

}; // namespace edision
