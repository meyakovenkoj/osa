#include "logerr.h"
#include "readstring.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int copy()
{
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;
    size_t cData;
    char *pData = readstring(&cData, 0, input_fd);
    if (!pData) {
	LOG_ERR("Error file read");
	exit(errno);
    }
    if (write(output_fd, pData, cData) == -1) {
	LOG_ERR("write() error");
    }

    free(pData);

    return 0;
}

int main(int argc, char **argv)
{
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;
    if (argc == 1) {
    } else if (argc == 3) {
	if (close(input_fd)) {
	    LOG_ERR("close() error");
	}

	if (close(output_fd)) {
	    LOG_ERR("close() error");
	}
	input_fd = open(argv[1], O_RDONLY);
	if (input_fd < 0) {
	    LOG_ERR("open input error");
	    exit(errno);
	}
	struct stat info = {0};
	if (stat(argv[1], &info)) {
	    LOG_ERR("stat error");
	}

	output_fd = open(argv[2], O_CREAT | O_WRONLY, info.st_mode);
	if (output_fd < 0) {
	    LOG_ERR("open output error");
	    exit(errno);
	}
	dup2(input_fd, STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
    } else {
	printf("File copy\nUsage program file1 file2\nIf there are no args we use stdin and stdout\n");
	exit(1);
    }
    copy();
    if (close(input_fd)) {
	LOG_ERR("close() error");
    }

    if (close(output_fd)) {
	LOG_ERR("close() error");
    }

    return 0;
}
