#include "FfmpegVideoEncoder.h"

namespace edision {

FfmpegVideoEncoder::FfmpegVideoEncoder(std::string codecName) : \
        IEncoder(codecName)
        , _mCodecCtx(nullptr)
        , _mCodec(nullptr)
        , _mFrame(nullptr)
        , _mPacket(nullptr)
        , _mPts(0) {
}

FfmpegVideoEncoder::~FfmpegVideoEncoder() {
    uninit();
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

    _mPts = 0;
}

AV_RET FfmpegVideoEncoder::setFormat(std::shared_ptr<IAVFormatBase> srcFmt, std::shared_ptr<IAVFormatBase> dstFmt) {
    IVideoFormatBase* sourceFmtBase = static_cast<IVideoFormatBase*>(srcFmt.get());
    IVideoFormatBase* sinkFmtBase = static_cast<IVideoFormatBase*>(dstFmt.get());

    if (VideoYUV == sourceFmtBase->_mVideoFormat) {
        LOGD("V Encoder", "Input video format is yuv");

        IYUVFormat* yuvFmt = static_cast<IYUVFormat*>(sourceFmtBase);
        if (VideoYUV_420P == yuvFmt->_mYUVPixelFormat) {
            _mCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
            _mFrame->format = AV_PIX_FMT_YUV420P;

            _mOffsetSize.push_back(std::make_pair(0, yuvFmt->_mYSize));
            _mOffsetSize.push_back(std::make_pair(yuvFmt->_mYSize, yuvFmt->_mUSize));
            _mOffsetSize.push_back(std::make_pair(yuvFmt->_mYSize + yuvFmt->_mUSize, yuvFmt->_mVSize));
        } else {
            LOGE("V Encoder", "Unsupported input pixel format");
            return AV_FMT_UNSUPPORT;
        }
    } else {
        LOGE("V Encoder", "Unsupported input pixel format");
        return AV_FMT_UNSUPPORT;
    }

    if (VideoH264 == sinkFmtBase->_mVideoFormat) {
        LOGD("V Encoder", "Encode video, dest format is h264");
        IH264Format* h264Fmt = static_cast<IH264Format*>(sinkFmtBase);

        // Set profile & level
        _mCodecCtx->profile  = h264Fmt->_mProfile;
        _mCodecCtx->level    = h264Fmt->_mLevel;

        // Resolution
        _mCodecCtx->width    = h264Fmt->_mWidth;
        _mCodecCtx->height   = h264Fmt->_mHeight;

        // Gop
        _mCodecCtx->gop_size = h264Fmt->_mGopSize;

        // B Frame
        if (h264Fmt->_mIsBFrame) {
            _mCodecCtx->has_b_frames = 1;
            _mCodecCtx->max_b_frames = h264Fmt->_mMaxBFrames;
        }

        if (h264Fmt->_mRefs > 0)
            _mCodecCtx->refs = h264Fmt->_mRefs;

        // Bitrate
        _mCodecCtx->bit_rate = h264Fmt->_mBitRate;
        // FrameRate
        _mCodecCtx->time_base = (AVRational){1, h264Fmt->_mFrameRate};
        _mCodecCtx->framerate = (AVRational){h264Fmt->_mFrameRate, 1};

    } else if (VideoH265 == sinkFmtBase->_mVideoFormat) {
        // Encode H265
        LOGD("V Encoder", "Encoder video, dest format is h265");
    } else {
        LOGE("V Encoder", "Encode video, unsupport dest format");
        return AV_FMT_UNSUPPORT;
    }

    int ret = avcodec_open2(_mCodecCtx, _mCodec, NULL);
    if (ret < 0) {
        LOGE("A Encoder", "Open encoder error, ret ({})", ret);
        return AV_OPEN_CODEC_ERR;
    }

    _mFrame->width = sinkFmtBase->_mWidth;
    _mFrame->height = sinkFmtBase->_mHeight;
    av_frame_get_buffer(_mFrame, 32);
    if (NULL == _mFrame || NULL == _mFrame->buf[0]) {
        LOGE("A Encoder", "Alloc AVFrame failed");
        return AV_ALLOC_FRAME_ERR;
    }

    return AV_SUCCESS;
}

AV_RET FfmpegVideoEncoder::encode(const uint8_t* data, size_t size) {
    for (int i = 0; i < _mOffsetSize.size(); i++) {
        if (nullptr != _mFrame->data[0])
            memcpy((void*)_mFrame->data[i], data + _mOffsetSize[i].first, _mOffsetSize[i].second);
        else {
            LOGW("V Encoder", "Copy data failed, maybe not initialize");
            return AV_UNINITIALIZE;
        }
    }

    int ret = 0;
    _mFrame->pts = _mPts++;
    ret = avcodec_send_frame(_mCodecCtx, _mFrame);
    if (ret < 0) {
        LOGE("A Encoder", "Send video encoder data error, ret ({})", ret);
        return AV_ENCODE_SEND_ERR;
    }

    ret = avcodec_receive_packet(_mCodecCtx, _mPacket);
    if (ret < 0) {
        if (ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            LOGE("A Encoder", "Encoding error");
            return AV_ENCODE_RECV_ERR;
        }
    }

    if (!_mDataSink.empty()) {
        std::shared_ptr<uint8_t> dataPtr(new uint8_t[_mPacket->size]);
        memcpy(dataPtr.get(), _mPacket->data, _mPacket->size);
        for (auto sink : _mDataSink)
            sink->onData(dataPtr, _mPacket->size);
    }

    return AV_SUCCESS;
}

void FfmpegVideoEncoder::onData(std::shared_ptr<uint8_t> data, size_t size) {
    encode(data.get(), size);
    return;
}

} // namespace edision
