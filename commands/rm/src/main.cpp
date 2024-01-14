#include <predef.h>
#include <Windows.h>
#include <io.h>

void printHelp() {
    std::cerr << "Usage: rm [-r] <file>..." << std::endl;
}

void print(const char *msg, str &name) {
    std::cerr << msg << '\'' << name << '\'' << std::endl;
}

void printIfError(DWORD err, str &name) {
    if (!err)
        return;
    switch (err) {
        case ERROR_DIR_NOT_EMPTY:
            print("Directory not empty: ", name);
            break;
        case ERROR_FILE_NOT_FOUND:
            print("No such file or directory: ", name);
            break;
        case ERROR_ACCESS_DENIED:
            print("Access denied: ", name);
            break;
        default:
            std::cerr << "error code:" << err << ": '" << name << '\'' << std::endl;
            break;
    }
}

bool exists(str &file) {
    return !_access(file.c_str(), 0);
}

bool isFile(str &file) {
    struct _stat64i32 stat{};
    _stat64i32(file.c_str(), &stat);
    return stat.st_mode & S_IFREG;
}

bool isDir(str &file) {
    struct _stat64i32 stat{};
    _stat64i32(file.c_str(), &stat);
    return stat.st_mode & S_IFDIR;
}

DWORD delFile(str &file) {
    return DeleteFileA(file.c_str()) ? 0 : GetLastError();
}

DWORD delEmptyDir(str &dir) {
    return RemoveDirectoryA(dir.c_str()) ? 0 : GetLastError();
}

DWORD delDir(str &dir) {
    WIN32_FIND_DATAA data;
    HANDLE hFind = FindFirstFileA((dir + "\\*").c_str(), &data);
    if (hFind == INVALID_HANDLE_VALUE) {
        return GetLastError();
    }
    bool err = false;
    while (FindNextFileA(hFind, &data)) {
        str name = data.cFileName;
        if (name == "." || name == "..")
            continue;
        str f = dir + "/";
        f += name;
        if (isFile(f)) {
            DWORD e = delFile(f);
            err |= e != 0;
            printIfError(e, f);
            continue;
        }
        if (isDir(f)) {
            err |= delDir(f) != 0;
        }
    }
    FindClose(hFind);
    return err ? 1 : delEmptyDir(dir);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp();
        return EXIT_FAILURE;
    }
    int ai = 1;
    bool r = false;
    str arg = argv[1];
    if (arg[0] == '-') {
        ai++;
        int i = 1;
        while (i < arg.size()) {
            char c = arg[i++];
            if (c == 'r') {
                r = true;
            } else {
                print("Unknown option: ", arg);
                return EXIT_FAILURE;
            }
        }
    }
    while (ai < argc) {
        str f = argv[ai];
        if (exists(f)) {
            if (isDir(f)) {
                printIfError(r ? delDir(f) : delEmptyDir(f), f);
            } else if (isFile(f)) {
                printIfError(delFile(f), f);
            } else {
                print("Not a file or directory: ", f);
            }
        } else {
            printIfError(ERROR_FILE_NOT_FOUND, f);
        }
        ai++;
    }
    return 0;
}
