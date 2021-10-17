#ifndef READSTRING_H
#define READSTRING_H
#include "logerr.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 10

/*! \brief Function for reading unlimited string until EOF.
    \param size pointer to size value. Can be NULL
    \param multiline value for read multiline string, in case for multiline must be equal to 0
    \param fd number of file descriptor. For standard input - stdin
    \return Alloced string with 0 at the end. size equals to length of string.
*/
char *readstring(size_t *size, int multiline, int fd)
{
    int read_size;
    char *buf = NULL;
    char *tmp = NULL;
    size_t current_size = 0;
    char buffer[BUF_SIZE];
    read_size = read(fd, buffer, BUF_SIZE - 1);
    if (read_size < 0) {
        LOG_ERR("read fail");
        return NULL;
    }
    buffer[read_size] = '\0';
    while (read_size > 0) {
        tmp = (char *)malloc(current_size + read_size + 1);
        if (tmp == NULL) {
            return NULL;
        }
        memcpy(tmp, buf, current_size);
        free(buf);
        buf = tmp;
        if (multiline && buffer[read_size - 1] == '\n') {
            memcpy(buf + current_size, buffer, read_size - 1);
            current_size += read_size - 1;
            buf[current_size] = '\0';
            break;
        }
        memcpy(buf + current_size, buffer, read_size);
        current_size += read_size;
        buf[current_size] = '\0';
        memset(buffer, '\0', BUF_SIZE);
        read_size = read(fd, buffer, BUF_SIZE - 1);
    }
    if (size) {
        *size = current_size;
    }
    return buf;
}

/*! \brief Function for reading unlimited line.
    readstring reads string till \n or EOF.
    \param size pointer to size value. Can be NULL
    \return Alloced string with 0 at the end. size equals to length of string.
*/
char *readline(size_t *size)
{
    return readstring(size, 1, STDIN_FILENO);
}

/*! \brief Function for reading unlimited string.
    \param size pointer to size value. Can be NULL
    \return Alloced string with 0 at the end. size equals to length of string.
*/
char *readstr(size_t *size)
{
    return readstring(size, 0, STDIN_FILENO);
}

#endif // READSTRING_H
