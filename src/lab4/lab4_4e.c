#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
  int b = 0;
  char buf[512];

  while ((b = read(0, buf, sizeof(buf))) > 0) {
    b = write(1, buf, b);
    memset(&buf, 0, 512);
  }

  return 0;
}
