#include "logerr.h"
#include "readstring.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    char *filepath = NULL;
    int mode = 0;
    int rmode = 1;
    off_t offset;
    size_t cData;
    char *pData;
    size_t buf_size;
    char *buffer;
    int filemode = O_CREAT;
    printf("Enter path to file: ");
    fflush(stdout);
    filepath = readline(NULL);
    printf("Enter file mode (4 - r/o, 2 - w/o, 6 - r/w): ");
    fflush(stdout);
    scanf("%d", &mode);
    switch (mode) {
    case 4:
        filemode |= O_RDONLY;
        break;
    case 2:
        filemode |= O_WRONLY;
        break;
    case 6:
        filemode |= O_RDWR;
        break;
    default:
        LOG_ERR("Mode must be 0 / 1 / 2");
        fflush(stdout);
        return 1;
    }
    int fd = open(filepath, filemode, mode << 6 | mode << 3 | mode);
    if (fd < 0) {
        LOG_ERR("open() error");
    }
    while (rmode > 0 && rmode < 4) {
        printf("Enter mode of editing: (0 - done, 1 - lseek, 2 - write, 3 - read)\n");
        char *md = readline(NULL);
        rmode = atoi(md);
        free(md);
        switch (rmode) {
        case 1:
            printf("Enter offset:\n");
            char *off = readline(NULL);
            offset = atoll(off);
            free(off);
            lseek(fd, offset, SEEK_SET);
            break;
        case 2:
            printf("Enter data to file(ctrl+d at end):\n");
            fflush(stdout);

            pData = readstr(&cData);
            if (write(fd, pData, cData) == -1) {
                LOG_ERR("write() error");
            }
            free(pData);
            break;
        case 3:
            buffer = readstring(&buf_size, 0, fd);
            if (!buffer) {
                LOG_ERR("read() from file error");
            } else {
                printf("file:\n%s\n", buffer);
                fflush(stdout);
            }
            free(buffer);
            break;
        default:
            break;
        }
    }

    if (close(fd)) {
        LOG_ERR("close() error");
    }

    free(filepath);
    return 0;
}
