#include <memory>

#include "IAVDataObserver.h"
#include "IAVDataSource.h"
#include "IAVFormat.h"
#include "IDevices.h"
#include "ICodec.h"
#include "IFileSink.h"

#include "MyLogger.h"

#include "SDL2/SDL.h"

using namespace edision;

void NV12_YUV420P(const unsigned char* image_src, unsigned char* image_dst,
                  int image_width, int image_height){
    unsigned char* p = image_dst;
    memcpy(p, image_src, image_width * image_height * 3 / 2);
    const unsigned char* pNV = image_src + image_width * image_height;
    unsigned char* pU = p + image_width * image_height;
    unsigned char* pV = p + image_width * image_height + ((image_width * image_height)>>2);
    for (int i = 0; i < (image_width * image_height)/2; ++i) {
        if (0 == (i % 2))
            *pU++ = *(pNV + i);
        else
            *pV++ = *(pNV + i);
    }
}

#if 0
int main(int argc, const char* argv[]) {
    auto logger = my_media::KooLogger::Instance();
    logger->initLogger(spdlog::level::debug, true, "", false);

    // IFileSink
    std::shared_ptr<IFileSink> fileSink(new IFileSink("/Users/gofran/Documents/workspace/gitproj/edision/build/output.h264"));
    fileSink->open();

    // SDLSink
    std::shared_ptr<SDLRenderSink> sdlSink(new SDLRenderSink);
    sdlSink->init(1280, 720);

    // Create FfmpegVideoRecorder
    std::shared_ptr<IInputDevice> ffmpegVideoRecorder(IInputDevice::createNew(DEVICE_FFMPEG_CAMERA));
    std::shared_ptr<IYUVFormat> yuvFmt(new IYUVFormat(VideoYUV_NV12, 1280, 720));
    yuvFmt->_mFrameRate = 30;
    ffmpegVideoRecorder->setFormat(yuvFmt);
    ffmpegVideoRecorder->setDataObserver(sdlSink);
    ffmpegVideoRecorder->init("0", "avfoundation");

    // Create FfmpegVideoEncoder
    std::string codecName = "libx264";
    std::shared_ptr<IEncoder> ffmpegVideoEncoder(IEncoder::createNew(FFMPEG_VIDEO_ENCODER, codecName));
    ffmpegVideoEncoder->init();
    std::shared_ptr<IH264Format> h264Format(new IH264Format(1280, 720, 30));
    h264Format->_mBitRate = 1000 * 1024;
    h264Format->_mProfile = 144;
    h264Format->_mGopSize = 25;
    h264Format->_mFrameRate = 30;
    std::shared_ptr<IYUVFormat> yuvFmt2(new IYUVFormat(VideoYUV_420P, 1280, 720));
    ffmpegVideoEncoder->setFormat(yuvFmt2, h264Format);
    ffmpegVideoEncoder->setDataObserver(fileSink);

    ffmpegVideoRecorder->setDataObserver(ffmpegVideoEncoder);

    for (int i = 0; i < 500; i++)
        ffmpegVideoRecorder->readData();

    return 0;
}
#endif

class SDLRenderSink : public IAVDataObserver {
public:
    SDLRenderSink() = default;
    ~SDLRenderSink();

    int init(int width, int height);
    virtual void onData(std::shared_ptr<uint8_t> data, size_t size) override;

private:
    SDL_Window*   _mSDLScreen;
    SDL_Renderer* _mSDLRenderer;
    SDL_Texture*  _mSDLTexture;
    SDL_Rect      _mSDLRect;
};

SDLRenderSink::~SDLRenderSink() {
    if (_mSDLScreen != nullptr)
        SDL_DestroyWindow(_mSDLScreen);

    SDL_Quit();
}

