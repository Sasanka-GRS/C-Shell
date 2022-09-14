#include "headers.h"

void echo()
{
    char delim[] = " \t\n";
    char *token = strtok(NULL, delim);
    while (token != NULL)
    {
        int l = strlen(token);
        if (token[l - 1] == '\n')
        {
            write(1, token, l - 1);
            break;
        }
        printf("%s ",token);
        token = strtok(NULL, delim);
    }
    printf("\n");
}