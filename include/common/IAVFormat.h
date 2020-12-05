#ifndef __EDISION_COMMON_IAVFORMAT_H__
#define __EDISION_COMMON_IAVFORMAT_H__

#include <map>
#include <string>

#include "MyLogger.h"

namespace edision {

enum MediaType {
    MediaUnknown = -1,
    AudioType = 0,
    VideoType,
};

///////////////////////////// Video Format Type
/** 
 * Video format type
 */
enum VideoFormatType {
    VideoUnknown = -1,

    // YUV format type
    VideoYUV = 0,

    // RGB format type
    VideoRGB,

    // Codeced video type
    VideoH264,
    VideoH265,
};

/** 
 * YUV format type
 */
enum YUVFormatType {
    YUVUnknown = -1,

    // YUV format type
    VideoYUV_420P = 0,
    VideoYUV_NV12,
    VideoYUV_NV21,
    VideoYUV_UYVY422,
    VideoYUV_YUYV422,
    VideoYUV_444P,
};

/**
 * Audio/Video format base
 */
struct IAVFormatBase {
    MediaType _mMediaType;

    IAVFormatBase(MediaType type);
};

/** 
 * Video format base
 */
struct IVideoFormatBase : public IAVFormatBase {
    VideoFormatType _mVideoFormat;
    int             _mWidth;
    int             _mHeight;
    int             _mFrameRate;

    IVideoFormatBase(VideoFormatType type, int width, int height);
};

/** 
 * YUV Format
 */
struct IYUVFormat : public IVideoFormatBase {
    IYUVFormat(YUVFormatType formatType, int wigth, int height);

    YUVFormatType _mYUVPixelFormat;
    int           _mFrameSize;

    // Y U V start location and size
    int           _mYOffset;
    int           _mYSize;
    int           _mUOffset;
    int           _mUSize;
    int           _mVOffset;
    int           _mVSize;

    static std::map<YUVFormatType, std::string> _mFmtUpon;
    static std::map<YUVFormatType, std::string> createFmtUpon();
};

/** 
 * H264 Format
 */
struct IH264Format : public IVideoFormatBase {
    IH264Format(int width, int height, int frameRate);

    // Required
    int _mProfile;
    int _mLevel;
    int _mGopSize;
    int _mBitRate;

    // Optional
    bool _mIsBFrame;
    int _mMaxBFrames;
    int _mRefs;
};

///////////////////////////// Audio Format Type








/** 
 * AudioFormatBase
 */
// struct AudioFormatBase : public IAVFormat {
//   AVSampleFormat _mSampleFmt;
//   int            _mSampleRate;
//   uint64_t       _mChannelLayout;
//   int            _mProfile;
  
//   AudioFormatBase() : IAVFormat(AudioType) {}
// };


} // namespace edision

#endif // __EDISION_FORMAT_IAVFORMAT_H__
