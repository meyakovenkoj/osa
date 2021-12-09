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
    int serverid;
    int err = 0;
    err = str2int(argv[1], &serverid);
    if (err) {
        LOG_ERR("bad serverid specified");
        exit(EXIT_FAILURE);
    }

    int msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Client started! <<<<\n");
    for (;;) {
        if (sendmsg("Client asks", serverid, msgqid, 0)) {
            LOG_ERR("sendmsg failed");
            exit(EXIT_FAILURE);
        }
        char *buf = readmsg(msgqid, 0);
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
