#include "daemon.h"

struct sockaddr_in client_name;
socklen_t size = sizeof(client_name);
int socket_fd = -1;
int msgqid = -1;
int logfd = -1;
pid_t pid1 = -1;
pid_t pid2 = -1;
pid_t pid3 = -1;

configSet daemonSet = {0};

char sockbuff[MAXLINE];

void cleanUp()
{
    FD_LOG("clean up processing...", logfd);
    if (socket_fd != -1) {
        close(socket_fd);
    }
    if (msgqid != -1) {
        int res_cnlt = msgctl(msgqid, IPC_RMID, 0);
        if (res_cnlt == -1) {
            FD_ERR("removing of queue failed", logfd);
        }
    }
    FD_LOG("clean up done", logfd);
    if (logfd != -1) {
        close(logfd);
    }
}

int updateConfig(enum upd needUpdate, int port, char *logfile)
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
            return 1;
        }
        listen(socket_fd, 20);
        daemonSet.port = port;
    }

    if (needUpdate == UPD_LOGFILE || needUpdate == UPD_BOTH) {
        if (msgqid != -1) {
            int res_cnlt = msgctl(msgqid, IPC_RMID, 0);
            if (res_cnlt == -1) {
                LOG_ERR("removing of queue failed");
                return 1;
            }
        }
        close(logfd);
        free(daemonSet.logfile);
        daemonSet.logfile = calloc(strlen(logfile) + 1, sizeof(char));
        strcpy(daemonSet.logfile, logfile);
        logfd = open(daemonSet.logfile, O_WRONLY | O_APPEND, 0644);
        if (logfd == -1) {
            LOG_ERR("Invailid logfile");
            return 1;
        }
        key_t key = ftok(logfile, 'A');
        if (key == -1) {
            LOG_ERR("failed to get identifier");
            return 1;
        }
        msgqid = msgget(key, IPC_CREAT | IPC_EXCL | 0660);
        // msgqid = msgget(key, IPC_CREAT | 0660);
        if (msgqid == -1) {
            LOG_ERR("failed to get queue id");
            return 1;
        }
        printf("<<<<<< msgqid = %d >>>>>>\n", msgqid);
    }
    return 0;
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
        return 1;
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
    err = 0;
    int needUpdate = NO_UPD;
    if (daemonSet.port != port) {
        needUpdate += UPD_PORT;
    }
    if (!daemonSet.logfile || strcmp(daemonSet.logfile, logfile)) {
        needUpdate += UPD_LOGFILE;
    }
    if (needUpdate) {
        err = updateConfig(needUpdate, port, logfile);
    }
    return err;
}

