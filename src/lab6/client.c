#include "logerr.h"
#include "str2int.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1024

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    int port;

    if (argc < 3) {
        printf("Usage ./client -p <port> <array of nums>\n");
        exit(1);
    }
    if (strcmp(argv[1], "-p") == 0) {
        int err = str2int(argv[2], &port);
        if (err) {
            LOG_ERR("invailid port");
            exit(EXIT_FAILURE);
        }
    } else {
        LOG_ERR("no -p port");
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOG_ERR("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n;
    socklen_t len;

    char *hello = NULL;
    int i = 3;
    while (i < argc) {
        int strl = strlen(argv[i]);
        hello = realloc(hello, strl + 1);
        strcat(hello, argv[i]);
        strcat(hello, " ");
        i++;
    }

    if (!hello) {
        LOG_ERR("no int args");
        exit(EXIT_FAILURE);
    }

    int err = sendto(sockfd, (const char *)hello, strlen(hello),
                     0, (const struct sockaddr *)&servaddr,
                     sizeof(servaddr));
    if (err == -1) {
        LOG_ERR("send failed");
        exit(EXIT_FAILURE);
    }

    if (err != strlen(hello)) {
        LOG_ERR("transmission failed");
        exit(EXIT_FAILURE);
    }

    printf("Hello message sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&servaddr,
                 &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}
