#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    while (1) {
        DIR* dir = opendir(argv[1]);
        if (!dir) {
            sleep(1);
            continue;
        }

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);

            struct stat st;
            if (stat(path, &st) == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR)) {
                pid_t pid = fork();
                if (pid == 0) {
                    execl(path, path, NULL);
                    exit(0);
                } else if (pid > 0) {
                    wait(NULL);
                    remove(path);
                }
            }
        }
        closedir(dir);
    }
    return 0;
}
