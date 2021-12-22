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
    key_t key_request = ftok(__FILE__, 'A');
    if (key_request == -1) {
        LOG_ERR("failed to get identifier");
        exit(errno);
    }
    int requestid = msgget(key_request, IPC_CREAT | 0660);
    if (requestid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    key_t key_answer = ftok(__FILE__, 'B');
    if (key_answer == -1) {
        LOG_ERR("failed to get identifier");
        exit(errno);
    }
    int answerid = msgget(key_answer, IPC_CREAT | 0660);
    if (answerid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    printf(">>>> Server request id is %d <<<<\n", requestid);
    printf(">>>> Server answer id is %d <<<<\n", answerid);
    printf(">>>> Server started! <<<<\n");
    for (;;) {
        char *buf = treadmsg(requestid, 0);
        if (!buf) {
            LOG_ERR("treadmsg failed");
            exit(EXIT_FAILURE);
        }

        if (tsendmsg("server answers", answerid, requestid, 1)) {
            LOG_ERR("tsendmsg failed");
            exit(EXIT_FAILURE);
        }
        printf("Answered to %d\n", answerid);
        free(buf);
        // sleep(1);
    }
    return 0;
}
