#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si = { sizeof(si) };
    SECURITY_ATTRIBUTES sa = { sizeof(sa), NULL, TRUE };

    HANDLE hInput = CreateFile("input.txt", GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, 0, NULL);
    HANDLE hOutput = CreateFile("output.txt", GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, 0, NULL);

    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hInput;
    si.hStdOutput = hOutput;
    si.hStdError = hOutput;

    PROCESS_INFORMATION pi;
    char cmd[] = "cmd.exe";

    if (CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    CloseHandle(hInput);
    CloseHandle(hOutput);
    return 0;
}
