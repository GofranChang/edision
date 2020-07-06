#include "AudioDevice.h"
#include "MyLogger.h"

using namespace edision;

static FILE* gFout = nullptr;

class MyDataSink : public AVDataSinkBase {
public:
  virtual void onData(uint8_t* data, size_t size) override;
};

void MyDataSink::onData(uint8_t *data, size_t size) {
  LOGD("Main", "onData, size ({})", size);
  if (gFout)
    fwrite(data, 1, size, gFout);
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
  AudioRecorder rec;
  std::string devName = ":0";
  std::string inpName = "avfoundation";
  rec.init(devName, inpName);
  
  std::shared_ptr<AVDataSinkBase> dataSink(new MyDataSink);
  rec.setDataSink(dataSink);
  
  gFout = fopen("/Users/gofran/Documents/workspace/gitproj/FfmpegDemo/resource/newout.pcm", "wb+");
  for (int i = 0; i < 500; i++) {
    rec.record();
  }
  
  fflush(gFout);
  fclose(gFout);
  
  return 0;
}
