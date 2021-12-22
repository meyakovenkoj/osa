#include "logerr.h"
#include "readstring.h"
#include "srvfunc.h"
#include "str2int.h"
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
    int serverid;
    int err = 0;
    err = str2int(argv[1], &serverid);
    if (err) {
        LOG_ERR("bad serverid specified");
        exit(EXIT_FAILURE);
    }

    msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Client started! <<<<\n");
    for (;;) {
        printf("enter reciever queue id\n");
        char *rmsgqidstr = readline(NULL);
        if (!rmsgqidstr) {
            LOG_ERR("readline failed");
            exit(EXIT_FAILURE);
        }
        int rmsgqid;
        err = str2int(rmsgqidstr, &rmsgqid);
        if (err) {
            LOG_ERR("bad reciever queue id specified");
            exit(EXIT_FAILURE);
        }
        printf("enter reciever type\n");
        char *rtypestr = readline(NULL);
        if (!rtypestr) {
            LOG_ERR("readline failed");
            exit(EXIT_FAILURE);
        }
        long rtype;
        err = str2long(rtypestr, &rtype);
        if (err) {
            LOG_ERR("bad type specified");
            exit(EXIT_FAILURE);
        }
        printf("enter sender type\n");
        char *stypestr = readline(NULL);
        if (!stypestr) {
            LOG_ERR("readline failed");
            exit(EXIT_FAILURE);
        }
        long stype;
        err = str2long(stypestr, &stype);
        if (err) {
            LOG_ERR("bad type specified");
            exit(EXIT_FAILURE);
        }
        printf("enter text\n");
        char *text = readline(NULL);
        if (!text) {
            LOG_ERR("readline failed");
            exit(EXIT_FAILURE);
        }
        if (tsendmsg(text, serverid, rmsgqid, stype)) {
            LOG_ERR("tsendmsg failed");
            exit(EXIT_FAILURE);
        }
        char *buf = treadmsg(msgqid, rtype);
        if (!buf) {
            LOG_ERR("treadmsg failed");
            exit(EXIT_FAILURE);
        }

        printf("S: '%s'\n", ((struct servermsg *)buf)->mtext);

        free(rmsgqidstr);
        free(buf);
        free(stypestr);
        free(rtypestr);
        free(text);
        sleep(1);
    }
    return 0;
}
