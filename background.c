#include "headers.h"
#include "cd.h"
#include "echo.h"
#include "pwd.h"
#include "ls.h"
#include "history.h"
#include "execute.h"
#include "pinfo.h"
#include "discover.h"

extern char *args[];
extern pid_t bgpCount[];
extern char foregroundName[];
extern char *foregroundArgs[];
extern int bgp;
extern int maxPathSize;
char home[10000];
char cwd[20000];
char display[20000];

void next1(char currPath[], char home[])
{
    write(2, "<", 1);
    write(2, "\033[0;32m\x1B[1m", strlen("\033[0;32m\x1B[1m"));
    write(2, display, strlen(display));
    write(2, "\033[0m:", strlen("\033[0m:"));
    write(2, "\033[0;34m\x1B[1m", strlen("\033[0;34m\x1B[1m"));
    write(2, currPath, strlen(currPath));
    write(2, "\033[0m\x1B[0m> ", strlen("\033[0m\x1B[0m> "));
}

void backgroundFun()
{
    int status;
    pid_t r = waitpid(-1, &status, WNOHANG);

    for (int i = 0; i < bgp; i++)
    {
        if (r < 0)
        {
            break;
        }
        int y = WEXITSTATUS(status);
        int cur = WIFEXITED(status);
        if (cur && r == bgpCount[i])
        {
            char buf[maxPathSize];
            //printf("bgpName is %s\n",bgpName[i]);
            strcpy(buf, bgpName[i]);
            char buf2[maxPathSize];
            //printf("wow\n");
            sprintf(buf2, " with pid %d exited ", bgpCount[i]);
            strcat(buf, buf2);

            //write(2,bgpName[i],strlen(bgpName(1)))
            if (y == 0)
            {
                strcat(buf, "normally\n");
            }
            else
            {
                strcat(buf, "abnormally\n");
            }
            write(1, "\n", 1);
            write(1, buf, strlen(buf));
            //flagGlobal = 1;
            next1(cwd, home);
        }
    }
    signal(SIGCHLD, backgroundFun);
}

void background(char *a, int count, char home1[], char cwd1[], char display1[], pid_t shell)
{
    strcpy(home, home1);
    strcpy(cwd, cwd1);
    strcpy(display, display1);
    char delim1[] = " \t\n";
    int l = strlen(a);
    int flagOnlyBgnd = 0;
    for (int j = l - 1; j >= 0; j--)
    {
        if (a[j] == '\n' && a[j] == '\t' && a[j] == ' ')
        {
            continue;
        }
        else
        {
            if (a[j] == '&')
            {
                flagOnlyBgnd = 1;
            }
            break;
        }
    }
    if (flagOnlyBgnd)
    {
        //It ends with an &
        pid_t temp;
        //int statusChildren[count];
        for (int j = 0; j < count; j++)
        {
            char *token = strtok(args[j], delim1);
            if (token != NULL)
            {
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
                temp = fork();
                strcpy(bgpName[bgp], comm);
                bgpCount[bgp] = temp;
                bgp++;
                //printf("%d\n",temp);
                //printf("[%d] %d\n", bgp, temp);
                if (temp < 0)
                {
                    printf("\033[1;31mfork() failure: ");
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                    continue;
                }
                if (temp == 0)
                {
                    if (signal(SIGTSTP, backgroundFun))
                    {
                    }
                    int h = execvp(comm, argExec);
                    if (h < 0)
                    {
                        printf("\033[1;31m%s: ", comm);
                        int err = errno;
                        printf("%s", strerror(err));
                        printf("\033[0m\n");
                        exit(1);
                    }
                    exit(0);
                }
                else
                {
                    printf("[%d] %d\n", bgp, temp);
                }
            }
        }
    }
    else
    {
        // Has foreground and background processes to execute
        count--;
        pid_t temp;
        //int statusChildren[count];
        for (int j = 0; j < count; j++)
        {
            char *token = strtok(args[j], delim1);
            if (token != NULL)
            {
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
                temp = fork();
                //printf("comm is %s\n",comm);
                strcpy(bgpName[bgp], comm);
                bgpCount[bgp] = temp;
                bgp++;
                if (temp < 0)
                {
                    printf("\033[1;31mfork() failure: ");
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                    continue;
                }
                if (temp == 0)
                {
                    if (signal(SIGTSTP, backgroundFun))
                    {
                    }
                    int h = execvp(comm, argExec);
                    if (h < 0)
                    {
                        printf("\033[1;31m%s: ", comm);
                        int err = errno;
                        printf("%s", strerror(err));
                        printf("\033[0m\n");
                        exit(1);
                    }
                    exit(0);
                }
                else
                {
                    printf("[%d] %d\n", bgp, temp);
                }
            }
        }
        // For foreground
        char *token = strtok(args[count], delim1);
        if (!strcmp(token, "cd"))
        {
            cd(home);
        }
        else if (!strcmp(token, "pwd"))
        {
            pwd();
        }
        else if (!strcmp(token, "echo"))
        {
            echo();
        }
        else if (!strcmp(token, "ls"))
        {
            ls(home);
        }
        else if (!strcmp(token, "history"))
        {
            history();
        }
        else if (!strcmp(token, "pinfo"))
        {
            pinfo(shell, home);
        }
        else if (!strcmp(token, "discover"))
        {
            discover(home);
        }
        else
        {
            if (token != NULL)
            {
                int l1 = strlen(token);
                char comm1[l];
                strcpy(comm1, token);
                int c1 = 0;
                char *argExec1[100];
                while (token != NULL)
                {
                    argExec1[c1] = token;
                    c1++;
                    token = strtok(NULL, delim1);
                }
                argExec1[c1] = NULL;
                c1++;
                pid_t child1;
                int statusChild1 = 0;
                child1 = fork();
                if (child1 < 0)
                {
                    printf("\033[1;31mfork() failure: ");
                    int err = errno;
                    printf("%s", strerror(err));
                    printf("\033[0m\n");
                    return;
                }
                int iter = 0;
                while(argExec1[iter])
                {
                    foregroundArgs[iter] = argExec1[iter];
                    iter++;
                }
                foregroundArgs[iter] = NULL;
                strcpy(foregroundName,comm1);
                if (child1 == 0)
                {
                    int h = execvp(comm1, argExec1);
                    if (h < 0)
                    {
                        printf("\033[1;31m%s: ", comm1);
                        int err = errno;
                        printf("%s", strerror(err));
                        printf("\033[0m\n");
                        exit(0);
                    }
                    exit(1);
                }
                while (waitpid(child1, &statusChild1, 0) == 0)
                {
                }
            }
        }
    }
}