#include "logerr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fp1[2], fp2[2], b = 0;
    char buf[512] = {0};

    if (pipe(fp1) != 0) {
        LOG_ERR("Failed to open pipe 1");
        exit(EXIT_FAILURE);
    }

    if (pipe(fp2) != 0) {
        LOG_ERR("Failed to open pipe 2");
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
    case -1:
        LOG_ERR("Fork failed");
        exit(EXIT_FAILURE);
    case 0:
        close(fp1[0]); // close read end of pipe 1
        close(fp2[1]); // wr of pipe2
        printf("\nChild process %d\n", getpid());

        while ((b = read(0, buf, sizeof(buf))) > 0) {
            printf("Child read %d bytes \n", b);
            b = write(fp1[1], buf, b);

            memset(&buf, 0, 512);

            b = read(fp2[0], buf, 512);

            if (b > 0) {
                printf("Child read more:%s\n", buf);
                memset(&buf, 0, 512);
            }
        }
        close(fp2[0]);
        close(fp1[1]); // close write end of pipe 1
        exit(EXIT_SUCCESS);
    default:
        close(fp1[1]); // close write end of pipe 1
        close(fp2[0]); // rd pipe2
        printf("Parent process %d\n", getpid());
        do {
            b = read(fp1[0], buf, 512);
            if (b == -1)
                sleep(1);
            if (b > 0) {
                printf("Parent wrote %s\n", buf);
                write(fp2[1], buf, b);
            }
            memset(&buf, 0, 512);
        } while (b != 0);
        close(fp2[1]); // wr pipe2
        close(fp1[0]); // close read end of pipe 1
        exit(EXIT_SUCCESS);
    }
}
