#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

void copyFile(char* src, char* dst) {
    int sfd = open(src, O_RDONLY);
    int dfd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    char buf[4096];
    int n;
    while ((n = read(sfd, buf, sizeof(buf))) > 0) {
        write(dfd, buf, n);
    }

    close(sfd);
    close(dfd);
}

int main(int argc, char* argv[]) {
    if (argc < 3) return 1;

    DIR* dir = opendir(argv[1]);
    if (!dir) return 1;

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_REG) {
            char src[1024], dst[1024];
            snprintf(src, sizeof(src), "%s/%s", argv[1], entry->d_name);
            snprintf(dst, sizeof(dst), "%s/%s", argv[2], entry->d_name);

            pid_t pid = fork();
            if (pid == 0) {
                copyFile(src, dst);
                exit(0);
            }
        }
    }

    while (wait(NULL) > 0);
    closedir(dir);
    return 0;
}
