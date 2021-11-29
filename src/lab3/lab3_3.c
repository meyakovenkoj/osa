#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void action(int sig)
{
    int c, p;
    while ((p = wait(&c)) != -1) {
        if (WIFEXITED(c)) // returns true if the child terminated normally
        {
            int exit_status = WEXITSTATUS(c);
            printf("Exit status of the child %d was %d\n", p, exit_status);
        }
    }
}

int main()
{
    struct sigaction act;
    act.sa_handler = SIG_IGN; //action;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NODEFER; // no blocks
    sigaction(SIGCHLD, &act, 0);
    int i;
    for (i = 0; i < 10; i++) {
        if (fork() == 0) {
            printf("child %d\n", getpid());
            sleep(5);
            exit(1);
        }
    }

    while (1)
        pause();
}
