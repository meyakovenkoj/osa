#include "daemon.h"

struct sockaddr_in client_name;
socklen_t size = sizeof(client_name);
int socket_fd = -1;
int msgqid = -1;
int logfd = -1;

configSet daemonSet = {0};

char sockbuff[MAXLINE];

char *getTime()
{
    char *ret;
    time_t now;
    struct tm *ptr;
    char tbuf[80];
    ret = (char *)malloc(100);
    memset(ret, 0, 100);
    time(&now);
    ptr = localtime(&now);
    strftime(tbuf, 80, "%Y-%B-%e %H:%M:%S", ptr);
    ret = tbuf;
    return (ret);
}

void cleanUp()
{
    FD_LOG(getTime(), "clean up processing...", logfd);
    if (socket_fd != -1) {
        close(socket_fd);
    }
    if (msgqid != -1) {
        int res_cnlt = msgctl(msgqid, IPC_RMID, 0);
        if (res_cnlt == -1) {
            FD_ERR("removing of queue failed", logfd);
        }
    }
    FD_LOG(getTime(), "clean up done", logfd);
    close(logfd);
}

void updateConfig(enum upd needUpdate, int port, char *logfile)
{
    if (needUpdate == UPD_PORT || needUpdate == UPD_BOTH) {
        if (socket_fd != -1) {
            close(socket_fd);
        }
        struct sockaddr_in name;
        socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
        name.sin_family = AF_INET;
        name.sin_addr.s_addr = INADDR_ANY;
        name.sin_port = htons(port);
        if (bind(socket_fd, (const struct sockaddr *)&name, sizeof(name)) == -1) {
            LOG_ERR("bind error");
            exit(errno);
        }
        listen(socket_fd, 20);
        daemonSet.port = port;
    }

    if (needUpdate == UPD_LOGFILE || needUpdate == UPD_BOTH) {
        if (msgqid != -1) {
            int res_cnlt = msgctl(msgqid, IPC_RMID, 0);
            if (res_cnlt == -1) {
                LOG_ERR("removing of queue failed");
            }
        }
        close(logfd);
        free(daemonSet.logfile);
        daemonSet.logfile = calloc(strlen(logfile) + 1, sizeof(char));
        strcpy(daemonSet.logfile, logfile);
        logfd = open(daemonSet.logfile, O_WRONLY | O_APPEND, 0644);
        if (logfd == -1) {
            LOG_ERR("Invailid logfile");
            exit(EXIT_FAILURE);
        }
        key_t key = ftok(logfile, 'A');
        if (key == -1) {
            LOG_ERR("failed to get identifier");
            exit(errno);
        }
        msgqid = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
        // msgqid = msgget(key, IPC_CREAT | 0660);
        if (msgqid == -1) {
            LOG_ERR("failed to get queue id");
            exit(errno);
        }
        printf("<<<<<< msgqid = %d >>>>>>\n", msgqid);
    }
}

