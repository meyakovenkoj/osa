#include "logerr.h"
#include "readstring.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct stat info = {0};
    if (argc != 2) {
        printf("Usage: program <file>\n");
        exit(1);
    }

    if (stat(argv[1], &info)) {
        LOG_ERR("stat error");
    }

    if (S_ISREG(info.st_mode)) {
        printf("regular file\n");
    }

    if (S_ISDIR(info.st_mode)) {
        printf("directory\n");
    }

    if (S_ISCHR(info.st_mode)) {
        printf("char device\n");
    }

    if (S_ISBLK(info.st_mode)) {
        printf("block device\n");
    }

    return 0;
}
