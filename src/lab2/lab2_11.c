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
        exit(EXIT_FAILURE);
    }
    char **new_argv = argv + 1;
    f = argv[1];

    pid = fork();
    switch (pid) {
    case -1:
        LOG_ERR("fork failed");
        exit(EXIT_FAILURE);
    case 0:
        execvp(f, new_argv); // execvp (file, argv)
        exit(EXIT_SUCCESS);
    default:
        system(argv[1]);
        exit(EXIT_SUCCESS);
    }
}
