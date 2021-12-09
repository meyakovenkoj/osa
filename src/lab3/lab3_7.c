#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    int status;
    switch ((pid = fork())) {
    case -1: {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    case 0: {
        printf("child process %d\n", getpid());
        pause();
        exit(EXIT_SUCCESS);
    }
    default: {
        sleep(1);
#ifdef TASK5
        kill(pid, SIGUSR1);
#endif
#ifdef TASK6
        kill(pid, SIGKILL);
#endif
        while (1) {
            pid = wait(&status);
            if (pid > 0) {
                printf("Child process %d finished with status %d , %.8x\n", pid, status, status);
                break;
            }
        }
        exit(EXIT_SUCCESS);
    }
    }
}
