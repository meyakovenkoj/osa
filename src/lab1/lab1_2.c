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
    printf("Enter data to file(ctrl+d at end):\n");
    fflush(stdout);
    size_t cData;
    char *pData;
    pData = readstr(&cData);
    if (write(fd, pData, cData) == -1) {
	LOG_ERR("write() error");
    }
    if (close(fd)) {
	LOG_ERR("close() error");
    }
    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
	LOG_ERR("open() error");
    }
    size_t buf_size;
    char *buffer = readstring(&buf_size, 0, fd);
    if (!buffer) {
	LOG_ERR("read() from file error");
    } else {
	printf("file:\n%s\n", buffer);
	fflush(stdout);
    }
    if (close(fd)) {
	LOG_ERR("close() error");
    }
    fd = open(filepath, O_RDWR);
    if (fd < 0) {
	LOG_ERR("open() error");
    }

    free(buffer);
    free(pData);
    free(filepath);
    return 0;
}
