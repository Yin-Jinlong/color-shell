#include <str/string-util.h>
#include <File.h>
#include <utility>
#include <sys/stat.h>
#include <fstream>
#include <corecrt_io.h>
#include <Windows.h>

csh::File::File(str path) {
    this->path = std::move(path);
}

csh::File::File(const str &dir, const str &child) {
    if (!dir.ends_with("\\") && !child.ends_with("/")) {
        path = dir + "\\" + child;
    } else {
        path = dir + child;
    }
    for (char &c: path) {
        if (c == L'/')
            c = L'\\';
    }
}

csh::File::~File() = default;

int csh::File::update() {
    return _stat64i32(path.c_str(), &stat);
}

#define CHECK(f) _access_s(path.c_str(), f)==0

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
    return remove(path.c_str()) == 0;
}

str csh::File::absolutePath() const {
    char buf[MAX_PATH];
    GetFullPathNameA(path.c_str(), MAX_PATH, buf, nullptr);
    return buf;
}

std::fstream csh::File::open(std::ios_base::openmode mode) {
    if (!isFile())
        throw std::runtime_error("not a file");
    return std::fstream(path, mode);
}

str csh::File::readAllTexts() {
    if (!isFile())
        throw std::runtime_error("not a file");
    FILE *fs;
    errno_t err = fopen_s(&fs, path.c_str(), "r");
    if (err)
        throw std::runtime_error("open file failed : " + std::to_string(err));
    str texts;
    char buf[4096];
    while (fgets(buf, 4096, fs)) {
        texts += buf;
    }
    fclose(fs);
    return texts;
}

str csh::File::getPath() const {
    return path;
}

str csh::File::getFileName() const {
    str fullPath = absolutePath();
    return fullPath.substr(fullPath.find_last_of("/\\") + 1);
}

csh::File csh::File::getParent() const {
    str p = absolutePath();
    unsigned int i = p.find_last_of('\\');
    if (p.find_first_of('\\') == i)
        return csh::File(p);
    return File(p.substr(0, i));
}

bool csh::File::operator==(const csh::File &other) const {
    return absolutePath() == other.absolutePath();
}

DWORD mk(const str &name) {
    CreateDirectoryA(name.c_str(), nullptr);
    return GetLastError();
}

void mks(const std::vector<str> &paths) {
    str p;
    for (const str &n: paths) {
        p += n;
        p += '\\';
        if (!_access(p.c_str(), 0))
            continue;
        if (mk(p) == ERROR_PATH_NOT_FOUND)
            return;
    }
}

DWORD csh::File::list(std::vector<str> &list, bool file, bool dir) {
    if (!isDir())
        return -1;
    // 列出文件
    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &data);
    if (hFind == INVALID_HANDLE_VALUE)
        return GetLastError();
    do {
        str name = data.cFileName;
        bool isDir = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (name != "." && name != "..") {
            if ((dir && isDir) || (file && !isDir)) {
                list.push_back(name);
            }
        }

    } while (FindNextFileA(hFind, &data));

    FindClose(hFind);
    return 0;
}

bool csh::File::mkdirs() const {
    if (exists())
        return false;
    std::vector<str> paths;
    strSplit(path, paths, '\\');
    mks(paths);
    return exists();
}

bool csh::File::createFile() {
    auto f = CreateFileA(path.c_str(),
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         nullptr,
                         CREATE_NEW,
                         FILE_ATTRIBUTE_NORMAL,
                         nullptr
    );
    if (f == INVALID_HANDLE_VALUE)
        return false;
    CloseHandle(f);
    return true;
}

