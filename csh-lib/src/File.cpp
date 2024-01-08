#include "str/wstring-util.h"
#include <File.h>

#include <utility>
#include <sys/stat.h>
#include <fstream>
#include <corecrt_io.h>
#include <Windows.h>

csh::File::File(wstr path) {
    this->path = std::move(path);
}

csh::File::File(const wstr &dir, const wstr &child) {
    if (!dir.ends_with(L"\\") && !child.ends_with(L"/")) {
        path = dir + L"\\" + child;
    } else {
        path = dir + child;
    }
    for (wchar_t &c: path) {
        if (c == L'/')
            c = L'\\';
    }
}

csh::File::~File() = default;

int csh::File::update() {
    return _wstat64i32(path.c_str(), &stat);
}

#define CHECK(f) _waccess_s(path.c_str(), f)==0

bool csh::File::exists() const {
    return CHECK(F_OK);
}

bool csh::File::canRead() const {
    return CHECK(R_OK);
}

bool csh::File::canWrite() const {
    return CHECK(W_OK);
}

#undef CHECK

bool csh::File::isDir() {
    update();
    return stat.st_mode & S_IFDIR;
}

bool csh::File::isFile() {
    update();
    return stat.st_mode & S_IFREG;
}

uint64_t csh::File::length() {
    update();
    return stat.st_size;
}

u32 csh::File::lastModified() {
    update();
    return stat.st_mtime;
}

bool csh::File::del() const {
    return _wremove(path.c_str()) == 0;
}

wstr csh::File::absolutePath() const {
    wchar_t buf[MAX_PATH];
    GetFullPathNameW(path.c_str(), MAX_PATH, buf, nullptr);
    return buf;
}

std::fstream csh::File::open(std::ios_base::openmode mode) {
    if (!isFile())
        throw std::runtime_error("not a file");
    return std::fstream(path, mode);
}

wstr csh::File::readAllTexts() {
    if (!isFile())
        throw std::runtime_error("not a file");
    FILE    *fs;
    errno_t err = _wfopen_s(&fs, path.c_str(), L"r");
    if (err)
        throw std::runtime_error(std::format("open file failed : ", err));
    wstr    texts;
    wchar_t buf[4096];
    while (fgetws(buf, 4096, fs)) {
        texts += buf;
    }
    fclose(fs);
    return texts;
}

wstr csh::File::getPath() const {
    return path;
}

wstr csh::File::getFileName() const {
    wstr fullPath = absolutePath();
    return fullPath.substr(fullPath.find_last_of(L"/\\") + 1);
}

csh::File csh::File::getParent() const {
    wstr         p = absolutePath();
    unsigned int i = p.find_last_of('\\');
    if (p.find_first_of('\\') == i)
        return csh::File(p);
    return File(p.substr(0, i));
}

bool csh::File::operator==(const csh::File &other) const {
    return absolutePath() == other.absolutePath();
}

DWORD mk(const wstr &name) {
    CreateDirectoryW(name.c_str(), nullptr);
    return GetLastError();
}

void mks(const std::vector<wstr> &paths) {
    wstr            p;
    for (const wstr &n: paths) {
        p += n;
        p += '\\';
        if (!_waccess(p.c_str(), 0))
            continue;
        if (mk(p) == ERROR_PATH_NOT_FOUND)
            return;
    }
}

DWORD csh::File::list(std::vector<wstr> &list, bool file, bool dir) {
    if (!isDir())
        return -1;
    // 列出文件
    WIN32_FIND_DATAW data;
    HANDLE           hFind = FindFirstFileW((path + L"\\*").c_str(), &data);
    if (hFind == INVALID_HANDLE_VALUE)
        return GetLastError();
    do {
        wstr name  = data.cFileName;
        bool isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (name != L"." && name != L"..") {
            if ((dir && isDir) || (file && !isDir)) {
                list.push_back(name);
            }
        }

    } while (FindNextFileW(hFind, &data));

    FindClose(hFind);
    return 0;
}

bool csh::File::mkdirs() const {
    if (exists())
        return false;
    std::vector<wstr> paths;
    wstr_split(path, paths, '\\');
    mks(paths);
    return exists();
}

