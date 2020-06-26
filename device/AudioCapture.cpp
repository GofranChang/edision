#include "AudioCapture.h"
// #include "MyLogger.h"

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);

  av_log_set_level(AV_LOG_DEBUG);

  // Register audio device
  avdevice_register_all();

  // Get format
  AVInputFormat* iformat = av_find_input_format("avfoundation");

  AVFormatContext* fmtCtx = NULL;

  const char* deviceName = ":0";

  char errors[1024] = {0};

  int ret = avformat_open_input(&fmtCtx, deviceName, iformat, NULL);
  if (ret < 0) {
    av_strerror(ret, errors, 1024);
    LOGE("Main", "Open microphone err, err_code {}, err_info {}", ret, errors);
    return -1;
  }
  
  AVPacket pkt;
  av_init_packet(&pkt);
    
  FILE* fOutput = NULL;
  const char* outPutPath = "/Users/gofran/Documents/workspace/gitproj/FfmpegDemo/resource/out.pcm";
  fOutput = fopen(outPutPath, "wb+");
  
  int cnt = 0;
  while ((0 == (ret = av_read_frame(fmtCtx, &pkt))) && (cnt++ < 500)) {
    LOGD("Main", "Read ({}) pkt size {}", cnt, pkt.size);
    fwrite(pkt.data, 1, pkt.size, fOutput);
    av_packet_unref(&pkt);
  }
  
  fclose(fOutput);
  avformat_close_input(&fmtCtx);
  LOGI("Main", "Return 0");
  return 0;
}
