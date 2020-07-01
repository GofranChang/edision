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

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);

  av_log_set_level(AV_LOG_DEBUG);
  
  AVFormatContext* fmtCtx = NULL;
  
  int ret = 0;
  ret = initDevice(&fmtCtx);
  
  AVPacket pkt;
  av_init_packet(&pkt);
  
  FILE* fOut = fopen("/Users/gofran/Documents/workspace/gitproj/FfmpegDemo/resource/out.pcm", "wb+");
  for (int i = 0; i < 500; i++) {
    if (recordAudio(fmtCtx, &pkt) < 0) {
      LOGE("device", "Record failed");
      return -1;
    } else {
      LOGD("device", "Read ({}) pkt size {}", i, pkt.size);
      fwrite(pkt.data, 1, pkt.size, fOut);
      av_packet_unref(&pkt);
    }
  }
  
  fclose(fOut);
  avformat_close_input(&fmtCtx);
  LOGI("device", "Return 0");
  
  return ret;
}

#if 0
int audioRecord() {
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
  
  SwrContext *swrCtx = NULL;
  swrCtx = swr_alloc_set_opts(NULL, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100, AV_CH_LAYOUT_MONO, AV_SAMPLE_FMT_FLT, 48000, 0, NULL);
  
  if (NULL == swrCtx) {
    LOGE("Main", "Alloc swr context error");
  }
  
  if (swr_init(swrCtx) < 0) {
    LOGE("Main", "Init swr context error");
  }
  
  uint8_t **srcData = NULL;
  int srcLinesize = 0;
  av_samples_alloc_array_and_samples(&srcData, &srcLinesize, 2, 256, AV_SAMPLE_FMT_FLT, 0);
  
  uint8_t **dstData = NULL;
  int dstLinesize = 0;
  av_samples_alloc_array_and_samples(&dstData, &dstLinesize, 2, 256, AV_SAMPLE_FMT_S16, 0);
  
  int cnt = 0;
  while ((0 == (ret = av_read_frame(fmtCtx, &pkt))) && (cnt++ < 500)) {
    LOGD("Main", "Read ({}) pkt size {}", cnt, pkt.size);
    
//    memcpy((void *)srcData[0], pkt.data, pkt.size);
//    swr_convert(swrCtx, dstData, 512, (const uint8_t **)srcData, 512);
    
    fwrite(pkt.data, 1, pkt.size, fOutput);
//    fwrite(dstData[0], 1, dstLinesize, fOutput);
    av_packet_unref(&pkt);
  }
  
  fclose(fOutput);
  
  if (srcData) av_freep(&srcData[0]);
  av_freep(&srcData);
  
  if (dstData) av_freep(&dstData[0]);
  av_freep(&dstData);
  
  swr_free(&swrCtx);
  
  avformat_close_input(&fmtCtx);
  LOGI("Main", "Return 0");
  return 0;
}

int encodeAudio() {
  int ret = 0;
  
//  avcodec_find_encoder_by_name("libfdk_aac");
  AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
  AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
  codecCtx->sample_fmt = AV_SAMPLE_FMT_FLT;
  codecCtx->channel_layout = AV_CH_LAYOUT_MONO;
  codecCtx->channels = 1;
  codecCtx->sample_rate = 48000;
  codecCtx->bit_rate = 0;
  codecCtx->profile = FF_PROFILE_AAC_HE_V2;
  
  return ret;
}

int main(int argc, const char* argv[]) {
  int ret = 0;
  
  ret = audioRecord();
  return ret;
}
#endif
