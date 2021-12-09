#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int fp[2]; //, b = 0;
    //     char buf[512] = "\0";
    if (pipe(fp) != 0) {
        LOG_ERR("Failed to open pipe 1");
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
    case -1:
        perror("Fork failed");
        exit(EXIT_FAILURE);
    case 0:
        close(fp[0]);
        printf("Child process %d\n", getpid());
        dup2(fp[1], 1);
        execlp("who", "who", NULL);
        exit(EXIT_FAILURE);
    default:
        close(fp[1]);
        printf("Parent process %d\n", getpid());
        // dup2(1, fp[1]);

        dup2(fp[0], 0);
        execlp("wc", "wc", "-l", NULL);
        exit(EXIT_FAILURE);
    }
}
