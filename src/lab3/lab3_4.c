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
    switch (fork()) {
    case -1: {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    case 0: {
        printf("child process %d\n", getpid());
        for (int i = 0; i < 100; i++) {
            printf("outer loop: i =  %d\n", i);
            for (int j = 0; j < 1000; j++) {
                for (int n = 0; n < 10000; n++) {
                }
            }
        }
        exit(9);
    }
    default: {
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
