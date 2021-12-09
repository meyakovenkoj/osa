#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ITER_NO 10

void zero_act(int signo)
{
    const char s1[] = "00\n";
    write(STDOUT_FILENO, s1, sizeof(s1));
}

void one_act(int signo)
{
    const char s2[] = "11\n";
    write(STDOUT_FILENO, s2, sizeof(s2));
}

int main(void)
{
    pid_t pid, p_pid;
    static struct sigaction zero_action, one_action;
    sigset_t mask;
    int c = 0;

    zero_action.sa_handler = zero_act;
    zero_action.sa_flags = 0;
    sigemptyset(&zero_action.sa_mask);
    sigaction(SIGUSR1, &zero_action, NULL);

    one_action.sa_handler = one_act;
    one_action.sa_flags = 0;
    sigemptyset(&one_action.sa_mask);
    sigaction(SIGUSR2, &one_action, NULL);

    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    switch ((pid = fork())) {
    case -1: {
        exit(EXIT_FAILURE);
        perror("Fork failed");
    }
    case 0: {
        p_pid = getppid();

        sigdelset(&mask, SIGUSR2); //release lock on SIGUSR2

        while (c != ITER_NO) {
            c++;
            kill(p_pid, SIGUSR1);
            sigsuspend(&mask);
        }

        exit(EXIT_SUCCESS);
    }
    default: {
        sigdelset(&mask, SIGUSR1); //release lock on SIGUSR1

        while (c != ITER_NO) {
            c++;
            sigsuspend(&mask);
            kill(pid, SIGUSR2);
        }

        wait(NULL);
        exit(EXIT_SUCCESS);
    }
    }
}
