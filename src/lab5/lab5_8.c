#include "logerr.h"
#include "srvfunc.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int msgqid = -1;

void action(int sig)
{
    int res_cnlt = msgctl(msgqid, IPC_RMID, 0);
    if (res_cnlt == -1) {
        write(STDOUT_FILENO, "removing of queue failed\n", sizeof("removing of queue failed\n"));
    } else {
        write(STDOUT_FILENO, "queue removed\n", sizeof("queue removed\n"));
    }
}

int main(int argc, char *argv[])
{
    struct sigaction act;
    act.sa_handler = action;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0; // set default after execute

    sigaction(SIGINT, &act, 0);

    key_t key = ftok(__FILE__, 'A');
    if (key == -1) {
        LOG_ERR("failed to get identifier");
        exit(errno);
    }
    msgqid = msgget(key, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Server started! <<<<\n");
    for (;;) {
        char *buf = readmsg(msgqid, 0);
        if (!buf) {
            LOG_ERR("readmsg failed");
            exit(1);
        }

        int answerid = ((struct servermsg *)buf)->msgqid;
        long type = ((struct servermsg *)buf)->mtype;
        if (sendmsg(((struct servermsg *)buf)->mtext, answerid, msgqid, type)) {
            LOG_ERR("sendmsg failed");
            exit(1);
        }
        printf("Answered to %d\n", answerid);
        free(buf);
        // sleep(1);
    }
    return 0;
}
