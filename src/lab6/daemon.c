#include "logerr.h"
#include "srvfunc.h"
#include "str2int.h"
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <resolv.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define PORT_CON 6823
#define MAXLINE 1024
#define CONFIG_NAME "/Users/ivanyakovenko/Documents/osa/src/lab6/daemon.config"

struct sockaddr_in client_name;
socklen_t size = sizeof(client_name);
int client_socket_fd;

typedef struct configSet
{
    int port;
    char *logfile;
} configSet;

configSet daemonSet;

char sockbuff[MAXLINE];
time_t now;
struct tm *ptr;
char tbuf[80];

void sig_child(int);
void sig_hup(int);
void sig_term(int);
int Daemon(void);
char *getTime();

char *getTime()
{
    char *ret;
    ret = (char *)malloc(100);
    bzero(ret, 100);
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf, 80, "%Y-%B-%e %H:%M:%S", ptr);
    ret = tbuf;
    return (ret);
}

int parseConfig(const char *config, const char *key, char *value)
{
    FILE *fp;

    if ((fp = fopen(config, "r+")) == NULL) {
        LOG_ERR("fopen()");
        return 1;
    }

    int port;
    char *logfile;
    char buf[100] = {0};
    if (fscanf(fp, "port = %s", buf) != 1) {
        if (feof(fp)) {
            return 1;
        }
        LOG_ERR("fscanf()");
        return 1;
    }
    int err = str2int(buf, &port);
    if (err) {
        LOG_ERR("port parse fail");
    }
    memset(buf, 0, 100);
    if (fscanf(fp, "logfile = %s", buf) != 1) {
        if (feof(fp)) {
            return 1;
        }
        LOG_ERR("fscanf()");
        return 1;
    }
    int lognameSize = strlen(buf);
    logfile = calloc(lognameSize + 1, sizeof(char));
    strcpy(logfile, buf);
    if (daemonSet.port != port) {
        //      daemonSet.port = port;
    }
    if (daemonSet.logfile && strcmp(daemonSet.logfile, logfile)) {
        //      daemonSet.port = port;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    pid_t parpid;

    if (argc < 2) {
        printf("Usage ./daemon -d for daemon or ./daemon -i for interactive\n");
        exit(1);
    }
    if (strcmp(argv[1], "-i") == 0)
        Daemon();
    else if (strcmp(argv[1], "-d") == 0) {
        if ((parpid = fork()) < 0) {
            printf("\ncan't fork");
            exit(1);
        } else if (parpid != 0)
            exit(0);
        setsid();
        Daemon();
    } else {
        printf("Usage ./daemon -d for daemon or ./daemon -i for interactive\n");
        exit(1);
    }
    return 0;
}

int Daemon(void)
{

    int socket_fd;

    signal(SIGHUP, sig_hup);
    signal(SIGTERM, sig_term);

    struct sockaddr_in name;
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_port = htons(PORT_CON);
    if (bind(socket_fd, (const struct sockaddr *)&name, sizeof(name)) == -1) {
        perror("bind");
        exit(0);
    }
    listen(socket_fd, 20);

    key_t key = ftok(CONFIG_NAME, 'A');
    if (key == -1) {
        LOG_ERR("failed to get identifier");
        exit(errno);
    }
    int msgqid = msgget(key, IPC_CREAT | 0660);
    if (msgqid == -1) {
        LOG_ERR("failed to get queue id");
        exit(errno);
    }

    printf(">>>> msgqid is %d <<<<\n", msgqid);
    printf(">>>> Server started! <<<<\n");

    for (;;) {
        socklen_t len;
        int n;

        len = sizeof(client_name);
        signal(SIGCHLD, sig_child);
        n = recvfrom(socket_fd, (char *)sockbuff, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&client_name,
                     &len);
        sockbuff[n] = '\0';
        pid_t pid;
        char result[MAXLINE] = {0};
        int fd2 = open("logfile", O_RDWR | O_CREAT, 0644);
        // for each in args
        for (char *ptr = strtok(sockbuff, " "); ptr; ptr = strtok(NULL, " ")) {
            int number;
            int errm = str2int(ptr, &number);
            if (errm) {
                LOG_ERR("INVAILID DATA");
                continue;
            }
            if ((pid = fork()) < 0)
                ;
            if (pid == 0) {
                char *buf = textmsg(msgqid, 1);
                if (!buf) {
                    LOG_ERR("treadmsg failed");
                    exit(EXIT_FAILURE);
                }
                int n;
                str2int(buf, &n);
                n = n - 1;
                write(fd2, "got from server 1\n", strlen("got from server 1\n"));
                char *buf2 = textmsg(msgqid, 3);
                if (!buf2) {
                    LOG_ERR("treadmsg failed");
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf2, &n2);
                n = n + n2;
                write(fd2, "got from proc 2\n", strlen("got from proc 2\n"));
                char buf3[10] = {0};
                sprintf(buf3, "%d", n);
                if (tsendmsg(buf3, msgqid, msgqid, 4)) {
                    LOG_ERR("tsendmsg failed");
                    exit(EXIT_FAILURE);
                }
                write(fd2, "sent from proc 1\n", strlen("sent from proc 1\n"));
                exit(0);
            }
            if (pid) {
                if (tsendmsg(ptr, msgqid, msgqid, 1)) {
                    LOG_ERR("tsendmsg failed");
                    exit(EXIT_FAILURE);
                }
                write(fd2, "sent from server 1\n", strlen("sent from server 1\n"));
            }

            if ((pid = fork()) < 0)
                ;
            if (pid == 0) {
                char *buf2 = textmsg(msgqid, 2);
                if (!buf2) {
                    LOG_ERR("treadmsg failed");
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf2, &n2);
                n2 = n2 << 1;
                char buf4[10] = {0};
                sprintf(buf4, "%d", n2);
                write(fd2, "got from server 2\n", strlen("got from server 2\n"));
                if (tsendmsg(buf4, msgqid, msgqid, 3)) {
                    LOG_ERR("tsendmsg failed");
                    exit(EXIT_FAILURE);
                }
                write(fd2, "sent from proc 2\n", strlen("sent from proc 2\n"));
                exit(0);
            }
            if (pid) {
                if (tsendmsg(ptr, msgqid, msgqid, 2)) {
                    LOG_ERR("tsendmsg failed");
                    exit(EXIT_FAILURE);
                }
                write(fd2, "sent from server 2\n", strlen("sent from server 2\n"));
            }

            if ((pid = fork()) < 0)
                ;
            if (pid == 0) {
                char *buf = textmsg(msgqid, 4);
                if (!buf) {
                    LOG_ERR("treadmsg failed");
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf, &n2);
                n2 = n2 >> 1;
                char buf4[10] = {0};
                sprintf(buf4, "%d", n2);
                write(fd2, "got from proc 1\n", strlen("got from proc 1\n"));
                if (tsendmsg(buf4, msgqid, msgqid, 5)) {
                    LOG_ERR("tsendmsg failed");
                    exit(EXIT_FAILURE);
                }
                write(fd2, "sent from proc 3\n", strlen("sent from proc 3\n"));
                exit(0);
            }
            if (pid) {
                char *buf = textmsg(msgqid, 5);
                if (!buf) {
                    LOG_ERR("treadmsg failed");
                    exit(EXIT_FAILURE);
                }
                printf("GET: %s\n", buf);
                strcat(result, buf);
                strcat(result, " ");
                sleep(2);
                write(fd2, "got from proc 3\n", strlen("got from proc 3\n"));
            }
        }
        close(fd2);
        sendto(socket_fd, result, strlen(result),
               0, (const struct sockaddr *)&client_name,
               len);
    }
}

void sig_child(int sig)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
    }
    return;
}

void sig_hup(int sig)
{
    return;
}

void sig_term(int sig)
{
    return;
}
