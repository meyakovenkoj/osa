#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void action(int sig)
{
    const char str[] = "\ndoing action on SIGQUIT\n";
    write(STDOUT_FILENO, str, sizeof(str));
    signal(SIGQUIT, SIG_DFL);
}

int main()
{
    signal(SIGQUIT, action);

    while (1) {
        printf("processing...\n");
        sleep(1);
    }
    return 0;
}
