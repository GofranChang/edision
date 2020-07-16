/*
 * copyright (c) 2020 Gofran Chang
 * 
 * FileName: AVDataSourceBase.h
 * 
 * Author  : Gofran Chang
 * E-mail  : zhanggaofan0827@gmail.com
 * 
 * Date    : 2020-07-16
 *
 * This file is part of Edision.
 */

#ifndef __EDISION_COMMON_BASE_AV_SOURCE_BASE__
#define __EDISION_COMMON_BASE_AV_SOURCE_BASE__

#include <stdint.h>
#include <stddef.h>
#include <memory>

#include "AVDataSinkBase.h"

namespace edision {

class AVDataSourceBase {
public:
  AVDataSourceBase() : _mDataSink(nullptr) {
  }
  ~AVDataSourceBase() = default;
  
  inline void setDataSink(std::shared_ptr<AVDataSinkBase> dataSink) {
    _mDataSink = dataSink;
  }
  
protected:
  std::shared_ptr<AVDataSinkBase> _mDataSink;
};

} // namespace edision

#endif // __EDISION_COMMON_BASE_AV_SOURCE_BASE__
