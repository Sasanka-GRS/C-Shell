#include "headers.h"
#include "background.h"
#include "history.h"

void next(char currPath[],char home[],char display[],time_t begin)
{
    time_t end = time(NULL);
    int totalTime = end - begin;
    printf("\r<");
    printf("\033[0;32m\x1B[1m%s", display);
    printf("\033[0m:");
    printf("\033[0;34m\x1B[1m%s", currPath);
    printf("\033[0m\x1B[0m");
    if (totalTime > 1)
    {
        printf("took %ds", totalTime);
    }
    printf("> ");
    for (int i = 0; i < 20000; i++)
    {
        inputArgument[i] = '\0';
    }
    /*if(flagGlobal)
    {
        flagGlobal = 0;
        return;
    }*/
    scanf("%[^\n]%*c", inputArgument);
    historyPush(inputArgument,home);
    signal(SIGCHLD,backgroundFun);
}