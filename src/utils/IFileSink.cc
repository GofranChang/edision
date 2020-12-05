#include "utils/IFileSink.h"
#include "MyLogger.h"

namespace edision {

IFileSink::IFileSink(std::string fileName) : _mFileName(fileName)
                                         , _mFp(nullptr) {
}

AV_RET IFileSink::open() {
    _mFp = ::fopen(_mFileName.c_str(), "wb");
    if (nullptr == _mFp) {
        LOGE("FileSink", "Open output file {} error", _mFileName);
        return AV_OPEN_FILE_ERR;
    }

    return AV_SUCCESS;
}

AV_RET IFileSink::close() {
    if (_mFp != nullptr)
        ::fclose(_mFp);
}

void IFileSink::onData(std::shared_ptr<uint8_t> data, size_t size) {
    if (_mFp != nullptr) {
        ::fwrite(data.get(), 1, size, _mFp);
    }
}

IFileSink::~IFileSink() {
    this->close();
}

} // namespace edision
