#include "common/IAVFormat.h"

namespace edision {

IAVFormatBase::IAVFormatBase(MediaType type) : _mMediaType(type) {
    // Create new audio/video format object
}

IVideoFormatBase::IVideoFormatBase(VideoFormatType type, \
                                    int width, \
                                    int height)
        : _mVideoFormat(type)
        , _mWidth(width)
        , _mHeight(height)
        , IAVFormatBase(VideoType) {
}

std::map<YUVFormatType, std::string>  IYUVFormat::_mFmtUpon = IYUVFormat::createFmtUpon();

IYUVFormat::IYUVFormat(YUVFormatType formatType, \
                        int width, \
                        int height)
        : _mYUVPixelFormat(formatType)
        , IVideoFormatBase(VideoYUV, width, height) {
    if (VideoYUV_420P == _mYUVPixelFormat) {
        _mYOffset = 0;
        _mYSize = width * height;
        _mUOffset = _mYSize;
        _mUSize = width * height / 4;
        _mVOffset = _mYSize + _mUSize;
        _mVSize = width * height / 4;
    }
}

std::map<YUVFormatType, std::string> IYUVFormat::createFmtUpon() {
    std::map<YUVFormatType, std::string> res;

    res[VideoYUV_NV12] = "nv12";
    res[VideoYUV_NV21] = "nv21";
    res[VideoYUV_420P] = "yuv420p";
    res[VideoYUV_UYVY422] = "uyvy422";
    res[VideoYUV_YUYV422] = "yuyv422";

    return res;
}

IH264Format::IH264Format(int width, int height, int framerate) : IVideoFormatBase(VideoH264, width, height) {
}

}; // namespace edision
