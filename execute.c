#include "headers.h"

extern time_t begin;

void execute(char *token)
{
    char delim1[] = " \t\n";
    int l = strlen(token);
    char comm[l];
    strcpy(comm, token);
    int c = 0;
    char *argExec[100];
    while (token != NULL)
    {
        argExec[c] = token;
        c++;
        token = strtok(NULL, delim1);
    }
    argExec[c] = NULL;
    c++;
    pid_t shell, child;
    int statusChild = 0;
    shell = getpid();
    child = fork();
    begin = time(NULL);
    if (child < 0)
    {
        printf("\033[1;31mfork() failure: ");
        int err = errno;
        printf("%s", strerror(err));
        printf("\033[0m\n");
        return;
    }
    if (child == 0)
    {
        int h = execvp(comm, argExec);
        if (h < 0)
        {
            printf("\033[1;31m%s: ", comm);
            int err = errno;
            printf("%s", strerror(err));
            printf("\033[0m\n");
            exit(0);
        }
        exit(1);
    }
    while(waitpid(child, &statusChild, 0)==0)
    {
    }
}