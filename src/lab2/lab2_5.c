#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logerr.h"

int main()
{
    pid_t pid;

    printf("Process\tPID\tPPID\tGroup\n");
    pid = fork();

    switch (pid) {
    case -1:
        LOG_ERR("Fork failed");
        exit(errno);
    case 0:
        printf("Child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
        sleep(1);
        printf("Child process finished\n");
        exit(0);
    default:
        printf("Parent\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
        sleep(5);
        printf("Parent process finished\n");
        exit(0);
    }
}
