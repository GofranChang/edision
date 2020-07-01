#include "AudioCapture.h"

#include <string.h>
// #include "MyLogger.h"

int initDevice(AVFormatContext** fmtCtx) {
  avdevice_register_all();
  
//  AVFormatContext* fmtCtx = NULL;
  char errors[1024] = {0};
  
  int ret = avformat_open_input(fmtCtx, ":0", av_find_input_format("avfoundation"), NULL);
  if (ret < 0) {
    av_strerror(ret, errors, 1024);
    LOGE("device", "Open microphone error, error code {}, err info {}", ret, errors);
    return -1;
  }
  
  return 0;
}

int recordAudio(AVFormatContext* fmtCtx, AVPacket* pkt) {
  return av_read_frame(fmtCtx, pkt);
}

int initResample(SwrContext **swrCtx) {
  *swrCtx = swr_alloc_set_opts(NULL, AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_S16, 44100, AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_FLT, 48000, 0, NULL);
  
  if (NULL == *swrCtx) {
    LOGE("Main", "Alloc swr context error");
    return -1;
  }
  
  if (swr_init(*swrCtx) < 0) {
    LOGE("Main", "Init swr context error");
    return -2;
  }
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);

  av_log_set_level(AV_LOG_DEBUG);
  
  AVFormatContext* fmtCtx = NULL;
  
  int ret = 0;
  ret = initDevice(&fmtCtx);
  
  SwrContext* swrCtx = NULL;
  ret = initResample(&swrCtx);
  
  uint8_t **srcData = NULL;
  int srcLinesize = 0;
  av_samples_alloc_array_and_samples(&srcData, &srcLinesize, 1, 512, AV_SAMPLE_FMT_FLT, 0);
  
  uint8_t **dstData = NULL;
  int dstLinesize = 0;
  av_samples_alloc_array_and_samples(&dstData, &dstLinesize, 1, 512, AV_SAMPLE_FMT_S16, 0);
  
  AVPacket pkt;
  av_init_packet(&pkt);
  
  FILE* fOut = fopen("/Users/gofran/Documents/workspace/gitproj/FfmpegDemo/resource/out.pcm", "wb+");
  for (int i = 0; i < 500; i++) {
    if (recordAudio(fmtCtx, &pkt) < 0) {
      LOGE("device", "Record failed");
      return -1;
    } else {
      LOGD("device", "Read ({}) pkt size {}", i, pkt.size);
      memcpy((void *)srcData[0], pkt.data, pkt.size);
      swr_convert(swrCtx, dstData, 512, (const uint8_t **)srcData, 512);
      
      fwrite(dstData[0], 1, dstLinesize, fOut);
//      fwrite(pkt.data, 1, pkt.size, fOut);
      av_packet_unref(&pkt);
    }
  }
  
  fclose(fOut);
  avformat_close_input(&fmtCtx);
  LOGI("device", "Return 0");
  
  return ret;
}
