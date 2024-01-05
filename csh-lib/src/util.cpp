#include <util.h>
#include <minwindef.h>
#include <processenv.h>

std::wstring getCurrentDirectory() {
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);
    return buffer;
}


std::string getProcessOutput(const std::wstring &cmdLine) {
    // TODO ReadFile卡死
    SECURITY_ATTRIBUTES sa = {0};
    HANDLE hRead = nullptr, hWrite = nullptr;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        throw std::runtime_error("CreatePipe failed:" + std::to_string(GetLastError()));
    }

    STARTUPINFOW si = {0};
    PROCESS_INFORMATION pi = {nullptr};
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfoW(&si);
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    if (!CreateProcessW(nullptr,
                        (wchar_t *) cmdLine.c_str(),
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
    char buf[256] = {0};
    DWORD rc = 0;
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

std::wstring getEnv(const std::wstring &name) {
    wchar_t buffer[4096];
    GetEnvironmentVariableW(name.c_str(), buffer, 4096);
    return buffer;
}