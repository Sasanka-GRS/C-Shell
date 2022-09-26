#include "headers.h"
#include "cd.h"
#include "echo.h"
#include "pwd.h"
#include "ls.h"
#include "history.h"
#include "execute.h"
#include "background.h"
#include "pinfo.h"
#include "discover.h"
#include "next.h"

int maxHostSize = 1000;
int maxHomeSize = 10000;
int maxPathSize = 20000;
int maxInputSize = 10000;

pid_t bgpCount[100000];
pid_t shell;
char *bgpComm[100000];
int bgp = 0;
char *foregroundArgs[10000];
char foregroundName[10000];
pid_t fore = -1;
char home[10000];
int totalTime = 0;
time_t begin = 0;
char display[20000];
char cwd[20000];
char prev[20000];
char *arguments[100];
char *args[100];
extern int errno;

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    pid_t x = getpid();
    if (x != shell)
    {
        kill(x, SIGINT);
    }
    else
    {
        write(2, "\n<", 2);
        write(2, "\033[0;32m\x1B[1m", strlen("\033[0;32m\x1B[1m"));
        write(2, display, strlen(display));
        write(2, "\033[0m:", strlen("\033[0m:"));
        write(2, "\033[0;34m\x1B[1m", strlen("\033[0;34m\x1B[1m"));
        write(2, cwd, strlen(cwd));
        write(2, "\033[0m\x1B[0m> ", strlen("\033[0m\x1B[0m> "));
    }
}

void sigtHandler(int sig_num)
{
    pid_t x = getpid();
    if (x != shell)
    {
        return;
    }
    else
    {
        if (fore == -1)
        {
            write(2, "\r\n<", 3);
            write(2, "\033[0;32m\x1B[1m", strlen("\033[0;32m\x1B[1m"));
            write(2, display, strlen(display));
            write(2, "\033[0m:", strlen("\033[0m:"));
            write(2, "\033[0;34m\x1B[1m", strlen("\033[0;34m\x1B[1m"));
            write(2, cwd, strlen(cwd));
            write(2, "\033[0m\x1B[0m> ", strlen("\033[0m\x1B[0m> "));
            signal(SIGTSTP, sigtHandler);
            return;
        }
        strcpy(bgpName[bgp], foregroundName);
        bgpCount[bgp] = fore;
        bgp++;
        kill(fore, SIGKILL);
        fore = -1;
    }
    signal(SIGTSTP, sigtHandler);
}

int main(int argc, char *argv[])
{
    char *user = getlogin();
    char host[maxHostSize];
    gethostname(host, maxHostSize);

    shell = getpid();

    int userLen = strlen(user), hostLen = strlen(host);
    strcpy(display, user);
    strcat(display, "@");
    strcat(display, host);

    char hist[] = ".history.txt";
    int fd = open(hist, O_RDWR | O_CREAT, 0600);
    close(fd);

    char home[maxHomeSize];
    getcwd(home, maxHomeSize);
    cwd[0] = '~';
    cwd[1] = '\0';
    prev[0] = '\0';

    // Signal Handlers
    signal(SIGINT, sigintHandler);
    signal(SIGTSTP, sigtHandler);

    // At startup
    begin = time(NULL);
    next("~", home, display, begin);
    if (!strlen(inputArgument))
    {
        exit(0);
    }
    int iter = 0;
    while (1)
    {

        iter = 0;
        char delim[] = ";";
        char *arg1 = strtok(inputArgument, delim);
      
        if (!strcmp(arg1, "\n"))
        {
            // printf("here\n");
            next(cwd, home, display, begin);
            continue;
        }
        //printf("here\n");
        //printf(" arg1 is %s***", arg1);
        while (arg1 != NULL)
        {
            if (arg1[0] == '\n')
            {
                arg1 = strtok(NULL, delim);
                continue;
            }
            //printf("arg2 is %s*****",arg1);
            arguments[iter] = arg1;
            arg1 = strtok(NULL, delim);
            iter++;
        }
        //printf("here");
        char delim1[] = " \t\n";
        char delim2[] = "&";
        for (int i = 0; i < iter; i++)
        {
            begin = time(NULL);
            int count = 0;
            //printf("here");
            char temp[maxPathSize];
            //printf("here");
            strcpy(temp, arguments[i]);
            //printf("here");
            char *tok = strtok(temp, delim2);
            if(tok==NULL)
            {
                continue;
            }
            while (tok != NULL)
            {
                args[count] = tok;
                count++;
                tok = strtok(NULL, delim2);
            }
            int flagAmp = 0, flagDir = 0;
            if (count == 1)
            {
                int l = strlen(arguments[i]);
                for (int j = 0; j < l; j++)
                {
                    if (arguments[i][j] == '&')
                    {
                        flagAmp = 1;
                        break;
                    }
                }
            }
            else
            {
                flagAmp = 1;
            }
            int l = strlen(arguments[i]);
            for (int j = 0; j < l; j++)
            {
                if (arguments[i][j] == '>' || arguments[i][j] == '<')
                {
                    flagDir = 1;
                    break;
                }
            }
            if (!flagAmp)
            {
                char *token = strtok(arguments[i], delim1);
                if (!strcmp(token, "exit"))
                {
                    return 0;
                }
                else if (!strcmp(token, "cd"))
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
                        execute(token);
                    }
                }
            }
            else
            {
                // run background process here
                // 1. If it ends with &
                // 2. If it has foreground also
                background(arguments[i], count, home, cwd, display, shell);
            }
        }
        next(cwd, home, display, begin);
        if (!strlen(inputArgument))
        {
            exit(0);
        }
        signal(SIGINT, sigintHandler);
        signal(SIGTSTP, sigtHandler);
        signal(SIGCHLD, backgroundFun);
    }

    return 0;
}
