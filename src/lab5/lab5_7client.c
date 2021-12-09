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

int main(int argc, char *argv[])
{
    int serverid;
    int err = 0;
    err = str2int(argv[1], &serverid);
    if (err) {
        LOG_ERR("bad serverid specified");
        exit(1);
    }

    int msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Client started! <<<<\n");
    for (;;) {
        printf("enter reciever type\n");
        char *rtypestr = readline(NULL);
        if (!rtypestr) {
            LOG_ERR("readline failed");
            exit(1);
        }
        long rtype;
        err = str2long(rtypestr, &rtype);
        if (err) {
            LOG_ERR("bad type specified");
            exit(1);
        }
        printf("enter sender type\n");
        char *stypestr = readline(NULL);
        if (!stypestr) {
            LOG_ERR("readline failed");
            exit(1);
        }
        long stype;
        err = str2long(stypestr, &stype);
        if (err) {
            LOG_ERR("bad type specified");
            exit(1);
        }
        printf("enter text\n");
        char *text = readline(NULL);
        if (!text) {
            LOG_ERR("readline failed");
            exit(1);
        }
        if (sendmsg(text, serverid, msgqid, stype)) {
            LOG_ERR("sendmsg failed");
            exit(1);
        }
        char *buf = readmsg(msgqid, rtype);
        if (!buf) {
            LOG_ERR("readmsg failed");
            exit(1);
        }

        printf("S: '%s'\n", ((struct servermsg *)buf)->mtext);

        free(buf);
        free(stypestr);
        free(rtypestr);
        free(text);
        sleep(1);
    }
    return 0;
}
