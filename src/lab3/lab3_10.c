#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void action(int sig)
{
    printf("SIGCHLD signal %d\n", sig);
    sleep(5);
    printf("SIGCHLD done\n");
}

void actionOnInt(int sig)
{
    printf("SIGINT %d\n", sig);
    sleep(5);
    printf("SIGINT done\n");
}

int main(void)
{
    static struct sigaction act, oldact, actINT;
    pid_t pid;

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    actINT.sa_handler = actionOnInt;
    sigemptyset(&actINT.sa_mask);
    actINT.sa_flags = SA_RESTART;
    sigaction(SIGINT, &actINT, NULL);

    act.sa_handler = action;
    act.sa_mask = mask;
    act.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &act, &oldact);

    switch (pid = fork()) {
    case -1: {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    case 0: {
        printf("child process %d\n", getpid());
        sleep(1);
        exit(EXIT_SUCCESS);
    }
    default: {
        printf("PARENT process %d\nignore Ctrl+C\n", getpid());
        sleep(5);
        int status;
        wait(&status);
        printf("Status: %x\n", status);
        sigaction(SIGCHLD, &oldact, NULL);
        exit(EXIT_SUCCESS);
    }
    }
}
