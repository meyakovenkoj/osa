#include "logerr.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO "fifo_file"

int main() {
  int fifo, rfd, n;
  char buf[15] = "\0";

  fifo = mkfifo(FIFO, 0777);
  if (fifo < 0) {
    LOG_ERR("FIFO creation failed");
    if (errno != EEXIST) {
      LOG_ERR("unknown error");
      goto err;
    }
    struct stat info = {0};
    if (stat(FIFO, &info)) {
      LOG_ERR("stat failed");
      goto err;
    }
    if (!(info.st_mode & S_IFIFO)) {
      LOG_ERR("unknown error");
      goto err;
    }
    printf("using existing fifo\n");
  }
  //   if ((rfd = open(FIFO, O_RDONLY)) < 0) {
  if ((rfd = open(FIFO, O_RDONLY | O_NDELAY)) < 0) {
    LOG_ERR("FIFO opening failed");
    goto err;
  }
  while ((n = read(rfd, buf, sizeof(buf))) > 0)
    write(1, buf, n);

  close(rfd);
  return 0;

err:
  return 1;
}
