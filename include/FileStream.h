#pragma once

#include <predef.h>
#include "File.h"

namespace csh {
    class LIB_API FileStream {
    private:
        const csh::File *file= nullptr;
        FILE *handle = nullptr;
    public:
        explicit FileStream(const csh::File *file, const wchar_t *mode);

        ~FileStream();
    };
}
