#ifndef LOGERR_H
#define LOGERR_H
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define STD_ERR(mes) log_to_fd(mes, __FILE__, __LINE__, errno, stderr)
#define LOG_ERR(mes) log_err(mes, __FILE__, __LINE__, errno)

void log_to_fd(const char *message, const char *file, int line, int err,
               FILE *fd) {
  fprintf(fd, "[%s:%d]: ", file, line);
  if (message) {
    fprintf(fd, "%s ", message);
  }
  fprintf(fd, "(errno=%d, %s)\n", err, strerror(err));
}

void log_err(const char *message, const char *file, int line, int err) {
  log_to_fd(message, file, line, err, stdout);
}

#endif
