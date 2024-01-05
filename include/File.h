#pragma once

#include <cstdlib>
#include <string>
#include <predef.h>
#include <Windows.h>
#include <FileStream.h>

#ifndef F_OK
#define F_OK 0
#endif

#ifndef W_OK
#define W_OK 2
#endif

#ifndef R_OK
#define R_OK 4
#endif

#ifndef RW_OK
#define RW_OK (W_OK | R_OK)
#endif

namespace csh {
    class DLL_OUT File {
    private:
        std::wstring path;
        struct _stat32i64 stat{};

        int update();
    public:
        explicit File(std::wstring path);

        explicit File(std::wstring dir, std::wstring child);

        ~File();

        USE_RET std::wstring absolutePath() const;

        USE_RET bool exists() const;

        USE_RET bool canRead() const;

        USE_RET bool canWrite() const;

        USE_RET bool isDir();

        USE_RET bool isFile();

        USE_RET u64 length();

        USE_RET u32 lastModified();

        USE_RET bool del() const;

        csh::FileStream open(const wchar_t *mode);

        USE_RET std::wstring getPath() const;

        USE_RET std::wstring getFileName() const;

        USE_RET File getParent() const;

        bool operator==(const File &other) const;

    };
}
