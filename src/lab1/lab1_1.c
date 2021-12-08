#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

//extern const char *const sys_errlist[];

int main()
{
    int fd = open("notexistingfile", O_RDONLY);
    if (fd < 0) {
        printf("Failed to open file\n");
    }
    printf("errno value is %d\n", errno);
    //printf("sys_errlist[errno] = %s\n", sys_errlist[errno]);
    perror("perror - open() error");
    return 0;
}
