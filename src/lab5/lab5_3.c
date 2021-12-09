#include "logerr.h"
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
    char *buf = calloc(4 + 1, sizeof(char));
    int buflen = 1;
    for (;;) {
        int rcv_res = msgrcv(msgqid, buf, buflen, type, IPC_NOWAIT);
        if (rcv_res > -1) {
            break;
        }
        if (errno == E2BIG) {
            buflen += 10;
            buf = realloc(buf, 4 + buflen);
            if (!buf) {
                LOG_ERR("Not enough memory");
                exit(1);
            }
        } else {
            LOG_ERR("msgrcv error");
            exit(errno);
        }
    }

    printf("%s\n", ((struct mymsg *)buf)->mtext);
    free(buf);
    return 0;
}