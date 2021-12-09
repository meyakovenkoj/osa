#include "logerr.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int pid, wfd, rfd, rb, wb, i = 0;
    char str[] = "111111111";
    char str2[] = "222222222";
    struct flock lock;

    if (argc != 2) {
        LOG_ERR("bad args");
        exit(EXIT_FAILURE);
    }

    switch (pid = fork()) {
    case -1:
        LOG_ERR("Fork failed");
        exit(EXIT_FAILURE);
    case 0:
        if ((rfd = open(argv[1], O_RDONLY | O_CREAT, 0777)) < 0) {
            LOG_ERR("File opening failed rfd\n");
            exit(EXIT_FAILURE);
        }
        char buf1[100];

        while ((rb = read(rfd, buf1, sizeof(buf1))) > 0) {
            // sleep(1);
            if (rb == -1) {
                LOG_ERR("Read error");
                kill(getppid(), SIGKILL);
                exit(EXIT_FAILURE);
            }
            write(1, buf1, rb);
        }

        close(rfd);
        exit(EXIT_SUCCESS);
    default:
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        if ((wfd = open(argv[1], O_WRONLY | O_CREAT, 0777)) < 0) {
            LOG_ERR("File opening failed wfd");
            exit(EXIT_FAILURE);
        }
        if (fcntl(wfd, F_SETLK, &lock) == -1) {
            LOG_ERR("FCNTL failed");
            exit(EXIT_FAILURE);
        }

        while (i++ < 100) {
            char *cur = i % 2 ? str : str2;
            if ((wb = write(wfd, cur, strlen(cur))) < 0) {
                LOG_ERR("Write error");
                kill(pid, SIGKILL);
                exit(EXIT_FAILURE);
            }
        }
        close(wfd);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
