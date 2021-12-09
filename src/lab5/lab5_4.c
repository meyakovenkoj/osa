#include "logerr.h"
#include "str2int.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define MAX_SZ 10

struct mmsg
{
    long mtype;    /* message type (+ve integer) */
    char mtext[1]; /* message body */
};

int main(int argc, char *argv[])
{
    int msgqid;
    long type;
    int err = 0;
    err = str2int(argv[1], &msgqid);
    if (err) {
        LOG_ERR("bad msgqid specified");
        exit(EXIT_FAILURE);
    }
    err = str2long(argv[2], &type);
    if (err) {
        LOG_ERR("bad type specified");
        exit(EXIT_FAILURE);
    }

    if (msgqid < 0) {
        LOG_ERR("bad msgqid specified");
        exit(EXIT_FAILURE);
    }
    char *buf = calloc(4 + MAX_SZ, sizeof(char));
    int buflen = MAX_SZ;
    int rcv_res = msgrcv(msgqid, buf, buflen, type, IPC_NOWAIT | MSG_NOERROR);
    if (rcv_res == -1) {
        LOG_ERR("msgrcv error");
        exit(errno);
    }

    printf("%s\n", ((struct mmsg *)buf)->mtext);
    free(buf);
    return 0;
}
