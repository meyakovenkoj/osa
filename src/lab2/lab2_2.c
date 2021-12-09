#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logerr.h"

/*
pid_t fork(void);
On success, the PID of the child process is returned in the
parent, and 0 is returned in the child.  On failure, -1 is
returned in the parent, no child process is created, and errno is
set to indicate the error.
*/
int main()
{
    pid_t pid, cur_pid;

    pid = fork();
    printf("fork returns pid %d\n", pid);

    switch (pid) {
    case -1:
        LOG_ERR("Fork failed");
        exit(errno);
    case 0:
        cur_pid = getpid();
        printf("Child with pid %d\n", cur_pid);
        exit(EXIT_SUCCESS);
    default:
        cur_pid = getpid();
        printf("Parent with pid %d\n", cur_pid);
        exit(EXIT_SUCCESS);
    }
}
