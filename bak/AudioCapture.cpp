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

int initEncoder(AVCodec** codec, AVCodecContext** codecCtx) {
  int ret = 0;
  
  *codec = avcodec_find_encoder_by_name("libfdk_aac");
  *codecCtx = avcodec_alloc_context3(*codec);
  (*codecCtx)->sample_fmt = AV_SAMPLE_FMT_S16;
  (*codecCtx)->sample_rate = 44100;
  (*codecCtx)->channel_layout = AV_CH_LAYOUT_MONO;
  (*codecCtx)->bit_rate = 64000;
 (*codecCtx)->profile = FF_PROFILE_AAC_HE_V2;
  
//  avcodec_open2(*codecCtx, codec, NULL);
  ret = avcodec_open2(*codecCtx, *codec, NULL);
  if (ret < 0) {
    LOGE("device", "Open code error, ret ({})", ret);
    return ret;
  }
  
  return 0;
}

int main(int argc, const char* argv[]) {
  auto logger = my_media::KooLogger::Instance();
  logger->initLogger(spdlog::level::debug, true, "", false);
  
  av_register_all();
  avcodec_register_all();

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
  
  AVCodec* codec = NULL;
  AVCodecContext* codecCtx = NULL;
  ret = initEncoder(&codec, &codecCtx);
  AVFrame* frame = av_frame_alloc();
  frame->nb_samples = 512;
  frame->format = AV_SAMPLE_FMT_S16;
  frame->channel_layout = AV_CH_LAYOUT_MONO;
  av_frame_get_buffer(frame, 0);
  if (!frame || !frame->buf[0]) {
    LOGE("device", "alloc frame error");
    return -2;
  }
  
  AVPacket* newPkt = av_packet_alloc();
  if (NULL == newPkt) {
    LOGE("device", "alloc avpacket error");
    return -3;
  }
  
  AVPacket pkt;
  av_init_packet(&pkt);
  
  FILE* fOut = fopen("/Users/gofran/Documents/workspace/gitproj/FfmpegDemo/resource/out.aac", "wb+");
  for (int i = 0; i < 500; i++) {
    if (recordAudio(fmtCtx, &pkt) < 0) {
      LOGE("device", "Record failed");
      return -1;
    } else {
      LOGD("device", "Read ({}) pkt size {}", i, pkt.size);
      memcpy((void *)srcData[0], pkt.data, pkt.size);
      swr_convert(swrCtx, dstData, 512, (const uint8_t **)srcData, 512);
      
      //Encode
      memcpy((void*)frame->data[0], dstData[0], dstLinesize);
      ret = avcodec_send_frame(codecCtx, frame);
      
      while (ret >= 0) {
        ret = avcodec_receive_packet(codecCtx, newPkt);
        if (ret < 0) {
          if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
          } else {
            LOGE("device", "Encoding error");
            return -3;
          }
        }
        fwrite(newPkt->data, 1, newPkt->size, fOut);
//        if (ret >= 0) {
//          fwrite(<#const void *__ptr#>, <#size_t __size#>, <#size_t __nitems#>, <#FILE *__stream#>)
//        } else {
//          break;
//        }
      }
      
//      fwrite(dstData[0], 1, dstLinesize, fOut);
//      fwrite(pkt.data, 1, pkt.size, fOut);
      av_packet_unref(&pkt);
    }
  }
  
  fclose(fOut);
  
  if (srcData) av_freep(&srcData[0]);
  av_freep(srcData);
  
  if (dstData) av_freep(&dstData[0]);
  av_freep(dstData);
  
  swr_free(&swrCtx);
  
  avformat_close_input(&fmtCtx);
  LOGI("device", "Return 0");
  
  return ret;
}
