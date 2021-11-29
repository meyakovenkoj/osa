#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  int fp1[2], fp2[2], b = 0;
  char buf[512];
  const char str1[] = "Some test text for lab";
  const char str2[] = "hello, world!";

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
    LOG_ERR("Fork failed");
    exit(1);
  case 0:
    close(fp1[1]); // close write end of pipe 1
    close(fp2[0]); // close read end of pipe 2
    printf("Child process %d\n", getpid());

    b = write(fp2[1], str2, sizeof(str2));
    printf("Child wrote %d bytes: %s\n", b, str2);

    b = read(fp1[0], buf, sizeof(buf));
    printf("Child read %d bytes: %s\n", b, buf);
    close(fp1[0]); // close write end of pipe 1
    close(fp2[1]); // close read end of pipe 2
    exit(0);
  default:
    close(fp1[0]); // close read end of pipe 1
    close(fp2[1]); // close write end of pipe 2
    printf("Parent process %d\n", getpid());

    b = write(fp1[1], str1, sizeof(str1));
    printf("Parent wrote %d bytes: %s\n", b, str1);

    b = read(fp2[0], buf, sizeof(buf));
    printf("Parent read %d bytes: %s\n", b, buf);
    close(fp1[1]); // close write end of pipe 1
    close(fp2[0]); // close read end of pipe 2
    exit(0);
  }
}
