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

#ifndef __EDISION_COMMON_BASE_IAVSOURCE__
#define __EDISION_COMMON_BASE_IAVSOURCE__

#include <stdint.h>
#include <stddef.h>
#include <memory>

#include "IAVDataSink.h"

namespace edision {

class IAVDataSource {
public:
  IAVDataSource() : _mDataSink(nullptr) {
  }
  ~IAVDataSource() = default;
  
  inline void setDataSink(std::shared_ptr<IAVDataSink> dataSink) {
    _mDataSink = dataSink;
  }
  
protected:
  std::shared_ptr<IAVDataSink> _mDataSink;
};

} // namespace edision

#endif // __EDISION_COMMON_BASE_IAVSOURCE__
