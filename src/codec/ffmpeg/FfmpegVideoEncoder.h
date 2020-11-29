#ifndef __EDISION_CODEC_FFMPEGVIDEOCODEC_H__
#define __EDISION_CODEC_FFMPEGVIDEOCODEC_H__

#include "codec/ICodec.h"

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

class FfmpegVideoEncoder : public IEncoder {
public:
    FfmpegVideoEncoder(std::string codecName);
    virtual ~FfmpegVideoEncoder();

    virtual AV_RET init() override;

    virtual void uninit() override;

    virtual AV_RET readData() override;

    virtual AV_RET setFormat(std::shared_ptr<IAVFormatBase> srcFmt, std::shared_ptr<IAVFormatBase> dstFmt) override;

private:
    std::vector<std::pair<int, int>> _mOffsetSize;

    AVCodecContext* _mCodecCtx;
    AVCodec *_mCodec;

    AVFrame* _mFrame;
    AVPacket* _mPacket;
    // std::string _mCodecName;
    // AVDictionary* _mVideoOptionals;

    // int           _mFrameSize;

    // AVPacket*        _mOutputPkt;
    // AVFormatContext* _mInputFmtCtx;
};

} // namespace edision

#endif // __EDISION_CODEC_FFMPEGVIDEOCODEC_H__
