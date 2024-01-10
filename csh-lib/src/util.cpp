#include <util.h>
#include <minwindef.h>
#include <processenv.h>

str getCurrentDirectory() {
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    return buffer;
}


str getProcessOutput(const str &cmdLine) {
    // TODO ReadFile卡死
    SECURITY_ATTRIBUTES sa    = {0};
    HANDLE              hRead = nullptr, hWrite = nullptr;
    sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle       = TRUE;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        throw std::runtime_error("CreatePipe failed:" + std::to_string(GetLastError()));
    }

    STARTUPINFOA        si = {0};
    PROCESS_INFORMATION pi = {nullptr};
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfoA(&si);
    si.hStdError   = hWrite;
    si.hStdOutput  = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    if (!CreateProcessA(nullptr,
                        (char *) cmdLine.data(),
                        nullptr, nullptr,
                        TRUE, NULL,
                        nullptr, nullptr,
                        &si, &pi)) {
        CloseHandle(hWrite);
        CloseHandle(hRead);
        throw std::runtime_error("CreateProcess failed:" + std::to_string(GetLastError()));
    }

    CloseHandle(hWrite);
    std::string out;
    char        buf[256] = {0};
    DWORD       rc       = 0;
    while (ReadFile(hRead, buf, 256, &rc, nullptr)) {
        buf[rc] = '\0';
        out += buf;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hRead);

    return out;
}

str getEnv(const str &name) {
    char buffer[4096];
    GetEnvironmentVariableA(name.c_str(), buffer, 4096);
    return buffer;
}
