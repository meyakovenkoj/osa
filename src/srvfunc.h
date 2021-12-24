#ifndef SRVFUNC_H
#define SRVFUNC_H
#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

/*
 * The Requirement for message type:
 * Example structure describing a message whose address is to be passed as
 * the second argument to the functions msgrcv() and msgsnd().  The only
 * actual hard requirement is that the first field be of type long, and
 * contain the message type.  The user is encouraged to define their own
 * application specific structure; this definition is included solely for
 * backward compatability with existing source code.
 */
struct servermsg
{
    long mtype;    /* message type (+ve integer) */
    int msgqid;    /* id of sender queue */
    char mtext[1]; /* message body */
};

char *treadmsg(int msgqid, long type)
{
    int buflen = 10;
    char *buf = (char *)calloc(sizeof(long) + buflen, sizeof(char));
    if (!buf) {
        LOG_ERR("Not enough memory");
        return NULL;
    }
    for (;;) {
        errno = 0;
        int rcv_res = msgrcv(msgqid, buf, buflen, type, IPC_NOWAIT);
        if (rcv_res > -1) {
        //     LOG_ERR("return from msrcv");
            break;
        }
        if (errno == E2BIG) {
            buf = (char *)realloc(buf, sizeof(long) + buflen);
            if (!buf) {
                LOG_ERR("Not enough memory");
                return NULL;
            }
            buflen += 10;
        } else if (errno == ENOMSG) {
            continue;
        } else {
            LOG_ERR("msgrcv error");
            return NULL;
        }
    }
    return buf;
}

char *textmsg(int msgqid, long type)
{
    char *tmp = treadmsg(msgqid, type);
    struct servermsg *mesg = (struct servermsg *)tmp;
//     printf("type:%ld text:%s\n", mesg->mtype, mesg->mtext);
    char *res = malloc(strlen(mesg->mtext) + 1);
    strcpy(res, mesg->mtext);
    free(tmp);
    return res;
}

int tsendmsg(char *msg, int msgqid, int senderid, long type)
{
    if (!msg) {
        errno = EINVAL;
        return -1;
    }
    int err = 0;
    int buflen = sizeof(senderid) + strlen(msg) + 1;
    char *buf = (char *)calloc(sizeof(long) + buflen, sizeof(char));
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
