#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define MAX_SZ 50
typedef struct buffer
{
    long mtype;
    char mtext[MAX_SZ];
} buffer;

int main(int argc, char *argv[])
{
    int err = 0;
    key_t key = ftok(__FILE__, 'A');
    if (key == -1) {
        LOG_ERR("failed to get identifier");
        exit(errno);
    }
    int msgqid = msgget(key, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    buffer buf = {0};
    buf.mtype = 10;
    strcpy(buf.mtext, "send some text 1");
    err = msgsnd(msgqid, &buf, strlen(buf.mtext) + 1, 0);
    if (err) {
        LOG_ERR("failed to send message");
        exit(errno);
    }
    memset(buf.mtext, 0, sizeof(buf.mtext));

    buf.mtype = 20;
    strcpy(buf.mtext, "send some text 2");
    err = msgsnd(msgqid, &buf, strlen(buf.mtext) + 1, 0);
    if (err) {
        LOG_ERR("failed to send message");
        exit(errno);
    }
    memset(buf.mtext, 0, sizeof(buf.mtext));

    buf.mtype = 30;
    strcpy(buf.mtext, "send some text 3");
    err = msgsnd(msgqid, &buf, strlen(buf.mtext) + 1, 0);
    if (err) {
        LOG_ERR("failed to send message");
        exit(errno);
    }
    memset(buf.mtext, 0, sizeof(buf.mtext));

    printf("msgqid = %d\n", msgqid);

    struct msqid_ds ds;
    err = msgctl(msgqid, IPC_STAT, &ds);
    if (err) {
        LOG_ERR("failed to get information about queue");
        exit(errno);
    }

    printf("Ownership and permissions\n");
    printf("Owner's user ID:\t\t%u\n", ds.msg_perm.uid);
    printf("Owner's group ID:\t\t%u\n", ds.msg_perm.gid);
    printf("Creator's user ID:\t\t%u\n", ds.msg_perm.cuid);
    printf("Creator's group ID:\t\t%u\n", ds.msg_perm.cgid);
    printf("Read/write permission:\t\t(%3o)\n", ds.msg_perm.mode & 0777);
    printf("Time of last msgsnd(2):\t\t%s", ctime(&ds.msg_stime));
    printf("Time of last msgrcv(2):\t\t%s", ctime(&ds.msg_rtime));
    printf("Time of creation or last\n  modification by msgctl():\t%s", ctime(&ds.msg_ctime));
    printf("# of bytes in queue:\t\t%ld\n", ds.msg_cbytes);
    printf("# number of messages in queue:\t%ld\n", ds.msg_qnum);
    printf("Maximum # of bytes in queue:\t%ld\n", ds.msg_qbytes);
    printf("PID of last msgsnd(2):\t\t%d\n", ds.msg_lspid);
    printf("PID of last msgrcv(2):\t\t%d\n", ds.msg_lrpid);
    return 0;
}
