#include "logerr.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int pid, wfd, rfd, rb, wb, i = 0;
  char str[] = "123456789";
  //   struct flock lock;

  if (argc != 2) {
    LOG_ERR("bad args");
    exit(1);
  }

  switch (pid = fork()) {
  case -1:
    LOG_ERR("Fork failed");
    exit(1);
  case 0:
    if ((rfd = open(argv[1], O_RDONLY | O_CREAT, 0777)) < 0) {
      LOG_ERR("File opening failed rfd\n");
      exit(1);
    }
    char buf1[100];

    while ((rb = read(rfd, buf1, sizeof(buf1))) > 0) {
      // sleep(1);
      if (rb == -1) {
        LOG_ERR("Read error");
        kill(getppid(), SIGKILL);
        exit(1);
      }
      write(1, buf1, rb);
    }

    close(rfd);
    exit(0);
  default:
    if ((wfd = open(argv[1], O_WRONLY | O_CREAT, 0777)) < 0) {
      LOG_ERR("File opening failed wfd");
      exit(1);
    }

    while (i++ < 100) {
      if ((wb = write(wfd, str, sizeof(str))) < 0) {
        LOG_ERR("Write error");
        kill(pid, SIGKILL);
        exit(1);
      }
    }
    close(wfd);
    wait(NULL);
    exit(0);
  }
}
