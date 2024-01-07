#pragma once

#include <predef.h>
#include "File.h"

namespace csh {
    /**
     * 文件流
     */
    class DLL_OUT FileStream {
    private:
        const csh::File *file   = nullptr;
        FILE            *handle = nullptr;
    public:
        explicit FileStream(const csh::File *file, const wchar_t *mode);

        ~FileStream();
    };
}
