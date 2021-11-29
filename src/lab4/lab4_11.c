#include "logerr.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

// static int rf, wf;

int main(int argc, char *argv[]) {
  int rb = 0;
  //   int rf = -1;
  //   int wf = -1;
  char str1[15] = "	CHILD:		";
  char str2[15] = "	PARENT:		";

  switch (fork()) {
  case -1:
    perror("Fork failed");
    exit(1);
  case 0: {

    struct flock wlock1, rlock1; //, checklk;
    //     rf = 0;
    //     wf = 0;
    //     wlock1.l_type = F_WRLCK;
    //     wlock1.l_whence = SEEK_CUR;
    //     wlock1.l_start = 0;
    //     wlock1.l_len = 0;

    //     rlock1.l_type = F_RDLCK;
    //     rlock1.l_whence = SEEK_CUR;
    //     rlock1.l_start = 0;
    //     rlock1.l_len = 0;

    //     int crf = -1, cwf = -1;
    char bufc[1024] = "\0";

    while (1) {
      rlock1.l_type = F_RDLCK;
      rlock1.l_whence = SEEK_CUR;
      rlock1.l_start = 0;
      rlock1.l_len = 0;

      wlock1.l_type = F_WRLCK;
      wlock1.l_whence = SEEK_CUR;
      wlock1.l_start = 0;
      wlock1.l_len = 0;
      //       while (1) {
      //         fcntl(0, F_GETLK, &checklk);
      //         if (checklk.l_type == F_UNLCK) {
      //           break;
      //         }
      //       }

      while (fcntl(0, F_SETLK, &rlock1) == -1) {
        // LOG_ERR("fcntl fail stdin set");
      }
      while (fcntl(1, F_SETLK, &wlock1) == -1) {
        // LOG_ERR("fcntl fail stdout set");
      }

      rb = read(0, bufc, sizeof(bufc));
      write(1, str1, sizeof(str1));
      write(1, bufc, rb);
      //       if (bufc[0] == '\\') {
      rlock1.l_type = F_UNLCK;
      wlock1.l_type = F_UNLCK;
      if (fcntl(1, F_SETLK, &wlock1) == -1) {
        // LOG_ERR("fcntl fail stdout unset");
      }
      if (fcntl(0, F_SETLK, &rlock1) == -1) {
        // LOG_ERR("fcntl fail stdin unset");
      }
      //       sleep(1);
      //         continue;
      //       }
    }
    exit(0);
  }
  default: {

    struct flock wlock2, rlock2; //, checklk;
    //     wlock2.l_type = F_WRLCK;
    //     wlock2.l_whence = SEEK_CUR;
    wlock2.l_start = 0;
    wlock2.l_len = 0;

    //     rlock2.l_type = F_RDLCK;
    //     rlock2.l_whence = SEEK_CUR;
    rlock2.l_start = 0;
    rlock2.l_len = 0;
    //     int prf = -1, pwf = -1;
    char bufp[1024] = "\0";
    while (1) {
      rlock2.l_type = F_RDLCK;
      rlock2.l_whence = SEEK_CUR;
      rlock2.l_start = 0;
      rlock2.l_len = 0;

      wlock2.l_type = F_WRLCK;
      wlock2.l_whence = SEEK_CUR;
      wlock2.l_start = 0;
      wlock2.l_len = 0;
      //       while (1) {
      //         fcntl(0, F_GETLK, &checklk);
      //         if (checklk.l_type == F_UNLCK) {
      //           break;
      //         }
      //       }

      while (fcntl(0, F_SETLK, &rlock2) == -1) {
        // LOG_ERR("fcntl fail stdin set");
      }
      while (fcntl(1, F_SETLK, &wlock2) == -1) {
        // LOG_ERR("fcntl fail stdout set");
      }

      rb = read(0, bufp, sizeof(bufp));
      write(1, str2, sizeof(str2));
      write(1, bufp, rb);
      //       if (bufc[0] == '\\') {
      rlock2.l_type = F_UNLCK;
      wlock2.l_type = F_UNLCK;
      if (fcntl(1, F_SETLK, &wlock2) == -1) {
        // LOG_ERR("fcntl fail stdout unset");
      }
      if (fcntl(0, F_SETLK, &rlock2) == -1) {
        // LOG_ERR("fcntl fail stdin unset");
      }
      //       sleep(1);
      //         continue;
      //       }
    }

    wait(NULL);
    exit(0);
  }
  }
}
