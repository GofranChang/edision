#include "AudioDevice.h"
#include "MyLogger.h"

using namespace edision;

static FILE* gFout = nullptr;

class MyRecDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
  inline void setResampler(std::shared_ptr<AudioResampler> rsp) {
    _mResampler = rsp;
  }
  
private:
  std::shared_ptr<AudioResampler> _mResampler;
};


class MyResampleDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
};


void MyRecDataSink::onData(uint8_t *data, size_t size) {
  _mResampler->resample(data, size);
}


void MyResampleDataSink::onData(uint8_t *data, size_t size) {
    LOGD("Main", "onData, size ({})", size);
    if (gFout)
      fwrite(data, 1, size, gFout);
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
  AudioRecorder recorder;
  std::string devName = ":0";
  std::string inpName = "avfoundation";
  recorder.init(devName, inpName);
  
  std::shared_ptr<MyRecDataSink> recDataSink(new MyRecDataSink);
  recorder.setDataSink(recDataSink.get());
  
  std::shared_ptr<AudioResampler> resampler(new AudioResampler);
  AudioConfig inCfg;
  AudioConfig outCfg;

  inCfg._mChannelLayout = AV_CH_LAYOUT_MONO;
  inCfg._mChannelNums = 1;
  inCfg._mSampleFmt = AV_SAMPLE_FMT_FLT;
  inCfg._mSampleRate = 48000;

  outCfg._mChannelLayout = AV_CH_LAYOUT_MONO;
  outCfg._mChannelNums = 1;
  outCfg._mSampleFmt = AV_SAMPLE_FMT_S16;
  outCfg._mSampleRate = 44100;
  
  resampler->init(inCfg, outCfg, 512);
  recDataSink->setResampler(resampler);
  
  std::shared_ptr<MyResampleDataSink> resampleSink(new MyResampleDataSink);
  resampler->setDataSink(resampleSink.get());
  
  gFout = fopen("/Users/gofran/Documents/workspace/gitproj/FfmpegDemo/resource/newout.pcm", "wb+");
  for (int i = 0; i < 500; i++) {
//  while (1) {
    recorder.record();
  }
  
  fflush(gFout);
  fclose(gFout);
  
  return 0;
}