int Daemon(void)
{
    signal(SIGHUP, sig_hup);
    signal(SIGTERM, sig_term);
    signal(SIGINT, sig_term);
    signal(SIGCHLD, sig_child);

    if (parseConfig()) {
        LOG_ERR("failed to load config");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        socklen_t len;
        int n;

        len = sizeof(client_name);

        n = recvfrom(socket_fd, (char *)sockbuff, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&client_name,
                     &len);

        sockbuff[n] = '\0';
        FD_LOG("recieved message from client", logfd);
        FD_LOG(sockbuff, logfd);
        pid_t pid;
        char result[MAXLINE] = {0};
        if ((pid = fork()) < 0)
            ;
        if (pid == 0) {
            for (;;) {
                char *buf = textmsg(msgqid, 1);
                if (!buf) {
                    FD_ERR("treadmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                int n;
                str2int(buf, &n);
                free(buf);
                n = n - 1;
                usleep(1000 * (rand() % 101 + 1));
                FD_LOG("got from server 1", logfd);
                char *buf2 = textmsg(msgqid, 3);
                if (!buf2) {
                    FD_ERR("treadmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf2, &n2);
                free(buf2);
                n = n + n2;
                usleep(1000 * (rand() % 101 + 1));
                FD_LOG("got from proc 2", logfd);
                char buf3[100] = {0};
                sprintf(buf3, "%d", n);
                if (tsendmsg(buf3, msgqid, msgqid, 4)) {
                    FD_ERR("tsendmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                FD_LOG("sent from proc 1", logfd);
            }
            exit(0);
        } else {
            pid1 = pid;
        }
        if ((pid = fork()) < 0)
            ;
        if (pid == 0) {
            for (;;) {
                char *buf2 = textmsg(msgqid, 2);
                if (!buf2) {
                    FD_ERR("treadmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf2, &n2);
                free(buf2);
                n2 = n2 << 1;
                usleep(1000 * (rand() % 101 + 1));
                char buf4[100] = {0};
                sprintf(buf4, "%d", n2);
                FD_LOG("got from server 2", logfd);
                if (tsendmsg(buf4, msgqid, msgqid, 3)) {
                    FD_ERR("tsendmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                FD_LOG("sent from proc 2", logfd);
            }
            exit(0);
        } else {
            pid2 = pid;
        }
        if ((pid = fork()) < 0)
            ;
        if (pid == 0) {
            for (;;) {
                char *buf = textmsg(msgqid, 4);
                if (!buf) {
                    FD_ERR("treadmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                int n2;
                str2int(buf, &n2);
                free(buf);
                n2 = n2 << 1;
                usleep(1000 * (rand() % 101 + 1));
                char buf4[100] = {0};
                sprintf(buf4, "%d", n2);
                FD_LOG("got from proc 1", logfd);
                if (tsendmsg(buf4, msgqid, msgqid, 5)) {
                    FD_ERR("tsendmsg failed", logfd);
                    cleanUp();
                    exit(EXIT_FAILURE);
                }
                FD_LOG("sent from proc 3", logfd);
            }
            exit(0);
        } else {
            pid3 = pid;
        }
        for (char *ptr = strtok(sockbuff, " "); ptr; ptr = strtok(NULL, " ")) {
            int number;
            int errm = str2int(ptr, &number);
            if (errm) {
                FD_ERR("INVAILID DATA", logfd);
                continue;
            }
            FD_LOG(ptr, logfd);
            if (tsendmsg(ptr, msgqid, msgqid, 1)) {
                FD_ERR("tsendmsg failed", logfd);
                cleanUp();
                exit(EXIT_FAILURE);
            }
            FD_LOG("sent from server 1", logfd);

            if (tsendmsg(ptr, msgqid, msgqid, 2)) {
                FD_ERR("tsendmsg failed", logfd);
                cleanUp();
                exit(EXIT_FAILURE);
            }
            FD_LOG("sent from server 2", logfd);

            char *buf = textmsg(msgqid, 5);
            if (!buf) {
                FD_ERR("treadmsg failed", logfd);
                exit(EXIT_FAILURE);
            }
            FD_LOG("Get result:", logfd);
            FD_LOG(buf, logfd);
            strcat(result, buf);
            strcat(result, " ");
            FD_LOG("got from proc 3", logfd);
            free(buf);
        }
        FD_LOG("send answer", logfd);
        FD_LOG(result, logfd);
        sendto(socket_fd, result, strlen(result),
               0, (const struct sockaddr *)&client_name,
               len);
        kill(pid1, 9);
        kill(pid2, 9);
        kill(pid3, 9);
    }
}

void sig_child(int sig)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        if (pid == pid1) {
            pid1 = -1;
        } else if (pid == pid2) {
            pid2 = -1;
        } else if (pid == pid3) {
            pid3 = -1;
        }
    }
    return;
}

void sig_hup(int sig)
{
    FD_LOG("recieved config update", logfd);
    parseConfig();
    FD_LOG("config update done", logfd);
    return;
}

void sig_term(int sig)
{
    if (pid1 != -1) {
        kill(pid1, 9);
    }
    if (pid2 != -1) {
        kill(pid2, 9);
    }
    if (pid3 != -1) {
        kill(pid3, 9);
    }
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
