#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fp1[2], b = 0;
  char buf[512] = {0};

  if (pipe(fp1) != 0) {
    LOG_ERR("Failed to open pipe 1");
    exit(1);
  }

  switch (fork()) {
  case -1:
    LOG_ERR("Fork failed");
    exit(1);
  case 0:
    close(fp1[0]); // close read end of pipe 1
    printf("\nChild process %d\n", getpid());

    while ((b = read(0, buf, sizeof(buf))) > 0) {
      printf("Child read %d bytes \n", b);
      b = write(fp1[1], buf, b);

      memset(&buf, 0, 512);
    }
    close(fp1[1]); // close write end of pipe 1
    exit(0);
  default:
    close(fp1[1]); // close write end of pipe 1
    printf("Parent process %d\n", getpid());
    do {
      b = read(fp1[0], buf, 512);
      if (b == -1)
        sleep(1);
      if (b > 0) {
        printf("Parent read: %s\n", buf);
      }
      memset(&buf, 0, 512);
    } while (b != 0);
    close(fp1[0]); // close read end of pipe 1
    exit(0);
  }
}
