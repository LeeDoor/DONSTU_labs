#include <windows.h>
#include <stdio.h>

int main() {
    FILE* file = fopen("config.txt", "r");
    if (!file) return 1;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char process[256];
        int timeout;
        sscanf(line, "%s %d", process, &timeout);

        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (CreateProcess(NULL, process, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hThread);
            DWORD result = WaitForSingleObject(pi.hProcess, timeout * 1000);
            if (result == WAIT_TIMEOUT) {
                printf("Process %s exceeded time limit\n", process);
                TerminateProcess(pi.hProcess, 0);
            } else {
                printf("Process %s completed within time\n", process);
            }
            CloseHandle(pi.hProcess);
        }
    }
    fclose(file);
    return 0;
}
