#include "logerr.h"
#include "str2int.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define MAX_SZ 10

int main(int argc, char *argv[])
{
    int msgqid;
    long type;
    int err = 0;
    err = str2int(argv[1], &msgqid);
    if (err) {
        LOG_ERR("bad msgqid specified");
        exit(1);
    }
    err = str2long(argv[2], &type);
    if (err) {
        LOG_ERR("bad type specified");
        exit(1);
    }

    if (msgqid < 0) {
        LOG_ERR("bad msgqid specified");
        exit(1);
    }
    char *buf = calloc(4 + MAX_SZ, sizeof(char));
    int buflen = MAX_SZ;
    int rcv_res = msgrcv(msgqid, buf, buflen, type, IPC_NOWAIT | MSG_NOERROR);
    if (rcv_res == -1) {
        LOG_ERR("msgrcv error");
        exit(errno);
    }

    printf("%s\n", ((struct mymsg *)buf)->mtext);
    free(buf);
    return 0;
}