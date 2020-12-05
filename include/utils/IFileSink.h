#ifndef __EDISION_UTILS_IFILESINK_H__
#define __EDISION_UTILS_IFILESINK_H__

#include <string>
#include <stdio.h>

#include "common/IAVDataSink.h"
#include "common/IAVError.h"

namespace edision {

class IFileSink : public IAVDataSink {
public:
    IFileSink(std::string fileName);
    AV_RET open();
    AV_RET close();

    virtual void onData(std::shared_ptr<uint8_t> data, size_t size) override;

    ~IFileSink();

private:
    std::string _mFileName;
    FILE* _mFp;
};

}; // namespace edision

#endif // __EDISION_UTILS_IFILESINK_H__
