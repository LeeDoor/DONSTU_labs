#include <windows.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    char path[MAX_PATH];
    strcpy(path, argv[1]);
    strcat(path, "\\*.*");

    while (1) {
        WIN32_FIND_DATA data;
        HANDLE hFind = FindFirstFile(path, &data);
        if (hFind == INVALID_HANDLE_VALUE) {
            Sleep(1000);
            continue;
        }

        do {
            char* ext = strrchr(data.cFileName, '.');
            if (ext && (!strcmp(ext, ".exe") || !strcmp(ext, ".bat") || !strcmp(ext, ".cmd"))) {
                char fullPath[MAX_PATH];
                sprintf(fullPath, "%s\\%s", argv[1], data.cFileName);

                STARTUPINFO si = { sizeof(si) };
                PROCESS_INFORMATION pi;
                char cmdLine[MAX_PATH] = "cmd.exe /c ";
                strcat(cmdLine, fullPath);

                if (CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                    WaitForSingleObject(pi.hProcess, INFINITE);
                    CloseHandle(pi.hThread);
                    CloseHandle(pi.hProcess);
                    DeleteFile(fullPath);
                }
            }
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
    }
    return 0;
}
