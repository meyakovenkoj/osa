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
    int msgqid;
    int err = 0;
    err = str2int(argv[1], &msgqid);
    if (err) {
        LOG_ERR("bad msgqid specified");
        exit(EXIT_FAILURE);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Client started! <<<<\n");
    for (;;) {
        if (tsendmsg("Client asks", msgqid, msgqid, 1)) {
            LOG_ERR("tsendmsg failed");
            exit(EXIT_FAILURE);
        }
        char *buf = treadmsg(msgqid, 2);
        if (!buf) {
            LOG_ERR("treadmsg failed");
            exit(EXIT_FAILURE);
        }

        printf("Server send: '%s'\n", ((struct servermsg *)buf)->mtext);

        free(buf);
        sleep(1);
    }
    return 0;
}
