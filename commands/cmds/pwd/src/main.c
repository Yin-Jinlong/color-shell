#include <stdio.h>
#include <direct.h>
#include <Windows.h>

int main() {
    wchar_t buffer[MAX_PATH];
    if (!_wgetcwd(buffer, MAX_PATH)) {
        fprintf(stderr, "Error getting current directory\n");
        return EXIT_FAILURE;
    }
    wprintf(L"%ls\n", buffer);
    return EXIT_SUCCESS;
}
