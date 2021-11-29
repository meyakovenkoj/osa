#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO "fifo_file"

int main() {
  int wfd; //, n;
  char buf[15] = "Hello, world\n";

  if ((wfd = open(FIFO, O_WRONLY)) < 0) {
    printf("FIFO opening failed\n");
    exit(1);
  }
  write(wfd, buf, sizeof(buf));

  close(wfd);
  unlink(FIFO);
  exit(0);
}
