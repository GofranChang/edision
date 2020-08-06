/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AVDataSinkBase.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_COMMON_BASE_IAVSINK__
#define __EDISION_COMMON_BASE_IAVSINK__

#include <stdint.h>
#include <stddef.h>

namespace edision {

class IAVDataSink {
public:
  virtual void onData(uint8_t* data, size_t size) = 0;
};

} // namespace edision

#endif // __EDISION_COMMON_BASE_IAVSINK__
