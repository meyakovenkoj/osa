#include "logerr.h"
#include "srvfunc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main(int argc, char *argv[])
{
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

    printf(">>>> Server msgqid id is %d <<<<\n", msgqid);
    printf(">>>> Server started! <<<<\n");
    for (;;) {
        char *buf = readmsg(msgqid, 1);
        if (!buf) {
            LOG_ERR("readmsg failed");
            exit(1);
        }

        if (sendmsg("server answers", msgqid, msgqid, 2)) {
            LOG_ERR("sendmsg failed");
            exit(1);
        }
        printf("Answered to %d\n", msgqid);
        free(buf);
        // sleep(1);
    }
    return 0;
}
