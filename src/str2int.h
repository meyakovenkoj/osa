#ifndef STR2INT_H
#define STR2INT_H
#include <limits.h>

/*! \brief Function for conversation of string to positive integer.
    \param str pointer to string
    \return Positive int or -1 on error
*/
int str2int(const char *str, int *result)
{
    long int res = 0;
    int sign = 1;
    const char *c = str;
    if (!str) {
        return -1;
    }
    if (!result) {
        return -1;
    }
    if (*c == 45) {
        sign = -1;
        c++;
    }
    for (; *c; c++) {
        if (*c < 48 || *c > 57) {
            return -1;
        }
        res *= 10;
        res += (long)(*c - 48);
        if (res > INT_MAX) {
            return -1;
        }
    }
    *result = (int)res * sign;
    return 0;
}

int str2long(const char *str, long *result)
{
    long long int res = 0;
    int sign = 1;
    const char *c = str;
    if (!str) {
        return -1;
    }
    if (!result) {
        return -1;
    }
    if (*c == 45) {
        sign = -1;
        c++;
    }
    for (; *c; c++) {
        if (*c < 48 || *c > 57) {
            return -1;
        }
        res *= 10;
        res += (long long)(*c - 48);
        if (res > LONG_MAX) {
            return -1;
        }
    }
    *result = (long)res * sign;
    return 0;
}

#endif // STR2INT_H