int parseConfig()
{
    FILE *fp;

    if ((fp = fopen(CONFIG_NAME, "r+")) == NULL) {
        LOG_ERR("fopen()");
        return 1;
    }

    int port;
    char *logfile;
    char buf[100] = {0};
    if (fscanf(fp, "port = %s\n", buf) != 1) {
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
    fclose(fp);
    int lognameSize = strlen(buf);
    logfile = calloc(lognameSize + 1, sizeof(char));
    if (!logfile) {
        LOG_ERR("no memory left");
        return 1;
    }
    strcpy(logfile, buf);

    int needUpdate = NO_UPD;
    if (daemonSet.port != port) {
        needUpdate += UPD_PORT;
    }
    if (!daemonSet.logfile || strcmp(daemonSet.logfile, logfile)) {
        needUpdate += UPD_LOGFILE;
    }
    if (needUpdate) {
        updateConfig(needUpdate, port, logfile);
    }
    return 0;
}

int Daemon(void)
{
    signal(SIGHUP, sig_hup);
    signal(SIGTERM, sig_term);
    signal(SIGCHLD, sig_child);

    parseConfig();

    for (;;) {
        socklen_t len;
        int n;

        len = sizeof(client_name);

        n = recvfrom(socket_fd, (char *)sockbuff, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&client_name,
                     &len);

        sockbuff[n] = '\0';
        FD_LOG(getTime(), "recieved message from client", logfd);
        FD_LOG(getTime(), sockbuff, logfd);
        pid_t pid;
        char result[MAXLINE] = {0};
        for (char *ptr = strtok(sockbuff, " "); ptr; ptr = strtok(NULL, " ")) {
            int number;
            int errm = str2int(ptr, &number);
            if (errm) {
                FD_ERR("INVAILID DATA", logfd);
                continue;
            }
            if ((pid = fork()) < 0)
                ;
            if (pid == 0) {
                char *buf = textmsg(msgqid, 1);
                if (!buf) {
                    FD_ERR("treadmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                int n;
                str2int(buf, &n);
                n = n - 1;
                usleep(1000 * (rand() % 101 + 1));
                FD_LOG(getTime(), "got from server 1", logfd);
                char *buf2 = textmsg(msgqid, 3);
                if (!buf2) {
                    FD_ERR("treadmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf2, &n2);
                n = n + n2;
                usleep(1000 * (rand() % 101 + 1));
                FD_LOG(getTime(), "got from proc 2", logfd);
                char buf3[100] = {0};
                sprintf(buf3, "%d", n);
                if (tsendmsg(buf3, msgqid, msgqid, 4)) {
                    FD_ERR("tsendmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                FD_LOG(getTime(), "sent from proc 1", logfd);
                exit(0);
            }
            if (pid) {
                if (tsendmsg(ptr, msgqid, msgqid, 1)) {
                    FD_ERR("tsendmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                FD_LOG(getTime(), "sent from server 1", logfd);
            }

            if ((pid = fork()) < 0)
                ;
            if (pid == 0) {
                char *buf2 = textmsg(msgqid, 2);
                if (!buf2) {
                    FD_ERR("treadmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf2, &n2);
                n2 = n2 << 1;
                usleep(1000 * (rand() % 101 + 1));
                char buf4[100] = {0};
                sprintf(buf4, "%d", n2);
                FD_LOG(getTime(), "got from server 2", logfd);
                if (tsendmsg(buf4, msgqid, msgqid, 3)) {
                    FD_ERR("tsendmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                FD_LOG(getTime(), "sent from proc 2", logfd);
                exit(0);
            }
            if (pid) {
                if (tsendmsg(ptr, msgqid, msgqid, 2)) {
                    FD_ERR("tsendmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                FD_LOG(getTime(), "sent from server 2", logfd);
            }

            if ((pid = fork()) < 0)
                ;
            if (pid == 0) {
                char *buf = textmsg(msgqid, 4);
                if (!buf) {
                    FD_ERR("treadmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf, &n2);
                n2 = n2 << 1;
                usleep(1000 * (rand() % 101 + 1));
                char buf4[100] = {0};
                sprintf(buf4, "%d", n2);
                FD_LOG(getTime(), "got from proc 1", logfd);
                if (tsendmsg(buf4, msgqid, msgqid, 5)) {
                    FD_ERR("tsendmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                FD_LOG(getTime(), "sent from proc 3", logfd);
                exit(0);
            }
            if (pid) {
                char *buf = textmsg(msgqid, 5);
                if (!buf) {
                    FD_ERR("treadmsg failed", logfd);
                    exit(EXIT_FAILURE);
                }
                printf("GET: %s\n", buf);
                strcat(result, buf);
                strcat(result, " ");
                sleep(2);
                FD_LOG(getTime(), "got from proc 3", logfd);
            }
        }
        FD_LOG(getTime(), "send answer", logfd);
        sendto(socket_fd, result, strlen(result),
               0, (const struct sockaddr *)&client_name,
               len);
    }
}

void sig_child(int sig)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        ;
    return;
}

void sig_hup(int sig)
{
    FD_LOG(getTime(), "recieved config update", logfd);
    parseConfig();
    FD_LOG(getTime(), "config update done", logfd);
    return;
}

void sig_term(int sig)
{
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTERM, SIG_IGN);
    killpg(0, SIGTERM);
    cleanUp();
    exit(EXIT_SUCCESS);
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
