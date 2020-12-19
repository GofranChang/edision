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

#ifndef __EDISION_COMMON_IAVDATASINK_H__
#define __EDISION_COMMON_IAVDATASINK_H__

#include <stdint.h>
#include <stddef.h>
#include <memory>

namespace edision {

class IAVDataObserver {
public:
    virtual void onData(std::shared_ptr<uint8_t> data, size_t size) = 0;
};

} // namespace edision

#endif // __EDISION_COMMON_IAVDATASINK_H__
