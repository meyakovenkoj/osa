#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logerr.h"

int main(int argc, char *argv[])
{
    pid_t pid;
    char *f;
    if (argc < 2) {
        exit(1);
    }
    char **new_argv = argv + 1;
    f = argv[1];

    pid = fork();
    switch (pid) {
    case -1:
        LOG_ERR("fork failed");
        exit(1);
    case 0:
        execvp(f, new_argv); // execvp (file, argv)
        exit(0);
    default:
        system(argv[1]);
        exit(0);
    }
}
