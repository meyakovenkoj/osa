#include "logerr.h"
#include "readstring.h"
#include "srvfunc.h"
#include "str2int.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

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

int main()
{
    struct sigaction act;
    act.sa_handler = action;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0; // set default after execute

    sigaction(SIGINT, &act, 0);
    msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }
    switch (fork()) {
    case -1: {
        LOG_ERR("failed fork");
        exit(errno);
    }
    case 0: {
        for (;;) {
            char *buf = treadmsg(msgqid, 1);
            if (!buf) {
                LOG_ERR("treadmsg failed");
                exit(EXIT_FAILURE);
            }

            write(STDOUT_FILENO, "0", sizeof("0"));

            if (tsendmsg("child", msgqid, msgqid, 2)) {
                LOG_ERR("tsendmsg failed");
                exit(EXIT_FAILURE);
            }
            free(buf);
            sleep(1);
        }
        break;
    }
    default: {
        for (;;) {
            if (tsendmsg("parent", msgqid, msgqid, 1)) {
                LOG_ERR("tsendmsg failed");
                exit(EXIT_FAILURE);
            }
            sleep(1);

            write(STDOUT_FILENO, "1", sizeof("1"));

            char *buf = treadmsg(msgqid, 2);
            if (!buf) {
                LOG_ERR("treadmsg failed");
                exit(EXIT_FAILURE);
            }
            free(buf);
        }
        break;
    }
    }
    return 0;
}
