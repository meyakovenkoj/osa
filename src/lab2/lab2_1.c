#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logerr.h"

extern char **environ;

int main()
{
    char *key = "5d41402abc4b2a76b9719d911017c592";
    printf("Setting key...\n");
    if (setenv("API_SECRET_KEY", key, 0) != 0) {
        LOG_ERR("setenv failed");
        exit(errno);
    }

    char *env_key = getenv("API_SECRET_KEY");
    if (env_key) {
        printf("your API_SECRET_KEY=%s is in env\n", env_key);
    } else {
        LOG_ERR("no API_SECRET_KEY set");
    }

    printf("Unsetting key...\n");
    if (unsetenv("API_SECRET_KEY") != 0) {
        LOG_ERR("unsetenv failed");
        exit(errno);
    }

    printf("\nAll environment vars:\n");
    for (char **iter = environ; *iter; iter++) {
        printf("%s\n", *iter);
    }
    return 0;
}
