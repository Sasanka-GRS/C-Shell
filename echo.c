#include "headers.h"

void echo()
{
    int flagg = 0, flaggg = 0, flagl = 0;
    char pathIn[10000], pathOut[10000];
    pathIn[0] = '\0';
    pathOut[0] = '\0';
    char delim1[] = " \t\n";
    char *token = strtok(NULL, delim1);
    char args[1000][1000];
    int stdout1 = dup(1), stdin1 = dup(0), fd, c = 0;
    while (token != NULL)
    {
        if (flagg == 0 && !strcmp(token, ">"))
        {
            // printf("here1\n");
            flagg = 1;
            token = strtok(NULL, delim1);
            strcpy(pathOut, token);
            // fd = open(pathOut, O_RDWR | O_CREAT | O_TRUNC, 0644);
            break;
        }
        if (flaggg == 0 && !strcmp(token, ">>"))
        {
            // printf("here2\n");
            flaggg = 1;
            token = strtok(NULL, delim1);
            strcpy(pathOut, token);
            // fd = open(pathOut, O_RDWR | O_CREAT | O_TRUNC, 0644);
            break;
        }
        char buf[1000];
        strcpy(buf, token);
        strcpy(args[c], buf);
        // write(1, args[c], strlen(args[c]));
        c++;
        token = strtok(NULL, delim1);
    }
    // printf("wow\n");
    if (!flagg && !flaggg)
    {
         //printf("here");
        for (int i = 0; i < c; i++)
        {
            printf("%s ", args[i]);
        }
        printf("\n");
        return;
    }
    int fdO, fdI;
    // printf("pathIn and pathOut are %s, %s\n", pathIn, pathOut);
    if (flagg)
    {
        fdO = open(pathOut, O_RDWR | O_CREAT | O_TRUNC, 0644);
        dup2(fdO, 1);
        // printf("Yaay");
    }
    if (flaggg)
    {
        fdO = open(pathOut, O_RDWR | O_CREAT | O_APPEND, 0644);
        dup2(fdO, 1);
    }
    for (int i = 0; i < c; i++)
    {
        printf("%s ", args[i]);
    }
    printf("\n");
    dup2(stdout1, 1);
    dup2(stdin1, 0);
}