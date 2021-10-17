#ifndef LOGERR_H
#define LOGERR_H
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#define LOG_ERR(mes) log_err(mes, __FILE__, __LINE__, errno)

extern const char *const sys_errlist[];

void log_err(const char *message, const char *file, int line, int err)
{
    printf("[%s:%d]: ", file, line);
    if (message) {
        printf("%s ", message);
    }
    printf("(errno=%d, %s)\n", err, sys_errlist[err]);
}

#endif
