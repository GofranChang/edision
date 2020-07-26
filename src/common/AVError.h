/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AVError.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_COMMON_AV_ERROR_H__
#define __EDISION_COMMON_AV_ERROR_H__

namespace edision {

enum AV_RET {
  AV_SUCCESS = 0,
  AV_BAD_PARAMETER,
  AV_UNINITIALIZE = 500,
  AV_FMT_UNSUPPORT,
  AV_OPEN_INPUT_ERR,
  AV_FIND_CODEC_ERR = 1000,
  AV_OPEN_CODEC_ERR,
  AV_ALLOC_FRAME_ERR,
  AV_ALLOC_PACKET_ERR,
  AV_ALLOC_CODEC_CTX_ERR,
  AV_ENCODE_SEND_ERR,
  AV_ENCODE_RECV_ERR,
  AV_ALLOC_SWR_CTX_ERR,
  AV_MEM_EMPTY,
};

} // namespace edision

#endif //__EDISION_COMMON_AV_ERROR_H__
