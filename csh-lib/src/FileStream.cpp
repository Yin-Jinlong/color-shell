#include <FileStream.h>
#include <stdexcept>


csh::FileStream::FileStream(const csh::File *file, const wchar_t *mode) {
    this->file = file;
    if (!_wfopen_s(&handle, file->getPath().c_str(), mode))
        throw std::runtime_error("Failed to open file");
}

csh::FileStream::~FileStream() {
    fclose(handle);
    handle = nullptr;
}

