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
    int input_fd;
    int output_fd;
    if (argc == 3) {
        input_fd = open(argv[1], O_RDONLY);
        if (input_fd < 0) {
            LOG_ERR("open input error");
            exit(errno);
        }

        output_fd = open(argv[2], O_RDONLY);
        if (output_fd < 0) {
            LOG_ERR("open output error");
            exit(errno);
        }
    } else {
        printf("File copy\nUsage program file1 file2\nIf there are no args we use stdin and stdout\n");
        exit(1);
    }
    off_t end1 = lseek(input_fd, 0, SEEK_END);
    off_t end2 = lseek(output_fd, 0, SEEK_END);

    if (end1 > end2) {
        printf("%s is bigger. size=%ld bytes\n", argv[1], (long)end1);
    } else if (end2 > end1) {
        printf("%s is bigger. size=%ld bytes\n", argv[2], (long)end2);
    } else {
        printf("They are equal. size=%ld bytes\n", (long)end1);
    }
    if (close(input_fd)) {
        LOG_ERR("close() error");
    }

    if (close(output_fd)) {
        LOG_ERR("close() error");
    }

    return 0;
}
