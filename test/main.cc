#include <memory>

#include "common/IAVDataSink.h"
#include "common/IAVDataSource.h"
#include "common/IAVFormat.h"
#include "devices/IDevices.h"
#include "codec/ICodec.h"
#include "utils/IFileSink.h"

#include "MyLogger.h"

#include "SDL2/SDL.h"

using namespace edision;

class SDLRenderSink : public IAVDataSink {
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

int main(int argc, const char* argv[]) {
    auto logger = my_media::KooLogger::Instance();
    logger->initLogger(spdlog::level::debug, true, "", false);
    
    // IFileSink
    std::shared_ptr<IFileSink> fileSink(new IFileSink("/Users/gofran/Documents/workspace/gitproj/edision/build/output.h264"));
    fileSink->open();

    // Create FfmpegVideoRecorder
    std::shared_ptr<IInputDevice> ffmpegVideoRecorder(IInputDevice::createNew(DEVICE_FFMPEG_CAMERA));
    std::shared_ptr<IYUVFormat> yuvFmt(new IYUVFormat(VideoYUV_NV12, 1280, 720));
    yuvFmt->_mFrameRate = 30;
    std::shared_ptr<SDLRenderSink> sdlSink(new SDLRenderSink);
    sdlSink->init(1280, 720);
    ffmpegVideoRecorder->setFormat(yuvFmt);
    ffmpegVideoRecorder->setDataSink(sdlSink);
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
    ffmpegVideoRecorder->setDataSink(ffmpegVideoEncoder);
    ffmpegVideoEncoder->setDataSink(fileSink);

    for (int i = 0; i < 500; i++)
        ffmpegVideoRecorder->readData();

    return 0;
}
