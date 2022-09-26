#include "headers.h"
#include "background.h"
#include "history.h"

void next(char currPath[], char home[], char display[], time_t begin)
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
    size_t b = 10000;
    char* buf = NULL;
    buf = (char*) malloc(b*sizeof(char));
    int len = getline(&buf,&b, stdin);
    strcpy(inputArgument,buf);
    //scanf("%[^\n]%*c", inputArgument);
    if(!len)
    {
        printf("in is %s*****", inputArgument);
    }
    while (!len)
    {
        printf("\n");
        exit(0);
        write(1,"\r<", strlen("\r<"));
        write(1,"\033[0;32m\x1B[1m",strlen("\033[0;32m\x1B[1m"));
        write(1,display,strlen(display));
        write(1,"\033[0m:",strlen("\033[0m:"));
        write(1,"\033[0;34m\x1B[1m", strlen("\033[0;34m\x1B[1m"));
        write(1,currPath,strlen(currPath));
        write(1,"\033[0m\x1B[0m",strlen("\033[0m\x1B[0m"));
        write(1,"> ",strlen("> "));
        scanf("%[^\n]%*c", inputArgument);
    }
    historyPush(inputArgument, home);
    signal(SIGCHLD, backgroundFun);
}