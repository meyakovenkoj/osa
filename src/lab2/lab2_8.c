#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "logerr.h"

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd, cn, pn, status;
#ifdef MODE
    int fd_child, fd_parent;
#endif
    char child_buf[256];
    char parent_buf[256];
    if (argc != 2) {
        printf("Usage ./out/lab2_8 <filename>\n");
        exit(1);
    }

    fd = open(argv[1], O_RDONLY);

    pid = fork();
    switch (pid) {
    case -1:
        LOG_ERR("fork failed");
        exit(1); //выход из родительского процесса
    case 0:
        printf("child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
#ifdef MODE
        fd_child = open("text_child.txt", O_RDWR | O_CREAT, 0644);
#endif
        do {
            cn = read(fd, child_buf, sizeof(child_buf));
#ifdef MODE
            write(fd_child, child_buf, cn);
#endif
#ifndef MODE
            printf("%s", child_buf);
#endif
            memset(child_buf, '\0', sizeof(child_buf));
        } while (cn == sizeof(child_buf));
#ifdef MODE
        close(fd_child);
#endif
        exit(0);
    default:
        printf("PARENT\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
#ifdef MODE
        fd_parent = open("text_parent.txt", O_RDWR | O_CREAT, 0644);
#endif
        do {
            pn = read(fd, parent_buf, sizeof(parent_buf));
#ifdef MODE
            write(fd_parent, parent_buf, pn);
#endif
#ifndef MODE
            printf("%s", parent_buf);
#endif
            memset(parent_buf, '\0', sizeof(parent_buf));
        } while (pn == sizeof(parent_buf));
#ifdef MODE
        close(fd_parent);
#endif
        wait(&status);
        break;
    }
    close(fd);

#ifdef MODE
    fd_child = open("text_child.txt", O_RDONLY);
    printf("\ntext_child.txt:\n");
    do {
        pn = read(fd_child, parent_buf, sizeof(parent_buf));
        printf("%s", parent_buf);
        memset(parent_buf, '\0', sizeof(parent_buf));
    } while (pn == sizeof(parent_buf));
    close(fd_child);

    fd_parent = open("text_parent.txt", O_RDONLY);
    printf("\ntext_parent.txt:\n");
    do {
        pn = read(fd_parent, parent_buf, sizeof(parent_buf));
        printf("%s", parent_buf);
        memset(parent_buf, '\0', sizeof(parent_buf));
    } while (pn == sizeof(parent_buf));
    close(fd_parent);
#endif
    return 0;
}
