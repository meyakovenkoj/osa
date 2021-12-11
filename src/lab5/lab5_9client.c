#include "logerr.h"
#include "srvfunc.h"
#include "str2int.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int requestid, answerid;
    int err = 0;
    err = str2int(argv[1], &requestid);
    if (err) {
        LOG_ERR("bad requestid specified");
        exit(EXIT_FAILURE);
    }

    err = str2int(argv[2], &answerid);
    if (err) {
        LOG_ERR("bad answerid specified");
        exit(EXIT_FAILURE);
    }

    printf(">>>> requestid is %d <<<<\n", requestid);
    printf(">>>> answerid is %d <<<<\n", answerid);
    printf(">>>> Client started! <<<<\n");
    for (;;) {
        if (sendmsg("Client asks", requestid, answerid, 1)) {
            LOG_ERR("sendmsg failed");
            exit(EXIT_FAILURE);
        }
        char *buf = readmsg(answerid, 0);
        if (!buf) {
            LOG_ERR("readmsg failed");
            exit(EXIT_FAILURE);
        }

        printf("Server send: '%s'\n", ((struct servermsg *)buf)->mtext);

        free(buf);
        sleep(1);
    }
    return 0;
}
