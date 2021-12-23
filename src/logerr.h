#ifndef LOGERR_H
#define LOGERR_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define STD_ERR(mes) log_to_fd(mes, __FILE__, __LINE__, errno, stderr)
#define LOG_ERR(mes) log_err(mes, __FILE__, __LINE__, errno)
#define FD_ERR(mes, fd) write_to_fd(mes, __FILE__, __LINE__, errno, fd)
#define FD_LOG(mes, fd) write_log_to_fd(mes, fd)

void log_to_fd(const char *message, const char *file, int line, int err,
               FILE *fd)
{
    fprintf(fd, "[%s:%d]: ", file, line);
    if (message) {
        fprintf(fd, "%s ", message);
    }
    fprintf(fd, "(errno=%d, %s)\n", err, strerror(err));
}

void write_to_fd(const char *message, const char *file, int line, int err,
                 int fd)
{
    char *buf = calloc(1024, sizeof(char));
    sprintf(buf, "[%s:%d]: %s (errno=%d, %s)\n", file, line, message, err, strerror(err));
    write(fd, buf, strlen(buf));
    free(buf);
}

void write_log_to_fd(const char *message, int fd)
{
    time_t now;
    struct tm *ptr;
    char tbuf[80];
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf, 80, "%Y-%B-%e %H:%M:%S", ptr);
    char *buf = calloc(1024, sizeof(char));
    sprintf(buf, "[%s]: %s\n", tbuf, message);
    write(fd, buf, strlen(buf));
    free(buf);
}

void log_err(const char *message, const char *file, int line, int err)
{
    log_to_fd(message, file, line, err, stdout);
}

#endif
