#include <Windows.h>
#include <iostream>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::wcout.imbue(std::locale(".UTF-8"));
    wchar_t buffer[MAX_PATH];
    if (!GetCurrentDirectoryW(MAX_PATH, buffer)) {
        std::wcerr << L"Error getting current directory\n";
        return EXIT_FAILURE;
    }
    std::wcout << buffer << std::endl;
    return EXIT_SUCCESS;
}
