#include <File.h>

#include <utility>
#include <sys/stat.h>

csh::File::File(wstr path) {
    this->path = std::move(path);
}

csh::File::File(const wstr &dir, const wstr &child) {
    if (!dir.ends_with(L"\\") && !child.ends_with(L"/")) {
        path = dir + L"\\" + child;
    } else {
        path = dir + child;
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

csh::FileStream csh::File::open(const wchar_t *mode) {
    return FileStream(this, mode);
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

