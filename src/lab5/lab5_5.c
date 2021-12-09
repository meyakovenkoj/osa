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
    int err = 0;
    int msgqid;
    err = str2int(argv[1], &msgqid);
    if (err) {
        LOG_ERR("bad msgqid specified");
        exit(EXIT_FAILURE);
    }

    int res_cnlt = msgctl(msgqid, IPC_RMID, 0);
    if (res_cnlt == -1) {
        LOG_ERR("removing of queue failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}
