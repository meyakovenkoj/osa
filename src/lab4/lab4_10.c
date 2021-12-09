#include "logerr.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int rb = 0;
    char buf[15];
    char str1[15] = "CHILD\n";
    char str2[15] = "PARENT\n";

    switch (fork()) {
    case -1:
        LOG_ERR("Fork failed");
        exit(EXIT_FAILURE);
    case 0:
        while (1) {
            rb = read(0, buf, sizeof(buf)); // sleep(1);
            write(1, str1, sizeof(str1));
            write(1, buf, rb);
        }
        exit(EXIT_SUCCESS);
    default:
        while (1) {
            rb = read(0, buf, sizeof(buf));
            write(1, str2, sizeof(str2));
            write(1, buf, rb);
        }
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
