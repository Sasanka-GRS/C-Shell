#include "headers.h"

extern int maxPathSize;

void pinfo(pid_t shell, char home[])
{
    pid_t here;
    //printf("pid of shell is %d\n",shell);
    char delim[] = " \n\t";
    int stdout1 = dup(1), stdin1 = dup(0);
    char *token = strtok(NULL, delim);
    if (token == NULL)
    {
        here = shell;
    }
    else
    {
        if (!strcmp(token, ">"))
        {
            here = shell;
            token = strtok(NULL, delim);
            int fdO = open(token, O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fdO, 1);
        }
        else if (!strcmp(token, ">>"))
        {
            here = shell;
            token = strtok(NULL, delim);
            int fdO = open(token, O_RDWR | O_CREAT | O_APPEND, 0644);
            dup2(fdO, 1);
        }
        else
        {
            here = atoi(token);
            token = strtok(NULL, delim);
            if (token != NULL)
            {
                if (!strcmp(token, ">"))
                {
                    token = strtok(NULL, delim);
                    int fdO = open(token, O_RDWR | O_CREAT | O_TRUNC, 0644);
                    dup2(fdO, 1);
                }
                else if (!strcmp(token, ">>"))
                {
                    token = strtok(NULL, delim);
                    int fdO = open(token, O_RDWR | O_CREAT | O_APPEND, 0644);
                    dup2(fdO, 1);
                }
                else
                {
                    printf("\033[1;31mpinfo: invalid input");
                    printf("\033[0m\n");
                }
            }
        }
    }
    pid_t console_pid = tcgetpgrp(STDOUT_FILENO);
    int foreground = 0;
    if (console_pid == here)
    {
        foreground = 1;
    }
    printf("pid : %d\n", here);
    char buffer[maxPathSize];

    sprintf(buffer, "/proc/%d/status", here);
    char bufs[1000];
    int fdHere = open(buffer, O_RDONLY);
    if (fdHere < 0)
    {
        dup2(stdout1,1);
        printf("\033[1;31mpinfo: cannot access process with pid = %d: No status", here);
        printf("\033[0m\n");
        return;
    }
    read(fdHere, bufs, 1000);
    char *t = strtok(bufs, "\n");
    t = strtok(NULL, "\n");
    t = strtok(NULL, "\n");
    printf("process status : %c", t[7]);
    if (foreground && (t[7] == 'R' || t[7] == 'S'))
    {
        printf("+");
    }
    printf("\n");

    sprintf(buffer, "/proc/%d/statm", here);
    char bufm[1000];
    int fdHereM = open(buffer, O_RDONLY);
    if (fdHereM < 0)
    {
        dup2(stdout1,1);
        printf("\033[1;31mpinfo: cannot access process with pid = %d: No Memory", here);
        printf("\033[0m\n");
        return;
    }
    read(fdHereM, bufm, 1000);
    char *tm = strtok(bufm, " ");
    printf("memory : %s\n", tm);

    sprintf(buffer, "/proc/%d/exe", here);
    char bufe[maxPathSize];
    int l1 = -1;
    l1 = readlink(buffer, bufe, maxPathSize);
    // printf("readlink is %s\n",bufe);
    if (l1 == -1)
    {
        dup2(stdout1,1);
        printf("\033[1;31mpinfo: cannot access process with pid = %d: No executable path", here);
        printf("\033[0m\n");
        return;
    }
    int yes = 0;
    char s[maxPathSize];
    int l = strlen(home);
    strncpy(s, bufe, l);
    s[l] = '\0';
    if (!strcmp(home, s))
    {
        yes = 1;
    }
    if (yes)
    {
        char buf[maxPathSize];
        buf[0] = '~';
        buf[1] = '\0';
        int l1 = strlen(bufe), count = 0, count1 = 0;
        for (int i = l; i < l1; i++)
        {
            buf[i - l + 1] = bufe[i];
        }
        buf[l1 - l + 1] = '\0';
        strcpy(bufe, buf);
    }

    printf("executable path : %s\n", bufe);
    dup2(stdout1, 1);
    dup2(stdin1, 0);
}