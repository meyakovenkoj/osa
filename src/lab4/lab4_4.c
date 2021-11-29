#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fp1[2], fp2[2], b = 0;
  char buf[512] = "\0";

  if (pipe(fp1) != 0) {
    LOG_ERR("Failed to open pipe 1");
    exit(1);
  }

  if (pipe(fp2) != 0) {
    LOG_ERR("Failed to open pipe 2");
    exit(1);
  }

  switch (fork()) {
  case -1:
    perror("Fork failed");
    exit(1);
  case 0:
    close(fp1[1]);
    close(fp2[0]);
    printf("Child process %d\n", getpid());
    dup2(fp2[1], 1);
    execl("out/lab4_4e", "out/lab4_4e", NULL);
    exit(EXIT_FAILURE);
  default:
    close(fp1[0]);
    close(fp2[1]);
    printf("Parent process %d\n", getpid());
    dup2(1, fp2[1]);
    do {
      b = read(fp2[0], buf, 512);
      if (b == -1)
        sleep(1);
      if (b > 0) {
        printf("Parent read: %s\n", buf);
      }
      memset(&buf, 0, 512);
    } while (b != 0);
    wait(NULL);
    close(fp1[1]);
    close(fp2[0]);
    exit(0);
  }
}
