#include "logerr.h"
#include "readstring.h"
#include "srvfunc.h"
#include "str2int.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main()
{
    int msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
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
            char *buf = readmsg(msgqid, 1);
            if (!buf) {
                LOG_ERR("readmsg failed");
                exit(1);
            }

            write(STDOUT_FILENO, "0", sizeof("0"));

            if (sendmsg("child", msgqid, msgqid, 2)) {
                LOG_ERR("sendmsg failed");
                exit(1);
            }
            free(buf);
            sleep(1);
        }
        break;
    }
    default: {
        for (;;) {
            if (sendmsg("parent", msgqid, msgqid, 1)) {
                LOG_ERR("sendmsg failed");
                exit(1);
            }
            sleep(1);

            write(STDOUT_FILENO, "1", sizeof("1"));

            char *buf = readmsg(msgqid, 2);
            if (!buf) {
                LOG_ERR("readmsg failed");
                exit(1);
            }
            free(buf);
        }
        break;
    }
    }
    return 0;
}
