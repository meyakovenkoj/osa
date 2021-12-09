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
    int status, child_pid;

    printf("Process\tPID\tPPID\tGroup\n");
    pid = fork();

    switch (pid) {
    case -1:
        LOG_ERR("Fork failed");
        exit(errno);
    case 0:
        printf("Child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
        exit(EXIT_SUCCESS);
    default:
        printf("Parent\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
        child_pid = wait(&status);
        printf("Child process with pid=%d finished\n", child_pid);
        if (WIFEXITED(status)) //не равно нулю, если дочерний процесс успешно завершился.
            printf("Child exited with code %d\n", WEXITSTATUS(status));
        else
            printf("Child was terminated\n");
        exit(EXIT_SUCCESS);
    }
}
