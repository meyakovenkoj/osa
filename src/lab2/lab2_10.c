#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logerr.h"

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid;
    int i = 0;
    char *env[] = {"LANG=C", NULL};

    if (argc != 2) {
        printf("Usage ./out/lab2_10 <path/to/program>\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch (pid) {
    case -1:
        LOG_ERR("fork failed");
        exit(EXIT_FAILURE);
    case 0:
        execle(argv[1], "l1", "l2", "l3", NULL, env);
        exit(EXIT_SUCCESS);
    default:
        printf("Number of args: %d\n", argc);
        printf("Arguments:\n");
        while (argv[i]) {
            printf("%s\n", argv[i]);
            i++;
        }
        i = 0;
        printf("Environment:\n");
        while (envp[i]) {
            printf("%s\n", envp[i]);
            i++;
        }
        break;
    }

    return 0;
}
