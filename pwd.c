#include "headers.h"

extern int maxPathSize;

void pwd()
{
    char delim[] = " \n\t";
    char *token = strtok(NULL, delim);
    int stdout1 = dup(1), stdin1 = dup(0), fd;
    if(token!=NULL)
    {
        if(!strcmp(token,">"))
        {
            token = strtok(NULL,delim);
            fd = open(token, O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd, 1);
        }
        else if(!strcmp(token,">>"))
        {
            token = strtok(NULL,delim);
            fd = open(token, O_RDWR | O_CREAT | O_APPEND, 0644);
            dup2(fd, 1);
        }
        else
        {
            dup2(stdout1,1);
            printf("\033[1;31mpwd: ");
            printf("invalid command");
            printf("\033[0m\n");
        }
    }
    char buf[maxPathSize];
    getcwd(buf, maxPathSize);
    printf("%s\n",buf);
    dup2(stdout1,1);
    dup2(stdin1,0);
}