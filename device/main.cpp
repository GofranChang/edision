#include "AudioDevice.h"
#include "MyLogger.h"

using namespace edision;

class MyDataSink : public DataSink {
public:
  virtual void onData(uint8_t* data, size_t size) override;
};

void MyDataSink::onData(uint8_t *data, size_t size) {
  LOGD("Main", "onData, size ({})", size);
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
  AudioRecorder rec;
  std::string devName = ":0";
  std::string inpName = "avfoundation";
  rec.init(devName, inpName);
  
  std::shared_ptr<DataSink> dataSink(new MyDataSink);
  rec.setDataSink(dataSink);
  
  for (int i = 0; i < 500; i++) {
    rec.record();
  }
  
  return 0;
}
