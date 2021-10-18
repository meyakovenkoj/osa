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
    int spgrp;

    printf("Process\tPID\tPPID\tGroup\tsetpgrp\n");
    pid = fork();

    switch (pid) {
    case -1:
        LOG_ERR("Fork failed");
        exit(errno);
    case 0:
        printf("Child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
        //sleep(3);
        spgrp = setpgrp();
        printf("Child\t%d\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()), spgrp);
        pause();
        printf("Child process finished\n");
        exit(0);
    default:
        printf("Parent\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
        pause(); //comment for finish parent
        printf("Parent process finished\n");
        exit(0);
    }
}
