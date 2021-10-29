#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void action(int sig)
{
    const char str[] = "\ndoing action on SIGQUIT\n";
    write(STDOUT_FILENO, str, sizeof(str));
}

int main()
{
    struct sigaction act;
    act.sa_handler = action;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESETHAND; // set default after execute

    sigaction(SIGQUIT, &act, 0);

    while (1) {
        printf("processing...\n");
        sleep(1);
    }
    return 0;
}