int SDLRenderSink::init(int width, int height) {
    if(SDL_Init(SDL_INIT_EVERYTHING)) {
        LOGE("main", "Could not initialize SDL - {}", SDL_GetError());
        return -1;
    }

    _mSDLScreen = SDL_CreateWindow("Simply Darwin player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
    if(!_mSDLScreen) {
        LOGE("SDL: could not create window - exiting:%s\n",SDL_GetError());
        return -1;
    }

    _mSDLRenderer = SDL_CreateRenderer(_mSDLScreen, -1, 0);
    _mSDLTexture = SDL_CreateTexture(_mSDLRenderer, SDL_PIXELFORMAT_NV12, SDL_TEXTUREACCESS_STREAMING, width, height);

    _mSDLRect.x = 0;
    _mSDLRect.y = 0;
    _mSDLRect.w = width;
    _mSDLRect.h = height;

    return 0;
}

void SDLRenderSink::onData(std::shared_ptr<uint8_t> data, size_t size) {
    LOGD("Rendor", "onData, size {}", size);
    SDL_UpdateTexture(_mSDLTexture, NULL, data.get(), 1280);

    SDL_RenderClear(_mSDLRenderer);
    SDL_RenderCopy(_mSDLRenderer, _mSDLTexture, NULL, NULL);
    SDL_RenderPresent(_mSDLRenderer);
}

class videoConvertSink : public IAVDataObserver {
public:
    videoConvertSink() = default;
    ~videoConvertSink() = default;
    
    AV_RET init();

    // int init(int width, int height);
    virtual void onData(std::shared_ptr<uint8_t> data, size_t size) override;
    
private:
    std::shared_ptr<IEncoder> _mEncoder;
    std::shared_ptr<IFileSink> _mEncoderedDataFileSink;
};

AV_RET videoConvertSink::init() {
    std::string codecName = "libx264";
    _mEncoder = IEncoder::createNew(FFMPEG_VIDEO_ENCODER, codecName);
    
    _mEncoderedDataFileSink.reset(new IFileSink("./output.h264"));
    
    _mEncoder->init();
    std::shared_ptr<IH264Format> h264Format(new IH264Format(1280, 720, 30));
    h264Format->_mBitRate = 1000 * 1024;
    h264Format->_mProfile = 144;
    h264Format->_mGopSize = 25;
    h264Format->_mFrameRate = 30;
    std::shared_ptr<IYUVFormat> yuvFmt2(new IYUVFormat(VideoYUV_420P, 1280, 720));
    _mEncoder->setFormat(yuvFmt2, h264Format);
    _mEncoder->setDataObserver(_mEncoderedDataFileSink);
    
    _mEncoderedDataFileSink->open();
    
    return AV_SUCCESS;
}

void videoConvertSink::onData(std::shared_ptr<uint8_t> data, size_t size) {
    uint8_t* yuv420pData = new uint8_t[1280 * 720 * 3 / 2];
    NV12_YUV420P(data.get(), yuv420pData, 1280, 720);
    
    _mEncoder->encode(yuv420pData, 1280 * 720 * 3 / 2);
    
    delete[] yuv420pData;
}

int main(int argc, const char* argv[]) {
    auto logger = my_media::KooLogger::Instance();
    logger->initLogger(spdlog::level::debug, true, "", false);

    // SDLSink
    std::shared_ptr<SDLRenderSink> sdlSink(new SDLRenderSink);
    sdlSink->init(1280, 720);

    // Create FfmpegVideoRecorder
    std::shared_ptr<IInputDevice>
        ffmpegVideoRecorder(IInputDevice::createNew(DEVICE_FFMPEG_CAMERA));

    std::shared_ptr<IYUVFormat> yuvFmt(new IYUVFormat(VideoYUV_NV12, 1280, 720));
    yuvFmt->_mFrameRate = 30;
    ffmpegVideoRecorder->setFormat(yuvFmt);
    ffmpegVideoRecorder->setDataObserver(sdlSink);
    ffmpegVideoRecorder->init("0", "avfoundation");
    
    std::shared_ptr<videoConvertSink> converterSink(new videoConvertSink);
    converterSink->init();
    ffmpegVideoRecorder->setDataObserver(converterSink);

    for (int i = 0; i < 500; i++)
        ffmpegVideoRecorder->readData();

    return 0;
}
