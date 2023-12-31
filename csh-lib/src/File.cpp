#include <File.h>
#include <direct.h>

#include <utility>

csh::File::File(std::wstring path) {
    this->path = std::move(path);
}

csh::File::~File() {

}

int csh::File::update() {
    return _wstat32i64(path.c_str(), &stat);
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

std::wstring csh::File::absolutePath() const {
    wchar_t buf[MAX_PATH];
    GetFullPathNameW(path.c_str(), MAX_PATH, buf, nullptr);
    return buf;
}

csh::FileStream csh::File::open(const wchar_t *mode) {
    return FileStream(this, mode);
}

std::wstring csh::File::getPath() const {
    return path;
}

std::wstring csh::File::getFileName() const {
    std::wstring fullPath = absolutePath();
    return fullPath.substr(fullPath.find_last_of(L"/\\") + 1);
}

csh::File csh::File::getParent() const {
    auto p = absolutePath();
    unsigned int i = p.find_last_of('\\');
    if (p.find_first_of('\\') == i)
        return csh::File(p);
    return File(p.substr(0, i));
}

bool csh::File::operator==(const csh::File &other) const {
    return absolutePath() == other.absolutePath();
}

