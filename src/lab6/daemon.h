#ifndef DAEMON_H
#define DAEMON_H
#include "logerr.h"
#include "srvfunc.h"
#include "str2int.h"
#include <errno.h>
#include <fcntl.h>
#include <resolv.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MAXLINE 1024
#define CONFIG_NAME "/Users/ivanyakovenko/Documents/osa/src/lab6/daemon.config"

typedef struct configSet
{
    int port;
    char *logfile;
} configSet;

enum upd
{
    NO_UPD,
    UPD_PORT,
    UPD_LOGFILE,
    UPD_BOTH
};

void sig_child(int);
void sig_hup(int);
void sig_term(int);
int Daemon(void);
char *getTime();
void cleanUp();
int parseConfig();
void updateConfig(enum upd needUpdate, int port, char *logfile);

#endif // DAEMON_H
