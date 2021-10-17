#include "readstring.h"
#include <stdio.h>

int main()
{
    char *string = NULL;
    size_t size = 0;
    printf("Enter string:\n");
    string = readline(&size);
    if (size) {
        printf("You have entered:\n%s\n", string);
        printf("Length of string is %zu\n", size);
    } else {
        return 1;
    }
    free(string);
    return 0;
}
