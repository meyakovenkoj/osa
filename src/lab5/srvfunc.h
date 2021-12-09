#ifndef SRVFUNC_H
#define SRVFUNC_H
#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

struct servermsg
{
    long mtype;    /* message type (+ve integer) */
    int msgqid;    /* id of sender queue */
    char mtext[1]; /* message body */
};

char *readmsg(int msgqid, long type)
{
    char *buf = (char *)calloc(4 + 1, sizeof(char));
    if (!buf) {
        LOG_ERR("Not enough memory");
        return NULL;
    }
    int buflen = 1;
    for (;;) {
        int rcv_res = msgrcv(msgqid, buf, buflen, type, 0);
        if (rcv_res > -1) {
            break;
        }
        if (errno == E2BIG) {
            buflen += 10;
            buf = (char *)realloc(buf, 4 + buflen);
            if (!buf) {
                LOG_ERR("Not enough memory");
                return NULL;
            }
        } else {
            LOG_ERR("msgrcv error");
            return NULL;
        }
    }
    return buf;
}

int sendmsg(char *msg, int msgqid, int senderid, long type)
{
    if (!msg) {
        errno = EINVAL;
        return -1;
    }
    int err = 0;
    int buflen = sizeof(senderid) + strlen(msg) + 1;
    char *buf = (char *)calloc(4 + buflen, sizeof(char));
    if (!buf) {
        LOG_ERR("Not enough memory");
        return -1;
    }
    struct servermsg *tmp = (struct servermsg *)buf;
    tmp->mtype = type;
    tmp->msgqid = senderid;
    strcpy(tmp->mtext, msg);
    err = msgsnd(msgqid, buf, buflen, 0);
    if (err) {
        LOG_ERR("failed to send message");
        return -1;
    }
    free(buf);
    return 0;
}

#endif //SRVFUNC_H
