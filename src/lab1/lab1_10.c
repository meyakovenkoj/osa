#include "logerr.h"
#include "readstring.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage: program <file>\n");
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        LOG_ERR("open() error");
    }

    off_t start = lseek(fd, 0, SEEK_SET);
    off_t end = lseek(fd, 0, SEEK_END);

    for (off_t i = end; i >= start - 1; i--) {
        char chr;
        read(fd, &chr, 1);
        printf("%c", chr);
        lseek(fd, i, SEEK_SET);
    }
    fflush(stdout);

    if (close(fd)) {
        LOG_ERR("close() error");
    }

    return 0;
}
