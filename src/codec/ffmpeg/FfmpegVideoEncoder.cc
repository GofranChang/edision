#include "FfmpegVideoEncoder.h"

namespace edision {

FfmpegVideoEncoder::FfmpegVideoEncoder(std::string codecName) : \
        IEncoder(codecName)
        , _mCodecCtx(nullptr)
        , _mCodec(nullptr)
        , _mFrame(nullptr)
        , _mPacket(nullptr) {
}

FfmpegVideoEncoder::~FfmpegVideoEncoder() {
}

AV_RET FfmpegVideoEncoder::init() {
    _mCodec = avcodec_find_encoder_by_name(_mCodecName.c_str());
    if (nullptr == _mCodec) {
        LOGE("A Encoder", "Find encoder \"{}\" failed", _mCodecName);
        return AV_FIND_CODEC_ERR;
    }

    _mCodecCtx = avcodec_alloc_context3(_mCodec);
    if (nullptr == _mCodecCtx) {
        LOGE("A Encoder", "Alloc codec context failed");
        return AV_ALLOC_CODEC_CTX_ERR;
    }

    _mFrame = av_frame_alloc();
    if (nullptr == _mFrame) {
        LOGE("A Encoder", "Alloc AVFrame failed");
        return AV_ALLOC_FRAME_ERR;
    }

    _mPacket = av_packet_alloc();
    if (nullptr == _mPacket) {
        LOGE("A Encoder", "Alloc AVPacket failed");
        return AV_ALLOC_PACKET_ERR;
    }

    return AV_SUCCESS;
}

void FfmpegVideoEncoder::uninit() {
    if (_mCodec != nullptr) {
        // TODO: release codec
    }

    if (_mCodecCtx != nullptr) {
        avcodec_free_context(&_mCodecCtx);
        _mCodecCtx = nullptr;
    }

    if (_mFrame != nullptr) {
        av_frame_free(&_mFrame);
        _mFrame = nullptr;
    }

    if (_mPacket != NULL) {
        av_packet_free(&_mPacket);
        _mPacket = nullptr;
    }
}

AV_RET FfmpegVideoEncoder::setFormat(std::shared_ptr<IAVFormatBase> srcFmt, std::shared_ptr<IAVFormatBase> dstFmt) {
    // _mFormat = dstFmt;
    IVideoFormatBase* sourceFmtBase = static_cast<IVideoFormatBase*>(srcFmt.get());
    IVideoFormatBase* sinkFmtBase = static_cast<IVideoFormatBase*>(dstFmt.get());

    if (VideoYUV == sourceFmtBase->_mMediaType) {
        LOGD("V Encoder", "Input video format is yuv");

        IYUVFormat* yuvFmt = static_cast<IYUVFormat*>(srcFmt.get());
        if (VideoYUV_420P == yuvFmt->_mYUVPixelFormat) {
            _mCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
            _mFrame->format = AV_PIX_FMT_YUV420P;

            _mOffsetSize.push_back(std::make_pair(0, yuvFmt->_mYSize));
            _mOffsetSize.push_back(std::make_pair(yuvFmt->_mYSize, yuvFmt->_mUSize));
            _mOffsetSize.push_back(std::make_pair(yuvFmt->_mYSize + yuvFmt->_mUSize, yuvFmt->_mVSize));
        }
    }

    return AV_SUCCESS;
}

} // namespace edision
