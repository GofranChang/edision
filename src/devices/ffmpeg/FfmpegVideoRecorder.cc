#include "FfmpegVideoRecorder.h"
#include "MyLogger.h"

namespace edision {

/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
FfmpegVideoRecorder::FfmpegVideoRecorder() : _mVideoOptionals(NULL)
                                           , _mOutputPkt(NULL)
                                           , _mInputFmtCtx(NULL) {
    av_register_all();
}

/**
* Initialize audio recorder:
*   Open input device, alloc AVPacket (for output)
*
* @param devName: Audio input device name
* @param inpName: Input format name.
*                eg. In macos is "avfoundation"
*
*/
FfmpegVideoRecorder::~FfmpegVideoRecorder() {
}

AV_RET FfmpegVideoRecorder::init(std::string inputName, std::string formatName) {
    if (nullptr == _mVideoOptionals) {
        LOGE("V Recorder", "video optional is null, maybe not set record parameter");
        return AV_UNINITIALIZE;
    }

    avdevice_register_all();
    int ret = avformat_open_input(&_mInputFmtCtx, inputName.c_str(), av_find_input_format(formatName.c_str()), &_mVideoOptionals);
    if (ret < 0) {
        char errors[1024];
        av_strerror(ret, errors, 1024);
        LOGE("V Recorder", "Open camera error, error message \"{}\"", errors);
        return AV_OPEN_INPUT_ERR;
    }

    LOGI("V Recorder", "Open camera {} success", inputName);

    _mOutputPkt = av_packet_alloc();
    if (NULL == _mOutputPkt) {
        LOGE("V Recorder", "Alloc AVPacket error");
        return AV_ALLOC_PACKET_ERR;
    }

    return AV_SUCCESS;
}

void FfmpegVideoRecorder::uninit() {
    if (NULL != _mInputFmtCtx) {
        avformat_close_input(&_mInputFmtCtx);
    }

    if (NULL != _mOutputPkt) {
        av_packet_free(&_mOutputPkt);
    }

    if (NULL != _mVideoOptionals) {
        av_dict_free(&_mVideoOptionals);
    }
}

AV_RET FfmpegVideoRecorder::setFormat(std::shared_ptr<IAVFormatBase> fmt) {
    //TODO: Set optionals
    // IInputDevice::setFormat(fmt);

    if (fmt->_mMediaType != VideoType) {
        LOGE("V Recoder", "Set video recorder format failed, format type not video");
        return AV_BAD_PARAMETER;
    }

    IVideoFormatBase* videoFmtBase = static_cast<IVideoFormatBase*>(fmt.get());

    // Set resolution
    char resolution[32];
    memset(resolution, '\0', 32);
    sprintf(resolution, "%dx%d", videoFmtBase->_mWidth, videoFmtBase->_mHeight);
    av_dict_set(&_mVideoOptionals, "video_size", resolution, 0);

    // Set frame rate
    char frameRate[8];
    memset(frameRate, '\0', 8);
    sprintf(frameRate, "%d", videoFmtBase->_mFrameRate);
    av_dict_set(&_mVideoOptionals, "framerate", frameRate, 0);

    if (VideoYUV == videoFmtBase->_mVideoFormat) {
        IYUVFormat* yuvFmt = static_cast<IYUVFormat*>(videoFmtBase);
        av_dict_set(&_mVideoOptionals, "pixel_format", IYUVFormat::_mFmtUpon[yuvFmt->_mYUVPixelFormat].c_str(), 0);

        switch (yuvFmt->_mYUVPixelFormat) {
        case VideoYUV_444P:
            _mFrameSize = videoFmtBase->_mWidth * videoFmtBase->_mHeight * 3;
            break;

        case VideoYUV_UYVY422:
        case VideoYUV_YUYV422:
            _mFrameSize = videoFmtBase->_mWidth * videoFmtBase->_mHeight * 2;
            break;

        case VideoYUV_NV12:
        case VideoYUV_NV21:
        case VideoYUV_420P:
            _mFrameSize = videoFmtBase->_mWidth * videoFmtBase->_mHeight * 3 / 2;
            break;

        default:
            LOGW("V Recorder", "Unsupport yuv format {}", yuvFmt->_mYUVPixelFormat);
            return AV_FMT_UNSUPPORT;
            break;
        }
    } else {
        LOGW("V Recorder", "Set ffmpeg video recorder format, unsupport format {}", videoFmtBase->_mVideoFormat);
        return AV_FMT_UNSUPPORT;
    }

    return AV_SUCCESS;
}

AV_RET FfmpegVideoRecorder::readData() {
    int ret = 0;
    if (NULL == _mOutputPkt || NULL == _mInputFmtCtx) {
        LOGE("V Recoder", "Read frame error, format context or output packet null, maybe not initialize");
        return AV_UNINITIALIZE;
    }

    ret = av_read_frame(_mInputFmtCtx, _mOutputPkt);
    if (!_mDataSink.empty())  {
        std::shared_ptr<uint8_t> dataPtr(new uint8_t[_mFrameSize]);
        memcpy(dataPtr.get(), _mOutputPkt->data, _mFrameSize);
        for (auto sink : _mDataSink)
            sink->onData(dataPtr, _mFrameSize);
    }

    av_packet_unref(_mOutputPkt);

    return AV_SUCCESS;
}

} // namespace edision
