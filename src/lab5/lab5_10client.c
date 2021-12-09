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
        exit(1);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Client started! <<<<\n");
    for (;;) {
        if (sendmsg("Client asks", msgqid, msgqid, 1)) {
            LOG_ERR("sendmsg failed");
            exit(1);
        }
        char *buf = readmsg(msgqid, 2);
        if (!buf) {
            LOG_ERR("readmsg failed");
            exit(1);
        }

        printf("Server send: '%s'\n", ((struct servermsg *)buf)->mtext);

        free(buf);
        sleep(1);
    }
    return 0;
}
