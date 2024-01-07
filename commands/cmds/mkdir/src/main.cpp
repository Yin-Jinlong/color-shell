#include <Windows.h>
#include <iostream>
#include <vector>
#include <io.h>

void printHelp() {
    std::cerr << "Usage: mkdir [-p] <directory>..." << std::endl;
}

DWORD mk(const std::string &name) {
    CreateDirectoryA(name.c_str(), nullptr);
    return GetLastError();
}

void printIfError(DWORD err, const std::string &name) {
    if (err == ERROR_SUCCESS)
        return;
    if (err == ERROR_ALREADY_EXISTS)
        std::cerr << "Directory '" << name << "' already exists" << std::endl;
    else if (err == ERROR_PATH_NOT_FOUND)
        std::cerr << "Path '" << name << "' does not exist" << std::endl;
    else
        std::cerr << "Error creating directory : '" << name << "' code:" << err << std::endl;
}

void mkdir(const std::string &name) {
    printIfError(mk(name), name);
}

void mkdirs(const std::vector<std::string> &paths) {
    std::string    p;
    for (const std::string &n: paths) {
        p += n;
        p += '\\';
        if (!_access(p.c_str(), 0))
            continue;
        DWORD err = mk(p);
        if (err) {
            printIfError(err, p);
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printHelp();
        return EXIT_FAILURE;
    }
    int         pi = 1;
    std::string s  = argv[1];
    bool        r  = s == "-p";
    if (r) {
        pi++;
        if (pi == argc) {
            printHelp();
            return EXIT_FAILURE;
        }
    }
    std::vector<std::string> paths;
    while (pi < argc) {
        s = argv[pi];
        if (r) {
            paths.clear();
            for (int i = 0; i < s.size(); i++) {
                if (s[i] == '\\' || s[i] == '/') {
                    paths.push_back(s.substr(0, i));
                    s = s.substr(i + 1);
                    i = -1;
                }
            }
            paths.push_back(s);
            mkdirs(paths);
        } else {
            mkdir(s);
        }
        pi++;
    }
    return EXIT_SUCCESS;
}
