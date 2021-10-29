#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void action(int sig)
{
    printf("signal %d\n", sig);
    sleep(1);
}

int main()
{
    pid_t pid;
    int status;
    switch ((pid = fork())) {
    case -1: {
        perror("Fork failed");
        exit(1);
    }
    case 0: {
        signal(SIGUSR1, action);
        printf("child process %d\n", getpid());
        for (int i = 0; i < 10000; i++) {
            printf("outer loop: i =  %d\n", i);
            for (int j = 0; j < 10000; j++) {
                for (int n = 0; n < 100000; n++) {
                }
            }
        }
        exit(0);
    }
    default: {
        sleep(1);
        printf("PARENT process %d\n", getpid());
        kill(pid, SIGUSR1);
        while (1) {
            pid = wait(&status);
            if (pid > 0) {
                printf("Child process %d finished with status %d , %.8x\n", pid, status, status);
                break;
            }
        }
        exit(0);
    }
    }
}
