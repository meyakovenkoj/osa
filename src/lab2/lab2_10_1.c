#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
    int i = 0;
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
    return 0;
}
